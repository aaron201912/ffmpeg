#ifndef __SS268PANEL_H__
#define __SS268PANEL_H__

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
#include "mi_panel_datatype.h"
#include "mi_panel.h"
#include "mi_disp_datatype.h"
#include "mi_disp.h"

#if DISPLAY_1024_600
#define  PANEL_MAX_W        1024
#define  PANEL_MAX_H        600
#endif

#endif

typedef enum
{
    E_MI_HDMI_OUT_1080P = 0,
    E_MI_HDMI_OUT_4K,
    E_MI_HDMI_OUT_720P,
    E_MI_HDMI_OUT_MAX,
} HDMI_Out_e;

int ss268_screen_init(void);
int ss268_screen_deinit(void);
void ss268_getpanel_wh(int *width, int *height);

int ss268_sys_init(void);
int ss268_sys_deinit(void);


#endif

