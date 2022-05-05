/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/
#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define VS_MODE (1 << 0)
#define VC_MODE (1 << 1)
#define AS_IN_MODE (1 << 2)
#define AS_OUT_MODE (1 << 3)
#define AC_MODE (1 << 4)

#define BUF_HANDLE_MODE_NONE 0
#define BUF_HANDLE_MODE_FILE 1
#define BUF_HANDLE_MODE_MI 2
#define BUF_HANDLE_MODE_FFMPEG 3

#define DEFAULT_DIRECTORY "/mnt"

/* control direction */
#define CONTROL_GET 0
#define CONTROL_SET 1

typedef enum
{
    DEMO_DBG_NONE = 0,
    DEMO_DBG_ERR,
    DEMO_DBG_WRN,
    DEMO_DBG_INFO,
    DEMO_DBG_DEBUG,
    DEMO_DBG_TRACE,
    DEMO_DBG_ALL
} DEMO_DBG_LEVEL_e;

#define ASCII_COLOR_RED                          "\033[1;31m"
#define ASCII_COLOR_WHITE                        "\033[1;37m"
#define ASCII_COLOR_YELLOW                       "\033[1;33m"
#define ASCII_COLOR_BLUE                         "\033[1;36m"
#define ASCII_COLOR_GREEN                        "\033[1;32m"
#define ASCII_COLOR_END                          "\033[0m"

extern DEMO_DBG_LEVEL_e demo_debug_level;

#define DEMO_SUPPORT_DEBUG
#ifdef DEMO_SUPPORT_DEBUG
#define DEMO_TRACE(dev, fmt, args...) ({do{if(demo_debug_level>=DEMO_DBG_TRACE)\
                {printf(ASCII_COLOR_GREEN"[APP TRACE][dev:%s]:%s[%d]: " fmt ASCII_COLOR_END"\n", dev?dev->path:NULL, __FUNCTION__,__LINE__,##args);}}while(0);})
#define DEMO_DEBUG(dev, fmt, args...) ({do{if(demo_debug_level>=DEMO_DBG_DEBUG)\
                {printf(ASCII_COLOR_GREEN"[APP DEBUG][dev:%s]:%s[%d]: " fmt ASCII_COLOR_END"\n", dev?dev->path:NULL, __FUNCTION__,__LINE__,##args);}}while(0);})
#define DEMO_INFO(dev, fmt, args...)     ({do{if(demo_debug_level>=DEMO_DBG_INFO)\
                {printf(ASCII_COLOR_GREEN"[APP INFO][dev:%s]:%s[%d]: " fmt ASCII_COLOR_END"\n", dev?dev->path:NULL, __FUNCTION__,__LINE__,##args);}}while(0);})
#define DEMO_WRN(dev, fmt, args...)      ({do{if(demo_debug_level>=DEMO_DBG_WRN)\
                {printf(ASCII_COLOR_YELLOW"[APP WRN][dev:%s]: %s[%d]: " fmt ASCII_COLOR_END"\n", dev?dev->path:NULL, __FUNCTION__,__LINE__, ##args);}}while(0);})
#define DEMO_ERR(dev, fmt, args...)      ({do{if(demo_debug_level>=DEMO_DBG_ERR)\
                {printf(ASCII_COLOR_RED"[APP ERR][dev:%s]: %s[%d]: " fmt ASCII_COLOR_END"\n", dev?dev->path:NULL, __FUNCTION__,__LINE__, ##args);}}while(0);})
#define DEMO_EXIT_ERR(fmt, args...) ({do\
                {printf(ASCII_COLOR_RED"<<<%s[%d] " fmt ASCII_COLOR_END"\n",__FUNCTION__,__LINE__,##args);}while(0);})
#define DEMO_ENTER()                ({do{if(DEMO_func_trace)\
                {printf(ASCII_COLOR_BLUE">>>%s[%d] \n" ASCII_COLOR_END"\n",__FUNCTION__,__LINE__);}}while(0);})
#define DEMO_EXIT_OK()              ({do{if(DEMO_func_trace)\
                {printf(ASCII_COLOR_BLUE"<<<%s[%d] \n" ASCII_COLOR_END"\n",__FUNCTION__,__LINE__);}}while(0);})
#else
#define DEMO_DEBUG(fmt, args...) NULL
#define DEMO_ERR(fmt, args...)   NULL
#define DEMO_INFO(fmt, args...)  NULL
#define DEMO_WRN(fmt, args...)   NULL
#endif

#endif
