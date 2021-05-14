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
        return 1;\
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

void ss_getpanel_wh(int *width, int *height)
{
#if 1
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
    printf("ssd20x_getpanel_wh = [%d %d]\n", *width, *height);
#endif
}

int ST_Screen_Init(void)
{
    MI_DISP_PubAttr_t stPubAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_PANEL_IntfType_e eIntfType;
    
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
    stLayerAttr.stVidLayerDispWin.u16Width = stPubAttr.stSyncInfo.u16Hact;
    stLayerAttr.stVidLayerDispWin.u16Height = stPubAttr.stSyncInfo.u16Vact;
    STCHECKRESULT(MI_DISP_BindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_SetVideoLayerAttr(0, &stLayerAttr));
    STCHECKRESULT(MI_DISP_EnableVideoLayer(0));

    memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
    stInputPortAttr.u16SrcWidth = u32DispInWidth;
    stInputPortAttr.u16SrcHeight = u32DispInHeight;
    stInputPortAttr.stDispWin.u16X = 0;
    stInputPortAttr.stDispWin.u16Y = 0;
    stInputPortAttr.stDispWin.u16Width = u32DispOutWidth;
    stInputPortAttr.stDispWin.u16Height = u32DispOutHeight;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));

    //init panel
    eIntfType = E_MI_PNL_INTF_TTL;
    STCHECKRESULT(MI_PANEL_Init(eIntfType));

    return 0;
}

int ST_Screen_DeInit(void)
{
    STCHECKRESULT(MI_DISP_DisableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_DisableVideoLayer(0));
    STCHECKRESULT(MI_DISP_UnBindVideoLayer(0, 0));
    STCHECKRESULT(MI_DISP_Disable(0));
    STCHECKRESULT(MI_PANEL_DeInit());

    return 0;
}

int ss_sys_init(void)
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

int ss_sys_deinit(void)
{
    MI_SYS_Exit(ST_DEFAULT_SOC_ID);
    return MI_SUCCESS;
}
#endif
