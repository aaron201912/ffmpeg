/*******************************************************************************
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
#include "video.h"
#include "audio.h"

AVPacket flush_pkt;

static player_stat_t *player_init(const char *p_input_file);
static int player_deinit(player_stat_t *is);

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

static MI_S32 ss_sys_Init(void)
{
    MI_SYS_Version_t stVersion;
    MI_U64 u64Pts = 0;

    CheckFuncResult(MI_SYS_Init());

    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));

    CheckFuncResult(MI_SYS_GetVersion(&stVersion));

    CheckFuncResult(MI_SYS_GetCurPts(&u64Pts));

    u64Pts = 0xF1237890F1237890;
    CheckFuncResult(MI_SYS_InitPtsBase(u64Pts));

    u64Pts = 0xE1237890E1237890;
    CheckFuncResult(MI_SYS_SyncPts(u64Pts));

    return SUCCESS;
}


static player_stat_t *player_init(const char *p_input_file)
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
    //init sdk sys
    CheckFuncResult(ss_sys_Init());

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
        //ss_disp_Deinit(is->p_vcodec_ctx);
        break;
    case AVMEDIA_TYPE_VIDEO:
        video_decoder_abort(is);
        
        //ss_ao_Deinit();
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
static int ss_module_Deinit(player_stat_t *is)
{
    ss_disp_Deinit(is->p_vcodec_ctx);
    ss_ao_Deinit();

    //deinit ss sys
    CheckFuncResult(MI_SYS_Exit());
    return SUCCESS;
}

static int player_deinit(player_stat_t *is)
{
    printf("player_deinit start\n");
    //CheckFuncResult(ss_module_Deinit(is));
    
    /* XXX: use a special url_shutdown call to abort parse cleanly */
    is->abort_request = 1;
    pthread_join(is->read_tid, NULL);

    
    CheckFuncResult(ss_module_Deinit(is));
    
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
    printf("1123\n");
    pthread_cond_destroy(&is->continue_read_thread);
    printf("1132\n");
    printf("11\n");
    sws_freeContext(is->img_convert_ctx);
    printf("22\n");
    av_free(is->filename);
    printf("33\n");
    
    av_freep(&is);
    printf("44\n");

    return 0;
}

/* pause or resume the video */
static void stream_toggle_pause(player_stat_t *is)
{
    if (is->paused)
    {
        // 这里表示当前是暂停状态，将切换到继续播放状态。在继续播放之前，先将暂停期间流逝的时间加到frame_timer中
        is->frame_timer += av_gettime_relative() / 1000000.0 - is->video_clk.last_updated;
        set_clock(&is->video_clk, get_clock(&is->video_clk), is->video_clk.serial);
    }
    is->paused = is->audio_clk.paused = is->video_clk.paused = !is->paused;
}

static void toggle_pause(player_stat_t *is)
{
    stream_toggle_pause(is);
    is->step = 0;
}


int player_running(const char *p_input_file)
{
    player_stat_t *is = NULL;
    char cmd = 0;

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
        default:
            break;
        }

    }

    return 0;
}
