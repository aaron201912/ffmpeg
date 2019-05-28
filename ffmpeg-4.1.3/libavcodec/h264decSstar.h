/*
 * H.26L/H.264/AVC/JVT/14496-10/... encoder/decoder
 * Copyright (c) 2003 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * H.264 / AVC / MPEG-4 part10 codec.
 * @author Michael Niedermayer <michaelni@gmx.at>
 */

#ifndef AVCODEC_SSH264DEC_H
#define AVCODEC_SSH264DEC_H

#include <stdbool.h>

#include "libavutil/buffer.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/thread.h"
#include "h264_parse.h"
#include "h264_ps.h"
#include "h2645_parse.h"

//sstar sdk lib
#include "mi_vdec.h"
#include "mi_vdec_datatype.h"
#include "mi_common.h"
#include "mi_common_datatype.h"
#include "mi_sys.h"
#include "mi_sys_datatype.h"
//#include "mi_divp.h"
//#include "mi_divp_datatype.h"
#include "mi_hdmi.h"
#include "mi_hdmi_datatype.h"
#include "mi_disp.h"
#include "mi_disp_datatype.h"
/**
 * H264Context
 */
typedef struct SsH264Context {
    AVFrame *f;
    int channel;
    int cropwidth;
    int cropheight;

    H2645Packet pkt;
    H264ParamSets ps;
    int is_avc;           ///< this flag is != 0 if codec is avc1
    int nal_length_size;  ///< Number of bytes used for nal length (1, 2 or 4)
    
} SsH264Context;
typedef struct SsCropContext {
    int x;
    int y;
    int cropwidth;
    int cropheight;
    
} SsCropContext;


typedef struct ST_Sys_BindInfo_s
{
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;
    MI_U32 u32SrcFrmrate;
    MI_U32 u32DstFrmrate;
} ST_Sys_BindInfo_t;

typedef struct
{
    bool bResume;
    bool bGotFrame;
    bool bReleaseFrame;
    bool brevP;
    MI_SYS_BufInfo_t stBufInfo;

} Main_Context;


#endif /* AVCODEC_SSH264DEC_H */
