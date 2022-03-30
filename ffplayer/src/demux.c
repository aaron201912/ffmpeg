#include "demux.h"
#include "packet.h"
#include "frame.h"
#include "player.h"
#include <sys/time.h>

extern AVPacket a_flush_pkt, v_flush_pkt, v_extra_pkt;
static int genpts = 0;
static struct timeval read_now, read_start, read_timeout;
static AVDictionary *format_opts, *codec_opts;

#define PLAYER_READ_TIMEOUT  10  //单位秒

AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
                                           AVDictionary *codec_opts)
{
    AVDictionary **opts;

    if (!s->nb_streams)
        return NULL;
    opts = av_mallocz_array(s->nb_streams, sizeof(*opts));
    if (!opts) {
        av_log(NULL, AV_LOG_ERROR,
               "Could not alloc memory for stream options.\n");
        return NULL;
    }
    return opts;
}

static int decode_interrupt_cb(void *ctx)
{
    player_stat_t *is = (player_stat_t *)ctx;
    bool flag;

    gettimeofday(&read_now, NULL);
    flag = (read_now.tv_sec - read_start.tv_sec > PLAYER_READ_TIMEOUT) ? true : false;
    if (flag) {
        is->time_out = true;
        av_log(NULL, AV_LOG_ERROR, "timeout of reading packet from network!\n");
    }
    return flag || is->abort_request;
}

static int stream_has_enough_packets(AVStream *st, int stream_id, packet_queue_t *queue, player_stat_t *is)
{
    int min_audio_frame_num = 0;

    /*according to the formula, video_frame_num * video_frame_intvl = audio_frame_num * audio_frame_intvl  */
    /*video and audio should keep the same pts for playing, so the total time for caching data should be the same*/

    if(is->audio_frame_duration && is->video_frame_duration)
        min_audio_frame_num = (int)(is->video_frame_duration * MIN_VIDEO_FRAMES / is->audio_frame_duration);
    else
        min_audio_frame_num = MIN_AUDIO_FRAMES;

    //printf("(%d %d %d %d), min_audio_frame_num: %d, min_video_frame_num: %d\n",
    //    stream_id, is->audio_idx, is->video_idx, queue->nb_packets, min_audio_frame_num, MIN_VIDEO_FRAMES);

    //printf("id: %d,disposition: %d,nb_packets: %d,duration: %d\n",stream_id,st->disposition,queue->nb_packets,queue->duration);
    if (stream_id == is->audio_idx)
    {
        return (stream_id < 0) || queue->abort_request ||
               (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
               (queue->nb_packets > min_audio_frame_num && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0));
    }
    if (stream_id == is->video_idx)
    {
        return (stream_id < 0) || queue->abort_request ||
               (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
               (queue->nb_packets > MIN_VIDEO_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0));
    }
    return 1;
}

static void step_to_next_frame(player_stat_t *is)
{
    /* if the stream is paused unpause it, then step */
    is->step = 1;
    if (is->paused) {
        stream_toggle_pause(is);
        av_log(NULL, AV_LOG_WARNING, "seeking after puase!\n");
    }
}

/* this thread gets the stream from the disk or the network */
static void * demux_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    int ret, extra_packet_count = 0;

    struct timeval now;
    struct timespec outtime;
    pthread_mutex_t wait_mutex;

    //AVPacket *pkt = av_packet_alloc();
    AVPacket packet, *pkt = &packet;

    ret = pthread_mutex_init(&wait_mutex, NULL);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "pthread_mutex_init failed!\n");
        return NULL;
    }

    is->eof = 0;

    // 4. 解复用处理
    while (1)
    {
        if (is->abort_request || is->play_status & AV_PLAY_ERROR)
        {
            printf("demux thread exit\n");
            break;
        }
        //printf("loop start paused: %d,last_paused: %d\n",is->paused,is->last_paused);
        if (is->start_play && (is->paused != is->last_paused)) {
            is->last_paused = is->paused;
            if (is->paused)
            {
                is->read_pause_return = av_read_pause(is->p_fmt_ctx);
            }
            else
            {
                av_read_play(is->p_fmt_ctx);
            }
        }

        if (g_opts.play_mode == AV_LOOP && is->play_status & AV_PLAY_COMPLETE) {
            stream_seek(is, is->p_fmt_ctx->start_time, 0, is->seek_by_bytes);
        }

        if (is->seek_req) {
            int64_t seek_target = is->seek_pos;
            int64_t seek_min    = is->seek_rel > 0 ? seek_target - is->seek_rel + 2: INT64_MIN;
            int64_t seek_max    = is->seek_rel < 0 ? seek_target - is->seek_rel - 2: INT64_MAX;

            // FIXME the +-2 is due to rounding being not done in the correct direction in generation
            // of the seek_pos/seek_rel variables
            //printf("video stream pos : %lld\n", seek_target);
            is->seek_flags |= AVSEEK_FLAG_BACKWARD;
            //ret = av_seek_frame(is->p_fmt_ctx, is->video_idx, seek_target, is->seek_flags);
            ret = avformat_seek_file(is->p_fmt_ctx, -1, seek_min, seek_target, seek_max, is->seek_flags);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "%s: error while seeking\n", is->p_fmt_ctx->url);
                if (is->play_status & AV_PLAY_LOOP && is->play_status & AV_PLAY_COMPLETE) {
                    is->play_status |= AV_PLAY_ERROR;
                }
            } else {
                if (is->audio_idx >= 0) {
                    pthread_mutex_lock(&is->audio_mutex);
                    is->seek_flags |= (1 << 5);
                    packet_queue_flush(&is->audio_pkt_queue);
                    packet_queue_put(&is->audio_pkt_queue, &a_flush_pkt);
                    //frame_queue_flush(&is->audio_frm_queue);
                    //pthread_cond_signal(&is->audio_frm_queue.cond);
                    pthread_mutex_unlock(&is->audio_mutex);
                }

                if (is->video_idx >= 0) {
                    pthread_mutex_lock(&is->video_mutex);
                    is->seek_flags |= (1 << 6);
                    packet_queue_flush(&is->video_pkt_queue);
                    packet_queue_put(&is->video_pkt_queue, &v_flush_pkt);
                    //frame_queue_flush(&is->video_frm_queue);
                    //pthread_cond_signal(&is->video_frm_queue.cond);
                    pthread_mutex_unlock(&is->video_mutex);
                }

                /*if (is->seek_flags & AVSEEK_FLAG_BYTE) {
                   set_clock(&is->extclk, NAN, 0);
                } else {
                   set_clock(&is->extclk, seek_target / (double)AV_TIME_BASE, 0);
                }*/
                is->play_status &= ~(AV_PLAY_COMPLETE);
            }
            is->seek_req = 0;
            is->eof = 0;
            is->audio_complete = (is->audio_idx >= 0) ? 0 : 1;
            is->video_complete = (is->video_idx >= 0) ? 0 : 1;
            if (is->paused)
                step_to_next_frame(is);
        }

        /* if the queue are full, no need to read more */
        if (is->audio_pkt_queue.size + is->video_pkt_queue.size > MAX_QUEUE_SIZE ||
            (stream_has_enough_packets(is->p_audio_stream, is->audio_idx, &is->audio_pkt_queue, is) &&
             stream_has_enough_packets(is->p_video_stream, is->video_idx, &is->video_pkt_queue, is)) )
        {
            /* wait 100 ms */
            pthread_mutex_lock(&wait_mutex);
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec;
            outtime.tv_nsec = now.tv_usec * 1000 + 10 * 1000 * 1000;//timeout 10ms
            pthread_cond_timedwait(&is->continue_read_thread,&wait_mutex,&outtime);
            pthread_mutex_unlock(&wait_mutex);
            //printf("queue size: %d\n",is->audio_pkt_queue.size + is->video_pkt_queue.size);

            if (is->video_idx >= 0 && is->audio_idx >= 0 && is->audio_pkt_queue.nb_packets <= 0) {
                double diff = get_clock(&is->audio_clk) - get_clock(&is->video_clk);
                if (!isnan(diff) && diff > AV_NOSYNC_THRESHOLD) {
                    printf("video lags audio for too long!\n");
                    is->play_status |= AV_NOSYNC;
                }
                //printf("wait video queue avalible pktnb: %d\n",is->video_pkt_queue.nb_packets);
                //printf("wait audio queue avalible pktnb: %d\n",is->audio_pkt_queue.nb_packets);
            }

            if (is->no_pkt_buf) {
                is->no_pkt_buf = 0;
            }
            //av_log(NULL, AV_LOG_WARNING, "packets queue is full!\n");
            continue;
        }

        // 4.1 从输入文件中读取一个packet
        gettimeofday(&read_start, NULL);
        ret = av_read_frame(is->p_fmt_ctx, pkt);
        if (ret < 0)
        {
            if (((ret == AVERROR_EOF) || avio_feof(is->p_fmt_ctx->pb)) && !is->eof)
            {
                // 输入文件已读完，则往packet队列中发送NULL packet，以冲洗(flush)解码器，否则解码器中缓存的帧取不出来
                if (is->video_idx >= 0)
                {
                    packet_queue_put_nullpacket(&is->video_pkt_queue, is->video_idx);
                }

                if (is->audio_idx >= 0)
                {
                    packet_queue_put_nullpacket(&is->audio_pkt_queue, is->audio_idx);
                }

                is->eof = (is->time_out) ? 0 : 1;
                av_log(NULL, AV_LOG_ERROR, "read packet over! ret : %d, feof : %d\n", ret, avio_feof(is->p_fmt_ctx->pb));
            } else {
                // 针对硬解码, 需要多送几张null包, 唤醒解码线程从vdec取出最后几张frame
                if (is->video_idx >= 0 && !is->paused && is->eof && extra_packet_count < 10 && !is->video_complete && is->decoder_type) {
                    //packet_queue_put_nullpacket(&is->video_pkt_queue, is->video_idx);
                    packet_queue_put(&is->video_pkt_queue, &v_extra_pkt);
                    extra_packet_count ++;
                }
            }

            if (is->time_out) {
                av_log(NULL, AV_LOG_ERROR, "av_read_frame time out!\n");
                is->time_out = false;
                is->play_status |= AV_READ_TIMEOUT;
            }

            if (read_start.tv_sec - read_timeout.tv_sec >= PLAYER_READ_TIMEOUT / 2) {
                is->no_pkt_buf = 0;
            }

            pthread_mutex_lock(&wait_mutex);
            gettimeofday(&now, NULL);
            outtime.tv_sec = now.tv_sec;
            outtime.tv_nsec = now.tv_usec * 1000 + 10 * 1000 * 1000;//timeout 10ms
            pthread_cond_timedwait(&is->continue_read_thread,&wait_mutex,&outtime);
            pthread_mutex_unlock(&wait_mutex);

            //av_log(NULL, AV_LOG_ERROR, "av_read_frame failed! ret : %d\n", ret);
            continue;
        }
        else
        {
            is->eof = 0;
            is->the_last_frame = is->keep_frames = false;
            if (!is->no_pkt_buf && !strncmp(is->p_fmt_ctx->iformat->name, "hls", 3)
                && is->audio_pkt_queue.size + is->video_pkt_queue.size < MIN_QUEUE_SIZE) {
                av_log(NULL, AV_LOG_WARNING, "no packets wait for buffer!\n");
                is->no_pkt_buf = 1;
                gettimeofday(&read_timeout, NULL);
            } else {
                if (read_start.tv_sec - read_timeout.tv_sec >= PLAYER_READ_TIMEOUT) {
                    is->no_pkt_buf = 0;
                }
            }
        }

        // 4.3 根据当前packet类型(音频、视频、字幕)，将其存入对应的packet队列
        if (pkt->stream_index == is->audio_idx && !g_opts.video_only)
        {
            packet_queue_put(&is->audio_pkt_queue, pkt);
            //printf("put audio pkt end, size = %d, pts:%lld\n", is->audio_pkt_queue.nb_packets, pkt->pts);
        }
        else if (pkt->stream_index == is->video_idx && !g_opts.audio_only)
        {
            packet_queue_put(&is->video_pkt_queue, pkt);
            //printf("put video pkt end, size = %d, pts:%lld\n", is->video_pkt_queue.nb_packets, pkt->pts);
        }
        else
        {
            av_packet_unref(pkt);
        }
    }
    //av_packet_free(pkt);

    pthread_mutex_destroy(&wait_mutex);
    return NULL;
}

static int demux_init(player_stat_t *is)
{
    AVFormatContext *p_fmt_ctx = NULL;
    int err, i, ret;
    int a_idx, v_idx;
    int st_index[AVMEDIA_TYPE_NB];
    AVH2645HeadInfo *head_info = NULL;

    avformat_network_init();

    memset(st_index, -1, sizeof(st_index));

    p_fmt_ctx = avformat_alloc_context();
    if (!p_fmt_ctx)
    {
        printf("Could not allocate context.\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    is->p_fmt_ctx = p_fmt_ctx;

    // 中断回调机制。为底层I/O层提供一个处理接口，比如中止IO操作。
    p_fmt_ctx->interrupt_callback.callback = decode_interrupt_cb;
    p_fmt_ctx->interrupt_callback.opaque = is;
    // 1. 构建AVFormatContext
    // 1.1 打开视频文件：读取文件头，将文件格式信息存储在"fmt context"中

    av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
    if (g_opts.resolution)
        av_dict_set(&format_opts, "max_resolution", g_opts.resolution, 0);
    else
        av_dict_set(&format_opts, "max_resolution", "2088960", 0);//1920x1088
    //av_dict_set(&format_opts, "max_delay", "10000000", 0);//设置超时10秒
    //av_dict_set(&format_opts, "probesize", "102400", 0);  //探测长度设置为100K
    //p_fmt_ctx->probesize = 100 * 1024;
    //p_fmt_ctx->format_probesize = 100 * 1024;
    //p_fmt_ctx->max_analyze_duration = 7 * AV_TIME_BASE;
    gettimeofday(&read_start, NULL);
    err = avformat_open_input(&p_fmt_ctx, is->filename, NULL, &format_opts);
    av_dict_free(&format_opts);
    if (err < 0)
    {
        if (is->time_out) {
            av_log(NULL, AV_LOG_ERROR, "avformat_open_input time out!\n");
            is->time_out = false;
            is->play_status |= AV_READ_TIMEOUT;
        } else {
            if (err == -101) {
                av_log(NULL, AV_LOG_ERROR, "network can't reachable!\n");
                is->play_status |= AV_NO_NETWORK;
            } else {
                printf("avformat_open_input(%s) failed %d\n",is->filename,err);
                is->play_status |= AV_INVALID_FILE;
            }
        }
        ret = err;
        goto fail;
    }

    // 构建私人结构体保存视频信息
    if (!p_fmt_ctx->opaque) {
        p_fmt_ctx->opaque = (AVH2645HeadInfo *)av_malloc(sizeof(AVH2645HeadInfo));
        if (!p_fmt_ctx->opaque) {
            printf("Could not allocate AVH2645HeadInfo.\n");
            ret = AVERROR(ENOMEM);
            goto fail;
        }
        memset(p_fmt_ctx->opaque, 0x0, sizeof(AVH2645HeadInfo));
    }

    if (genpts)
        p_fmt_ctx->flags |= AVFMT_FLAG_GENPTS;

    av_format_inject_global_side_data(p_fmt_ctx);

    // 1.2 搜索流信息：读取一段视频文件数据，尝试解码，将取到的流信息填入p_fmt_ctx->streams
    //     p_fmt_ctx->streams是一个指针数组，数组大小是p_fmt_ctx->nb_streams
    AVDictionary **opts = setup_find_stream_info_opts(p_fmt_ctx, codec_opts);
    int orig_nb_streams = p_fmt_ctx->nb_streams;

    for (i = 0; i < orig_nb_streams;i ++) {
        if (p_fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            if (g_opts.resolution)
                av_dict_set(opts, "max_resolution", g_opts.resolution, 0);
            else
                av_dict_set(opts, "max_resolution", "2088960", 0);//1920x1088
        }
    }

    gettimeofday(&read_start, NULL);
    err = avformat_find_stream_info(p_fmt_ctx, opts);

    for (i = 0; i < orig_nb_streams; i ++)
        av_dict_free(&opts[i]);
    av_freep(&opts);

    if (err < 0) {
        if (p_fmt_ctx->opaque) {
            av_freep(&p_fmt_ctx->opaque);
        }
        if (is->time_out) {
            av_log(NULL, AV_LOG_ERROR, "avformat_open_input time out!\n");
            is->time_out = false;
            is->play_status |= AV_READ_TIMEOUT;
        }
        printf("avformat_find_stream_info() failed %d\n", err);
        ret = err;
        goto fail;
    }

    if (p_fmt_ctx->pb) {
        p_fmt_ctx->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end
        av_log(NULL, AV_LOG_INFO, "avio buffer size = %d, probesize = %lld\n", p_fmt_ctx->pb->buffer_size, p_fmt_ctx->probesize);
    }

    is->seek_by_bytes = !!(p_fmt_ctx->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", p_fmt_ctx->iformat->name);

    av_log(NULL, AV_LOG_INFO, "avformat demuxer name : %s\n", p_fmt_ctx->iformat->name);
    is->no_pkt_buf = 0;

    if (p_fmt_ctx->opaque) {
        head_info = (AVH2645HeadInfo *)p_fmt_ctx->opaque;
        printf("frame_mbs_only_flag = %d\n", head_info->frame_mbs_only_flag);
        printf("max_bytes_per_pic_denom = %d\n", head_info->max_bytes_per_pic_denom);
        printf("frame_cropping_flag = %d\n", head_info->frame_cropping_flag);
        printf("conformance_window_flag = %d\n", head_info->conformance_window_flag);
        printf("video codec w/h = [%d %d]\n", head_info->coded_width, head_info->coded_height);
    }

    st_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(p_fmt_ctx,
                                   AVMEDIA_TYPE_VIDEO,
                                   st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);

    st_index[AVMEDIA_TYPE_AUDIO] = av_find_best_stream(p_fmt_ctx,
                                   AVMEDIA_TYPE_AUDIO,
                                   st_index[AVMEDIA_TYPE_AUDIO], st_index[AVMEDIA_TYPE_VIDEO], NULL, 0);

    // 2. 查找第一个音频流/视频流
    a_idx = st_index[AVMEDIA_TYPE_AUDIO];
    v_idx = st_index[AVMEDIA_TYPE_VIDEO];
    if (a_idx < 0 && v_idx < 0)
    {
        printf("Cann't find any audio/video stream\n");
        ret = -1;
        goto fail;
    }
    printf("audio idx: %d, video idx: %d\n", a_idx, v_idx);

    if (p_fmt_ctx->duration == AV_NOPTS_VALUE) {
        p_fmt_ctx->duration = 0;
    }
    if (p_fmt_ctx->start_time == AV_NOPTS_VALUE) {
        p_fmt_ctx->start_time = 0;
    }

    double totle_seconds = p_fmt_ctx->duration * av_q2d(AV_TIME_BASE_Q);
    printf("start time : %.3f, total time of input file : %0.3f\n", p_fmt_ctx->start_time * av_q2d(AV_TIME_BASE_Q), totle_seconds);
    av_dump_format(p_fmt_ctx, 0, is->filename, 0);

    //根据设置的参数选择播放声音或视频
    if (g_opts.audio_only) {
        v_idx = -1;
        is->av_sync_type = AV_SYNC_AUDIO_MASTER;
    }

    if (g_opts.video_only) {
        a_idx = -1;
        is->av_sync_type = AV_SYNC_VIDEO_MASTER;
    }

    if (a_idx >= 0) {
        is->p_audio_stream = p_fmt_ctx->streams[a_idx];
        is->audio_complete = 0;
        is->av_sync_type = AV_SYNC_AUDIO_MASTER;
    }

    if (v_idx >= 0) {
        is->p_video_stream = p_fmt_ctx->streams[v_idx];
        is->video_complete = 0;
        if (a_idx < 0) {
            is->av_sync_type = AV_SYNC_VIDEO_MASTER;
        }
    }

    /*reset file pos to the beginning, by raines*/
    ret = avformat_seek_file(p_fmt_ctx, v_idx, INT64_MIN, 0, INT64_MAX, AVSEEK_FLAG_BACKWARD);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "avformat_seek_file failed!\n");
        ret = -1;
        goto fail;
    }
    else
    {
        av_log(NULL, AV_LOG_INFO, "avformat_seek_file success!\n");
    }

    prctl(PR_SET_NAME, "demux_read");
    ret = pthread_create(&is->read_tid, NULL, demux_thread, (void *)is);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "demux_thread create failed!\n");
        ret = -1;
        is->read_tid = 0;
        goto fail;
    }

    is->audio_idx = a_idx;
    is->video_idx = v_idx;

    return 0;

fail:
    if (p_fmt_ctx != NULL)
    {
        if (p_fmt_ctx->opaque)
        {
            av_freep(&p_fmt_ctx->opaque);
        }
        avformat_close_input(&p_fmt_ctx);
    }
    avformat_network_deinit();
    return ret;
}


int demux_deinit()
{
    return 0;
}

int open_demux(player_stat_t *is)
{
    if (demux_init(is) != 0)
    {
        printf("demux_init() failed\n");
        is->demux_status = false;
        return -1;
    }
    is->demux_status = true;

    return 0;
}

