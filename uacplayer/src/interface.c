
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <arpa/inet.h>

#include "audio.h"
#include "video.h"
#include "demux.h"
#include "player.h"
#include "frame.h"
#include "blitutil.h"
#include "interface.h"

#include "mi_common.h"
#include "mi_sys.h"
#include "mi_disp.h"
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
#include "mi_divp.h"
#elif (defined CHIP_IS_SS268)
#include "mi_scl.h"
#endif
#include "mi_vdec.h"
#include "mi_gfx.h"
#include "mi_ao.h"
#include "mi_vdec_extra.h"
#ifdef UAC_SUPPORT
#include "mi_ai.h"
#include "mi_ai_datatype.h"
#endif
#ifdef ST_DEFAULT_SOC_ID
#undef ST_DEFAULT_SOC_ID
#define ST_DEFAULT_SOC_ID 0
#else
#define ST_DEFAULT_SOC_ID 0
#endif

#define LOCAL_X             0
#define LOCAL_Y             0
#define MIN_WIDTH           128
#define MIN_HEIGHT          64
#define MAX_WIDTH           1920
#define MAX_HEIGHT          1080

#define SCL_DEV             1
#define SCL_CHN             0
#define SCL_PORT            0

#define DISP_DEV            0
#define DISP_CHN            0
#define DISP_LAYER          0
#define DISP_INPUTPORT      0

#define DIVP_DEV            0
#define DIVP_CHN            0
#define DIVP_PORT           0

#define AUDIO_DEV           0
#define AUDIO_CHN           0

#define MIN_AO_VOLUME           -60
#define MAX_AO_VOLUME           30
#define MIN_ADJUST_AO_VOLUME    -10
#define MAX_ADJUST_AO_VOLUME    20

#define AUDIO_INPUT_SAMPRATE        48000
#define AUDIO_INPUT_CHLAYOUT        AV_CH_LAYOUT_STEREO
#define AUDIO_INPUT_SAMPFMT         AV_SAMPLE_FMT_S16

#define AUDIO_OUTPUT_SAMPRATE       E_MI_AUDIO_SAMPLE_RATE_48000
#define AUDIO_OUTPUT_CHLAYOUT       E_MI_AUDIO_SOUND_MODE_STEREO
#define AUDIO_OUTPUT_SAMPFMT        E_MI_AUDIO_BIT_WIDTH_16

#define MI_AUDIO_SAMPLE_PER_FRAME   4096

#define MI_AUDIO_MAX_DATA_SIZE      25000

#define MI_AUDIO_MAX_SAMPLES_PER_FRAME     4096
#define MI_AUDIO_MAX_FRAME_NUM             6

#define MI_AO_PCM_BUF_SIZE_BYTE     (MI_AUDIO_MAX_SAMPLES_PER_FRAME * MI_AUDIO_MAX_FRAME_NUM * 2 * 4)

#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK  MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE  MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED    MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN  MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE   MAKE_YUYV_VALUE(29,225,107)

#define STR1(R)             #R
#define MACRO_TO_STRING(R)  STR1(R)

typedef struct {
    Surface src;
    RECT dst;
    MI_PHY phy_addr[3];
    int direction;
} gfx_param_t;

static pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool audio_mute = false;
static struct timeval time_start, time_end;
static int64_t time0, time1;

//定义全局变量
player_opts_t g_opts = {0, 0, AV_SCREEN_MODE, AV_ROTATE_NONE, AUDIO_DEV, 0, 0, "2088960", AV_ONCE,0,0};
player_stat_t *g_mmplayer = NULL;

typedef enum
{
    E_SOUND_MODE_MONO =0, /* mono */
    E_SOUND_MODE_STEREO =1, /* stereo */
}SoundMode_e;

typedef enum
{
    E_SAMPLE_RATE_8000 =8000, /* 8kHz sampling rate */
    E_SAMPLE_RATE_16000 =16000, /* 16kHz sampling rate */
    E_SAMPLE_RATE_32000 =32000, /* 32kHz sampling rate */
    E_SAMPLE_RATE_48000 =48000, /* 48kHz sampling rate */
}SampleRate_e;

#define AI_AMIC_CHN_MAX 	        (2)
#define AI_DMIC_CHN_MAX		        (4)
#define AI_I2S_NOR_CHN 		        (2)
#define AI_LINE_IN_CHN_MAX 	        (2)
#define AI_AMIC_AND_I2S_RX_CHN_MAX 	(4)
#define AI_DMIC_AND_I2S_RX_CHN_MAX 	(4)
#define AI_AMIC_AND_DMIC_CHN_MAX 	(6)


#define AI_DEV_ID_MAX   (7)
#define AO_DEV_ID_MAX   (2)

#define AI_DEV_AMIC             (0)
#define AI_DEV_DMIC             (1)
#define AI_DEV_I2S_RX           (2)
#define AI_DEV_LINE_IN          (3)
#define AI_DEV_AMIC_AND_I2S_RX  (4)
#define AI_DEV_DMIC_AND_I2S_RX  (5)
#define AI_DEV_AMIC_AND_DMIC    (6)


#define AI_VOLUME_AMIC_MIN      (0)
#define AI_VOLUME_AMIC_MAX      (21)
#define AI_VOLUME_LINEIN_MAX    (7)

#define AI_VOLUME_DMIC_MIN      (0)
#define AI_VOLUME_DMIC_MAX      (4)

//static MI_U8*   pu8AiOutputPath = NULL;
static MI_AUDIO_SampleRate_e eAiOutputResampleRate = E_MI_AUDIO_SAMPLE_RATE_48000;
static MI_U32   u32AiChnCnt = 1;
static MI_S32   s32AiVolume = 0;
static MI_BOOL  bAiSetVolume = FALSE;
static MI_AUDIO_DEV AiDevId = AI_DEV_I2S_RX;
static MI_AUDIO_SampleRate_e eAiSampleRate = E_MI_AUDIO_SAMPLE_RATE_48000;
static MI_AUDIO_SoundMode_e eAiSoundMode = E_MI_AUDIO_SOUND_MODE_STEREO;
static MI_S32   AiChnFd[MI_AUDIO_MAX_CHN_NUM] = {[0 ... MI_AUDIO_MAX_CHN_NUM-1] = -1};
static MI_S32   AiChnSrcPcmFd[MI_AUDIO_MAX_CHN_NUM] = {[0 ... MI_AUDIO_MAX_CHN_NUM-1] = -1};

static bool b_MI_AO = FALSE;

#define TOTAL_BUF_DEPTH		(20)
MI_AUDIO_Frame_t g_stAiChFrame;

#define STCHECKRESULT(result)\
    if (result != MI_SUCCESS)\
    {\
        printf("[%s %d]exec function failed\n", __FUNCTION__, __LINE__);\
    }\
    else\
    {\
        printf("(%s %d)exec function pass\n", __FUNCTION__,__LINE__);\
    }

#define ExecFunc(func, _ret_) \
    printf("%d Start test: %s\n", __LINE__, #func);\
    if (func != _ret_)\
    {\
        printf("AO_TEST [%d] %s exec function failed\n",__LINE__, #func);\
    }\
    else\
    {\
        printf("AO_TEST [%d] %s  exec function pass\n", __LINE__, #func);\
    }\
    printf("%d End test: %s\n", __LINE__, #func);

#define ExecFuncNoExit(func, _ret_, __ret) \
do{	\
	MI_S32 s32TmpRet;	\
    printf("%d Start test: %s\n", __LINE__, #func);\
    s32TmpRet = func;	\
    __ret = s32TmpRet;	\
    if (s32TmpRet != _ret_)\
    {\
        printf("AUDIO_TEST [%d] %s exec function failed, result:0x%x\n",__LINE__, #func, s32TmpRet);\
    }\
    else\
    {\
        printf("AUDIO_TEST [%d] %s  exec function pass\n", __LINE__, #func);\
        printf("%d End test: %s\n", __LINE__, #func);	\
    }\
}while(0);

static int mm_audio_dump_ai_buf(void *args, uint8_t *audio_buf, int *length, void *ai_frame)
{
    MI_S32 s32Ret;
    //MI_AUDIO_Frame_t stAiChFrame;
    MI_AUDIO_AecFrame_t stAecFrame;
	uint8_t * data_test ;
    memset(&g_stAiChFrame, 0, sizeof(MI_AUDIO_Frame_t));
    memset(&stAecFrame, 0, sizeof(MI_AUDIO_AecFrame_t));
	s32Ret = MI_AI_GetFrame(AiDevId, 0, &g_stAiChFrame, &stAecFrame, -1);
	if (MI_SUCCESS == s32Ret)
	{
		*length = (int) g_stAiChFrame.u32Len;
		//audio_buf = g_stAiChFrame.apVirAddr[0];
		memcpy(audio_buf,(MI_U8*)g_stAiChFrame.apVirAddr[0],g_stAiChFrame.u32Len );
		ai_frame = (void *) &g_stAiChFrame;
		data_test = audio_buf;
	}
	else
	{
		printf("Dev%dChn%d get frame failed!!!error:0x%x\n", AiDevId, 0, s32Ret);
	}
	
    return 0;

fail:
    return -1;
}
static int mm_audio_release_ai_buf(void *args,  void *ai_frame)
{
    MI_S32 s32Ret;
	s32Ret = MI_AI_ReleaseFrame(AiDevId, 0,&g_stAiChFrame, NULL);	
	if (MI_SUCCESS == s32Ret)
	{
		 //printf("mm_audio_release_ai_buf successfully \n");
	}
	else
	{
		printf("Dev%dChn%d release frame failed!!!error:0x%x\n", AiDevId, 0, s32Ret);
	}

    return 0;
}

static int sstar_ao_init(void)
{
    MI_S32 s32Ret = MI_SUCCESS;
    MI_AUDIO_Attr_t stSetAttr;
    MI_AUDIO_Attr_t stGetAttr;
    MI_AUDIO_DEV AoDevId = 3;
    MI_AO_CHN AoChn = 0;

    MI_S32 s32SetVolumeDb;
    MI_S32 s32GetVolumeDb;

    //set Ao Attr struct
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
    stSetAttr.eBitwidth = AUDIO_OUTPUT_SAMPFMT;
    stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stSetAttr.u32FrmNum = 6;
    stSetAttr.u32PtNumPerFrm = MI_AUDIO_SAMPLE_PER_FRAME;
	if(b_MI_AO == FALSE)
	{
    	stSetAttr.u32ChnCnt   = 2;		
    	stSetAttr.eSoundmode  = AUDIO_OUTPUT_CHLAYOUT;
	}	
	else
	{
		stSetAttr.u32ChnCnt   = 1;		
		stSetAttr.eSoundmode  = E_MI_AUDIO_SOUND_MODE_MONO;
	}	
    stSetAttr.eSoundmode  = AUDIO_OUTPUT_CHLAYOUT;
    stSetAttr.eSamplerate = AUDIO_OUTPUT_SAMPRATE;

    /* set ao public attr*/
    ExecFunc(MI_AO_SetPubAttr(AoDevId, &stSetAttr), MI_SUCCESS);
    /* get ao device*/
    ExecFunc(MI_AO_GetPubAttr(AoDevId, &stGetAttr), MI_SUCCESS);
    /* enable ao device */
    ExecFunc(MI_AO_Enable(AoDevId), MI_SUCCESS);
    /* enable ao channel of device*/
    ExecFunc(MI_AO_EnableChn(AoDevId, AoChn), MI_SUCCESS);

#ifdef CHIP_IS_SSD20X
    /* if test AO Volume */
    s32SetVolumeDb = -10;
    ExecFunc(MI_AO_SetVolume(AoDevId, s32SetVolumeDb), MI_SUCCESS);
    /* get AO volume */
    ExecFunc(MI_AO_GetVolume(AoDevId, &s32GetVolumeDb), MI_SUCCESS);
#else
    MI_AO_SetVolume(AoDevId, AoChn, s32SetVolumeDb, E_MI_AO_GAIN_FADING_16_SAMPLE);
    MI_AO_GetVolume(AoDevId, AoChn, &s32GetVolumeDb);
#endif

    return 0;
}

static int sstar_ai_init(void)
{
    MI_AUDIO_Attr_t     stAiSetAttr;
    MI_AUDIO_Attr_t     stAiGetAttr;
    MI_U32              u32ChnIdx;
    MI_U32              u32ChnCnt;
    MI_S8               s8OutputFileName[512];
    MI_S8               s8SrcPcmOutputFileName[512];
    MI_S32              s32Ret;
    MI_SYS_ChnPort_t    stAiChnOutputPort0[MI_AUDIO_MAX_CHN_NUM];

    memset(&stAiSetAttr, 0x0, sizeof(MI_AUDIO_Attr_t));
    memset(&stAiGetAttr, 0x0, sizeof(MI_AUDIO_Attr_t));
    stAiSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
    stAiSetAttr.eSamplerate = eAiSampleRate;
    stAiSetAttr.eSoundmode = eAiSoundMode;
    stAiSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stAiSetAttr.u32ChnCnt = u32AiChnCnt;
    stAiSetAttr.u32CodecChnCnt = 0; // useless
    stAiSetAttr.u32FrmNum = 6;  // useless
    stAiSetAttr.u32PtNumPerFrm = 1024;//stAiSetAttr.eSamplerate / 16; // for aec
    stAiSetAttr.WorkModeSetting.stI2sConfig.bSyncClock = FALSE; // useless
    stAiSetAttr.WorkModeSetting.stI2sConfig.eFmt = E_MI_AUDIO_I2S_FMT_I2S_MSB;
    stAiSetAttr.WorkModeSetting.stI2sConfig.eMclk = E_MI_AUDIO_I2S_MCLK_0;

    u32ChnCnt = u32AiChnCnt;

    ExecFuncNoExit(MI_AI_SetPubAttr(AiDevId, &stAiSetAttr), MI_SUCCESS, s32Ret);
    if (MI_SUCCESS != s32Ret)
    {
		goto ERROR_RETURN;
    }

    ExecFuncNoExit(MI_AI_GetPubAttr(AiDevId, &stAiGetAttr), MI_SUCCESS, s32Ret);
    if (MI_SUCCESS != s32Ret)
    {
		goto ERROR_RETURN;
    }

    ExecFuncNoExit(MI_AI_Enable(AiDevId), MI_SUCCESS, s32Ret);
    if (MI_SUCCESS != s32Ret)
    {
		goto ERROR_RETURN;
    }

    if (bAiSetVolume)
    {
        for (u32ChnIdx = 0; u32ChnIdx < u32ChnCnt; u32ChnIdx++)
        {
            if ((AI_DEV_AMIC_AND_I2S_RX == AiDevId)
                || ((AI_DEV_DMIC_AND_I2S_RX == AiDevId)))
            {
                if ((E_MI_AUDIO_SOUND_MODE_MONO == eAiSoundMode)
                    || (E_MI_AUDIO_SOUND_MODE_QUEUE == eAiSoundMode))
                {
                    if ((2 == u32ChnIdx) || (3 == u32ChnIdx))
                    {
                        continue;
                    }
                }
                else if (E_MI_AUDIO_SOUND_MODE_STEREO == eAiSoundMode)
                {
                    if (1 == u32ChnIdx)
                    {
                        continue;
                    }
                }
            }

            if (AiDevId == AI_DEV_AMIC_AND_DMIC)
            {
                if ((E_MI_AUDIO_SOUND_MODE_MONO == eAiSoundMode)
                    || (E_MI_AUDIO_SOUND_MODE_QUEUE == eAiSoundMode))
                {
                    switch (u32ChnIdx)
                    {
                        case 0:
                        case 1:
                            s32AiVolume = 19;
                            break;

                        case 2:
                        case 3:
                        case 4:
                        case 5:
                            s32AiVolume = 4;
                            break;
                    }
                }
                else if (E_MI_AUDIO_SOUND_MODE_STEREO == eAiSoundMode)
                {
                    switch (u32ChnIdx)
                    {
                        case 0:
                            s32AiVolume = 19;
                            break;

                        case 2:
                        case 3:
                            s32AiVolume = 4;
                            break;
                    }
                }
            }
            ExecFuncNoExit(MI_AI_SetVqeVolume(AiDevId, u32ChnIdx, s32AiVolume), MI_SUCCESS, s32Ret);
            if (MI_SUCCESS != s32Ret)
            {
                goto DISABLE_DEVICE;
            }
        }
    }

    memset(&stAiChnOutputPort0, 0x0, sizeof(stAiChnOutputPort0));

    if ((E_MI_AUDIO_SOUND_MODE_QUEUE == eAiSoundMode))
    {
        u32ChnCnt = 1;
    }
    else
    {
        u32ChnCnt = u32AiChnCnt;
    }

    for (u32ChnIdx = 0; u32ChnIdx < u32ChnCnt; u32ChnIdx++)
    {
        stAiChnOutputPort0[u32ChnIdx].eModId = E_MI_MODULE_ID_AI;
        stAiChnOutputPort0[u32ChnIdx].u32DevId = AiDevId;
        stAiChnOutputPort0[u32ChnIdx].u32ChnId = u32ChnIdx;
        stAiChnOutputPort0[u32ChnIdx].u32PortId = 0;
        ExecFunc(MI_SYS_SetChnOutputPortDepth(&stAiChnOutputPort0[u32ChnIdx], 8, TOTAL_BUF_DEPTH), MI_SUCCESS);
    }

    for (u32ChnIdx = 0; u32ChnIdx < u32ChnCnt; u32ChnIdx++)
    {
        ExecFuncNoExit(MI_AI_EnableChn(AiDevId, u32ChnIdx), MI_SUCCESS, s32Ret);
        if (MI_SUCCESS != s32Ret)
        {
			goto DISABLE_DEVICE;
        }
    }

    printf("create ai thread.\n");
    return 0;

DISABLE_CHANNEL:
	for (u32ChnIdx = 0; u32ChnIdx < u32ChnCnt; u32ChnIdx++)
	{
		ExecFunc(MI_AI_DisableChn(AiDevId, u32ChnIdx), MI_SUCCESS);
	}
DISABLE_DEVICE:
	ExecFunc(MI_AI_Disable(AiDevId), MI_SUCCESS);

ERROR_RETURN:
	return s32Ret;
}
static int sstar_ao_deinit(void)
{
    MI_AUDIO_DEV AoDevId = 3;
    MI_AO_CHN AoChn = 0;
	printf("===> sstar_ao_deinit\n");
	
    /* disable ao channel of */
    ExecFunc(MI_AO_DisableChn(AoDevId, AoChn), MI_SUCCESS);
    /* disable ao device */
    ExecFunc(MI_AO_Disable(AoDevId), MI_SUCCESS);

    return 0;
}

static int sstar_ai_deinit(void)
{
    MI_U32 u32ChnIdx;
    MI_U32 u32ChnCnt;
	printf("deinitAi\n");

    if ((E_MI_AUDIO_SOUND_MODE_QUEUE == eAiSoundMode))
    {
        u32ChnCnt = 1;
    }
    else
    {
        u32ChnCnt = u32AiChnCnt;
    }

    for (u32ChnIdx = 0; u32ChnIdx < u32ChnCnt; u32ChnIdx++)
    {
        ExecFunc(MI_AI_DisableChn(AiDevId, u32ChnIdx), MI_SUCCESS);
    }
    ExecFunc(MI_AI_Disable(AiDevId), MI_SUCCESS);
	
    return 0;
}

static int mm_audio_init(void *args)
{
    MI_AUDIO_Attr_t stSetAttr;
    MI_AUDIO_Attr_t stGetAttr;
    MI_AUDIO_DEV AoDevId = g_opts.audio_dev;
    MI_AO_CHN AoChn = AUDIO_CHN;

    MI_S32 s32SetVolumeDb;
    MI_S32 s32GetVolumeDb;

    player_stat_t *is = (player_stat_t *)args;

    //system("echo 12 > /sys/class/gpio/export");
    //system("echo out > /sys/class/gpio/gpio12/direction");
    //system("echo 1 > /sys/class/gpio/gpio12/value");

    //set Ao Attr struct
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
    stSetAttr.eBitwidth = E_MI_AUDIO_BIT_WIDTH_16;
    stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stSetAttr.u32FrmNum = 6;
    stSetAttr.u32PtNumPerFrm = MI_AUDIO_SAMPLE_PER_FRAME;

    if (!g_opts.audio_layout) {
        if (is->p_audio_stream->codecpar->channel_layout == AV_CH_LAYOUT_STEREO) {
            g_opts.audio_layout = AV_CH_LAYOUT_STEREO; // 如果不是立体声, 默认使用单声道
        } else {
            g_opts.audio_layout = AV_CH_LAYOUT_MONO;
        }
        av_log(NULL, AV_LOG_INFO, "audio channel layout[%d]\n", g_opts.audio_layout);
    }

    if (g_opts.audio_layout == AV_CH_LAYOUT_STEREO) {
#if ((defined CHIP_IS_SS268) || (defined CHIP_IS_SS22X))
        stSetAttr.u32ChnCnt = 1;
#else
        stSetAttr.u32ChnCnt = 2;
#endif
        stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
    } else {
        stSetAttr.u32ChnCnt = 1;
        stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_MONO;
    }

    stSetAttr.eSamplerate = E_MI_AUDIO_SAMPLE_RATE_48000;

    /* set ao public attr*/
    MI_AO_SetPubAttr(AoDevId, &stSetAttr);

    /* get ao device*/
    MI_AO_GetPubAttr(AoDevId, &stGetAttr);

    /* enable ao device */
    MI_AO_Enable(AoDevId);

    /* enable ao channel of device*/
    MI_AO_EnableChn(AoDevId, AoChn);

#ifdef CHIP_IS_SS268
    s32SetVolumeDb = MIN_ADJUST_AO_VOLUME;
    MI_AO_SetVolume(AoDevId, AoChn, s32SetVolumeDb, E_MI_AO_GAIN_FADING_16_SAMPLE);
    MI_AO_GetVolume(AoDevId, AoChn, &s32GetVolumeDb);
#elif defined CHIP_IS_SS22X
    s32SetVolumeDb = MIN_ADJUST_AO_VOLUME;
    MI_AO_SetVolume(AoDevId, AoChn, s32SetVolumeDb, E_MI_AO_GAIN_FADING_16_SAMPLE);
    MI_AO_GetVolume(AoDevId, AoChn, &s32GetVolumeDb);
#else
    /* if test AO Volume */
    s32SetVolumeDb = MIN_ADJUST_AO_VOLUME;
    MI_AO_SetVolume(AoDevId, s32SetVolumeDb);

    /* get AO volume */
    MI_AO_GetVolume(AoDevId, &s32GetVolumeDb);
#endif

	sstar_ai_init();

	av_log(NULL, AV_LOG_INFO, "mm_audio_init dev[%d] done!\n", AoDevId);

    return g_opts.audio_layout;
}

static int mm_audio_deinit(void *args)
{
    MI_AUDIO_DEV AoDevId = g_opts.audio_dev;
    MI_AO_CHN AoChn = AUDIO_CHN;
	
	sstar_ai_deinit();
    //system("echo 0 > /sys/class/gpio/gpio12/value");

    /* disable ao channel of */
    MI_AO_DisableChn(AoDevId, AoChn);

    /* disable ao device */
    MI_AO_Disable(AoDevId);

    return MI_SUCCESS;
}

static int mm_audio_pause(void)
{
    if (MI_SUCCESS == MI_AO_PauseChn(g_opts.audio_dev, AUDIO_CHN))
    {
        g_mmplayer->play_status |= AV_AUDIO_PAUSE;
    }
    return 0;
}

static int mm_audio_resume(void)
{
    if (MI_SUCCESS == MI_AO_ResumeChn(g_opts.audio_dev, AUDIO_CHN))
    {
        g_mmplayer->play_status &= ~AV_AUDIO_PAUSE;
    }
    return 0;
}

static int mm_audio_clear_buf(void)
{
    if (!g_mmplayer)
    {
        av_log(NULL, AV_LOG_ERROR, "g_mmplayer is null!\n");
    }

    if (g_mmplayer->paused)
    {
        MI_AO_ResumeChn(g_opts.audio_dev, AUDIO_CHN);
        MI_AO_ClearChnBuf(g_opts.audio_dev, AUDIO_CHN);
        MI_AO_PauseChn(g_opts.audio_dev, AUDIO_CHN);
    }
    else
    {
        MI_AO_ClearChnBuf(g_opts.audio_dev, AUDIO_CHN);
    }
    return 0;
}

static int mm_audio_play(void *args, char *data, int len)
{
    int data_idx = 0, data_len = len;
    char *audio_pcm = (char *)data;
    int audio_write_buf_size = 0;

    MI_AUDIO_Frame_t stAoSendFrame;
    MI_S32 s32RetSendStatus = 0;
    MI_AO_CHN AoChn = AUDIO_CHN;
    MI_AO_ChnState_t stState;

    MI_AO_QueryChnStat(g_opts.audio_dev, AoChn, &stState);
    audio_write_buf_size = stState.u32ChnBusyNum + 4096;
    //printf("remain audio_write_buf_size = %d\n", audio_write_buf_size);

    do {
        if (data_len <= MI_AUDIO_MAX_DATA_SIZE)
        {
#ifdef CHIP_IS_SS268
            stAoSendFrame.u32Len[0] = data_len;
#elif defined CHIP_IS_SS22X
            stAoSendFrame.u32Len[0] = data_len;
#else
            stAoSendFrame.u32Len = data_len;
#endif
        }
        else
        {
#ifdef CHIP_IS_SS268
            stAoSendFrame.u32Len[0] = MI_AUDIO_MAX_DATA_SIZE;
#elif defined CHIP_IS_SS22X
            stAoSendFrame.u32Len[0] = MI_AUDIO_MAX_DATA_SIZE;
#else
            stAoSendFrame.u32Len = MI_AUDIO_MAX_DATA_SIZE;
#endif
        }
		
        stAoSendFrame.apVirAddr[0] = &audio_pcm[data_idx];
        stAoSendFrame.apVirAddr[1] = NULL;  
        data_len -= MI_AUDIO_MAX_DATA_SIZE;
        data_idx += MI_AUDIO_MAX_DATA_SIZE;

        do{
            s32RetSendStatus = MI_AO_SendFrame(g_opts.audio_dev, AoChn, &stAoSendFrame, 128);
        }while(s32RetSendStatus == MI_AO_ERR_NOBUF);

        if(s32RetSendStatus != MI_SUCCESS)
        {
            printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n",s32RetSendStatus);
        }
    }while(data_len > 0);

    return audio_write_buf_size;
}

static void mm_video_rotate(gfx_param_t *gfx_info, MI_PHY yAddr, MI_PHY uvAddr)
{
    Surface srcY, dstY;
    Surface srcUV, dstUV;
    RECT r;
    srcY.eGFXcolorFmt   = E_MI_GFX_FMT_I8;
    srcY.BytesPerPixel  = 1;
    srcY.w              = gfx_info->src.w;
    srcY.h              = gfx_info->src.h;
    srcY.phy_addr       = gfx_info->phy_addr[0];
    srcY.pitch          = gfx_info->src.pitch;

    dstY.eGFXcolorFmt   = E_MI_GFX_FMT_I8;
    dstY.BytesPerPixel  = 1;
    if (gfx_info->direction == AV_ROTATE_180 || gfx_info->direction == AV_ROTATE_NONE) {
        dstY.w              = srcY.w;
        dstY.h              = srcY.h;
        dstY.pitch          = ALIGN_UP(srcY.w, 16);
        dstY.phy_addr       = yAddr;
    } else {
        dstY.w              = srcY.h;
        dstY.h              = srcY.w;
        dstY.pitch          = ALIGN_UP(srcY.h, 16);
        dstY.phy_addr       = yAddr;
    }

    r.left   = gfx_info->dst.left;
    r.top    = gfx_info->dst.top;
    r.bottom = gfx_info->dst.bottom;
    r.right  = gfx_info->dst.right;
    if (gfx_info->direction == AV_ROTATE_90) {
        SstarBlitCW(&srcY, &dstY, &r);
    }
    else if (gfx_info->direction == AV_ROTATE_270) {
        SstarBlitCCW(&srcY, &dstY, &r);
    }
    else if (gfx_info->direction == AV_ROTATE_180) {
        SstarBlitHVFlip(&srcY, &dstY, &r);
    }
    else {
        SstarBlitNormal(&srcY, &dstY, &r);
    }

    srcUV.eGFXcolorFmt  = E_MI_GFX_FMT_ARGB4444;
    srcUV.BytesPerPixel = 2;
    srcUV.w             = gfx_info->src.w / 2;
    srcUV.h             = gfx_info->src.h / 2;
    srcUV.pitch         = gfx_info->src.pitch;
    srcUV.phy_addr      = gfx_info->phy_addr[1];

    dstUV.eGFXcolorFmt  = E_MI_GFX_FMT_ARGB4444;
    dstUV.BytesPerPixel = 2;
    if (gfx_info->direction == AV_ROTATE_180 || gfx_info->direction == AV_ROTATE_NONE) {
        dstUV.w             = srcUV.w;
        dstUV.h             = srcUV.h;
        dstUV.pitch         = ALIGN_UP(srcY.w, 16);
        dstUV.phy_addr      = uvAddr;
    } else {
        dstUV.h             = srcUV.w;
        dstUV.w             = srcUV.h;
        dstUV.pitch         = ALIGN_UP(srcY.h, 16);
        dstUV.phy_addr      = uvAddr;
    }

    r.left   = gfx_info->dst.left;
    r.top    = gfx_info->dst.top;
    r.bottom = gfx_info->dst.bottom / 2;
    r.right  = gfx_info->dst.right / 2;
    if (gfx_info->direction == AV_ROTATE_90) {
        SstarBlitCW(&srcUV, &dstUV, &r);
    }
    else if (gfx_info->direction == AV_ROTATE_270) {
        SstarBlitCCW(&srcUV, &dstUV, &r);
    }
    else if (gfx_info->direction == AV_ROTATE_180) {
        SstarBlitHVFlip(&srcUV, &dstUV, &r);
    }
    else {
        SstarBlitNormal(&srcUV, &dstUV, &r);
    }
}

bool printf_flag = false;
static int mm_video_play(void *args, void *data)
{
    player_stat_t *is = (player_stat_t *)args;
    AVFrame *frame = (AVFrame *)data;

    if (is->decoder_type == AV_SOFT_DECODING)
    {
        MI_SYS_ChnPort_t  stInputChnPort;
        memset(&stInputChnPort, 0, sizeof(MI_SYS_ChnPort_t));
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
        stInputChnPort.eModId     = E_MI_MODULE_ID_DIVP;
        stInputChnPort.u32ChnId   = DIVP_CHN;
        stInputChnPort.u32DevId   = DIVP_DEV;
        stInputChnPort.u32PortId  = DIVP_PORT;
#elif (defined CHIP_IS_SS268)
        stInputChnPort.eModId     = E_MI_MODULE_ID_SCL;
        stInputChnPort.u32ChnId   = SCL_CHN;
        stInputChnPort.u32DevId   = SCL_DEV;
        stInputChnPort.u32PortId  = SCL_PORT;
#endif
        //gettimeofday(&time_start, NULL);
        // YUV格式统一转换成NV12
        sws_scale(is->img_convert_ctx,                  // sws context
                  (const uint8_t *const *)frame->data,  // src slice
                  frame->linesize,                      // src stride
                  0,                                    // src slice y
                  is->p_vcodec_ctx->height,             // src slice height
                  is->p_frm_yuv->data,                  // dst planes
                  is->p_frm_yuv->linesize               // dst strides
                  );
        //gettimeofday(&time_end, NULL);
        //time0 = ((int64_t)time_end.tv_sec * 1000000 + time_end.tv_usec) - ((int64_t)time_start.tv_sec * 1000000 + time_start.tv_usec);

        //int length = is->p_frm_yuv->width * is->p_frm_yuv->height * 3 / 2;
        //fwrite(is->p_frm_yuv->data[0], length, 1, dump_fp);

        MI_SYS_BufConf_t stBufConf;
        MI_SYS_BufInfo_t stBufInfo;
        MI_SYS_BUF_HANDLE bufHandle;
        memset(&stBufConf, 0, sizeof(MI_SYS_BufConf_t));
        stBufConf.eBufType              = E_MI_SYS_BUFDATA_FRAME;
        stBufConf.stFrameCfg.eFormat    = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
        if (is->display_mode != AV_ROTATE_NONE && is->display_mode != AV_ROTATE_180) {
            stBufConf.stFrameCfg.u16Height  = is->p_frm_yuv->width;
            stBufConf.stFrameCfg.u16Width   = is->p_frm_yuv->height;
        } else {
            stBufConf.stFrameCfg.u16Height  = is->p_frm_yuv->height;
            stBufConf.stFrameCfg.u16Width   = is->p_frm_yuv->width;
        }
        stBufConf.u32Flags              = MI_SYS_MAP_VA;
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
        stBufConf.stFrameCfg.stFrameBufExtraConf.u16BufHAlignment = 16;
#endif
        if (MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&stInputChnPort, &stBufConf, &stBufInfo, &bufHandle, 0))
        {
            // flush需要时间分辨率大于720P就不执行该操作但是会导致图像拉丝
            if (is->p_frm_yuv->width * is->p_frm_yuv->height < 1280 * 720 || is->flush) {
                MI_SYS_FlushInvCache(is->vir_addr, is->buf_size);
            }

            gfx_param_t gfx_info;
            gfx_info.direction   = is->display_mode;
            gfx_info.phy_addr[0] = is->phy_addr;
            gfx_info.phy_addr[1] = is->phy_addr + is->p_vcodec_ctx->width * is->p_vcodec_ctx->height;
            gfx_info.src.pitch = is->p_vcodec_ctx->width;
            gfx_info.src.w = is->p_vcodec_ctx->width;
            gfx_info.src.h = is->p_vcodec_ctx->height;
            gfx_info.dst.left   = 0;
            gfx_info.dst.top    = 0;
            gfx_info.dst.right  = is->p_vcodec_ctx->width;
            gfx_info.dst.bottom = is->p_vcodec_ctx->height;

            //gettimeofday(&time_start, NULL);
            mm_video_rotate(&gfx_info, stBufInfo.stFrameData.phyAddr[0], stBufInfo.stFrameData.phyAddr[1]);
            //gettimeofday(&time_end, NULL);

            MI_SYS_ChnInputPortPutBuf(bufHandle, &stBufInfo, FALSE);
        }

        //time1 = ((int64_t)time_end.tv_sec * 1000000 + time_end.tv_usec) - ((int64_t)time_start.tv_sec * 1000000 + time_start.tv_usec);
        //printf("time of sws_scale : %lldus, time of rotate : %lldus\n", time0, time1);
    }
    else if (is->decoder_type == AV_HARD_DECODING)
    {
        MI_SYS_ChnPort_t  stInputChnPort;
        memset(&stInputChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        if (g_opts.enable_scaler)
        {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
            stInputChnPort.eModId    = E_MI_MODULE_ID_DIVP;
            stInputChnPort.u32ChnId  = DIVP_CHN;
            stInputChnPort.u32DevId  = DIVP_DEV;
            stInputChnPort.u32PortId = DIVP_PORT;
#elif (defined CHIP_IS_SS268)
            stInputChnPort.eModId    = E_MI_MODULE_ID_SCL;
            stInputChnPort.u32ChnId  = SCL_CHN;
            stInputChnPort.u32DevId  = SCL_DEV;
            stInputChnPort.u32PortId = SCL_PORT;
#endif
        }
        else
        {
            stInputChnPort.eModId    = E_MI_MODULE_ID_DISP;
            stInputChnPort.u32ChnId  = DISP_CHN;
            stInputChnPort.u32DevId  = DISP_DEV;
            stInputChnPort.u32PortId = DISP_INPUTPORT;
        }

        SS_Vdec_BufInfo *stVdecBuf = (SS_Vdec_BufInfo *)frame->opaque;

#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
        if (is->display_mode == AV_ROTATE_NONE && !g_opts.enable_scaler && !is->keep_frames)
        {
            if (MI_SUCCESS == MI_SYS_ChnPortInjectBuf(stVdecBuf->stVdecHandle, &stInputChnPort)) {
                stVdecBuf->stVdecHandle = NULL;
                //av_log(NULL, AV_LOG_WARNING, "Inject Buf To Disp u32SequenceNumber: %u, eBufType: %d\n", stVdecBuf->stVdecBufInfo.u32SequenceNumber, stVdecBuf->stVdecBufInfo.eBufType);
            } else {
                av_log(NULL, AV_LOG_ERROR, "MI_SYS_ChnPortInjectBuf Failed!\n");
            }
        }
        else
#endif
        {
            MI_SYS_BufConf_t stBufConf;
            MI_SYS_BufInfo_t stBufInfo;
            MI_SYS_BUF_HANDLE bufHandle;
            memset(&stBufConf, 0, sizeof(MI_SYS_BufConf_t));
            stBufConf.eBufType              = E_MI_SYS_BUFDATA_FRAME;
            stBufConf.stFrameCfg.eFormat    = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
            if (is->display_mode != AV_ROTATE_NONE && is->display_mode != AV_ROTATE_180) {
                if (stVdecBuf->bType)
                {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
                    mi_vdec_DispFrame_t *pstVdecInfo = (mi_vdec_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;
                    stBufConf.stFrameCfg.u16Height = pstVdecInfo->stFrmInfo.u16Width;
                    stBufConf.stFrameCfg.u16Width  = pstVdecInfo->stFrmInfo.u16Height;
#elif (defined CHIP_IS_SS268)
                    MI_VDEC_DispFrame_t *pstVdecInfo = (MI_VDEC_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;
                    stBufConf.stFrameCfg.u16Height = pstVdecInfo->u16Width;
                    stBufConf.stFrameCfg.u16Width  = pstVdecInfo->u16Height;
#endif
                }
                else
                {
                    stBufConf.stFrameCfg.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.u16Width;
                    stBufConf.stFrameCfg.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.u16Height;
                }
            } else {
                if (stVdecBuf->bType)
                {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
                    mi_vdec_DispFrame_t *pstVdecInfo = (mi_vdec_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;
                    stBufConf.stFrameCfg.u16Width  = pstVdecInfo->stFrmInfo.u16Width;
                    stBufConf.stFrameCfg.u16Height = pstVdecInfo->stFrmInfo.u16Height;
#elif (defined CHIP_IS_SS268)
                    MI_VDEC_DispFrame_t *pstVdecInfo = (MI_VDEC_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;
                    stBufConf.stFrameCfg.u16Width  = pstVdecInfo->u16Width;
                    stBufConf.stFrameCfg.u16Height = pstVdecInfo->u16Height;
#endif
                }
                else
                {
                    stBufConf.stFrameCfg.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.u16Height;
                    stBufConf.stFrameCfg.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.u16Width;
                }
            }
            stBufConf.u32Flags              = MI_SYS_MAP_VA;
#if (defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X)
            stBufConf.stFrameCfg.stFrameBufExtraConf.u16BufHAlignment = 16;
#endif
            if (MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&stInputChnPort, &stBufConf, &stBufInfo, &bufHandle, 0))
            {
                stBufInfo.stFrameData.eCompressMode = E_MI_SYS_COMPRESS_MODE_NONE;
                stBufInfo.stFrameData.eFieldType    = E_MI_SYS_FIELDTYPE_NONE;
                stBufInfo.stFrameData.eTileMode     = E_MI_SYS_FRAME_TILE_MODE_NONE;
                stBufInfo.bEndOfStream              = FALSE;

                gfx_param_t gfx_info;
                if (stVdecBuf->bType) {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
                    mi_vdec_DispFrame_t *pstVdecInfo = (mi_vdec_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;

                    gfx_info.direction   = is->display_mode;
                    gfx_info.phy_addr[0] = pstVdecInfo->stFrmInfo.phyLumaAddr;
                    gfx_info.phy_addr[1] = pstVdecInfo->stFrmInfo.phyChromaAddr;
                    gfx_info.src.pitch   = pstVdecInfo->stFrmInfo.u16Stride;
                    gfx_info.src.w       = pstVdecInfo->stFrmInfo.u16Width;
                    gfx_info.src.h       = pstVdecInfo->stFrmInfo.u16Height;
                    gfx_info.dst.left    = pstVdecInfo->stDispInfo.u16CropLeft;
                    gfx_info.dst.top     = pstVdecInfo->stDispInfo.u16CropTop;
                    gfx_info.dst.right   = pstVdecInfo->stDispInfo.u16CropRight;
                    gfx_info.dst.bottom  = pstVdecInfo->stDispInfo.u16CropBottom;

                    if (is->display_mode == AV_ROTATE_90) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stFrmInfo.u16Height - pstVdecInfo->stDispInfo.u16CropBottom - pstVdecInfo->stDispInfo.u16CropTop;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stDispInfo.u16CropLeft;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stDispInfo.u16CropBottom;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stDispInfo.u16CropRight;
                    }
                    else if (is->display_mode == AV_ROTATE_270) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stDispInfo.u16CropTop;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stFrmInfo.u16Width - pstVdecInfo->stDispInfo.u16CropRight - pstVdecInfo->stDispInfo.u16CropLeft;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stDispInfo.u16CropBottom;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stDispInfo.u16CropRight;
                    }
                    else if (is->display_mode == AV_ROTATE_180) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stFrmInfo.u16Width - pstVdecInfo->stDispInfo.u16CropRight;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stFrmInfo.u16Height - pstVdecInfo->stDispInfo.u16CropBottom;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stDispInfo.u16CropRight;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stDispInfo.u16CropBottom;
                    }
                    else
                    {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stDispInfo.u16CropLeft;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stDispInfo.u16CropTop;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stDispInfo.u16CropRight;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stDispInfo.u16CropBottom;
                    }

                    if (!printf_flag)
                    {
                        printf_flag = true;
                        av_log(NULL, AV_LOG_INFO, "vdec output crop info: [%d %d %d %d]\n", pstVdecInfo->stDispInfo.u16CropLeft,
                        pstVdecInfo->stDispInfo.u16CropTop, pstVdecInfo->stDispInfo.u16CropRight, pstVdecInfo->stDispInfo.u16CropBottom);
                    }
#elif (defined CHIP_IS_SS268)
                    MI_VDEC_DispFrame_t *pstVdecInfo = (MI_VDEC_DispFrame_t *)stVdecBuf->stVdecBufInfo.stMetaData.pVirAddr;

                    gfx_info.direction   = is->display_mode;
                    gfx_info.phy_addr[0] = pstVdecInfo->phyAddr[0];
                    gfx_info.phy_addr[1] = pstVdecInfo->phyAddr[1];
                    gfx_info.src.pitch   = pstVdecInfo->u32Stride[0];
                    gfx_info.src.w       = pstVdecInfo->u16Width;
                    gfx_info.src.h       = pstVdecInfo->u16Height;
                    gfx_info.dst.left    = pstVdecInfo->stCropWindow.u16X;
                    gfx_info.dst.top     = pstVdecInfo->stCropWindow.u16Y;
                    gfx_info.dst.right   = pstVdecInfo->stCropWindow.u16X + pstVdecInfo->stCropWindow.u16Width;
                    gfx_info.dst.bottom  = pstVdecInfo->stCropWindow.u16Y + pstVdecInfo->stCropWindow.u16Height;

                    if (is->display_mode == AV_ROTATE_90) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->u16Height - pstVdecInfo->stCropWindow.u16Height - pstVdecInfo->stCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stCropWindow.u16Height;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stCropWindow.u16Width;
                    }
                    else if (is->display_mode == AV_ROTATE_270) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->u16Width - pstVdecInfo->stCropWindow.u16Width - pstVdecInfo->stCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stCropWindow.u16Height;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stCropWindow.u16Width;
                    }
                    else if (is->display_mode == AV_ROTATE_180) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->u16Width - pstVdecInfo->stCropWindow.u16Width - pstVdecInfo->stCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->u16Height - pstVdecInfo->stCropWindow.u16Height - pstVdecInfo->stCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stCropWindow.u16Width;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stCropWindow.u16Height;
                    }
                    else
                    {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = pstVdecInfo->stCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = pstVdecInfo->stCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = pstVdecInfo->stCropWindow.u16Width;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = pstVdecInfo->stCropWindow.u16Height;
                    }

                    if (!printf_flag)
                    {
                        printf_flag = true;
                        av_log(NULL, AV_LOG_INFO, "vdec output crop info: [%d %d %d %d]\n", pstVdecInfo->stCropWindow.u16X,
                        pstVdecInfo->stCropWindow.u16Y, pstVdecInfo->stCropWindow.u16Width, pstVdecInfo->stCropWindow.u16Height);
                    }
#endif
                } else {
                    gfx_info.direction   = is->display_mode;
                    gfx_info.phy_addr[0] = stVdecBuf->stVdecBufInfo.stFrameData.phyAddr[0];
                    gfx_info.phy_addr[1] = stVdecBuf->stVdecBufInfo.stFrameData.phyAddr[1];
                    gfx_info.src.pitch   = stVdecBuf->stVdecBufInfo.stFrameData.u32Stride[0];
                    gfx_info.src.w       = stVdecBuf->stVdecBufInfo.stFrameData.u16Width;
                    gfx_info.src.h       = stVdecBuf->stVdecBufInfo.stFrameData.u16Height;
                    gfx_info.dst.left    = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16X;
                    gfx_info.dst.top     = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Y;
                    gfx_info.dst.right   = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                    gfx_info.dst.bottom  = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;

                    if (is->display_mode == AV_ROTATE_90) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = stVdecBuf->stVdecBufInfo.stFrameData.u16Height - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                    }
                    else if (is->display_mode == AV_ROTATE_270) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = stVdecBuf->stVdecBufInfo.stFrameData.u16Width - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                    }
                    else if (is->display_mode == AV_ROTATE_180) {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = stVdecBuf->stVdecBufInfo.stFrameData.u16Width - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = stVdecBuf->stVdecBufInfo.stFrameData.u16Height - stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;
                    }
                    else
                    {
                        stBufInfo.stFrameData.stContentCropWindow.u16X = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16X;
                        stBufInfo.stFrameData.stContentCropWindow.u16Y = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Y;
                        stBufInfo.stFrameData.stContentCropWindow.u16Width  = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width;
                        stBufInfo.stFrameData.stContentCropWindow.u16Height = stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height;
                    }

                    if (!printf_flag)
                    {
                        printf_flag = true;
                        av_log(NULL, AV_LOG_INFO, "vdec output crop info: [%d %d %d %d]\n", stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16X,
                        stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Y, stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Width,
                        stVdecBuf->stVdecBufInfo.stFrameData.stContentCropWindow.u16Height);
                    }
                }
#if (defined CHIP_IS_SS268)
                if (g_opts.enable_scaler)
                {
                    MI_SCL_DEV SclDevId = SCL_DEV;
                    MI_SCL_CHANNEL SclChnId = SCL_CHN;
                    MI_SYS_WindowRect_t stCropWin;

                    memset(&stCropWin, 0x0, sizeof(MI_SYS_WindowRect_t));
                    stCropWin.u16X      = stBufInfo.stFrameData.stContentCropWindow.u16X;
                    stCropWin.u16Y      = stBufInfo.stFrameData.stContentCropWindow.u16Y;
                    stCropWin.u16Width  = stBufInfo.stFrameData.stContentCropWindow.u16Width;
                    stCropWin.u16Height = stBufInfo.stFrameData.stContentCropWindow.u16Height;
                    MI_SCL_SetInputPortCrop(SclDevId, SclChnId, &stCropWin);
                }
#endif
                //gettimeofday(&time_start, NULL);
                mm_video_rotate(&gfx_info, stBufInfo.stFrameData.phyAddr[0], stBufInfo.stFrameData.phyAddr[1]);
                //gettimeofday(&time_end, NULL);
                //time1 = ((int64_t)time_end.tv_sec * 1000000 + time_end.tv_usec) - ((int64_t)time_start.tv_sec * 1000000 + time_start.tv_usec);
                //printf("time of mm_video_rotate : %lldus, time of rotate : %lldus\n", time0, time1);
                MI_SYS_ChnInputPortPutBuf(bufHandle, &stBufInfo, FALSE);
            }
        }
        //frame_queue_putbuf(frame);
    }

    return 0;
}

static int mm_video_putbuf(void *args)
{
    AVFrame * frame = (AVFrame *)args;
    SS_Vdec_BufInfo *stVdecBuf = (SS_Vdec_BufInfo *)frame->opaque;
    //printf("Vdec Put Buf u32SequenceNumber: %u, eBufType: %d\n", stVdecBuf->stVdecBufInfo.u32SequenceNumber, stVdecBuf->stVdecBufInfo.eBufType);
    if (stVdecBuf->stVdecHandle) {
        if (MI_SUCCESS != MI_SYS_ChnOutputPortPutBuf(stVdecBuf->stVdecHandle)) {
            printf("frame_queue_putbuf failed!\n");
        }
        //av_log(NULL, AV_LOG_ERROR, "Vdec Put Buf u32SequenceNumber: %u, eBufType: %d\n", stVdecBuf->stVdecBufInfo.u32SequenceNumber, stVdecBuf->stVdecBufInfo.eBufType);
    }
    av_freep(&frame->opaque);

    return 0;
}

static int mm_video_init(void *args)
{
    MI_SYS_ChnPort_t stDispChnPort;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_RotateConfig_t stRotateConfig;
    player_stat_t *is = (player_stat_t *)args;

    if (!is) {
        av_log(NULL, AV_LOG_ERROR, "sstar_display_set failed!\n");
        return -1;
    }

    av_log(NULL, AV_LOG_WARNING, "display w/h = [%d %d], src w/h = [%d %d]!\n", is->out_width, is->out_height, is->dst_width, is->dst_height);

#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
    #ifdef ENABLE_STR
    MI_DISP_InitParam_t stInitDispParam;
    memset(&stInitDispParam, 0x0, sizeof(MI_DISP_InitParam_t));
    stInitDispParam.u32DevId = 0;
    stInitDispParam.u8Data = NULL;
    MI_DISP_InitDev(&stInitDispParam);

    MI_SYS_InitParam_t stInitSysParam;
    memset(&stInitSysParam, 0x0, sizeof(MI_SYS_InitParam_t));
    stInitSysParam.u32DevId = 0;
    stInitSysParam.u8Data = NULL;
    MI_SYS_InitDev(&stInitSysParam);
    av_log(NULL, AV_LOG_WARNING, "Init disp and sys module dev!\n");

    MI_GFX_InitParam_t stInitGfxParam;
    memset(&stInitGfxParam, 0x0, sizeof(MI_GFX_InitParam_t));
    stInitGfxParam.u32DevId = 0;
    stInitGfxParam.u8Data = NULL;
    MI_GFX_InitDev(&stInitGfxParam);
    av_log(NULL, AV_LOG_WARNING, "Init divp and gfx module dev!\n");
    #endif
    MI_GFX_Open();
#elif (defined CHIP_IS_SS268)
    MI_GFX_DEV stGfxDevId = 0;
    MI_GFX_Open(stGfxDevId);
#endif

    memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
    MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr);
    stInputPortAttr.u16SrcWidth         = is->dst_width;
    stInputPortAttr.u16SrcHeight        = is->dst_height;
    stInputPortAttr.stDispWin.u16X      = is->pos_x;
    stInputPortAttr.stDispWin.u16Y      = is->pos_y;
    stInputPortAttr.stDispWin.u16Width  = is->out_width;
    stInputPortAttr.stDispWin.u16Height = is->out_height;
    MI_DISP_SetInputPortAttr(DISP_LAYER, DISP_INPUTPORT, &stInputPortAttr);
    MI_DISP_EnableInputPort(DISP_LAYER, DISP_INPUTPORT);
    MI_DISP_SetInputPortSyncMode(DISP_LAYER, DISP_INPUTPORT, E_MI_DISP_SYNC_MODE_FREE_RUN);

    if (is->decoder_type == AV_SOFT_DECODING || g_opts.enable_scaler)
    {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
        MI_SYS_ChnPort_t stDivpChnPort;
        MI_DIVP_ChnAttr_t stDivpChnAttr;
        MI_DIVP_OutputPortAttr_t stOutputPortAttr;

        #ifdef ENABLE_STR
        MI_DIVP_InitParam_t stInitDivpParam;
        memset(&stInitDivpParam, 0x0, sizeof(MI_DIVP_InitParam_t));
        stInitDivpParam.u32DevId = 0;
        stInitDivpParam.u8Data = NULL;
        MI_DIVP_InitDev(&stInitDivpParam);
        #endif

        memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
        stDivpChnAttr.bHorMirror            = FALSE;
        stDivpChnAttr.bVerMirror            = FALSE;
        stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
        stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
        stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
        stDivpChnAttr.stCropRect.u16X       = 0;
        stDivpChnAttr.stCropRect.u16Y       = 0;
        stDivpChnAttr.stCropRect.u16Width   = 0;
        stDivpChnAttr.stCropRect.u16Height  = 0;
        stDivpChnAttr.u32MaxWidth           = 1920;
        stDivpChnAttr.u32MaxHeight          = 1080;

        MI_DIVP_CreateChn(0, &stDivpChnAttr);
        MI_DIVP_SetChnAttr(0, &stDivpChnAttr);

        memset(&stOutputPortAttr, 0, sizeof(MI_DIVP_OutputPortAttr_t));
        stOutputPortAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
        stOutputPortAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
        stOutputPortAttr.u32Width           = is->dst_width;
        stOutputPortAttr.u32Height          = is->dst_height;
        MI_DIVP_SetOutputPortAttr(0, &stOutputPortAttr);
        MI_DIVP_StartChn(0);

        memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDispChnPort.eModId                = E_MI_MODULE_ID_DISP;
        stDispChnPort.u32DevId              = DISP_DEV;
        stDispChnPort.u32ChnId              = DISP_CHN;
        stDispChnPort.u32PortId             = DISP_INPUTPORT;

        memset(&stDivpChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDivpChnPort.eModId                = E_MI_MODULE_ID_DIVP;
        stDivpChnPort.u32DevId              = DIVP_DEV;
        stDivpChnPort.u32ChnId              = DIVP_CHN;
        stDivpChnPort.u32PortId             = DIVP_PORT;

        MI_SYS_SetChnOutputPortDepth(&stDivpChnPort, 0, 3);
        MI_SYS_BindChnPort(&stDivpChnPort, &stDispChnPort, 30, 30);
#elif (defined CHIP_IS_SS268)
        MI_SCL_DEV SclDevId = SCL_DEV;
        MI_SCL_PORT SclOutPortId = SCL_PORT;
        MI_SCL_CHANNEL SclChnId = SCL_CHN;
        MI_SCL_DevAttr_t stCreateDevAttr;
        MI_SCL_ChnParam_t stSclChnParam;
        MI_SCL_OutPortParam_t stSclOutputParam;
        MI_SYS_ChnPort_t stSclChnPort;

        memset(&stCreateDevAttr, 0x0, sizeof(MI_SCL_DevAttr_t));
        stCreateDevAttr.u32NeedUseHWOutPortMask = E_MI_SCL_HWSCL2;
        MI_SCL_CreateDevice(SclDevId, &stCreateDevAttr);

        MI_SCL_ChannelAttr_t  stSclChnAttr;
        memset(&stSclChnAttr, 0x0, sizeof(MI_SCL_ChannelAttr_t));
        MI_SCL_CreateChannel(SclDevId, SclChnId, &stSclChnAttr);

        memset(&stSclChnParam, 0x0, sizeof(MI_SCL_ChnParam_t));
        stSclChnParam.eRot = E_MI_SYS_ROTATE_NONE;
        MI_SCL_SetChnParam(SclDevId, SclChnId, &stSclChnParam);

        MI_SCL_StartChannel(SclDevId, SclChnId);

        memset(&stSclOutputParam, 0x0, sizeof(MI_SCL_OutPortParam_t));
        stSclOutputParam.stSCLOutputSize.u16Width   = is->dst_width;
        stSclOutputParam.stSCLOutputSize.u16Height  = is->dst_height;
        stSclOutputParam.eCompressMode      = E_MI_SYS_COMPRESS_MODE_NONE;
        stSclOutputParam.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
        stSclOutputParam.bMirror            = FALSE;
        stSclOutputParam.bFlip              = FALSE;

        MI_SCL_SetOutputPortParam(SclDevId, SclChnId, SclOutPortId, &stSclOutputParam);

        MI_SCL_EnableOutputPort(SclDevId, SclChnId, SclOutPortId);

        memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDispChnPort.eModId                = E_MI_MODULE_ID_DISP;
        stDispChnPort.u32DevId              = DISP_DEV;
        stDispChnPort.u32ChnId              = DISP_CHN;
        stDispChnPort.u32PortId             = DISP_INPUTPORT;

        memset(&stSclChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stSclChnPort.eModId                 = E_MI_MODULE_ID_SCL;
        stSclChnPort.u32DevId               = SCL_DEV;
        stSclChnPort.u32ChnId               = SCL_CHN;
        stSclChnPort.u32PortId              = SCL_PORT;

        MI_SYS_SetChnOutputPortDepth(ST_DEFAULT_SOC_ID, &stSclChnPort, 0, 3);
        MI_SYS_BindChnPort(ST_DEFAULT_SOC_ID, &stSclChnPort, &stDispChnPort, 30, 30);
#endif
    }

    memset(&stRotateConfig, 0, sizeof(MI_DISP_RotateConfig_t));
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    MI_DISP_SetVideoLayerRotateMode(DISP_LAYER, &stRotateConfig);

    return MI_SUCCESS;
}

int mm_video_deinit(void *args)
{
    player_stat_t *is = (player_stat_t *)args;

    MI_DISP_RotateConfig_t stRotateConfig;
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    MI_DISP_SetVideoLayerRotateMode(DISP_LAYER, &stRotateConfig);

    if (is->decoder_type == AV_SOFT_DECODING || g_opts.enable_scaler)
    {
#if ((defined CHIP_IS_SSD20X) || (defined CHIP_IS_SS22X))
        MI_SYS_ChnPort_t stDispChnPort;
        MI_SYS_ChnPort_t stDivpChnPort;

        memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDispChnPort.eModId                = E_MI_MODULE_ID_DISP;
        stDispChnPort.u32DevId              = DISP_DEV;
        stDispChnPort.u32ChnId              = DISP_CHN;
        stDispChnPort.u32PortId             = DISP_INPUTPORT;

        memset(&stDivpChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDivpChnPort.eModId                = E_MI_MODULE_ID_DIVP;
        stDivpChnPort.u32DevId              = DIVP_DEV;
        stDivpChnPort.u32ChnId              = DIVP_CHN;
        stDivpChnPort.u32PortId             = DIVP_PORT;

        MI_SYS_UnBindChnPort(&stDivpChnPort, &stDispChnPort);

        MI_DIVP_StopChn(0);
        MI_DIVP_DestroyChn(0);

        #ifdef ENABLE_STR
        MI_DIVP_DeInitDev();
        av_log(NULL, AV_LOG_WARNING, "DeInit divp and gfx module dev!\n");
        #endif
#elif (defined CHIP_IS_SS268)
        MI_SCL_DEV SclDevId = SCL_DEV;
        MI_SCL_PORT SclOutPortId = SCL_PORT;
        MI_SCL_CHANNEL SclChnId = SCL_CHN;
        MI_SYS_ChnPort_t stDispChnPort;
        MI_SYS_ChnPort_t stSclChnPort;

        memset(&stDispChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stDispChnPort.eModId                = E_MI_MODULE_ID_DISP;
        stDispChnPort.u32DevId              = DISP_DEV;
        stDispChnPort.u32ChnId              = DISP_CHN;
        stDispChnPort.u32PortId             = DISP_INPUTPORT;

        memset(&stSclChnPort, 0, sizeof(MI_SYS_ChnPort_t));
        stSclChnPort.eModId                 = E_MI_MODULE_ID_SCL;
        stSclChnPort.u32DevId               = SCL_DEV;
        stSclChnPort.u32ChnId               = SCL_CHN;
        stSclChnPort.u32PortId              = SCL_PORT;

        MI_SYS_UnBindChnPort(ST_DEFAULT_SOC_ID, &stSclChnPort, &stDispChnPort);

        MI_SCL_DisableOutputPort(SclDevId, SclChnId, SclOutPortId);
        MI_SCL_StopChannel(SclDevId, SclChnId);
        MI_SCL_DestroyChannel(SclDevId, SclChnId);
        MI_SCL_DestroyDevice(SclDevId);
#endif
    }
    else
    {
        if (is->play_status & AV_PLAY_ERROR)
        {
            MI_DISP_ClearInputPortBuffer(DISP_LAYER, DISP_INPUTPORT, TRUE);
            MI_DISP_HideInputPort(DISP_LAYER, DISP_INPUTPORT);
            MI_DISP_DisableInputPort(DISP_LAYER, DISP_INPUTPORT);
        }
        av_usleep(20 * 1000);
    }

#if (defined CHIP_IS_SSD20X || defined CHIP_IS_SS22X)
    MI_GFX_Close();
    #ifdef ENABLE_STR
    MI_DISP_DeInitDev();
    MI_SYS_DeInitDev();
    MI_GFX_DeInitDev();
    av_log(NULL, AV_LOG_WARNING, "DeInit disp and sys module dev!\n");
    #endif
#elif (defined CHIP_IS_SS268)
    MI_GFX_DEV stGfxDevId = 0;
    MI_GFX_Close(stGfxDevId);
#endif

    return 0;
}

static int mm_sys_malloc(void *name, void **vir_addr, void *phy_addr, int size)
{
    int ret;

#ifdef CHIP_IS_SS268
    ret = MI_SYS_MMA_Alloc(0, (MI_U8 *)name, (MI_U32)size, (MI_PHY *)phy_addr);
#else
    ret = MI_SYS_MMA_Alloc((MI_U8 *)name, (MI_U32)size, (MI_PHY *)phy_addr);
#endif
    if (ret != MI_SUCCESS) {
        av_log(NULL, AV_LOG_ERROR, "MI_SYS_MMA_Alloc Falied!\n");
        return -1;
    }

    ret = MI_SYS_Mmap(*(MI_PHY*)phy_addr, (MI_U32)size, (void **)vir_addr, TRUE);
    if (ret != MI_SUCCESS) {
        av_log(NULL, AV_LOG_ERROR, "MI_SYS_Mmap Falied!\n");
        return -1;
    }

    return 0;
}

static int mm_sys_free(void *vir_addr, unsigned long long phy_addr, int size)
{
    int ret;

    MI_SYS_FlushInvCache(vir_addr, (MI_U32)size);

    ret = MI_SYS_Munmap(vir_addr, (MI_U32)size);
    if (ret != MI_SUCCESS) {
        av_log(NULL, AV_LOG_ERROR, "MI_SYS_Munmap Falied!\n");
        return -1;
    }
#ifdef CHIP_IS_SS268
    ret = MI_SYS_MMA_Free(0, (MI_PHY)phy_addr);
#else
    ret = MI_SYS_MMA_Free((MI_PHY)phy_addr);
#endif
    if (ret != MI_SUCCESS) {
        av_log(NULL, AV_LOG_ERROR, "MI_SYS_MMA_Free Falied!\n");
        return -1;
    }

    return 0;
}

static int mm_player_callback(void)
{
    if (!g_mmplayer)
    {
        av_log(NULL, AV_LOG_ERROR, "g_mmplayer is null!\n");
        return -1;
    }
    memset(&(g_mmplayer->functions), 0x0, sizeof(player_func_t));
    g_mmplayer->functions.audio_init   = mm_audio_init;
    g_mmplayer->functions.audio_deinit = mm_audio_deinit;
    g_mmplayer->functions.audio_clear_buf = mm_audio_clear_buf;
    g_mmplayer->functions.audio_pause  = mm_audio_pause;
    g_mmplayer->functions.audio_resume = mm_audio_resume;
    g_mmplayer->functions.audio_play   = mm_audio_play;

    g_mmplayer->functions.video_init   = mm_video_init;
    g_mmplayer->functions.video_deinit = mm_video_deinit;
    g_mmplayer->functions.video_play   = mm_video_play;
    g_mmplayer->functions.video_putbuf = mm_video_putbuf;

    g_mmplayer->functions.sys_free     = mm_sys_free;
    g_mmplayer->functions.sys_malloc   = mm_sys_malloc;
    g_mmplayer->functions.audio_dump_ai_buf   = mm_audio_dump_ai_buf;
    g_mmplayer->functions.audio_release_ai_buf   = mm_audio_release_ai_buf;

    return 0;
}

static void * mm_layer_handler(void * arg)
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
        mplayer->audio_idx = -1;
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

int mm_player_open(const char *fp, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (g_mmplayer || width <= 0 || height <= 0) {
        av_log(NULL, AV_LOG_ERROR, "input parameter check error!\n");
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "### enter mm_player_open\n");
    av_log(NULL, AV_LOG_INFO, "%s\n", MACRO_TO_STRING(SSPLAYER_VERSION));

    pthread_mutex_init(&player_mutex, NULL);

    if (width < MIN_WIDTH || width > MAX_WIDTH)
    {
        av_log(NULL, AV_LOG_WARNING, "set width must be in [%d, %d]\n", MIN_WIDTH, MAX_HEIGHT);
        if (width < MIN_WIDTH)
            width = MIN_WIDTH;
        else if (width > MAX_WIDTH)
            width = MAX_WIDTH;
    }
    if (height < MIN_HEIGHT || height > MAX_HEIGHT)
    {
        av_log(NULL, AV_LOG_WARNING, "set height must be in [%d, %d]\n", MIN_HEIGHT, MAX_HEIGHT);
        if (height < MIN_HEIGHT)
            height = MIN_HEIGHT;
        else if (height > MAX_HEIGHT)
            height = MAX_HEIGHT;
    }

    g_mmplayer = player_init(fp);
    if (!g_mmplayer)
    {
        av_log(NULL, AV_LOG_ERROR, "player init failed[%s]\n",fp);
        return -1;
    }
    mm_player_callback();

    if (g_opts.video_rotate != AV_ROTATE_NONE && g_opts.video_rotate != AV_ROTATE_180)
    {
        g_mmplayer->in_height = width;
        g_mmplayer->in_width  = height;
    }
    else
    {
        g_mmplayer->in_height = height;
        g_mmplayer->in_width  = width;
    }
    g_mmplayer->pos_x     = x;
    g_mmplayer->pos_y     = y;
    g_mmplayer->display_mode = g_opts.video_rotate;
	g_mmplayer->uac_device = g_opts.uac_device;
	g_mmplayer->buf_handle_mode = g_opts.buf_handle_mode;

    av_log(NULL, AV_LOG_INFO, "set out width/height = [%d %d]\n", g_mmplayer->in_width, g_mmplayer->in_height);

    gettimeofday(&g_mmplayer->tim_open, NULL);
    if(mm_layer_handler(g_mmplayer) == NULL)
    {
        mm_player_close();
        return -1;
    }
    av_log(NULL, AV_LOG_INFO, "leave mm_player_open ###\n");

    return 0;
}

int mm_player_close(void)
{
    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_WARNING, "mm_player has been closed!\n");
        return -1;
    }

    int ret;

    av_log(NULL, AV_LOG_INFO, "### enter mm_player_close\n");

    if (g_mmplayer->play_status & AV_AUDIO_PAUSE) {
        mm_audio_resume();
    }

    pthread_mutex_lock(&player_mutex);

    ret = player_deinit(g_mmplayer);

    g_mmplayer = NULL;
    audio_mute = false;

    pthread_mutex_unlock(&player_mutex);
    pthread_mutex_destroy(&player_mutex);

    av_log(NULL, AV_LOG_INFO, "leave mm_player_close ###\n");

    return ret;
}

int mm_player_pause(void)
{
    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer || g_mmplayer->paused) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_pause failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "mm_player_pause!\n");
    toggle_pause(g_mmplayer);
    g_mmplayer->play_status |= AV_PLAY_PAUSE;

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_resume(void)
{
    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer || !g_mmplayer->paused) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_resume failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "mm_player_resume!\n");
    toggle_pause(g_mmplayer);
    g_mmplayer->play_status &= ~AV_PLAY_PAUSE;

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_getposition(double *position)
{
    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_getposition failed\n");
        *position = NAN;
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    *position = NAN;
    if (g_mmplayer->video_idx >= 0) {
        *position = g_mmplayer->video_clk.pts;
    } else {
        *position = g_mmplayer->audio_clk.pts;
    }

    if (isnan(*position)) {
        pthread_mutex_unlock(&player_mutex);
        return -1;
    } else {
        double start_time = g_mmplayer->p_fmt_ctx->start_time * av_q2d(AV_TIME_BASE_Q);
        *position = (isnan(start_time)) ? *position : (*position - start_time);
    }
    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_getduration(double *duration)
{
    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_getduration failed\n");
        *duration = NAN;
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    if (g_mmplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE) {
        *duration = g_mmplayer->p_fmt_ctx->duration * av_q2d(AV_TIME_BASE_Q);
        av_log(NULL, AV_LOG_INFO, "get file duration time [%.3lf]\n", *duration); 
    } else {
        av_log(NULL, AV_LOG_WARNING, "get invalid duration time\n"); 
        *duration = NAN;
    }

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_seek(double time)
{
    double pos = 0.0f;

    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_seek failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    if (g_mmplayer->seek_by_bytes) {
        av_log(NULL, AV_LOG_WARNING, "this file don't support to seek!\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    pos = get_master_clock(g_mmplayer);
    if (isnan(pos))
        pos = (double)g_mmplayer->seek_pos / AV_TIME_BASE;
    pos += time;
    if (g_mmplayer->p_fmt_ctx->start_time != AV_NOPTS_VALUE && pos <= g_mmplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
        pos = g_mmplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
    if (g_mmplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE && pos >= g_mmplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE)
        pos = g_mmplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE;
    av_log(NULL, AV_LOG_INFO, "start to seek to %.3f\n", pos);
    stream_seek(g_mmplayer, (int64_t)(pos * AV_TIME_BASE), (int64_t)(time * AV_TIME_BASE), g_mmplayer->seek_by_bytes);
    g_mmplayer->start_play = false;
    gettimeofday(&g_mmplayer->tim_open, NULL);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_seek2time(double time)
{
    double pos = 0.0f, diff = 0.0f, target = 0.0f;

    pthread_mutex_lock(&player_mutex);

    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_seek2time failed\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    if (g_mmplayer->seek_by_bytes) {
        av_log(NULL, AV_LOG_WARNING, "this file don't support to seek!\n");
        pthread_mutex_unlock(&player_mutex);
        return -1;
    }

    pos = get_master_clock(g_mmplayer);
    if (isnan(pos))
    {
        pos = (double)g_mmplayer->seek_pos / AV_TIME_BASE;
    }
    diff = time - pos;
    target = time;
    if (g_mmplayer->p_fmt_ctx->start_time != AV_NOPTS_VALUE && time <= g_mmplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE)
        target = g_mmplayer->p_fmt_ctx->start_time / (double)AV_TIME_BASE;
    if (g_mmplayer->p_fmt_ctx->duration != AV_NOPTS_VALUE && time >= g_mmplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE)
        target= g_mmplayer->p_fmt_ctx->duration / (double)AV_TIME_BASE;
    av_log(NULL, AV_LOG_INFO, "now [%.3f], start to seek2 to %.3f\n", pos, target);
    stream_seek(g_mmplayer, (int64_t)(target * AV_TIME_BASE), (int64_t)(diff * AV_TIME_BASE), g_mmplayer->seek_by_bytes);
    g_mmplayer->start_play = false;
    gettimeofday(&g_mmplayer->tim_open, NULL);

    pthread_mutex_unlock(&player_mutex);

    return 0;
}

int mm_player_set_volumn(int volumn)
{
    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_INFO, "mm_player_set_volumn failed!\n");
        return -1;
    }

    if (g_mmplayer->audio_idx >= 0) {
        MI_S32 vol;
        MI_AO_ChnState_t stAoState;
        MI_AO_CHN AoChn = AUDIO_CHN;

        if (volumn > 0) {
            //vol = volumn * (MAX_ADJUST_AO_VOLUME - MIN_ADJUST_AO_VOLUME) / 100 + MIN_ADJUST_AO_VOLUME;
            //vol = (vol > MAX_ADJUST_AO_VOLUME) ? MAX_ADJUST_AO_VOLUME : vol;
            //vol = (vol < MIN_ADJUST_AO_VOLUME) ? MIN_ADJUST_AO_VOLUME : vol;
            vol = (int)(log10(volumn * 1.0) * 45 - 60);
            vol = (vol > 30) ? 30 : vol;
            audio_mute = false;
        } else {
            vol = MIN_AO_VOLUME;
            audio_mute = true;
        }

        memset(&stAoState, 0, sizeof(MI_AO_ChnState_t));
        if (MI_SUCCESS == MI_AO_QueryChnStat(g_opts.audio_dev, AUDIO_CHN, &stAoState))
        {
#ifdef CHIP_IS_SS268
            MI_AO_SetVolume(g_opts.audio_dev, AoChn, vol, E_MI_AO_GAIN_FADING_16_SAMPLE);
            MI_AO_SetMute(g_opts.audio_dev, AoChn, audio_mute);
#elif defined CHIP_IS_SS22X
            MI_AO_SetVolume(g_opts.audio_dev, AoChn, vol, E_MI_AO_GAIN_FADING_16_SAMPLE);
            MI_AO_SetMute(g_opts.audio_dev, AoChn, audio_mute);
#else
            MI_AO_SetVolume(g_opts.audio_dev, vol);
            MI_AO_SetMute(g_opts.audio_dev, audio_mute);
#endif
        }
    }

    return 0;
}

int mm_player_set_mute(bool mute)
{
    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_set_mute failed!\n");
        return -1;
    }

    if (g_mmplayer->audio_idx >= 0) {
        audio_mute = mute;
#ifdef CHIP_IS_SS268
        MI_AO_CHN AoChn = AUDIO_CHN;
        MI_AO_SetMute(g_opts.audio_dev, AoChn, audio_mute);
#elif defined CHIP_IS_SS22X
        MI_AO_CHN AoChn = AUDIO_CHN;
        MI_AO_SetMute(g_opts.audio_dev, AoChn, audio_mute);
#else
        MI_AO_SetMute(g_opts.audio_dev, audio_mute);
#endif
        if (audio_mute)
            g_mmplayer->play_status |= AV_AUDIO_MUTE;
        else
            g_mmplayer->play_status &= ~AV_AUDIO_MUTE;
    }

    return 0;
}

int mm_player_set_window(int x, int y, int width, int height)
{
    if (width < 8 || height < 8) {
        av_log(NULL, AV_LOG_ERROR, "set window param is invalid!\n");
        return -1;
    }

    if (g_mmplayer && g_opts.enable_scaler)
    {
        g_mmplayer->src_width  = width;
        g_mmplayer->src_height = height;
        g_mmplayer->out_width  = width;
        g_mmplayer->out_height = height;
        g_mmplayer->pos_x = x;
        g_mmplayer->pos_y = y;

#if (defined CHIP_IS_SSD20X || defined CHIP_IS_SS22X)
        MI_DIVP_OutputPortAttr_t stOutputPortAttr;

        MI_DIVP_GetOutputPortAttr(0, &stOutputPortAttr);
        stOutputPortAttr.u32Width  = g_mmplayer->src_width;
        stOutputPortAttr.u32Height = g_mmplayer->src_height;
        MI_DIVP_SetOutputPortAttr(0, &stOutputPortAttr);
#elif (defined CHIP_IS_SS268)
        MI_SCL_DEV SclDevId = SCL_DEV;
        MI_SCL_PORT SclOutPortId = SCL_PORT;
        MI_SCL_CHANNEL SclChnId = SCL_CHN;
        MI_SCL_OutPortParam_t stSclOutputParam;

        memset(&stSclOutputParam, 0x0, sizeof(MI_SCL_OutPortParam_t));
        stSclOutputParam.stSCLOutCropRect.u16X      = 0;
        stSclOutputParam.stSCLOutCropRect.u16Y      = 0;
        stSclOutputParam.stSCLOutCropRect.u16Width  = 0;
        stSclOutputParam.stSCLOutCropRect.u16Height = 0;
        stSclOutputParam.stSCLOutputSize.u16Width   = g_mmplayer->src_width;
        stSclOutputParam.stSCLOutputSize.u16Height  = g_mmplayer->src_height;
        stSclOutputParam.eCompressMode = E_MI_SYS_COMPRESS_MODE_FRAME;
        stSclOutputParam.ePixelFormat  = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
        stSclOutputParam.bMirror       = FALSE;
        stSclOutputParam.bFlip         = FALSE;

        MI_SCL_SetOutputPortParam(SclDevId, SclChnId, SclOutPortId, &stSclOutputParam);
#endif

        MI_DISP_InputPortAttr_t stInputPortAttr;
        MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr);
        stInputPortAttr.u16SrcWidth         = g_mmplayer->src_width;
        stInputPortAttr.u16SrcHeight        = g_mmplayer->src_height;
        stInputPortAttr.stDispWin.u16X      = g_mmplayer->pos_x;
        stInputPortAttr.stDispWin.u16Y      = g_mmplayer->pos_y;
        stInputPortAttr.stDispWin.u16Width  = g_mmplayer->out_width;
        stInputPortAttr.stDispWin.u16Height = g_mmplayer->out_height;
        MI_DISP_SetInputPortAttr(DISP_LAYER, DISP_INPUTPORT, &stInputPortAttr);
    }
    else
    {
        av_log(NULL, AV_LOG_INFO, "cannot set windows when disable scaler!\n");
        return -1;
    }

    av_log(NULL, AV_LOG_INFO, "mm_player_set_window =  [%d %d %d %d]\n", x, y, width, height);

    return 0;
}

int mm_player_set_opts(const char *key, const char *value, int flags)
{
    if (!strcmp(key, "audio_only")) {
        g_opts.audio_only = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer audio_only = %d\n", g_opts.audio_only);

        return 0;
    }

    if (!strcmp(key, "video_only")) {
        g_opts.video_only = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer video_only = %d\n", g_opts.video_only);

        return 0;
    }

    if (!strcmp(key, "video_rotate")) {
        g_opts.video_rotate = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer video_rotate = %d\n", g_opts.video_rotate);

        if (g_opts.video_rotate > AV_ROTATE_270 || g_opts.video_rotate < AV_ROTATE_NONE)
        {
            av_log(NULL, AV_LOG_ERROR, "video rotate parameter error!\n");
            g_opts.video_rotate = AV_ROTATE_NONE;
            return -1;
        }

        return 0;
    }

    if (!strcmp(key, "video_ratio")) {
        g_opts.video_ratio = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer video_ratio = %d\n", g_opts.video_ratio);

        if (g_opts.video_ratio > AV_SAR_16_9_MODE || g_opts.video_ratio < AV_ORIGIN_MODE)
        {
            av_log(NULL, AV_LOG_ERROR, "video ratio parameter error!\n");
            g_opts.video_ratio = AV_SCREEN_MODE;
            return -1;
        }

        return 0;
    }

    if (!strcmp(key, "audio_device")) {
        g_opts.audio_dev = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer audio_dev = %d\n", g_opts.audio_dev);

        if (g_opts.audio_dev > 4 || g_opts.audio_dev < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "video audio device error!\n");
            g_opts.audio_dev = AUDIO_DEV;
            return -1;
        }

        return 0;
    }

    if (!strcmp(key, "audio_layout")) {
        g_opts.audio_layout = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer audio_layout = %d\n", g_opts.audio_layout);

        return 0;
    }

    if (!strcmp(key, "resolution") && value) {
        memset(g_opts.resolution, '\0', sizeof(g_opts.resolution));
        memcpy(g_opts.resolution, value, strlen(value));
        int resolution = atoi(g_opts.resolution);
        av_log(NULL, AV_LOG_INFO, "mmplayer resolution = %d\n", resolution);

        return 0;
    }

    if (!strcmp(key, "enable_scaler")) {
        g_opts.enable_scaler = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer enable_scaler = %d\n", g_opts.enable_scaler);

        return 0;
    }

    if (!strcmp(key, "play_mode")) {
        g_opts.play_mode = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer play_mode = %d\n", g_opts.play_mode);

        return 0;
    }
    if (!strcmp(key, "uac_device")) {
        g_opts.uac_device = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer uac_device = %d\n", g_opts.uac_device);

        return 0;
    }
    if (!strcmp(key, "buf_handle_mode")) {
        g_opts.buf_handle_mode = flags;
        av_log(NULL, AV_LOG_INFO, "mmplayer buf_handle_mode = %d\n", g_opts.buf_handle_mode);

        return 0;
    }
    av_log(NULL, AV_LOG_ERROR, "unknowed key words: %s!\n", key);
    return -1;
}

int mm_player_get_status(void)
{
    if (!g_mmplayer) {
        av_log(NULL, AV_LOG_ERROR, "mm_player_get_status failed!\n");
        return -1;
    }

    return g_mmplayer->play_status;
}

int mm_player_flush_screen(bool enable)
{
    MI_DISP_QueryChannelStatus_t stDispStatus;
    MI_DISP_QueryInputPortStat(DISP_LAYER, DISP_INPUTPORT, &stDispStatus);

    if (g_mmplayer) {
        if (enable) {
            MI_DISP_HideInputPort(DISP_LAYER, DISP_INPUTPORT);
        } else {
            MI_DISP_ShowInputPort(DISP_LAYER, DISP_INPUTPORT);
        }
    } else {
        if (enable && stDispStatus.bEnable) {
            MI_DISP_DisableInputPort(DISP_LAYER, DISP_INPUTPORT);
        }
    }

    return 0;
}
