#include "player.h"
#include "packet.h"
#include "frame.h"
#include "audio.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <poll.h>
#include <poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

extern AVPacket a_flush_pkt;

// 从packet_queue中取一个packet，解码生成frame
static int audio_decode_frame(AVCodecContext *p_codec_ctx, packet_queue_t *p_pkt_queue, AVFrame *frame)
{
    int ret;

    while (1)
    {
        AVPacket pkt;

        while (1)
        {
            if (p_pkt_queue->abort_request) {
                return -1;
            }

            // 3.2 一个音频packet含一至多个音频frame，每次avcodec_receive_frame()返回一个frame，此函数返回。
            // 下次进来此函数，继续获取一个frame，直到avcodec_receive_frame()返回AVERROR(EAGAIN)，
            // 表示解码器需要填入新的音频packet
            ret = avcodec_receive_frame(p_codec_ctx, frame);
            if (ret >= 0)
            {
                // 时基转换，从d->avctx->pkt_timebase时基转换到1/frame->sample_rate时基
                AVRational tb = (AVRational) { 1, frame->sample_rate};
                if (frame->pts != AV_NOPTS_VALUE)
                {
                    //printf("frame pts before convert : %lld.\n", frame->pts);
                    frame->pts = av_rescale_q(frame->pts, p_codec_ctx->pkt_timebase, tb);
                }
                else
                {
                    //av_log(NULL, AV_LOG_WARNING, "frame->pts no\n");
                }
                //printf("audio convert before pts : %lld\n", frame->pts);
                return 1;
            }
            else if (ret == AVERROR_EOF)
            {
                av_log(NULL, AV_LOG_INFO, "audio avcodec_receive_frame(): the decoder has been flushed\n");
                avcodec_flush_buffers(p_codec_ctx);
                return 0;
            }
            else if (ret == AVERROR(EAGAIN))
            {
                //av_log(NULL, AV_LOG_INFO, "audio avcodec_receive_frame(): input is not accepted in the current state\n");
                break;
            }
            else
            {
                if (!(g_mmplayer->play_status & AV_ACODEC_ERROR))
                {
                    av_log(NULL, AV_LOG_ERROR, "audio avcodec_receive_frame(): other errors\n");
                    g_mmplayer->play_status |= AV_ACODEC_ERROR;
                }
                av_usleep(10 * 1000);
                return -1;
            }
        }
        // 1. 取出一个packet。使用pkt对应的serial赋值给d->pkt_serial
        if (packet_queue_get(p_pkt_queue, &pkt, true) < 0)
        {
            printf("audio packet_queue_get fail\n");
            return -1;
        }
        // packet_queue中第一个总是flush_pkt。每次seek操作会插入flush_pkt，更新serial，开启新的播放序列
        if (pkt.data == a_flush_pkt.data)
        {
            pthread_mutex_lock(&g_mmplayer->audio_mutex);
            if ((g_mmplayer->seek_flags & (1 << 5)) && p_codec_ctx->frame_number > 1) {
                g_mmplayer->seek_flags &= ~(1 << 5);
                //if (g_mmplayer->functions.audio_clear_buf)
                //    g_mmplayer->functions.audio_clear_buf();
            }
            pthread_mutex_unlock(&g_mmplayer->audio_mutex);

            // 复位解码器内部状态/刷新内部缓冲区。当seek操作或切换流时应调用此函数。
            avcodec_flush_buffers(p_codec_ctx);

            printf("avcodec_flush_buffers for audio!\n");
        }
        else
        {
            // 2. 将packet发送给解码器
            //    发送packet的顺序是按dts递增的顺序，如IPBBPBB
            //    pkt.pos变量可以标识当前packet在视频文件中的地址偏移
            //printf("audio avcodec_send_packet! data:%p, size:%d, pos:%lld\n", pkt.data, pkt.size, pkt.pos);
            ret = avcodec_send_packet(p_codec_ctx, &pkt);
            if (ret == AVERROR(EAGAIN))
            {
                av_log(NULL, AV_LOG_ERROR, "receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
            }
            else if (ret < 0)
            {
                if (!(g_mmplayer->play_status != AV_ACODEC_ERROR))
                {
                    av_log(NULL, AV_LOG_ERROR, "audio decoding occur error!\n");
                    g_mmplayer->play_status |= AV_ACODEC_ERROR;
                }
            }
        }
        av_packet_unref(&pkt);
    }
}

// 音频解码线程：从音频packet_queue中取数据，解码后放入音频frame_queue
static void * audio_decode_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    AVFrame *p_frame = av_frame_alloc();
    frame_t *af;

    int got_frame = 0;
    AVRational tb;

    if (p_frame == NULL)
    {
        return NULL;
    }

    is->audio_frame_duration = 0;

    while (1)
    {
        if(is->abort_request) {
            printf("audio decode thread exit\n");
            break;
        }

        /*wait video flush done*/
        if(!is->start_play)
        {
            av_usleep(10*1000);
            continue;
        }

        got_frame = audio_decode_frame(is->p_acodec_ctx, &is->audio_pkt_queue, p_frame);
        if (got_frame < 0)
        {
            printf("audio get frame error!\n");
            goto the_end;
        }
        else if (got_frame > 0)
        {
            tb = (AVRational) {1, p_frame->sample_rate};
            //if (!(af = frame_queue_peek_writable(&is->audio_frm_queue)))
            //   goto the_end;
            frame_queue_t *f = &is->audio_frm_queue;

            af = &f->queue[f->windex];

            af->pts = (p_frame->pts == AV_NOPTS_VALUE) ? NAN : p_frame->pts * av_q2d(tb);
            af->pos = p_frame->pkt_pos;
            //printf("audio convert later pts : %.3f\n", af->pts);
            //af->serial = is->auddec.pkt_serial;
            // 当前帧包含的(单个声道)采样数/采样率就是当前帧的播放时长
            af->duration = av_q2d((AVRational) { p_frame->nb_samples, p_frame->sample_rate });
            if(is->audio_frame_duration == 0)
                is->audio_frame_duration = af->duration;

            //printf("audio frame pts : %lld, time pts : %f, audio duration : %f.\n", p_frame->pts, af->pts, af->duration);
            // 将frame数据拷入af->frame，af->frame指向音频frame队列尾部
            av_frame_move_ref(af->frame, p_frame);
            // 更新音频frame队列大小及写指针
            frame_queue_push(&is->audio_frm_queue);
            av_frame_unref(p_frame);

            pthread_mutex_lock(&f->mutex);
            while (f->size >= f->max_size && !f->pktq->abort_request) {
                pthread_cond_wait(&f->cond, &f->mutex);
                if (is->seek_flags & (1 << 5)) {
                    break;
                }
            }
            pthread_mutex_unlock(&f->mutex);

            if (f->pktq->abort_request) {
                continue;
            }
        }
    }

the_end:
    av_frame_free(&p_frame);
    return NULL;
}

int open_audio_stream(player_stat_t *is)
{
    AVCodecContext *p_codec_ctx;
    AVCodecParameters *p_codec_par = NULL;
    AVCodec* p_codec = NULL;
    int ret;

    // 1. 为音频流构建解码器AVCodecContext

    // 1.1 获取解码器参数AVCodecParameters
    p_codec_par = is->p_audio_stream->codecpar;
    // 1.2 获取解码器
    p_codec = avcodec_find_decoder(p_codec_par->codec_id);
    if (p_codec == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "Cann't find audio codec!\n");
        return -1;
    }
    printf("open audio codec: %s\n",p_codec->name);

    // 1.3 构建解码器AVCodecContext
    // 1.3.1 p_codec_ctx初始化：分配结构体，使用p_codec初始化相应成员为默认值
    p_codec_ctx = avcodec_alloc_context3(p_codec);
    if (p_codec_ctx == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_alloc_context3() failed\n");
        return -1;
    }
    // 1.3.2 p_codec_ctx初始化：p_codec_par ==> p_codec_ctx，初始化相应成员
    ret = avcodec_parameters_to_context(p_codec_ctx, p_codec_par);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_to_context() failed %d\n", ret);
        return -1;
    }
    // 1.3.3 p_codec_ctx初始化：使用p_codec初始化p_codec_ctx，初始化完成
    ret = avcodec_open2(p_codec_ctx, p_codec, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_open2() failed %d\n", ret);
        return -1;
    }

    p_codec_ctx->pkt_timebase = is->p_audio_stream->time_base;
    is->p_acodec_ctx = p_codec_ctx;

    // 2. 创建视频解码线程
    prctl(PR_SET_NAME, "audio_decode");
    ret = pthread_create(&is->audio_decode_tid, NULL, audio_decode_thread, (void *)is);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "audio_decode_thread create failed!\n");
        is->audio_decode_tid = 0;
        return -1;
    }

    return 0;
}

static int audio_resample(player_stat_t *is, int64_t audio_callback_time)
{
    int data_size, resampled_data_size;
    int64_t dec_channel_layout;
    av_unused double audio_clock0;
    int wanted_nb_samples;
    frame_t *af = NULL;
    frame_queue_t *f = &is->audio_frm_queue;
    struct timeval now;
    struct timespec outtime;

replay:
    if (is->paused || is->step) {
        av_usleep(10 * 1000);
        return AV_PLAY_PAUSE;
    }

    if (is->no_pkt_buf || is->play_status & AV_PLAY_ERROR) {
        av_usleep(10 * 1000);
        return -1;
    }

    // 若队列头部可读，则由af指向可读帧
    //if (!(af = frame_queue_peek_readable(&is->audio_frm_queue)))
    //    return -1;
    pthread_mutex_lock(&f->mutex);
    while (f->size - f->rindex_shown <= 0 && !f->pktq->abort_request)
    {
        if (!is->audio_complete && is->eof && !is->audio_pkt_queue.nb_packets)
        {
            is->audio_complete = 1;
            if (is->video_complete && is->audio_complete) {
                is->play_status |= AV_PLAY_COMPLETE;
            } else {
                is->play_status |= AV_AUDIO_COMPLETE;
            }
            av_log(NULL, AV_LOG_INFO, "audio play completely!\n");
        }
        gettimeofday(&now, NULL);
        outtime.tv_sec  = now.tv_sec + 1;
        outtime.tv_nsec = now.tv_usec * 1000;
        pthread_cond_timedwait(&f->cond, &f->mutex, &outtime);
    }
    pthread_mutex_unlock(&f->mutex);

    if (f->size - f->rindex_shown <= 0 || f->pktq->abort_request || is->step)
        return -1;

    af = &f->queue[(f->rindex + f->rindex_shown) % f->max_size];
    frame_queue_next(&is->audio_frm_queue);
    if (!af->frame->channel_layout && !af->frame->sample_rate && af->frame->format == -1)
    {
        printf("invalid audio frame layout sample_rate and format!\n");
        return -1;
    }

    enum AVSampleFormat sample_fmt = (enum AVSampleFormat)(af->frame->format);

    // 根据frame中指定的音频参数获取缓冲区的大小
    data_size = av_samples_get_buffer_size(NULL, af->frame->channels,   // 本行两参数：linesize，声道数
        af->frame->nb_samples,       // 本行一参数：本帧中包含的单个声道中的样本数
        sample_fmt, 1);       // 本行两参数：采样格式，不对齐

    // 获取声道布局
    dec_channel_layout =
        (af->frame->channel_layout && af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
        af->frame->channel_layout : av_get_default_channel_layout(af->frame->channels);
    wanted_nb_samples = af->frame->nb_samples;
    // is->audio_param_tgt是SDL可接受的音频帧数，是audio_open()中取得的参数
    // 在audio_open()函数中又有“is->audio_src = is->audio_param_tgt”
    // 此处表示：如果frame中的音频参数 == is->audio_src == is->audio_param_tgt，那音频重采样的过程就免了(因此时is->swr_ctr是NULL)
    // 　　　　　否则使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->swr_ctx，并使用frame中的音频参数来赋值is->audio_src
    if (sample_fmt != is->audio_param_src.fmt ||
        dec_channel_layout != is->audio_param_src.channel_layout ||
        af->frame->sample_rate != is->audio_param_src.freq)
    {
        swr_free(&is->audio_swr_ctx);
        //printf("in layout: %lld,format: %d,samrate: %d\n",dec_channel_layout,af->frame->format,af->frame->sample_rate);
        //printf("out layout: %lld,format: %d,samrate: %d\n",is->audio_param_tgt.channel_layout,is->audio_param_tgt.fmt,is->audio_param_tgt.freq);

        // 使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->audio_swr_ctx
        is->audio_swr_ctx = swr_alloc_set_opts(NULL,
            is->audio_param_tgt.channel_layout, is->audio_param_tgt.fmt, is->audio_param_tgt.freq,
            dec_channel_layout, sample_fmt, af->frame->sample_rate,
            0, NULL);
        if (!is->audio_swr_ctx || swr_init(is->audio_swr_ctx) < 0)
        {
            av_log(NULL, AV_LOG_ERROR,
                "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                af->frame->sample_rate, av_get_sample_fmt_name(sample_fmt), af->frame->channels,
                is->audio_param_tgt.freq, av_get_sample_fmt_name(is->audio_param_tgt.fmt), is->audio_param_tgt.channels);
            swr_free(&is->audio_swr_ctx);
            return -1;
        }
        // 使用frame中的参数更新is->audio_src，第一次更新后后面基本不用执行此if分支了，因为一个音频流中各frame通用参数一样
        is->audio_param_src.channel_layout = dec_channel_layout;
        is->audio_param_src.channels = af->frame->channels;
        is->audio_param_src.freq = af->frame->sample_rate;
        is->audio_param_src.fmt = sample_fmt;
    }

    if (is->audio_swr_ctx)
    {
        // 重采样输入参数1：输入音频样本数是af->frame->nb_samples
        // 重采样输入参数2：输入音频缓冲区
        const uint8_t **in = (const uint8_t **)af->frame->extended_data;
        // 重采样输出参数1：输出音频缓冲区尺寸
        // 重采样输出参数2：输出音频缓冲区
        uint8_t **out = &is->audio_frm_rwr;
        // 重采样输出参数：输出音频样本数(多加了256个样本)
        int out_count = (int64_t)wanted_nb_samples * is->audio_param_tgt.freq / af->frame->sample_rate + 256;
        // 重采样输出参数：输出音频缓冲区尺寸(以字节为单位)
        int out_size = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, out_count, is->audio_param_tgt.fmt, 0);
        int len2;
        if (out_size < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
            return -1;
        }

        av_fast_malloc(&is->audio_frm_rwr, &is->audio_frm_rwr_size, out_size);
        if (!is->audio_frm_rwr)
        {
            return AVERROR(ENOMEM);
        }
        //printf("tgt count: %d,channel: %d,size: %d\n",out_count,is->audio_param_tgt.channels,is->audio_frm_rwr_size);
        //printf("in count: %d,channel: %d\n",af->frame->nb_samples,af->frame->channels);

        // 音频重采样：返回值是重采样后得到的音频数据中单个声道的样本数
        len2 = swr_convert(is->audio_swr_ctx, out, out_count, in, af->frame->nb_samples);
        if (len2 < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
            goto fail;
        }

        if (len2 == out_count)
        {
            av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
            if (swr_init(is->audio_swr_ctx) < 0)
                swr_free(&is->audio_swr_ctx);
            goto fail;
        }

        is->p_audio_frm = is->audio_frm_rwr;

        //printf("len2 : %d, out_count : %d, out_size : %d, nb_sample : %d\n", len2, out_count, out_size, wanted_nb_samples);
        // 重采样返回的一帧音频数据大小(以字节为单位)
        resampled_data_size = len2 * is->audio_param_tgt.channels * av_get_bytes_per_sample(is->audio_param_tgt.fmt);

        //printf("resampled_data_size : %d, channels : %d\n", resampled_data_size, is->audio_param_tgt.channels);
    }
    else
    {
        // 未经重采样，则将指针指向frame中的音频数据
        is->p_audio_frm = af->frame->data[0];
        resampled_data_size = data_size;
    }
    //printf("before audio clock: %lf,pts: %lf,nb: %d,samrate: %d\n",is->audio_clock,af->pts,af->frame->nb_samples,af->frame->sample_rate);
    /* update the audio clock with the pts */
    if (!isnan(af->pts))
    {
        is->audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate;
    }
    else
    {
        is->audio_clock = NAN;
    }
    //printf("after pts: %lf,clock: %lf\n",af->pts,is->audio_clock);
    is->audio_clock_serial = af->serial;
    return resampled_data_size;
fail:
    av_freep(&is->audio_frm_rwr);
    return -1;
}

static void * audio_playing_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    int audio_size;
    int pause = 0;
    int last_pause =0;
    bool audio_has_data = false;

    while(1)
    {
        if(is->abort_request)
        {
            break;
        }
        int64_t audio_callback_time = av_gettime_relative();

        audio_size = audio_resample(is, audio_callback_time);
        if (audio_size < 0 || !is->start_play)
        {
            continue;
        }
        else
        {
            if (audio_size == AV_PLAY_PAUSE)
            {
                if (!audio_has_data)
                    continue;

                pause = 1;
                if(pause != last_pause)
                {
                    last_pause = pause;
                    if (is->functions.audio_pause)
                        is->functions.audio_pause();
                }
                is->p_audio_frm = NULL;
                is->audio_frm_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_param_tgt.frame_size * is->audio_param_tgt.frame_size;
            }
            else
            {
                pause = 0;
                if(pause != last_pause)
                {
                    last_pause = pause;
                    if (is->functions.audio_resume)
                        is->functions.audio_resume();
                }
                is->audio_frm_size = audio_size;
            }

            if (is->p_audio_frm != NULL)
            {
                //printf("save audio len: %d\n",is->audio_frm_size);
                if (is->functions.audio_play)
                {
                    is->audio_write_buf_size = is->functions.audio_play(is, (void *)is->p_audio_frm, is->audio_frm_size);
                    audio_has_data = true;
                }
                //printf("audio frame pts: %f\n", get_clock(&is->audio_clk));
            }
            //is->audio_write_buf_size是本帧中尚未拷入SDL音频缓冲区的数据量
            //is->audio_write_buf_size = is->audio_frm_size - is->audio_cp_index;
            /* Let's assume the audio driver that is used by SDL has two periods. */
            // 3. 更新时钟
            if (!isnan(is->audio_clock))
            {
                audio_callback_time = av_gettime_relative();
                // 更新音频时钟，更新时刻：每次往声卡缓冲区拷入数据后
                // 前面audio_decode_frame中更新的is->audio_clock是以音频帧为单位，所以此处第二个参数要减去未拷贝数据量占用的时间
                set_clock_at(&is->audio_clk,
                    is->audio_clock - (double)(is->audio_write_buf_size) / is->audio_param_tgt.bytes_per_sec,
                    is->audio_clock_serial,
                    audio_callback_time / 1000000.0);
                //printf("audio clk: %lf,curtime: %ld,audio_callback_time: %ld\n",is->audio_clock,is->audio_clock_serial,audio_callback_time);
                //printf("update clk pts: %lf,lud: %lf,dif: %lf\n",is->audio_clk.pts,is->audio_clk.last_updated,is->audio_clk.pts_drift);

                is->audio_write_buf_size = 0;
            }
        }
    }

    av_log(NULL, AV_LOG_INFO, "audio play thread exit\n");
    return NULL;
}

static int open_audio_playing(void *arg)
{
    int ret, audio_chlayout = AV_CH_LAYOUT_MONO;
    player_stat_t *is = (player_stat_t *)arg;

    // 2.2 根据SsPlayer音频参数构建音频重采样参数
    // wanted_spec是期望的参数，actual_spec是实际的参数，wanted_spec和auctual_spec都是SDL中的参数。
    // 此处audio_param是FFmpeg中的参数，此参数应保证是SDL播放支持的参数，后面重采样要用到此参数
    // 音频帧解码后得到的frame中的音频格式未必被SDL支持，比如frame可能是planar格式，但SDL2.0并不支持planar格式，
    // 若将解码后的frame直接送入SDL音频缓冲区，声音将无法正常播放。所以需要先将frame重采样(转换格式)为SDL支持的模式，
    // 然后送再写入SDL音频缓冲区
    if (is->functions.audio_init)
    {
        audio_chlayout = is->functions.audio_init(is);
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR, "mm_audio_init isn't set\n");
        return -1;
    }
    is->enable_audio=  true;

    is->audio_param_tgt.fmt  = AV_SAMPLE_FMT_S16;
    is->audio_param_tgt.freq = 48000;
    //is->audio_param_tgt.channel_layout = AV_CH_LAYOUT_MONO;
    is->audio_param_tgt.channel_layout = audio_chlayout;

    is->audio_param_tgt.channels = av_get_channel_layout_nb_channels(is->audio_param_tgt.channel_layout);

    is->audio_param_tgt.frame_size = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, 1, is->audio_param_tgt.fmt, 1);

    is->audio_param_tgt.bytes_per_sec = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, is->audio_param_tgt.freq, is->audio_param_tgt.fmt, 1);

    if (is->audio_param_tgt.bytes_per_sec <= 0 || is->audio_param_tgt.frame_size <= 0)
    {
        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
        return -1;
    }
    is->audio_param_src = is->audio_param_tgt;
    is->audio_hw_buf_size = 64 * 1024;//is->audio_param_tgt.frame_size;   // SDL音频缓冲区大小
    is->audio_frm_size = 0;
    is->audio_cp_index = 0;

    prctl(PR_SET_NAME, "audio_play");
    ret = pthread_create(&is->audio_play_tid, NULL, audio_playing_thread, (void *)is);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "audio_playing_thread create failed!\n");
        is->audio_play_tid = 0;
        return -1;
    }

    return 0;
}

int open_audio(player_stat_t *is)
{
    int ret;

    if (is && is->audio_idx >= 0)
    {
        ret = open_audio_stream(is);
        if (ret < 0)
            return ret;

        ret = open_audio_playing(is);
        if (ret < 0)
            return ret;
    }

    return 0;
}

