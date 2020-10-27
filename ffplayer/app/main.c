#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "sd20xpanel.h"

#include "player.h"
#include "audio.h"
#include "video.h"
#include "demux.h"

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

/*******************************************************************************************/
static pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
static player_stat_t *ssplayer = NULL;
static player_opts_t g_opts;
static bool g_mute = false;
static int g_rotate = E_MI_DISP_ROTATE_NONE;

int sstar_player_open(const char *fp, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
int sstar_player_close(void);

static void * sstar_layer_handler(void * arg)
{
    int ret;
    player_stat_t * mplayer = (player_stat_t *)arg;

    ret = open_demux(mplayer);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "open_demux failed!\n");
        return NULL;
    }
    av_log(NULL, AV_LOG_INFO, "open_demux successful\n");

    ret = open_video(mplayer);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "open_video failed!\n");
        return NULL;
    }
    av_log(NULL, AV_LOG_INFO, "open_video successful\n");

    ret = open_audio(mplayer);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "open_audio failed!\n");
        return NULL;
    }
    av_log(NULL, AV_LOG_INFO, "open_audio successful\n");

    return mplayer;
}

int sstar_player_open(const char *fp, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (ssplayer || width <= 0 || height <= 0) {
        av_log(NULL, AV_LOG_ERROR, "input parameter check error!\n");
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "enter sstar_player_open\n");

    pthread_mutex_init(&player_mutex, NULL);

    ssplayer = player_init(fp);
    if (ssplayer == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "player init failed[%s]\n",fp);
        return -1;
    }

    gettimeofday(&ssplayer->tim_open, NULL);

    memcpy(&ssplayer->opts, &g_opts, sizeof(player_opts_t));
    //ssplayer->display_mode = g_rotate;
    ssplayer->display_mode = g_opts.rotate_attr;
    ssplayer->in_height = height;
    ssplayer->in_width  = width;
    ssplayer->pos_x     = x;
    ssplayer->pos_y     = y;

    av_log(NULL, AV_LOG_INFO, "set out width/height = [%d %d]\n", ssplayer->in_width, ssplayer->in_height);

    if(sstar_layer_handler(ssplayer) == NULL)
    {
        int ret = (ssplayer->play_status < 0) ? ssplayer->play_status : -1;
        sstar_player_close();
        return ret;
    }
    av_log(NULL, AV_LOG_INFO, "leave sstar_player_open\n");

    return 0;
}

int sstar_player_close(void)
{
    if(ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_close failed\n");
        return -1;
    }

    int ret;

    av_log(NULL, AV_LOG_INFO, "enter sstar_player_close\n");

    pthread_mutex_lock(&player_mutex);

    if (ssplayer->enable_video) {
        ret = sstar_display_unset(ssplayer);
        av_log(NULL, AV_LOG_INFO, "sstar_display_unset done!\n"); 
    }

    ret = player_deinit(ssplayer);

    if (ssplayer->enable_audio) {
        ret = sstar_audio_deinit();
        av_log(NULL, AV_LOG_INFO, "sstar_audio_deinit done!\n");
    }

    ssplayer = NULL;
    g_mute = false;
    memset(&g_opts, 0, sizeof(player_opts_t));

    pthread_mutex_unlock(&player_mutex);
    pthread_mutex_destroy(&player_mutex);

    av_log(NULL, AV_LOG_INFO, "leave sstar_player_close\n");

    return ret;
}

int sstar_player_pause(void)
{
    pthread_mutex_lock(&player_mutex);

    if (ssplayer == NULL || ssplayer->paused) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_pause failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "sstar_player_pause!\n");
    toggle_pause(ssplayer);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_resume(void)
{
    pthread_mutex_lock(&player_mutex);

    if(ssplayer == NULL || !ssplayer->paused) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_resume failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "sstar_player_resume!\n");
    toggle_pause(ssplayer);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_getposition(double *position)
{
    pthread_mutex_lock(&player_mutex);

    if(ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_getposition failed\n");
        *position = NAN;
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    *position = get_master_clock(ssplayer);

    if (isnan(*position)) {
        av_log(NULL, AV_LOG_WARNING, "get invalid position time\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    } else {
        double start_time = ssplayer->p_fmt_ctx->start_time * av_q2d(AV_TIME_BASE_Q);
        *position = (isnan(start_time)) ? *position : (*position - start_time);
    }
    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_getduration(double *duration)
{
    pthread_mutex_lock(&player_mutex);

    if(ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_getduration failed\n");
        *duration = NAN;
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    if (ssplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE) {
        *duration = ssplayer->p_fmt_ctx->duration * av_q2d(AV_TIME_BASE_Q);
        av_log(NULL, AV_LOG_INFO, "get file duration time [%.3lf]\n", *duration); 
    } else {
        av_log(NULL, AV_LOG_INFO, "get invalid duration time\n"); 
        *duration = NAN;
    }

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_seek(double time)
{
    double pos = 0.0f;

    pthread_mutex_lock(&player_mutex);

    if(ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_seek failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    pos = get_master_clock(ssplayer);
    if (isnan(pos))
        pos = (double)ssplayer->seek_pos / AV_TIME_BASE;
    pos += time;
    if (ssplayer->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos <= ssplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
        pos = ssplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
    if (ssplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE && pos >= ssplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE)
        pos = ssplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE;
    av_log(NULL, AV_LOG_INFO, "start to seek to %.3f\n", pos);
    stream_seek(ssplayer, (int64_t)(pos * AV_TIME_BASE), (int64_t)(time * AV_TIME_BASE), 0);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_seek2time(double time)
{
    double pos = 0.0f, diff = 0.0f, target = 0.0f;

    pthread_mutex_lock(&player_mutex);

    if(ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_seek2time failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    pos = get_master_clock(ssplayer);
    if (isnan(pos))
    {
        pos = (double)ssplayer->seek_pos / AV_TIME_BASE;
    }
    diff = time - pos;
    target = time;
    if (ssplayer->p_fmt_ctx->start_time != AV_NOPTS_VALUE && time <= ssplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
        target = ssplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
    if (ssplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE && time >= ssplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE)
        target= ssplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE;
    av_log(NULL, AV_LOG_INFO, "start to seek to %.3f\n", target);
    stream_seek(ssplayer, (int64_t)(target * AV_TIME_BASE), (int64_t)(diff * AV_TIME_BASE), 0);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_set_rotate(int rotate)
{
    if (rotate < 0 || rotate > 4) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_set_rotate failed!\n");
        return -1;
    }

    g_rotate = rotate;
    av_log(NULL, AV_LOG_INFO, "sstar_player_set_rotate[%d]\n", g_rotate);

    return 0;
}

int sstar_player_set_volumn(int volumn)
{
    pthread_mutex_lock(&player_mutex);
    if (ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR, "sstar_player_set_volumn failed!\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    if (ssplayer->audio_idx >= 0) {
        MI_S32 vol;
        MI_AO_ChnState_t stAoState;

        if (volumn) {
            vol = volumn * (MAX_ADJUST_AO_VOLUME - MIN_ADJUST_AO_VOLUME) / 100 + MIN_ADJUST_AO_VOLUME;
            vol = (vol > MAX_ADJUST_AO_VOLUME) ? MAX_ADJUST_AO_VOLUME : vol;
            vol = (vol < MIN_ADJUST_AO_VOLUME) ? MIN_ADJUST_AO_VOLUME : vol;
        } else {
            vol = MIN_AO_VOLUME;
        }

        memset(&stAoState, 0, sizeof(MI_AO_ChnState_t));
        if (MI_SUCCESS == MI_AO_QueryChnStat(AUDIO_DEV, AUDIO_CHN, &stAoState))
        {
            MI_AO_SetVolume(AUDIO_DEV, vol);
            MI_AO_SetMute(AUDIO_DEV, g_mute);
        }
    }

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int sstar_player_set_mute(bool mute)
{
    if (ssplayer == NULL) {
        av_log(NULL, AV_LOG_ERROR,"sstar_player_set_mute failed!\n");
        return -1;
    }

    if (ssplayer->audio_idx >= 0) {
        g_mute = mute;
        MI_AO_SetMute(AUDIO_DEV, mute);
    }

    return 0;
}

//设置播放器的一些客制化属性, 包括单独音频模式\单独视频模式\旋转^M
int sstar_player_setopts(const char *key, const char *value, int flags)
{
    if (!strcmp(key, "audio_only")) {
        sscanf(value, "%d", &g_opts.audio_only);
        printf("player options audio_only = %d\n", g_opts.audio_only);
    }

    if (!strcmp(key, "video_only")) {
        sscanf(value, "%d", &g_opts.video_only);
        printf("player options video_only = %d\n", g_opts.video_only);
    }

    if (!strcmp(key, "rotate")) {
        int rotate_attr;
        sscanf(value, "%d", &rotate_attr);
        if (rotate_attr == 90) {
            g_opts.rotate_attr = E_MI_DISP_ROTATE_90;
        } else if (rotate_attr == 180) {
            g_opts.rotate_attr = E_MI_DISP_ROTATE_180;
        } else if (rotate_attr == 270) {
            g_opts.rotate_attr = E_MI_DISP_ROTATE_270;
        } else {
            g_opts.rotate_attr = E_MI_DISP_ROTATE_NONE;
        }
        printf("player options rotate_attr = %d\n", g_opts.rotate_attr);
    }

    if (!strcmp(key, "displayer")) {
        if (!strcmp(value, "hdmi")) {
            audio_dev = 3;
        } else {
            audio_dev = 0;
        }
        printf("player options audio_dev = %d\n", g_opts.audio_dev);
    }

    return 0;
}


/*******************************************************************************************/

static bool b_exit = false;

void signal_handler_fun(int signum) {
    printf("catch signal [%d]\n", signum);
    b_exit = true;
}

int main(int argc, char *argv[])
{
    int ret, width, height;
    int volumn = 0;
    bool mute = false;
    char cmd;

    if (!argv[1]) {
        printf("invalid input format, please retey!\n");
        printf("such as : ./SsPlayer filename\n");
        return -1;
    }

    printf("welcome to test ssplayer!\n");

    signal(SIGINT, signal_handler_fun);

    sd20x_sys_init();

    #ifdef SUPPORT_HDMI
    sd20x_panel_init(E_MI_DISP_INTF_HDMI, 0);
    #else
    sd20x_panel_init(E_MI_DISP_INTF_LCD, 0);
    #endif

    printf("try playing %s ...\n", argv[1]);

    ssd20x_getpanel_wh(&width, &height);

    //sstar_player_setopts("video_only", "1", 0);
    //sstar_player_setopts("audio_only", "1", 0);
    //sstar_player_setopts("displayer", "panel", 0);
    sstar_player_setopts("rotate", "0", 0);

    ret = sstar_player_open(argv[1], 0, 0, width, height);
    if (ret < 0) {
        goto exit;
    }

    b_exit = false;
    while (!b_exit)
    {
        fflush(stdin);
        cmd = getchar();
        switch (cmd) 
        {
            case 'o':
                sstar_player_open(argv[1], 0, 0, width, height);
            break;

            case 'c':
                sstar_player_close();
            break;

            case 'f':
                sstar_player_seek(5);
            break;

            case 'b':
                sstar_player_seek(-5);
            break;

            case 'r':
                sstar_player_resume();
            break;

            case 'p':
                sstar_player_pause();
            break;

            case 'g': {
                double duration = 0;
                sstar_player_getduration(&duration);
            }
            break;

            case 'd': {
                double position = 0;
                sstar_player_getposition(&position);
                printf("play %s in [%.3f]\n", argv[1], position);
            }
            break;

            case 'm':
                mute = !mute;
                sstar_player_set_mute(mute);
            break;

            case '+':
                volumn += 5;
                volumn  = (volumn > 100) ? 100 : volumn;
                sstar_player_set_volumn(volumn);
            break;

            case '-':
                volumn -= 5;
                volumn  = (volumn < 0) ? 0 : volumn;
                sstar_player_set_volumn(volumn);
            break;

            case 'q': 
                b_exit = true;
            break;

            default : break;
        }
        fflush(stdout);
        cmd = '\0';
    }
exit:
    sstar_player_close();

    #ifdef SUPPORT_HDMI
    sd20x_panel_deinit(E_MI_DISP_INTF_HDMI);
    #else
    sd20x_panel_deinit(E_MI_DISP_INTF_LCD);
    #endif
    sd20x_sys_deinit();

    return 0; 
}


