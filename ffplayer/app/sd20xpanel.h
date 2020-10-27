#ifndef __SD20XPANEL_H__
#define __SD20XPANEL_H__

#include "mi_common.h"
#include "mi_sys.h"
#include "mi_disp.h"
#include "mi_ao.h"

#define  DISPLAY_1024_600   1

#ifdef SUPPORT_HDMI
#include "mi_hdmi.h"

#define  HDMI_MAX_W         1920
#define  HDMI_MAX_H         1080
#else
#include "mi_panel.h"

#if DISPLAY_1024_600
#define  PANEL_MAX_W        1024
#define  PANEL_MAX_H        600
#endif

#endif

#define  LOCAL_X            0
#define  LOCAL_Y            0

typedef enum
{
    E_MI_HDMI_OUT_1080P = 0,
    E_MI_HDMI_OUT_4K,
    E_MI_HDMI_OUT_720P,
    E_MI_HDMI_OUT_MAX,
} HDMI_Out_e;

int sd20x_panel_init(MI_DISP_Interface_e eType, HDMI_Out_e outMode);
int sd20x_panel_deinit(MI_DISP_Interface_e eType);
void ssd20x_getpanel_wh(int *width, int *height);

int sd20x_sys_init(void);
int sd20x_sys_deinit(void);


#endif

