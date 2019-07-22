﻿/*******************************************************************************
 * player.c
 *
 * history:
 *   2018-11-27 - [lei]     Create file: a simplest ffmpeg player
 *   2018-12-01 - [lei]     Playing audio
 *   2018-12-06 - [lei]     Playing audio&vidio
 *   2019-01-06 - [lei]     Add audio resampling, fix bug of unsupported audio 
 *                          format(such as planar)
 *   2019-01-16 - [lei]     Sync video to audio.
 *
 * details:
 *   A simple ffmpeg player.
 *
 * refrence:
 *   ffplay.c in FFmpeg 4.1 project.
 *******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "player.h"
#include "frame.h"
#include "packet.h"
#include "demux.h"
#include "videostream.h"
#include "audiostream.h"

AVPacket flush_pkt;
static int av_sync_type = AV_SYNC_AUDIO_MASTER;
static float seek_interval = 10;

//player_stat_t *player_init(const char *p_input_file);
//int player_deinit(player_stat_t *is);
static int get_master_sync_type(player_stat_t *is) {
    if (is->av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (is->p_video_stream)
            return AV_SYNC_VIDEO_MASTER;
        else
            return AV_SYNC_AUDIO_MASTER;
    } else if (is->av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (is->p_audio_stream)
            return AV_SYNC_AUDIO_MASTER;
        else
            return AV_SYNC_EXTERNAL_CLOCK;
    } else {
        return AV_SYNC_EXTERNAL_CLOCK;
    }
}


/* get the current master clock value */
static double get_master_clock(player_stat_t *is)
{
    double val;

    switch (get_master_sync_type(is)) {
        case AV_SYNC_VIDEO_MASTER:
            val = get_clock(&is->video_clk);
            break;
        case AV_SYNC_AUDIO_MASTER:
            val = get_clock(&is->audio_clk);
            break;
        default:
            val = get_clock(&is->extclk);
            break;
    }
    return val;
}


// 返回值：返回上一帧的pts更新值(上一帧pts+流逝的时间)
double get_clock(play_clock_t *c)
{
    if (*c->queue_serial != c->serial)
    {
        return NAN;
    }
    if (c->paused)
    {
        return c->pts;
    }
    else
    {
        double time = av_gettime_relative() / 1000000.0;
        double ret = c->pts_drift + time;   // 展开得： c->pts + (time - c->last_updated)
        return ret;
    }
}

void set_clock_at(play_clock_t *c, double pts, int serial, double time)
{
    c->pts = pts;
    c->last_updated = time;
    c->pts_drift = c->pts - time;
    c->serial = serial;
}

void set_clock(play_clock_t *c, double pts, int serial)
{
    double time = av_gettime_relative() / 1000000.0;
    set_clock_at(c, pts, serial, time);
}

static void set_clock_speed(play_clock_t *c, double speed)
{
    set_clock(c, get_clock(c), c->serial);
    c->speed = speed;
}

void init_clock(play_clock_t *c, int *queue_serial)
{
    c->speed = 1.0;
    c->paused = 0;
    c->queue_serial = queue_serial;
    set_clock(c, NAN, -1);
}

static void sync_play_clock_to_slave(play_clock_t *c, play_clock_t *slave)
{
    double clock = get_clock(c);
    double slave_clock = get_clock(slave);
    if (!isnan(slave_clock) && (isnan(clock) || fabs(clock - slave_clock) > AV_NOSYNC_THRESHOLD))
        set_clock(c, slave_clock, slave->serial);
}

static void do_exit(player_stat_t *is)
{
    if (is)
    {
        player_deinit(is);
    }
#if 0
    if (is->sdl_video.renderer)
        SDL_DestroyRenderer(is->sdl_video.renderer);
    if (is->sdl_video.window)
        SDL_DestroyWindow(is->sdl_video.window);
#endif
    //avformat_network_deinit();

    //SDL_Quit();

    exit(0);
}

player_stat_t *player_init(const char *p_input_file)
{
    player_stat_t *is;

    is = av_mallocz(sizeof(player_stat_t));
    if (!is)
    {
        return NULL;
    }

    is->filename = av_strdup(p_input_file);
    if (is->filename == NULL)
    {
        goto fail;
    }

    /* start video display */
    if (frame_queue_init(&is->video_frm_queue, &is->video_pkt_queue, VIDEO_PICTURE_QUEUE_SIZE, 1) < 0 ||
        frame_queue_init(&is->audio_frm_queue, &is->audio_pkt_queue, SAMPLE_QUEUE_SIZE, 1) < 0)
    {
        goto fail;
    }

    if (packet_queue_init(&is->video_pkt_queue) < 0 ||
        packet_queue_init(&is->audio_pkt_queue) < 0)
    {
        goto fail;
    }

    av_init_packet(&flush_pkt);
    flush_pkt.data = (uint8_t *)&flush_pkt;
    
    
    packet_queue_put(&is->video_pkt_queue, &flush_pkt);
    packet_queue_put(&is->audio_pkt_queue, &flush_pkt);

    CheckFuncResult(pthread_cond_init(&is->continue_read_thread,NULL));

    init_clock(&is->video_clk, &is->video_pkt_queue.serial);
    init_clock(&is->audio_clk, &is->audio_pkt_queue.serial);

    is->abort_request = 0;
    is->p_frm_yuv = av_frame_alloc();
    if (is->p_frm_yuv == NULL)
    {
        printf("av_frame_alloc() for p_frm_raw failed\n");
        return -1;
    }
    is->av_sync_type = av_sync_type;
    return is;
fail:
    player_deinit(is);

    return is;
}

static void audio_decoder_abort(player_stat_t *is)
{
    packet_queue_abort(&is->audio_pkt_queue);
    frame_queue_signal(&is->audio_frm_queue);
    //SDL_WaitThread(d->decoder_tid, NULL);
    packet_queue_flush(&is->audio_pkt_queue);

    avcodec_free_context(&is->p_acodec_ctx);
}

static void video_decoder_abort(player_stat_t *is)
{
    packet_queue_abort(&is->video_pkt_queue);
    frame_queue_signal(&is->video_frm_queue);
    //SDL_WaitThread(d->decoder_tid, NULL);
    packet_queue_flush(&is->video_pkt_queue);

    avcodec_free_context(&is->p_vcodec_ctx);
}

static void stream_component_close(player_stat_t *is, int stream_index)
{
    AVFormatContext *ic = is->p_fmt_ctx;
    AVCodecParameters *codecpar;
    printf("stream_index: %d,nb_streams: %d\n",stream_index,ic->nb_streams);
    if (stream_index < 0 || stream_index >= ic->nb_streams)
        return;
    codecpar = ic->streams[stream_index]->codecpar;

    switch (codecpar->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        audio_decoder_abort(is);
        swr_free(&is->audio_swr_ctx);
        av_freep(&is->p_audio_frm);
        is->audio_frm_size = 0;
        is->audio_frm_rwr_size = 0;
        is->audio_frm_rwr = NULL;
        break;
    case AVMEDIA_TYPE_VIDEO:
        video_decoder_abort(is);
        break;

    default:
        break;
    }

    ic->streams[stream_index]->discard = AVDISCARD_ALL;
    switch (codecpar->codec_type) {
    case AVMEDIA_TYPE_AUDIO:
        is->p_audio_stream = NULL;
        is->audio_idx = -1;
        break;
    case AVMEDIA_TYPE_VIDEO:
        is->p_video_stream = NULL;
        is->video_idx = -1;
        break;
    default:
        break;
    }
}

int player_deinit(player_stat_t *is)
{
    printf("player_deinit start\n");
    
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    is->abort_request = 1;
    pthread_join(is->read_tid, NULL);

    /* close each stream */
    if (is->audio_idx >= 0)
    {
        stream_component_close(is, is->audio_idx);
        printf("audio stream_component_close finish\n");
    }
    if (is->video_idx >= 0)
    {
        stream_component_close(is, is->video_idx);
        printf("video stream_component_close finish\n");
    }
    pthread_join(is->audioDecode_tid, NULL);
    pthread_join(is->videoDecode_tid, NULL);
    pthread_join(is->audioPlay_tid, NULL);
    pthread_join(is->videoPlay_tid, NULL);
    printf("pthread_join finish 11\n");
    av_frame_free(&is->p_frm_yuv);
    
    printf("pthread_join finish\n");
    avformat_close_input(&is->p_fmt_ctx);
    printf("avformat_close_input finish\n");

    packet_queue_destroy(&is->video_pkt_queue);
    printf("video packet_queue_destroy finish\n");
    
    packet_queue_destroy(&is->audio_pkt_queue);
    printf("audio packet_queue_destroy finish\n");
    
    /* free all pictures */
    frame_queue_destory(&is->video_frm_queue);
    printf("video frame_queue_destory finish\n");
    frame_queue_destory(&is->audio_frm_queue);
    printf("audio frame_queue_destory finish\n");

    pthread_cond_destroy(&is->continue_read_thread);
    sws_freeContext(is->img_convert_ctx);
    av_free(is->filename);
    av_freep(&is);

    return 0;
}

/* pause or resume the video */
void stream_toggle_pause(player_stat_t *is)
{
    if (is->paused)
    {
        // 这里表示当前是暂停状态，将切换到继续播放状态。在继续播放之前，先将暂停期间流逝的时间加到frame_timer中
        is->frame_timer += av_gettime_relative() / 1000000.0 - is->video_clk.last_updated;
        if (is->read_pause_return != AVERROR(ENOSYS)) {
            is->video_clk.paused = 0;
        }
        set_clock(&is->video_clk, get_clock(&is->video_clk), is->video_clk.serial);
    }
    is->paused = is->audio_clk.paused = is->video_clk.paused = !is->paused;
}

void toggle_pause(player_stat_t *is)
{
    stream_toggle_pause(is);
    is->step = 0;
}

static void stream_seek(player_stat_t *is, int64_t pos, int64_t rel, int seek_by_bytes)
{
    if (!is->seek_req) {
        is->seek_pos = pos;
        is->seek_rel = rel;
        is->seek_flags &= ~AVSEEK_FLAG_BYTE;
        if (seek_by_bytes)
            is->seek_flags |= AVSEEK_FLAG_BYTE;
        is->seek_req = 1;
        pthread_cond_signal(&is->continue_read_thread);
    }
}



int player_running(const char *p_input_file)
{
    player_stat_t *is = NULL;
    char cmd = 0;
    double incr,pos, frac;

    is = player_init(p_input_file);
    if (is == NULL)
    {
        printf("player init failed\n");
        do_exit(is);
    }

    open_demux(is);
    open_video(is);
    open_audio(is);

    //SDL_Event event;

    while (1)
    {
        cmd = getchar();
        switch (cmd){
        case 'q':
            do_exit(is);
            break;
        case 'p':
            toggle_pause(is);
            break;
        case 'b':
            incr = seek_interval ? -seek_interval : -10.0;
            goto do_seek;
        case 'f':
            incr = seek_interval ? seek_interval : 10.0;

do_seek:

        pos = get_master_clock(is);
        if (isnan(pos))
            pos = (double)is->seek_pos / AV_TIME_BASE;
        pos += incr;
        if (is->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos < is->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
            pos = is->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
        stream_seek(is, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
        
            break;
        default:
            break;
        }

    }

    return 0;
}
