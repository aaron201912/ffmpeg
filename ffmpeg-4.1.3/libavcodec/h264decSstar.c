/*
 * H.26L/H.264/AVC/JVT/14496-10/... decoder
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

#define UNCHECKED_BITSTREAM_READER 1

#include "libavutil/avassert.h"
#include "libavutil/display.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/stereo3d.h"
#include "libavutil/timer.h"
#include "internal.h"
#include "bytestream.h"
#include "cabac.h"
#include "cabac_functions.h"
#include "error_resilience.h"
#include "avcodec.h"
#include "h264.h"
#include "h264dec.h"
#include "h2645_parse.h"
#include "h264data.h"
#include "h264chroma.h"
#include "h264_mvpred.h"
#include "h264_ps.h"
#include "golomb.h"
#include "hwaccel.h"
#include "mathops.h"
#include "me_cmp.h"
#include "mpegutils.h"
#include "profiles.h"
#include "rectangle.h"
#include "thread.h"

//sstar sdk lib
#include "mi_vdec.h"
#include "mi_vdec_datatype.h"
#include "mi_common.h"
#include "mi_common_datatype.h"
#include "mi_sys.h"
#include "mi_sys_datatype.h"


static av_cold int ss_h264_decode_end(AVCodecContext *avctx)
{
    printf("h264_decode_end\n");
    return 0;
}

static av_cold int ss_h264_decode_init(AVCodecContext *avctx)
{
    MI_VDEC_ChnAttr_t stVdecChnAttr;
    MI_S32 s32Ret;

    printf("h264_decode_init width: %d,height: %d\n",avctx->width,avctx->height);
    #if 0
    memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
    stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 2;
    stVdecChnAttr.eVideoMode    = E_MI_VDEC_VIDEO_MODE_FRAME;
    stVdecChnAttr.u32BufSize    = 1 * 1024 * 1024;
    stVdecChnAttr.u32PicWidth   = avctx->width;
    stVdecChnAttr.u32PicHeight  = avctx->height;
    stVdecChnAttr.u32Priority   = 0;
    stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;

    s32Ret = MI_VDEC_CreateChn(0, &stVdecChnAttr);
    if (s32Ret != MI_SUCCESS)
    {
        printf("MI_VDEC_CreateChn error, chn:%d, %X\n", 0, s32Ret);
        return 1;
    }

    s32Ret = MI_VDEC_StartChn(0);
    if (s32Ret != MI_SUCCESS)
    {
        printf("MI_VDEC_StartChn error, chn:%d, %X\n", 0, s32Ret);
        return 1;
    }
    #endif
    return 0;
}

static int ss_h264_decode_frame(AVCodecContext *avctx, void *data,
                             int *got_frame, AVPacket *avpkt)
{
    MI_SYS_ChnPort_t stChnPort;
    MI_VDEC_VideoStream_t stVdecStream;
    MI_S32 s32Ret;
    
    printf("h264_decode_frame: %d\n",avctx->flags);
#if 0
    if(!avctx->flags)
    {
        MI_VDEC_ChnAttr_t stVdecChnAttr;
        MI_S32 s32Ret;

        printf("decode_frame width: %d,height: %d\n",avctx->width,avctx->height);
        memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
        stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 2;
        stVdecChnAttr.eVideoMode    = E_MI_VDEC_VIDEO_MODE_FRAME;
        stVdecChnAttr.u32BufSize    = 1 * 1024 * 1024;
        stVdecChnAttr.u32PicWidth   = avctx->width;
        stVdecChnAttr.u32PicHeight  = avctx->height;
        stVdecChnAttr.u32Priority   = 0;
        stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;

        s32Ret = MI_VDEC_CreateChn(0, &stVdecChnAttr);
        if (s32Ret != MI_SUCCESS)
        {
            printf("MI_VDEC_CreateChn error, chn:%d, %X\n", 0, s32Ret);
            return 1;
        }

        s32Ret = MI_VDEC_StartChn(0);
        if (s32Ret != MI_SUCCESS)
        {
            printf("MI_VDEC_StartChn error, chn:%d, %X\n", 0, s32Ret);
            return 1;
        }
        avctx->flags = 1;
    }

    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;//0 1 2 3
    stChnPort.u32PortId = 0;

    s32Ret = MI_SYS_SetChnOutputPortDepth(&stChnPort, 5, 5);
    if (MI_SUCCESS != s32Ret)
    {
        printf("MI_SYS_SetChnOutputPortDepth error, %X\n", s32Ret);
        return NULL;
    }

    stVdecStream.pu8Addr = avpkt->data;
    stVdecStream.u32Len = avpkt->size;
    stVdecStream.u64PTS = avpkt->pts;
    stVdecStream.bEndOfFrame = 1;
    stVdecStream.bEndOfStream = 0;

    s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
#endif
    return 0;
}

static int ss_h264_receive_frame(AVCodecContext *avctx, AVFrame *frame)
{
    MI_SYS_ChnPort_t stChnPort;
    MI_SYS_BufInfo_t stBufInfo;
    MI_SYS_BUF_HANDLE hSysBuf = MI_HANDLE_NULL;
    MI_U32 u32CheckSum = 0;
    
    printf("h264_receive_frame\n");
#if 0
    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;
    
    memset(&stBufInfo, 0x0, sizeof(MI_SYS_BufInfo_t));
    if (MI_SUCCESS == MI_SYS_ChnOutputPortGetBuf(&stChnPort, &stBufInfo, &hSysBuf))
    {
        frame->data[0] = stBufInfo.stMetaData.pVirAddr;
        frame->metabufHandle = hSysBuf;
        frame->width = avctx->width;
        frame->height = avctx->height;
        return 0;
    }
#endif
    return -1;
}


AVCodec ff_ssh264_decoder = {
    .name                  = "ssh264",
    .long_name             = NULL_IF_CONFIG_SMALL("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"),
    .type                  = AVMEDIA_TYPE_VIDEO,
    .id                    = AV_CODEC_ID_H264,
    .priv_data_size        = sizeof(H264Context),
    .init                  = ss_h264_decode_init,
    .close                 = ss_h264_decode_end,
    .decode                = ss_h264_decode_frame,
    .receive_frame         = ss_h264_receive_frame,
    .capabilities          = /*AV_CODEC_CAP_DRAW_HORIZ_BAND |*/ AV_CODEC_CAP_DR1 |
                             AV_CODEC_CAP_DELAY | AV_CODEC_CAP_SLICE_THREADS |
                             AV_CODEC_CAP_FRAME_THREADS,
    .hw_configs            = (const AVCodecHWConfigInternal*[]) {
                               NULL
                           },

};
