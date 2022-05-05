#include <string.h>
#include <stdio.h>
#ifdef CHIP_IS_SS268
#include "ss268_panel.h"

#ifdef ST_DEFAULT_SOC_ID
#undef ST_DEFAULT_SOC_ID
#define ST_DEFAULT_SOC_ID 0
#else
#define ST_DEFAULT_SOC_ID 0
#endif

#define STCHECKRESULT(result)\
    if (result != MI_SUCCESS)\
    {\
        printf("[%s %d]exec function failed\n", __FUNCTION__, __LINE__);\
    }\
    else\
    {\
        printf("(%s %d)exec function pass\n", __FUNCTION__,__LINE__);\
    }

#define DISP_INPUT_WIDTH    1024
#define DISP_INPUT_HEIGHT   600

#define MAKE_YUYV_VALUE(y,u,v)  ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK              MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE              MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED                MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN              MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE               MAKE_YUYV_VALUE(29,225,107)

static unsigned long u32DispInWidth = 1024;
static unsigned long u32DispInHeight = 600;

static unsigned long u32DispOutWidth = 1024;
static unsigned long u32DispOutHeight = 600;

static HDMI_Out_e stHdmiMode = E_MI_HDMI_OUT_1080P;

#ifdef SUPPORT_HDMI
static MI_S32 Hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
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
#endif

void ss268_getpanel_wh(int *width, int *height)
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
    printf("ss268_getpanel_wh = [%d %d]\n", *width, *height);
}

int ss268_screen_init(void)
{
#ifndef SUPPORT_HDMI
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
    stInputPortAttr.u16SrcWidth  = u32DispInWidth;
    stInputPortAttr.u16SrcHeight = u32DispInHeight;
    stInputPortAttr.stDispWin.u16X = 0;
    stInputPortAttr.stDispWin.u16Y = 0;
    stInputPortAttr.stDispWin.u16Width  = u32DispOutWidth;
    stInputPortAttr.stDispWin.u16Height = u32DispOutHeight;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
#else
    MI_HDMI_InitParam_t stInitParam;
    MI_HDMI_Attr_t stAttr;
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;

    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.u32BgColor =YUYV_BLACK;
    stPubAttr.eIntfType  = E_MI_DISP_INTF_HDMI;
    if (stHdmiMode == E_MI_HDMI_OUT_720P)
        stPubAttr.eIntfSync  = E_MI_DISP_OUTPUT_720P60;
    else
        stPubAttr.eIntfSync  = E_MI_DISP_OUTPUT_1080P60;
    STCHECKRESULT(MI_DISP_SetPubAttr(0, &stPubAttr));
    STCHECKRESULT(MI_DISP_Enable(0));

    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.eIntfType = E_MI_DISP_INTF_HDMI;
    STCHECKRESULT(MI_DISP_GetPubAttr(0,&stPubAttr));
    printf("get hdmi pubattr = [%d %d]\n", stPubAttr.stSyncInfo.u16Hact, stPubAttr.stSyncInfo.u16Vact);

    memset(&stLayerAttr, 0, sizeof(MI_DISP_VideoLayerAttr_t));
    stLayerAttr.stVidLayerDispWin.u16X      = 0;
    stLayerAttr.stVidLayerDispWin.u16Y      = 0;
    stLayerAttr.stVidLayerSize.u16Width     = HDMI_MAX_W;
    stLayerAttr.stVidLayerSize.u16Height    = HDMI_MAX_H;
    if(stHdmiMode == E_MI_HDMI_OUT_720P) {
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
    stInitParam.pfnHdmiEventCallback = Hdmi_callback_impl;
    STCHECKRESULT(MI_HDMI_Init(&stInitParam));
    STCHECKRESULT(MI_HDMI_Open(eHdmi));

    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    stAttr.stEnInfoFrame.bEnableAudInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame  = FALSE;
    stAttr.stAudioAttr.bEnableAudio    = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 1;//0: mono; 1: stereo
    stAttr.stAudioAttr.eBitDepth       = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType       = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate     = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo    = TRUE;
    stAttr.stVideoAttr.eColorType      = E_MI_HDMI_COLOR_TYPE_YCBCR420;
    stAttr.stVideoAttr.eDeepColorMode  = E_MI_HDMI_DEEP_COLOR_24BIT;

    if (stHdmiMode == E_MI_HDMI_OUT_720P)
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_720_60P;
    else
        stAttr.stVideoAttr.eTimingType = E_MI_HDMI_TIMING_1080_60P;

    stAttr.stVideoAttr.eOutputMode = E_MI_HDMI_OUTPUT_MODE_HDMI;
    STCHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr));
    STCHECKRESULT(MI_HDMI_Start(eHdmi));
    STCHECKRESULT(MI_HDMI_SetAvMute(eHdmi, false));
#endif
    return 0;
}

int ss268_screen_deinit(void)
{
    STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));
#ifndef SUPPORT_HDMI
    STCHECKRESULT(MI_PANEL_DeInit());
#else
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;
    STCHECKRESULT(MI_HDMI_SetAvMute(eHdmi, true));
    STCHECKRESULT(MI_HDMI_Stop(eHdmi));
    STCHECKRESULT(MI_HDMI_Close(eHdmi));
    STCHECKRESULT(MI_HDMI_DeInit());
#endif
    return 0;
}

int ss268_sys_init(void)
{
    MI_SYS_Version_t stVersion;
    MI_U64 u64Pts = 0;
    MI_SYS_Init(ST_DEFAULT_SOC_ID);
    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));
    MI_SYS_GetVersion(ST_DEFAULT_SOC_ID, &stVersion);
    MI_SYS_GetCurPts(ST_DEFAULT_SOC_ID, &u64Pts);
    u64Pts = 0xF1237890F1237890;
    MI_SYS_InitPtsBase(ST_DEFAULT_SOC_ID, u64Pts);
    u64Pts = 0xE1237890E1237890;
    MI_SYS_SyncPts(ST_DEFAULT_SOC_ID, u64Pts);
    return MI_SUCCESS;
}

int ss268_sys_deinit(void)
{
    MI_SYS_Exit(ST_DEFAULT_SOC_ID);
    return MI_SUCCESS;
}
#endif
