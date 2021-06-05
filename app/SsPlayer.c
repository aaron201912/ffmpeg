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
//封装格式
#include "libavformat/avformat.h"
//解码
#include "libavcodec/avcodec.h"
//缩放
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

//sstar sdk lib
#include "mi_common.h"
#include "mi_common_datatype.h"
#include "mi_sys.h"
#include "mi_sys_datatype.h"
#include "mi_ao.h"
#include "mi_ao_datatype.h"
#include "mi_vdec.h"
#include "mi_vdec_datatype.h"
#include "mi_hdmi.h"
#include "mi_hdmi_datatype.h"
#include "mi_disp.h"
#include "mi_disp_datatype.h"
#include "mi_panel.h"
#include "mi_panel_datatype.h"

#ifdef CHIP_IS_SSD20X
#include "SAT070CP50_1024x600.h"
#endif

#ifdef ST_DEFAULT_SOC_ID
#undef ST_DEFAULT_SOC_ID
#define ST_DEFAULT_SOC_ID 0
#else
#define ST_DEFAULT_SOC_ID 0
#endif

//sdk audio input/outout param
#define     AUDIO_INPUT_SAMPRATE        48000
#define     AUDIO_INPUT_CHLAYOUT        AV_CH_LAYOUT_MONO
#define     AUDIO_INPUT_SAMPFMT         AV_SAMPLE_FMT_S16

#define     AUDIO_OUTPUT_SAMPRATE       E_MI_AUDIO_SAMPLE_RATE_48000
#define     AUDIO_OUTPUT_CHLAYOUT       E_MI_AUDIO_SOUND_MODE_MONO
#define     AUDIO_OUTPUT_SAMPFMT        E_MI_AUDIO_BIT_WIDTH_16

#define     MI_AUDIO_MAX_DATA_SIZE      25000

#ifdef SUPPORT_HDMI
#ifdef CHIP_IS_SSD20X
#define AUDIO_DEV   3
#else
#define AUDIO_DEV   4
#endif
#else
#define AUDIO_DEV   0
#endif

//func return value
#define     SUCCESS     0
#define     FAIL        1

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

#define MI_AUDIO_SAMPLE_PER_FRAME 4096

#define WAV_G711A 0x06
#define WAV_G711U 0x7
#define WAV_G726 0x45
#define WAV_PCM  0x1

#define G726_16 2
#define G726_24 3
#define G726_32 4
#define G726_40 5

#define DMA_BUF_SIZE_8K (8000)
#define DMA_BUF_SIZE_16K    (16000)
#define DMA_BUF_SIZE_32K    (32000)
#define DMA_BUF_SIZE_48K    (48000)
/*=============================================================*/
// Global Variable definition
/*=============================================================*/
// WAVE file header format
typedef struct _WavHeader_s{
    MI_U8   riff[4];                // RIFF string
    MI_U32  ChunkSize;              // overall size of file in bytes
    MI_U8   wave[4];                // WAVE string
    MI_U8   fmt_chunk_marker[4];    // fmt string with trailing null char
    MI_U32  length_of_fmt;          // length of the format data
    MI_U16  format_type;            // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
    MI_U16  channels;               // no.of channels
    MI_U32  sample_rate;            // sampling rate (blocks per second)
    MI_U32  byterate;               // SampleRate * NumChannels * BitsPerSample/8
    MI_U16  block_align;            // NumChannels * BitsPerSample/8
    MI_U16  bits_per_sample;        // bits per sample, 8- 8bits, 16- 16 bits etc
    MI_U8   data_chunk_header [4];  // DATA string or FLLR string
    MI_U32  data_size;              // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
}_WavHeader_t;

typedef struct WAVE_FORMAT
{
    signed short wFormatTag;
    signed short wChannels;
    unsigned int  dwSamplesPerSec;
    unsigned int  dwAvgBytesPerSec;
    signed short wBlockAlign;
    signed short wBitsPerSample;
} WaveFormat_t;

typedef struct WAVEFILEHEADER
{
    char chRIFF[4];
    unsigned int  dwRIFFLen;
    char chWAVE[4];
    char chFMT[4];
    unsigned int  dwFMTLen;
    WaveFormat_t wave;
    char chDATA[4];
    unsigned int  dwDATALen;
} WaveFileHeader_t;

typedef enum
{
    E_SOUND_MODE_MONO =0, /* mono */
    E_SOUND_MODE_STERO =1, /* stereo */
}SoundMode_e;

typedef enum
{
    E_SAMPLE_RATE_8000 =8000, /* 8kHz sampling rate */
    E_SAMPLE_RATE_16000 =16000, /* 16kHz sampling rate */
    E_SAMPLE_RATE_32000 =32000, /* 32kHz sampling rate */
    E_SAMPLE_RATE_48000 =48000, /* 48kHz sampling rate */
}SampleRate_e;

typedef enum
{
    E_ANEC_TYPE_G711A = 0,
    E_ANEC_TYPE_G711U,
    E_AENC_TYPE_G726_16,
    E_AENC_TYPE_G726_24,
    E_AENC_TYPE_G726_32,
    E_AENC_TYPE_G726_40,
    PCM,
}AencType_e;


#define ALIGN_BACK(x, a)            (((x) / (a)) * (a))
#define DISP_WIDTH_ALIGN    2
#define DISP_HEIGHT_ALIGN   2

#define NALU_PACKET_SIZE            512*1024
#define MI_U32VALUE(pu8Data, index) (pu8Data[index]<<24)|(pu8Data[index+1]<<16)|(pu8Data[index+2]<<8)|(pu8Data[index+3])

#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE MAKE_YUYV_VALUE(29,225,107)


#define SAVE_YUV_TO_FILE 0

int add_wave_header(WaveFileHeader_t* tWavHead, AencType_e eAencType, SoundMode_e eSoundMode, SampleRate_e eSampleRate, int raw_len)
{
    tWavHead->chRIFF[0] = 'R';
    tWavHead->chRIFF[1] = 'I';
    tWavHead->chRIFF[2] = 'F';
    tWavHead->chRIFF[3] = 'F';

    tWavHead->chWAVE[0] = 'W';
    tWavHead->chWAVE[1] = 'A';
    tWavHead->chWAVE[2] = 'V';
    tWavHead->chWAVE[3] = 'E';

    tWavHead->chFMT[0] = 'f';
    tWavHead->chFMT[1] = 'm';
    tWavHead->chFMT[2] = 't';
    tWavHead->chFMT[3] = 0x20;
    tWavHead->dwFMTLen = 0x10;

    if(eAencType == E_ANEC_TYPE_G711A)
    {
        tWavHead->wave.wFormatTag = 0x06;
    }

    if(eAencType == E_ANEC_TYPE_G711U)
    {
        tWavHead->wave.wFormatTag = 0x07;
    }

    if(eAencType == E_ANEC_TYPE_G711U || eAencType == E_ANEC_TYPE_G711A)
    {
        if(eSoundMode == E_SOUND_MODE_MONO)
            tWavHead->wave.wChannels = 0x01;
        else
            tWavHead->wave.wChannels = 0x02;

        tWavHead->wave.wBitsPerSample = 8;//bitWidth;g711encode鍑烘潵鏄?bit锛岃繖閲岄渶瑕佸啓姝?        tWavHead->wave.dwSamplesPerSec = eSampleRate;
        tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
        tWavHead->wave.wBlockAlign = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.wChannels) / 8;
    }
    else if(eAencType == PCM)
    {
        if(eSoundMode == E_SOUND_MODE_MONO)
            tWavHead->wave.wChannels = 0x01;
        else
            tWavHead->wave.wChannels = 0x02;

        tWavHead->wave.wFormatTag = 0x1;
        tWavHead->wave.wBitsPerSample = 16; //16bit
        tWavHead->wave.dwSamplesPerSec = eSampleRate;
        tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample  * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
        tWavHead->wave.wBlockAlign = 1024 ;
    }
    else //g726
    {
        tWavHead->wave.wFormatTag = 0x45;
        tWavHead->wave.wChannels  = 0x01;
        switch(eAencType)
        {
            case E_AENC_TYPE_G726_40:
                tWavHead->wave.wBitsPerSample = 5;
                tWavHead->wave.wBlockAlign =  5;
                break;
            case E_AENC_TYPE_G726_32:
                tWavHead->wave.wBitsPerSample = 4;
                tWavHead->wave.wBlockAlign =  4;
                break;
            case E_AENC_TYPE_G726_24:
                tWavHead->wave.wBitsPerSample = 3;
                tWavHead->wave.wBlockAlign =  3;
                break;
            case E_AENC_TYPE_G726_16:
                tWavHead->wave.wBitsPerSample = 2;
                tWavHead->wave.wBlockAlign =  2;
                break;
            default:
                printf("eAencType error:%d\n", eAencType);
                return -1;
        }

        tWavHead->wave.dwSamplesPerSec = eSampleRate;
        tWavHead->wave.dwAvgBytesPerSec = (tWavHead->wave.wBitsPerSample * tWavHead->wave.dwSamplesPerSec * tWavHead->wave.wChannels) / 8;
    }

    tWavHead->chDATA[0] = 'd';
    tWavHead->chDATA[1] = 'a';
    tWavHead->chDATA[2] = 't';
    tWavHead->chDATA[3] = 'a';
    tWavHead->dwDATALen = raw_len;
    tWavHead->dwRIFFLen = raw_len + sizeof(WaveFileHeader_t) - 8;

    return -1;
}

static MI_S32 sstar_sys_init(void)
{
    MI_SYS_Version_t stVersion;
    MI_U64 u64Pts = 0;

#ifdef CHIP_IS_SSD20X
    STCHECKRESULT(MI_SYS_Init());
    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
    STCHECKRESULT(MI_SYS_GetVersion(&stVersion));
    STCHECKRESULT(MI_SYS_GetCurPts(&u64Pts));
    u64Pts = 0xF1237890F1237890;
    STCHECKRESULT(MI_SYS_InitPtsBase(u64Pts));
    u64Pts = 0xE1237890E1237890;
    STCHECKRESULT(MI_SYS_SyncPts(u64Pts));
#else
    STCHECKRESULT(MI_SYS_Init(ST_DEFAULT_SOC_ID));
    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
    STCHECKRESULT(MI_SYS_GetVersion(ST_DEFAULT_SOC_ID, &stVersion));
    STCHECKRESULT(MI_SYS_GetCurPts(ST_DEFAULT_SOC_ID, &u64Pts));
    u64Pts = 0xF1237890F1237890;
    STCHECKRESULT(MI_SYS_InitPtsBase(ST_DEFAULT_SOC_ID, u64Pts));
    u64Pts = 0xE1237890E1237890;
    STCHECKRESULT(MI_SYS_SyncPts(ST_DEFAULT_SOC_ID, u64Pts));
#endif
    return MI_SUCCESS;
}

static MI_S32 sstar_sys_deinit(void)
{
#ifdef CHIP_IS_SSD20X
    STCHECKRESULT(MI_SYS_Exit());
#else
    STCHECKRESULT(MI_SYS_Exit(ST_DEFAULT_SOC_ID));
#endif
    return MI_SUCCESS;
}

static MI_S32 hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
{
    switch (Event)
    {
        case E_MI_HDMI_EVENT_HOTPLUG:
            printf("E_MI_HDMI_EVENT_HOTPLUG.\n");
            break;
        case E_MI_HDMI_EVENT_NO_PLUG:
            printf("E_MI_HDMI_EVENT_NO_PLUG.\n");
            break;
        default:
            printf("Unsupport event.\n");
            break;
    }

    return MI_SUCCESS;
}

static MI_S32 sstar_hdmi_init(MI_DISP_OutputTiming_e stHdmiMode)
{
    MI_HDMI_InitParam_t stInitParam;
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    MI_HDMI_Attr_t stAttr;
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;

    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.u32BgColor = YUYV_BLACK;
    stPubAttr.eIntfType  = E_MI_DISP_INTF_HDMI;
    stPubAttr.eIntfSync  = stHdmiMode;//E_MI_DISP_OUTPUT_1080P60
    STCHECKRESULT(MI_DISP_SetPubAttr(0, &stPubAttr));
    STCHECKRESULT(MI_DISP_Enable(0));

    memset(&stLayerAttr, 0, sizeof(stLayerAttr));
    stLayerAttr.stVidLayerDispWin.u16X      = 0;
    stLayerAttr.stVidLayerDispWin.u16Y      = 0;
    stLayerAttr.stVidLayerSize.u16Width     = 1920;
    stLayerAttr.stVidLayerSize.u16Height    = 1080;
    if(stHdmiMode == E_MI_DISP_OUTPUT_720P60) {
        stLayerAttr.stVidLayerDispWin.u16Width  = 1280;
        stLayerAttr.stVidLayerDispWin.u16Height = 720;
    } else {
        stLayerAttr.stVidLayerDispWin.u16Width  = 1920;
        stLayerAttr.stVidLayerDispWin.u16Height = 1080;
    }
    STCHECKRESULT(MI_DISP_BindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_SetVideoLayerAttr(0, &stLayerAttr));
    STCHECKRESULT(MI_DISP_EnableVideoLayer(0));

    STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.u16SrcWidth         = stLayerAttr.stVidLayerDispWin.u16Width;
    stInputPortAttr.u16SrcHeight        = stLayerAttr.stVidLayerDispWin.u16Height;
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = stLayerAttr.stVidLayerDispWin.u16Width;
    stInputPortAttr.stDispWin.u16Height = stLayerAttr.stVidLayerDispWin.u16Height;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));

    memset(&stInitParam, 0x0, sizeof(MI_HDMI_InitParam_t));
    stInitParam.pCallBackArgs = NULL;
    stInitParam.pfnHdmiEventCallback = hdmi_callback_impl;
    STCHECKRESULT(MI_HDMI_Init(&stInitParam));
    STCHECKRESULT(MI_HDMI_Open(eHdmi));

    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    stAttr.stEnInfoFrame.bEnableAudInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame  = FALSE;
    stAttr.stAudioAttr.bEnableAudio    = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 0;
    stAttr.stAudioAttr.eBitDepth       = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType       = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate     = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo    = TRUE;
    stAttr.stVideoAttr.eColorType      = E_MI_HDMI_COLOR_TYPE_RGB444;
    stAttr.stVideoAttr.eDeepColorMode  = E_MI_HDMI_DEEP_COLOR_MAX;
    if (stHdmiMode == E_MI_DISP_OUTPUT_720P60)
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_720_60P;
    else
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_1080_60P;
    stAttr.stVideoAttr.eOutputMode     = E_MI_HDMI_OUTPUT_MODE_HDMI;
    STCHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr));
    STCHECKRESULT(MI_HDMI_Start(eHdmi));

    return MI_SUCCESS;
}

static MI_S32 sstar_hdmi_deinit(void)
{
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;

    //STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));

    STCHECKRESULT(MI_HDMI_Stop(eHdmi));
    STCHECKRESULT(MI_HDMI_Close(eHdmi));
    STCHECKRESULT(MI_HDMI_DeInit());

    return MI_SUCCESS;
}

static int sstar_ao_deinit(void)
{
    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
    MI_AO_CHN AoChn = 0;

    /* disable ao channel of */
    ExecFunc(MI_AO_DisableChn(AoDevId, AoChn), MI_SUCCESS);
    /* disable ao device */
    ExecFunc(MI_AO_Disable(AoDevId), MI_SUCCESS);

    return 0;
}

static int sstar_ao_init(void)
{
    MI_AUDIO_Attr_t stSetAttr;
    MI_AUDIO_Attr_t stGetAttr;
    MI_AUDIO_DEV AoDevId = AUDIO_DEV;
    MI_AO_CHN AoChn = 0;

    MI_S32 s32SetVolumeDb;
    MI_S32 s32GetVolumeDb;

    //set Ao Attr struct
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
    stSetAttr.eBitwidth = AUDIO_OUTPUT_SAMPFMT;
    stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stSetAttr.u32FrmNum = 6;
    stSetAttr.u32PtNumPerFrm = MI_AUDIO_SAMPLE_PER_FRAME;
    stSetAttr.u32ChnCnt   = 1;
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

static int sstar_panel_init(void)
{
#ifdef CHIP_IS_SSD20X
    MI_DISP_PubAttr_t stDispPubAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    memset(&stDispPubAttr, 0x0, sizeof(MI_DISP_PubAttr_t));
    stDispPubAttr.stSyncInfo.u16Vact       = stPanelParam.u16Height;
    stDispPubAttr.stSyncInfo.u16Vbb        = stPanelParam.u16VSyncBackPorch;
    stDispPubAttr.stSyncInfo.u16Vfb        = stPanelParam.u16VTotal - (stPanelParam.u16VSyncWidth +
                                             stPanelParam.u16Height + stPanelParam.u16VSyncBackPorch);
    stDispPubAttr.stSyncInfo.u16Hact       = stPanelParam.u16Width;
    stDispPubAttr.stSyncInfo.u16Hbb        = stPanelParam.u16HSyncBackPorch;
    stDispPubAttr.stSyncInfo.u16Hfb        = stPanelParam.u16HTotal - (stPanelParam.u16HSyncWidth +
                                             stPanelParam.u16Width + stPanelParam.u16HSyncBackPorch);
    stDispPubAttr.stSyncInfo.u16Bvact      = 0;
    stDispPubAttr.stSyncInfo.u16Bvbb       = 0;
    stDispPubAttr.stSyncInfo.u16Bvfb       = 0;
    stDispPubAttr.stSyncInfo.u16Hpw        = stPanelParam.u16HSyncWidth;
    stDispPubAttr.stSyncInfo.u16Vpw        = stPanelParam.u16VSyncWidth;
    stDispPubAttr.stSyncInfo.u32FrameRate  = stPanelParam.u16DCLK * 1000000 / (stPanelParam.u16HTotal * stPanelParam.u16VTotal);
    stDispPubAttr.eIntfSync                = E_MI_DISP_OUTPUT_USER;
    stDispPubAttr.eIntfType                = E_MI_DISP_INTF_LCD;
    stDispPubAttr.u32BgColor               = YUYV_BLACK;

    MI_DISP_SetPubAttr(0, &stDispPubAttr);
    MI_DISP_Enable(0);
    MI_DISP_BindVideoLayer(0, 0);
    MI_DISP_EnableVideoLayer(0);

    MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr);
    stInputPortAttr.u16SrcWidth         = stDispPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.u16SrcHeight        = stDispPubAttr.stSyncInfo.u16Vact;
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = stDispPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.stDispWin.u16Height = stDispPubAttr.stSyncInfo.u16Vact;

    MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr);
    MI_DISP_EnableInputPort(0, 0);
    MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN);

    MI_PANEL_Init(E_MI_PNL_LINK_TTL);
    MI_PANEL_SetPanelParam(&stPanelParam);
#else
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_PANEL_IntfType_e eIntfType;

    //init panel
    eIntfType = E_MI_PNL_INTF_TTL;
    STCHECKRESULT(MI_PANEL_Init(eIntfType));

    //config disp
    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.u32BgColor = YUYV_BLACK;
    stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_USER;
    stPubAttr.eIntfType = E_MI_DISP_INTF_TTL;
    MI_DISP_SetPubAttr(0, &stPubAttr);
    STCHECKRESULT(MI_DISP_Enable(0));

    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.eIntfType = E_MI_DISP_INTF_TTL;
    MI_DISP_GetPubAttr(0,&stPubAttr);

    memset(&stLayerAttr, 0, sizeof(MI_DISP_VideoLayerAttr_t));
    stLayerAttr.stVidLayerDispWin.u16X = 0;
    stLayerAttr.stVidLayerDispWin.u16Y = 0;
    stLayerAttr.stVidLayerDispWin.u16Width  = stPubAttr.stSyncInfo.u16Hact;
    stLayerAttr.stVidLayerDispWin.u16Height = stPubAttr.stSyncInfo.u16Vact;
    STCHECKRESULT(MI_DISP_BindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_SetVideoLayerAttr(0, &stLayerAttr));
    STCHECKRESULT(MI_DISP_EnableVideoLayer(0));

    memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
    stInputPortAttr.u16SrcWidth  = stPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.u16SrcHeight = stPubAttr.stSyncInfo.u16Vact;
    stInputPortAttr.stDispWin.u16X = 0;
    stInputPortAttr.stDispWin.u16Y = 0;
    stInputPortAttr.stDispWin.u16Width  = stPubAttr.stSyncInfo.u16Hact;
    stInputPortAttr.stDispWin.u16Height = stPubAttr.stSyncInfo.u16Vact;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
#endif

    return 0;
}

static int sstar_panel_deinit(void)
{
    STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));
    STCHECKRESULT(MI_PANEL_DeInit());

    return 0;
}

static void sstar_getpanel_wh(uint16_t *width, uint16_t *height)
{
#ifndef SUPPORT_HDMI
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_GetPubAttr(0,&stPubAttr);
    *width = stPubAttr.stSyncInfo.u16Hact;
    *height = stPubAttr.stSyncInfo.u16Vact;
#else
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_GetVideoLayerAttr(0, &stLayerAttr);
    *width = stLayerAttr.stVidLayerDispWin.u16Width;
    *height = stLayerAttr.stVidLayerDispWin.u16Height;
#endif
    printf("sstar_getpanel_wh = [%d %d]\n", *width, *height);
}

static MI_S32 sstar_module_unbind(void)
{
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;

    //bind vdec to disp
    stSrcChnPort.eModId    = E_MI_MODULE_ID_VDEC;
    stSrcChnPort.u32DevId  = 0;
    stSrcChnPort.u32ChnId  = 0;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId    = E_MI_MODULE_ID_DISP;
    stDstChnPort.u32DevId  = 0;
    stDstChnPort.u32ChnId  = 0;
    stDstChnPort.u32PortId = 0;
#ifdef CHIP_IS_SSD20X
    STCHECKRESULT(MI_SYS_UnBindChnPort(&stSrcChnPort, &stDstChnPort));
#else
    STCHECKRESULT(MI_SYS_UnBindChnPort(ST_DEFAULT_SOC_ID, &stSrcChnPort, &stDstChnPort));
#endif

    STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));

    return SUCCESS;
}


static MI_S32 sstar_module_bind(void)
{
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;

    //bind vdec to disp
    stSrcChnPort.eModId    = E_MI_MODULE_ID_VDEC;
    stSrcChnPort.u32DevId  = 0;
    stSrcChnPort.u32ChnId  = 0;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId    = E_MI_MODULE_ID_DISP;
    stDstChnPort.u32DevId  = 0;
    stDstChnPort.u32ChnId  = 0;
    stDstChnPort.u32PortId = 0;
#ifdef CHIP_IS_SSD20X
    STCHECKRESULT(MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, 30, 30));
#else
    STCHECKRESULT(MI_SYS_BindChnPort(ST_DEFAULT_SOC_ID, &stSrcChnPort, &stDstChnPort, 30, 30));
#endif
    return SUCCESS;
}

int sstar_sdk_init(void)
{
    //init system
    sstar_sys_init();

    //init audio
    sstar_ao_init();

    //init hdmi/panel
#ifdef SUPPORT_HDMI
    sstar_hdmi_init(E_MI_DISP_OUTPUT_1080P60);
#else
    sstar_panel_init();
#endif

    return 0;
}

int sstar_sdk_deinit(void)
{
#ifdef SUPPORT_HDMI
    sstar_hdmi_deinit();
#else
    sstar_panel_deinit();
#endif

    sstar_ao_deinit();

    sstar_sys_deinit();

    return MI_SUCCESS;
}

static bool b_exit = false;
MI_U16 u16OutWidth, u16OutHeight;
MI_U16 u16SrcWidth, u16SrcHeight;

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

static int sstar_video_config(AVCodecContext *pVideoCodeCtx)
{
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_RotateConfig_t stRotateConfig;

    //获取屏幕宽高参数
    sstar_getpanel_wh(&u16OutWidth, &u16OutHeight);

    //取较小值进行缩小
    u16SrcWidth  = FFMIN(pVideoCodeCtx->width , u16OutWidth);
    u16SrcHeight = FFMIN(pVideoCodeCtx->height, u16OutHeight);
    pVideoCodeCtx->flags  = u16SrcWidth;
    pVideoCodeCtx->flags2 = u16SrcHeight;

    //重设DISP参数
    memset(&stInputPortAttr, 0, sizeof(stInputPortAttr));
    STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = u16OutWidth;
    stInputPortAttr.stDispWin.u16Height = u16OutHeight;
    stInputPortAttr.u16SrcWidth         = u16SrcWidth;
    stInputPortAttr.u16SrcHeight        = u16SrcHeight;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));

    memset(&stRotateConfig, 0x0, sizeof(MI_DISP_RotateConfig_t));
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    STCHECKRESULT(MI_DISP_SetVideoLayerRotateMode(0, &stRotateConfig));

    return 0;
}


void signal_handler_fun(int signum) {
    printf("catch signal [%d]\n", signum);
    b_exit = true;
}

int main (int argc, char **argv)
{
    int ret;
    const char *filename = NULL;
    struct SwsContext *video_sws_ctx = NULL;
    struct SwrContext *audio_swr_ctx = NULL;
    AVCodecContext *pAudioCodeCtx = NULL;
    AVCodecContext *pVideoCodeCtx = NULL;
    AVFrame *pFrameYUV = NULL;
    uint8_t *video_buffer = NULL;
    uint8_t *audio_buffer = NULL;
    AVPacket pkt, *packet = &pkt;
    AVFrame *frame = NULL;

    //输入的采样格式, 采样率, 声道布局
    enum AVSampleFormat in_sample_fmt = AV_SAMPLE_FMT_S16;
    int in_sample_rate = 48000;
    uint64_t in_ch_layout = AV_CH_LAYOUT_MONO;
    int in_channels;
    int audio_frm_rwr_size = 0;
    //输出的采样格式, 采样率, 声道布局
    enum AVSampleFormat out_sample_fmt = AUDIO_INPUT_SAMPFMT;
    int out_sample_rate = AUDIO_INPUT_SAMPRATE;
    uint64_t out_ch_layout = AUDIO_INPUT_CHLAYOUT;
    int out_channels = av_get_channel_layout_nb_channels(out_ch_layout);

    filename = argv[1];
    if (!argv[1])
    {
        printf("please input a vaild filename!\n");
        return -1;
    }

    printf("welcome to test mmplayer!\n");

    signal(SIGINT, signal_handler_fun);

    sstar_sdk_init();

    //1.封装格式上下文
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    if (!pFormatCtx)
    {
        printf("avformat_alloc_context fail!\n");
        goto fail;
    }

    //2.打开输入音频文件
    if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0) {
        printf("%s %d fail!\n",__FUNCTION__,__LINE__);
        goto fail;
    }
 
    //3.获取音频信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("%s %d fail!\n",__FUNCTION__,__LINE__);
        goto fail;
    }

    //音频解码，需要找到对应的AVStream所在的pFormatCtx->streams的索引位置
    int audio_stream_idx = -1;
    int video_stream_idx = -1;
    int i = 0;
    for (; i < pFormatCtx->nb_streams; i++) {
        //根据类型判断是否是音频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_idx = i;
            break;
        }
    }
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        //根据类型判断是否是音频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_idx = i;
            break;
        }
    }
    if (audio_stream_idx < 0 && video_stream_idx < 0)
    {
        printf("cannot find valid audio or video stream!\n",__FUNCTION__,__LINE__);
        goto fail;
    }

    //4.获取解码器
    //根据索引拿到对应的流,根据流拿到解码器上下文
    if (audio_stream_idx >= 0)
    {
        AVCodecParameters *pAudioPar = pFormatCtx->streams[audio_stream_idx]->codecpar;

        //再根据上下文拿到编解码id，通过该id拿到解码器
        AVCodec *pAudioCodec = avcodec_find_decoder(pAudioPar->codec_id);
        if (pAudioCodec == NULL) {
            printf("%s %d fail!\n",__FUNCTION__,__LINE__);
            goto fail;
        }
        av_log(NULL, AV_LOG_INFO, "open audio codec: %s\n", pAudioCodec->name);

        pAudioCodeCtx = avcodec_alloc_context3(pAudioCodec);
        if (pAudioCodeCtx == NULL)
        {
            av_log(NULL, AV_LOG_ERROR, "audio avcodec_alloc_context3 failed\n");
            goto fail;
        }
        ret = avcodec_parameters_to_context(pAudioCodeCtx, pAudioPar);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "audio avcodec_parameters_to_context failed %d\n", ret);
            goto fail;
        }

        //5.打开解码器
        if (avcodec_open2(pAudioCodeCtx, pAudioCodec, NULL) < 0) {
            printf("%s %d fail!\n",__FUNCTION__,__LINE__);
            goto fail;
        }
    }

    if (video_stream_idx >= 0)
    {
        AVCodecParameters *pVideoPar = pFormatCtx->streams[video_stream_idx]->codecpar;

        AVCodec *pVideoCodec = NULL;
        if (pVideoPar->codec_id == AV_CODEC_ID_H264)
        {
#ifdef CHIP_IS_SSD20X
            pVideoCodec = avcodec_find_decoder_by_name("ssh264");
#else
            pVideoCodec = avcodec_find_decoder_by_name("ssh264_ss268");
#endif
        }
        else if (pVideoPar->codec_id == AV_CODEC_ID_HEVC)
        {
#ifdef CHIP_IS_SSD20X
            pVideoCodec = avcodec_find_decoder_by_name("sshevc");
#else
            pVideoCodec = avcodec_find_decoder_by_name("sshevc_ss268");
#endif
        }
        else
        {
            pVideoCodec = avcodec_find_decoder(pVideoPar->codec_id);
        }
        if (pVideoCodec == NULL) {
            printf("%s %d fail!\n",__FUNCTION__,__LINE__);
            goto fail;
        }
        av_log(NULL, AV_LOG_INFO, "open video codec: %s\n", pVideoCodec->name);

        pVideoCodeCtx = avcodec_alloc_context3(pVideoCodec);
        if (pVideoCodeCtx == NULL)
        {
            av_log(NULL, AV_LOG_ERROR, "video avcodec_alloc_context3 failed\n");
            goto fail;
        }
        ret = avcodec_parameters_to_context(pVideoCodeCtx, pVideoPar);
        if (ret < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "video avcodec_parameters_to_context failed %d\n", ret);
            goto fail;
        }
        printf("video code width: %d, height: %d, codeid: %d\n", pVideoCodeCtx->width, pVideoCodeCtx->height, pVideoCodeCtx->codec_id);

        sstar_video_config(pVideoCodeCtx);

        if (avcodec_open2(pVideoCodeCtx, pVideoCodec, NULL) < 0) {
            printf("%s %d fail!\n",__FUNCTION__,__LINE__);
            goto fail;
        }

        //none h264 sw decode need transcode to NV12 for disp input
        if(pVideoCodeCtx->codec_id != AV_CODEC_ID_H264 && pVideoCodeCtx->codec_id != AV_CODEC_ID_HEVC)
        {
            //转换成YUV420P
            pFrameYUV = av_frame_alloc();
            //只有指定了AVFrame的像素格式,画面大小才能真正分配内存
            //缓冲区分配内存
            video_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_NV12, u16SrcWidth, u16SrcHeight, 1));
            //初始化缓冲区
            pFrameYUV->width  = u16SrcWidth;
            pFrameYUV->height = u16SrcHeight;
            ret = av_image_fill_arrays(pFrameYUV->data,     // dst data[]
                                       pFrameYUV->linesize, // dst linesize[]
                                       video_buffer,        // src buffer
                                       AV_PIX_FMT_NV12,     // pixel format
                                       u16SrcWidth,
                                       u16SrcHeight,
                                       1                    // align
                                       );
            if (ret < 0)
            {
                av_log(NULL, AV_LOG_ERROR, "av_image_fill_arrays() failed %d\n", ret);
                goto fail;
            }

            //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
            video_sws_ctx = sws_getContext(pVideoCodeCtx->width, pVideoCodeCtx->height, pVideoCodeCtx->pix_fmt,
                                           u16SrcWidth, u16SrcHeight, AV_PIX_FMT_NV12,
                                           SWS_BICUBIC, NULL, NULL, NULL);
        }

        //h264 hw decode need bind vdec 2 disp
        if(pVideoCodeCtx->codec_id == AV_CODEC_ID_H264 || pVideoCodeCtx->codec_id == AV_CODEC_ID_HEVC)
        {
            pVideoCodeCtx->debug = 1;
            pVideoCodeCtx->flags |= (1 << 8);
            sstar_module_bind();
        }
    }

    av_dump_format(pFormatCtx, 0, filename, 0);

    frame = av_frame_alloc();
    if (!frame)
    {
        av_log(NULL, AV_LOG_ERROR, "av_frame_alloc failed\n");
        goto fail;
    }

retry:
    //6.一帧一帧读取压缩的音频数据AVPacket
    while ((ret = av_read_frame(pFormatCtx, packet)) >= 0 && !b_exit)
    {
        if(packet->stream_index == audio_stream_idx)
        {
            MI_AUDIO_Frame_t stAoSendFrame;
            MI_S32 s32RetSendStatus = 0;
            MI_AUDIO_DEV AoDevId = AUDIO_DEV;
            MI_AO_CHN AoChn = 0;
            MI_S16 s16DataLen = 0;
            MI_U16 u16DataIndex = 0;
            MI_S32 s32TimeOut = 128;

            //解码AVPacket->AVFrame
            ret = avcodec_send_packet(pAudioCodeCtx, packet);
            if (ret < 0)
            {
                printf("avcodec_send_packet fail!\n");
                continue;
            }

            ret = avcodec_receive_frame(pAudioCodeCtx, frame);
            if (ret < 0 && ret != AVERROR(EAGAIN))
            {
                printf("avcodec_receive_frame fail\n");
                return ret;
            }
            else if (ret >= 0)
            {
                enum AVSampleFormat sample_fmt = (enum AVSampleFormat)(frame->format);
                // 根据frame中指定的音频参数获取缓冲区的大小
                int data_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, sample_fmt, 1);

                // 获取声道布局
                uint64_t dec_channel_layout = (frame->channel_layout && frame->channels == av_get_channel_layout_nb_channels(frame->channel_layout)) ?
                                      frame->channel_layout : av_get_default_channel_layout(frame->channels);
                int wanted_nb_samples = frame->nb_samples;

                //printf("data size = %d, layout = %d, wanted_nb_samples = %d\n", data_size, dec_channel_layout, wanted_nb_samples);

                if (sample_fmt != in_sample_fmt ||
                    dec_channel_layout != in_ch_layout ||
                    frame->sample_rate != in_sample_rate)
                {
                    swr_free(&audio_swr_ctx);

                    // 使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->audio_swr_ctx
                    audio_swr_ctx = swr_alloc_set_opts(NULL, out_ch_layout, out_sample_fmt, out_sample_rate,
                                                dec_channel_layout, sample_fmt, frame->sample_rate, 0, NULL);
                    if (!audio_swr_ctx || swr_init(audio_swr_ctx) < 0)
                    {
                        av_log(NULL, AV_LOG_ERROR,
                            "cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                            frame->sample_rate, av_get_sample_fmt_name(sample_fmt), frame->channels,
                            out_sample_rate, av_get_sample_fmt_name(out_sample_fmt), out_ch_layout);
                        swr_free(&audio_swr_ctx);
                        return -1;
                    }

                    in_ch_layout = dec_channel_layout;
                    in_channels = frame->channels;
                    in_sample_rate = frame->sample_rate;
                    in_sample_fmt = sample_fmt;
                    av_log(NULL, AV_LOG_INFO, "swr_alloc_set_opts ok!\n");
                }

                if (audio_swr_ctx)
                {
                    const uint8_t **in = (const uint8_t **)frame->extended_data;
                    // 重采样输出参数:输出音频样本数(多加了256个样本)
                    int out_count = (int64_t)wanted_nb_samples * out_sample_rate / frame->sample_rate + 256;
                    // 重采样输出参数:输出音频缓冲区尺寸(以字节为单位)
                    int out_size = av_samples_get_buffer_size(NULL, out_channels, out_count, out_sample_fmt, 1);
                    if (out_size < 0)
                    {
                        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
                        return -1;
                    }
                    
                    av_fast_malloc(&audio_buffer, &audio_frm_rwr_size, out_size);
                    if (!audio_buffer)
                    {
                        return AVERROR(ENOMEM);
                    }

                    // 音频重采样：返回值是重采样后得到的音频数据中单个声道的样本数
                    int len2 = swr_convert(audio_swr_ctx, &audio_buffer, out_count, in, frame->nb_samples);
                    if (len2 < 0)
                    {
                        av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
                        return -1;
                    }

                    if (len2 == out_count)
                    {
                        av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
                        if (swr_init(audio_swr_ctx) < 0)
                            swr_free(&audio_swr_ctx);
                        return -1;
                    }

                    // 重采样返回的一帧音频数据大小(以字节为单位)
                    s16DataLen = len2 * out_channels * av_get_bytes_per_sample(out_sample_fmt);
                    //av_log(NULL, AV_LOG_INFO, "audio frame data size: %d, wanted_nb_samples: %d\n", s16DataLen, wanted_nb_samples);
                }
                else
                {
                    audio_buffer = frame->data[0];
                    s16DataLen = data_size;
                }

                if (s16DataLen < 0)
                {
                    av_log(NULL, AV_LOG_ERROR, "valid audio frame data size: %d\n", s16DataLen);
                }

                //read data and send to ao module
                do {
#ifdef CHIP_IS_SS268
                    if (s16DataLen <= MI_AUDIO_MAX_DATA_SIZE)
                        stAoSendFrame.u32Len[0] = s16DataLen;
                    else
                        stAoSendFrame.u32Len[0] = MI_AUDIO_MAX_DATA_SIZE;
#else
                    if (s16DataLen <= MI_AUDIO_MAX_DATA_SIZE)
                        stAoSendFrame.u32Len = s16DataLen;
                    else
                        stAoSendFrame.u32Len = MI_AUDIO_MAX_DATA_SIZE;
#endif
                    stAoSendFrame.apVirAddr[0] = &audio_buffer[u16DataIndex];
                    stAoSendFrame.apVirAddr[1] = NULL;

                    s32TimeOut = s16DataLen * 1000 / (out_sample_rate * out_channels);
                    s32TimeOut = (s32TimeOut < 24) ? 24 : s32TimeOut;
                    //av_log(NULL, AV_LOG_INFO, "channels: %d, mi_ao_sendframe timeout: %d\n", out_channels, s32TimeOut);

                    s16DataLen   -= MI_AUDIO_MAX_DATA_SIZE;
                    u16DataIndex += MI_AUDIO_MAX_DATA_SIZE;

                    do{
                        s32RetSendStatus = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, s32TimeOut);
                    }while(s32RetSendStatus == MI_AO_ERR_NOBUF);

                    if(s32RetSendStatus != MI_SUCCESS)
                    {
                        printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n",s32RetSendStatus);
                    }
                }while(s16DataLen > 0);

                av_frame_unref(frame);
            }
        }
        if(packet->stream_index == video_stream_idx)
        {
            #if 0
            if(pVideoCodeCtx->codec_id == AV_CODEC_ID_H264)
            {
                const char start_code[4] = { 0, 0, 0, 1 };
                if(memcmp(start_code, packet->data, 4) != 0)
                {//is avc1 code, have no start code of H264
                    int len = 0;
                    uint8_t *p = packet->data;

                    do 
                    {//add start_code for each NAL, one frame may have multi NALs.
                        len = ntohl(*((long*)p));
                        memcpy(p, start_code, 4);

                        p += 4;
                        p += len;
                        if(p >= packet->data + packet->size)
                        {
                            break;
                        }
                    } while (1);
                }
            }
            #endif
            //7.解码一帧视频压缩数据
            ret = avcodec_send_packet(pVideoCodeCtx, packet);
            if(ret < 0)
            {
                printf("avcodec_send_packet fail!\n");
                continue;
            }

            ret = avcodec_receive_frame(pVideoCodeCtx, frame);
            if (ret < 0 && ret != AVERROR(EAGAIN))
            {
                printf("avcodec_receive_frame fail\n");
                return ret;
            }
            else if (ret >= 0)
            {
                //none h264 sw decode need put frame to disp
                if(pVideoCodeCtx->codec_id != AV_CODEC_ID_H264 && pVideoCodeCtx->codec_id != AV_CODEC_ID_HEVC)
                {
                    sws_scale(video_sws_ctx, frame->data, frame->linesize, 0, pVideoCodeCtx->height,pFrameYUV->data, pFrameYUV->linesize);

                    int y_size = pFrameYUV->width * pFrameYUV->height;

                    MI_SYS_BUF_HANDLE hHandle;
                    MI_SYS_ChnPort_t pstSysChnPort;
                    MI_SYS_BufConf_t stBufConf;
                    MI_SYS_BufInfo_t stBufInfo;

                    pstSysChnPort.eModId    = E_MI_MODULE_ID_DISP;
                    pstSysChnPort.u32ChnId  = 0;
                    pstSysChnPort.u32DevId  = 0;
                    pstSysChnPort.u32PortId = 0;

                    memset(&stBufInfo , 0 , sizeof(MI_SYS_BufInfo_t));
                    memset(&stBufConf , 0 , sizeof(MI_SYS_BufConf_t));

                    stBufConf.eBufType = E_MI_SYS_BUFDATA_FRAME;
                    stBufConf.u64TargetPts = 0;
                    stBufConf.stFrameCfg.u16Width  = pFrameYUV->width;
                    stBufConf.stFrameCfg.u16Height = pFrameYUV->height;
                    stBufConf.stFrameCfg.eFormat   = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
                    stBufConf.stFrameCfg.eFrameScanMode = E_MI_SYS_FRAME_SCAN_MODE_PROGRESSIVE;

                    if(MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&pstSysChnPort,&stBufConf,&stBufInfo,&hHandle, -1))
                    {
                        stBufInfo.stFrameData.eCompressMode = E_MI_SYS_COMPRESS_MODE_NONE;
                        stBufInfo.stFrameData.eFieldType = E_MI_SYS_FIELDTYPE_NONE;
                        stBufInfo.stFrameData.eTileMode = E_MI_SYS_FRAME_TILE_MODE_NONE;
                        stBufInfo.bEndOfStream = FALSE;

                        memcpy(stBufInfo.stFrameData.pVirAddr[0], pFrameYUV->data[0], y_size);
                        memcpy(stBufInfo.stFrameData.pVirAddr[1], pFrameYUV->data[1], y_size / 2);

                        MI_SYS_ChnInputPortPutBuf(hHandle ,&stBufInfo , FALSE);
                    }

                    if (audio_stream_idx < 0)
                    {
                        av_usleep(30 * 1000);
                    }
                }
                av_frame_unref(frame);
            }
        }
        av_packet_unref(packet);
    }

    if ((ret == AVERROR_EOF || avio_feof(pFormatCtx->pb)) && !b_exit)
    {
        printf("avio buffer has been read finish!\n");
        int seekflag = AVSEEK_FLAG_ANY;
        seekflag &= ~AVSEEK_FLAG_BYTE;
        avformat_seek_file(pFormatCtx, -1, INT64_MIN, pFormatCtx->start_time, INT64_MAX, seekflag);
        goto retry;
    }

    if(pVideoCodeCtx && (pVideoCodeCtx->codec_id == AV_CODEC_ID_H264 || pVideoCodeCtx->codec_id == AV_CODEC_ID_HEVC))
    {
        sstar_module_unbind();
    }

fail:
    av_frame_free(&frame);
    av_frame_free(&pFrameYUV);
    av_freep(&audio_buffer);
    av_freep(&video_buffer);
    swr_free(&audio_swr_ctx);
    sws_freeContext(video_sws_ctx);
    avcodec_close(pAudioCodeCtx);
    avcodec_close(pVideoCodeCtx);
    avformat_close_input(&pFormatCtx);

    sstar_sdk_deinit();

    return SUCCESS;
}
