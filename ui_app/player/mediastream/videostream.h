#ifndef __VIDEO_STREAM_H__
#define __VIDEO_STREAM_H__

#include "player.h"

#include "mi_hdmi.h"
#include "mi_hdmi_datatype.h"
#include "mi_disp.h"
#include "mi_disp_datatype.h"
#include "mi_gfx.h"
#include "mi_gfx_datatype.h"
#include "mi_panel.h"
#include "mi_panel_datatype.h"


#define MAKE_YUYV_VALUE(y,u,v)  ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK              MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE              MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED                MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN              MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE               MAKE_YUYV_VALUE(29,225,107)

#define DIVP_CHN        0
#define DISP_DEV        0
#define DISP_LAYER      0
#define DISP_INPUTPORT  0

int open_video(player_stat_t *is);
MI_S32 sstar_disp_init(MI_PANEL_ParamConfig_t *stPanelParam, MI_U16 width, MI_U16 height);
void sstar_disp_deinit(void);

MI_S32 sstar_open_display(void);
void sstar_close_display(void);


#endif

