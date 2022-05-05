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

#ifdef CHIP_IS_SS268
#include "ss268_panel.h"
#elif defined CHIP_IS_SS22X
#include "ss22x_panel.h"
#else
#include "sd20xpanel.h"
#endif
#include "player.h"
#include "interface.h"

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

/*******************************************************************************************/
static int width, height;
static bool b_exit = false;
static bool player_working = false;
#ifdef UAC_SUPPORT
static int g_device_num;
static int g_buf_mode;
#endif
static int g_play_mode = AV_LOOP;

void signal_handler_fun(int signum) {
    printf("catch signal [%d]\n", signum);
    b_exit = true;
}

static void * mm_player_thread(void *args)
{
    int ret;
    char *filename = (char *)args;

    while (!b_exit)
    {
        if (!player_working)
        {
            sleep(1);
            continue;
        }

        ret = mm_player_get_status();
        if (ret < 0)
        {
            printf("mmplayer has been closed!\n");
            player_working = false;
            continue;
        }

        if (ret & AV_PLAY_ERROR)
        {
            mm_player_close();
            b_exit = true;
        }
        else if (ret & AV_PLAY_LOOP)
        {
        }
        else if ((ret & AV_PLAY_COMPLETE) == AV_PLAY_COMPLETE)
        {
            player_working = false;
            mm_player_close();
			if(g_play_mode == AV_ONCE)
			{
            	b_exit = true;
			}
			else
			{
	            ret = mm_player_open(filename, 0, 0, width, height);
	            if (ret < 0)
	            {
	                b_exit = true;
	            }
	            player_working = true;
			}
        }
        av_usleep(50 * 1000);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int ret, index = 1;
    int volumn = 0;
    bool mute = false, win_down = false;
    char cmd;
    double duration, position;
    pthread_t mm_thread = NULL;
    char url[128];
    bool disp_flag = false;

    if (!argv[1]) {
        printf("invalid input format, please retey!\n");
        printf("such as : ./ssplayer filename\nor: ./ssplayer file1 file2\n");
        return -1;
    }
#ifdef UAC_SUPPORT	
    if (!argv[2]) {
		g_device_num = 1;
    }
	else
	{
		g_device_num = atoi(argv[2]);
	}
    if (!argv[3]) {
		g_buf_mode = 3;
    }
	else
	{
		g_buf_mode = atoi(argv[3]);
	}
    if (!argv[4]) {
		g_play_mode = 1;
    }
	else
	{
		g_play_mode = atoi(argv[4]);
	}
#endif	
    printf("welcome to test ssplayer!\n");

    signal(SIGINT, signal_handler_fun);

#ifdef CHIP_IS_SS268
    ss268_sys_init();
    ss268_screen_init();
    #ifdef SUPPORT_HDMI
    mm_player_set_opts("audio_device", "", 4);
    mm_player_set_opts("audio_layout", "", AV_CH_LAYOUT_STEREO);//keep the same with hdmi init
    #else
    mm_player_set_opts("audio_device", "", 0);
    #endif

    ss268_getpanel_wh(&width, &height);
#elif defined CHIP_IS_SS22X
    ss22x_sys_init();
    ss22x_screen_init();
    #ifdef SUPPORT_HDMI
    mm_player_set_opts("audio_device", "", 4);
    mm_player_set_opts("audio_layout", "", AV_CH_LAYOUT_STEREO);//keep the same with hdmi init
    #else
    mm_player_set_opts("audio_device", "", 0);
    #endif

    ss22x_getpanel_wh(&width, &height);
#else
    sd20x_sys_init();

    #ifdef SUPPORT_HDMI
    sd20x_panel_init(E_MI_DISP_INTF_HDMI, 0);
    mm_player_set_opts("audio_device", "", 3);
#ifdef UAC_SUPPORT
    mm_player_set_opts("audio_layout", "", AV_CH_LAYOUT_STEREO);//keep the same with hdmi init
#else
    mm_player_set_opts("audio_layout", "", AV_CH_LAYOUT_MONO);//keep the same with hdmi init
#endif
    #else
    sd20x_panel_init(E_MI_DISP_INTF_LCD, 0);
    mm_player_set_opts("audio_device", "", 0);
    #endif
#ifdef UAC_SUPPORT	
    mm_player_set_opts("uac_device", "", g_device_num);
    mm_player_set_opts("buf_handle_mode",  "", g_buf_mode);
#endif
    ssd20x_getpanel_wh(&width, &height);
#endif

    printf("try playing %s ...\n", argv[1]);

    mm_player_set_opts("video_rotate", "", AV_ROTATE_NONE);
    mm_player_set_opts("video_only", "", 0);
    mm_player_set_opts("video_ratio", "", AV_SCREEN_MODE);
    mm_player_set_opts("enable_scaler", "", 0);
    mm_player_set_opts("resolution", "8294400", 0);
    mm_player_set_opts("play_mode", "", g_play_mode);

    ret = mm_player_open(argv[1], 0, 0, width, height);
    if (ret < 0) {
        goto exit;
    }
    mm_player_getduration(&duration);
    player_working = true;

    memset(url, '\0', sizeof(url));
    strncpy(url, argv[1], strlen(argv[1]));
	
    prctl(PR_SET_NAME, "mm_player");
    ret = pthread_create(&mm_thread, NULL, mm_player_thread, (void *)url);
    if (ret != 0) {
        goto exit;
    }

    b_exit = false;
    while (!b_exit)
    {
        fflush(stdin);
        cmd = getchar();
        switch (cmd)
        {
            case 's':
                mm_player_open(url, 0, 0, width, height);
                player_working = true;
            break;

            case 't':
                player_working = false;
                mm_player_close();
            break;

            case 'f':
                mm_player_getposition(&position);
                position += 5.0;
                position = (position >= duration) ? duration : position;
                mm_player_seek2time(position);
            break;

            case 'b':
                mm_player_getposition(&position);
                position -= 5.0;
                position = (position <= 0) ? 0 : position;
                mm_player_seek2time(position);
            break;

            case 'u':
                mm_player_resume();
            break;

            case 'p':
                mm_player_pause();
            break;

            case 'g': {
                mm_player_getduration(&duration);
            }
            break;

            case 'd': {
                mm_player_getposition(&position);
                printf("play %s in [%.3f]\n", argv[1], position);
            }
            break;

            case 'm':
                mute = !mute;
                mm_player_set_mute(mute);
                printf("audio mute status: %d\n", mm_player_get_status());
            break;

            case '+':
                volumn += 5;
                volumn  = (volumn > 100) ? 100 : volumn;
                mm_player_set_volumn(volumn);
            break;

            case '-':
                volumn -= 5;
                volumn  = (volumn < 0) ? 0 : volumn;
                mm_player_set_volumn(volumn);
            break;

            case 'w':
                if (!win_down)
                {
                    mm_player_set_window(0, 0, width / 2, height / 2);
                    win_down = true;
                }
                else
                {
                    mm_player_set_window(0, 0, width, height);
                    win_down = false;
                }
            break;

            case 'r':
                if (argv[2] && player_working)
                {
                    player_working = false;
                    mm_player_close();
                    if (index == 1)
                    {
                        memset(url, '\0', sizeof(url));
                        strncpy(url, argv[2], strlen(argv[2]));
                        index ++;
                    }
                    else if (index == 2)
                    {
                        memset(url, '\0', sizeof(url));
                        strncpy(url, argv[1], strlen(argv[1]));
                        index =1;
                    }
                    ret = mm_player_open(url, 0, 0, width, height);
                    if (ret < 0)
                    {
                        b_exit = true;
                    }
                    player_working = true;
                }
            break;

            case 'c':
                disp_flag = !disp_flag;
                mm_player_flush_screen(disp_flag);
            break;

            case 'q':
                b_exit = true;
            break;

            default : break;
        }
        fflush(stdout);
        cmd = '\0';
    }

    if (mm_thread)
        pthread_join(mm_thread, NULL);

exit:
	
    mm_player_close();

#ifdef CHIP_IS_SS268
    ss268_screen_deinit();
    ss268_sys_deinit();
#elif defined CHIP_IS_SS22X
    ss22x_screen_deinit();
    ss22x_sys_deinit();
#else
    #ifdef SUPPORT_HDMI
    sd20x_panel_deinit(E_MI_DISP_INTF_HDMI);
    #else
    sd20x_panel_deinit(E_MI_DISP_INTF_LCD);
    #endif
    sd20x_sys_deinit();
#endif

    return 0;
}


