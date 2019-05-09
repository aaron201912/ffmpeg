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

#include <stdio.h>
#include <stdbool.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/stat.h>


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
#include "h264decSstar.h"
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

#define ALIGN_BACK(x, a)            (((x) / (a)) * (a))
#define DISP_WIDTH_ALIGN    2
#define DISP_HEIGHT_ALIGN   2

#define NALU_PACKET_SIZE            512*1024
#define MI_U32VALUE(pu8Data, index) (pu8Data[index]<<24)|(pu8Data[index+1]<<16)|(pu8Data[index+2]<<8)|(pu8Data[index+3])

static struct pollfd pfd[1] =
{
    {0, POLLIN | POLLERR},
};

static bool bExit;

static MI_S32 ST_Sys_Init(void)
{
    MI_SYS_Version_t stVersion;
    MI_U64 u64Pts = 0;

    STCHECKRESULT(MI_SYS_Init());

    memset(&stVersion, 0x0, sizeof(MI_SYS_Version_t));

    STCHECKRESULT(MI_SYS_GetVersion(&stVersion));

    STCHECKRESULT(MI_SYS_GetCurPts(&u64Pts));

    u64Pts = 0xF1237890F1237890;
    STCHECKRESULT(MI_SYS_InitPtsBase(u64Pts));

    u64Pts = 0xE1237890E1237890;
    STCHECKRESULT(MI_SYS_SyncPts(u64Pts));

    return 0;
}

static MI_S32 ST_Sys_Bind(ST_Sys_BindInfo_t *pstBindInfo)
{
    STCHECKRESULT(MI_SYS_BindChnPort(&pstBindInfo->stSrcChnPort, &pstBindInfo->stDstChnPort, \
        pstBindInfo->u32SrcFrmrate, pstBindInfo->u32DstFrmrate));

    return 0;
}

static MI_S32 ST_Sys_UnBind(ST_Sys_BindInfo_t *pstBindInfo)
{
    STCHECKRESULT(MI_SYS_UnBindChnPort(&pstBindInfo->stSrcChnPort, &pstBindInfo->stDstChnPort));

    return 0;
}


static av_cold int ss_h264_decode_end(AVCodecContext *avctx)
{
    ST_Sys_BindInfo_t stBindInfo;
    SsH264Context *s = avctx->priv_data;

    if(!bExit)
    {
        memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));

        stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
        stBindInfo.stSrcChnPort.u32DevId = 0;
        stBindInfo.stSrcChnPort.u32ChnId = s->channel;
        stBindInfo.stSrcChnPort.u32PortId = 0;

        stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DIVP;
        stBindInfo.stDstChnPort.u32DevId = 0;
        stBindInfo.stDstChnPort.u32ChnId = s->channel;
        stBindInfo.stDstChnPort.u32PortId = 0;
        
        STCHECKRESULT(ST_Sys_UnBind(&stBindInfo));
        
        STCHECKRESULT(MI_VDEC_StopChn(s->channel));
        STCHECKRESULT(MI_VDEC_DestroyChn(s->channel));

        STCHECKRESULT(MI_DIVP_StopChn(s->channel));
        STCHECKRESULT(MI_DIVP_DestroyChn(s->channel));
        
        STCHECKRESULT(MI_SYS_Exit());
        bExit = 1;
    }
    return 0;
}

static av_cold int ss_h264_decode_init(AVCodecContext *avctx)
{
    MI_VDEC_ChnAttr_t stVdecChnAttr;
    MI_S32 s32Ret;
    MI_SYS_ChnPort_t stChnPort;

    SsH264Context *s = avctx->priv_data;
    
    s->channel = 0;
    bExit = 0;
    s->f = av_frame_alloc();
    if (!s->f) 
        return 0;
    s->f->format = AV_PIX_FMT_NV12;
    s->f->width = avctx->width;
    s->f->height = avctx->height;
    s32Ret = av_frame_get_buffer(s->f, 32);
    if (s32Ret < 0) 
    {
        av_frame_free(&s->f);
    }
    
    ST_Sys_Init();

    //init vdec
    memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
    stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 2;
    stVdecChnAttr.eVideoMode    = E_MI_VDEC_VIDEO_MODE_FRAME;
    stVdecChnAttr.u32BufSize    = 1 * 1024 * 1024;
    stVdecChnAttr.u32PicWidth   = avctx->width;
    stVdecChnAttr.u32PicHeight  = avctx->height;
    stVdecChnAttr.u32Priority   = 0;
    stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;

    STCHECKRESULT(MI_VDEC_CreateChn(s->channel, &stVdecChnAttr));

    STCHECKRESULT(MI_VDEC_StartChn(s->channel));

    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;

    STCHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 0, 5));

    //init divp
    MI_DIVP_ChnAttr_t stDivpChnAttr;
    MI_DIVP_OutputPortAttr_t stOutputPortAttr;
    
    memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
    stDivpChnAttr.bHorMirror            = FALSE;
    stDivpChnAttr.bVerMirror            = FALSE;
    stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
    stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
    stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
    stDivpChnAttr.stCropRect.u16X       = 0;
    stDivpChnAttr.stCropRect.u16Y       = 0;
    stDivpChnAttr.stCropRect.u16Width   = avctx->width;
    stDivpChnAttr.stCropRect.u16Height  = avctx->height;
    stDivpChnAttr.u32MaxWidth           = avctx->width;
    stDivpChnAttr.u32MaxHeight          = avctx->height;

    STCHECKRESULT(MI_DIVP_CreateChn(0, &stDivpChnAttr));
    STCHECKRESULT(MI_DIVP_StartChn(0));

    memset(&stOutputPortAttr, 0, sizeof(stOutputPortAttr));
    stOutputPortAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
    stOutputPortAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;//E_MI_SYS_PIXEL_FRAME_YUV422_YUYV;
    stOutputPortAttr.u32Width           = ALIGN_BACK(avctx->width, DISP_WIDTH_ALIGN);
    stOutputPortAttr.u32Height          = ALIGN_BACK(avctx->height, DISP_HEIGHT_ALIGN);

    STCHECKRESULT(MI_DIVP_SetOutputPortAttr(0, &stOutputPortAttr));

    stChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;
    MI_SYS_SetChnOutputPortDepth(&stChnPort, 2, 7);

   
    if (MI_SUCCESS != MI_SYS_GetFd(&stChnPort, (MI_S32 *)&pfd[0].fd)) 
    {
        printf("%s MI_SYS_GetFd fail\n", __FUNCTION__);
    }

    //bind vdec - divp

    ST_Sys_BindInfo_t stBindInfo;
    memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));

    stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stBindInfo.stSrcChnPort.u32DevId = 0;
    stBindInfo.stSrcChnPort.u32ChnId = 0;
    stBindInfo.stSrcChnPort.u32PortId = 0;

    stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stBindInfo.stDstChnPort.u32DevId = 0;
    stBindInfo.stDstChnPort.u32ChnId = 0;
    stBindInfo.stDstChnPort.u32PortId = 0;

    stBindInfo.u32SrcFrmrate = 0;
    stBindInfo.u32DstFrmrate = 0;

    STCHECKRESULT(ST_Sys_Bind(&stBindInfo));

    return 0;
}

static int DumpAVFrame(AVFrame *pict)
{
    int i;
    for(i = 0;i < AV_NUM_DATA_POINTERS;i++)
    {
        printf("data[%d]: %p,linesize[%d]: %d,buf[%d]: %p\n",i,pict->data[i],i,pict->linesize[i],i,pict->buf[i]);
    }
    printf("width: %d,height: %d,format: %d,keyframe: %d\n",pict->width,pict->height,pict->format,pict->key_frame);
    printf("pict_type: %d,sample_aspect_ratio: %d:%d\n",pict->pict_type,pict->sample_aspect_ratio.den,pict->sample_aspect_ratio.num);
    printf("pts: %lld,pkt_dts: %lld\n",pict->pts,pict->pkt_dts);
    printf("coded_picture_number: %d,display_picture_number: %d,quality: %d\n",pict->coded_picture_number,pict->display_picture_number,pict->quality);
    printf("repeat_pict: %d,interlaced_frame: %d,top_field_first: %d,palette_has_changed: %d\n",pict->repeat_pict,pict->interlaced_frame,pict->top_field_first,pict->palette_has_changed);
    printf("reordered_opaque: %lld,nb_extended_buf: %d,nb_side_data: %d\n",pict->reordered_opaque,pict->nb_extended_buf,pict->nb_side_data);
    printf("metadata: %p,decode_error_flags: %d,pkt_size: %d\n",pict->metadata,pict->decode_error_flags,pict->pkt_size);

    return 0;
}


static int ss_h264_decode_frame(AVCodecContext *avctx, void *data,
                             int *got_frame, AVPacket *avpkt)
{
    MI_VDEC_VideoStream_t stVdecStream;
    MI_S32 s32Ret;
    AVFrame *pFrame;
    int bufsize;
    int i;
    int rval = 0;
    
    SsH264Context *s = avctx->priv_data;
    pFrame = (AVFrame *)data;
    
    /* end of stream */
    if(!avpkt->size)
        return 0;
    stVdecStream.pu8Addr = avpkt->data;
    stVdecStream.u32Len = avpkt->size;
    stVdecStream.u64PTS = avpkt->pts;
    stVdecStream.bEndOfFrame = 1;
    stVdecStream.bEndOfStream = 0;
    
    //printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
    //        stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
    s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
    if(MI_SUCCESS != s32Ret)
    {
        printf("MI_VDEC_SendStream fail\n");
        return AVERROR_INVALIDDATA;
    }
TryAgain:
    rval = poll(&pfd, 1, 200);
    if(rval < 0)
    {
        printf("poll error!\n");
        return AVERROR_BUG;
    }
    if(rval == 0)
    {
        printf("get fd time out!\n");
        goto TryAgain;
    }

    
    MI_SYS_BUF_HANDLE hHandle;
    MI_SYS_BufInfo_t stBufInfo;
    MI_SYS_ChnPort_t stChnPort;
    
    stChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;

    s32Ret = MI_SYS_ChnOutputPortGetBuf(&stChnPort, &stBufInfo, &hHandle);
    if (s32Ret) 
    {
        printf("%s MI_SYS_ChnOutputPortGetBuf %d!\n", __FUNCTION__, s32Ret);
        return AVERROR_BUG;
    }
    else
    {
        s32Ret = av_frame_ref(pFrame, s->f);
        if (s32Ret < 0)
            return s32Ret;
        bufsize = stBufInfo.stFrameData.u16Height * stBufInfo.stFrameData.u16Width;
        
        pFrame->pts = stBufInfo.u64Pts;

        memcpy(pFrame->data[0],stBufInfo.stFrameData.pVirAddr[0],bufsize);
        memcpy(pFrame->data[1],stBufInfo.stFrameData.pVirAddr[0]+bufsize,bufsize/2);

        //frame->repeat_pict =;
        //frame->top_field_first =;
        //frame->interlaced_frame =;
        //frame->pict_type = ff_qsv_map_pictype(out_frame->dec_info.FrameType);
        //Key frame is IDR frame is only suitable for H264. For HEVC, IRAPs are key frames.
        //if (avctx->codec_id == AV_CODEC_ID_H264)
        //frame->key_frame = !!(out_frame->dec_info.FrameType & MFX_FRAMETYPE_IDR);
        
        MI_SYS_ChnOutputPortPutBuf(hHandle);
        *got_frame = true;
        
    }
    
    return avpkt->size;
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
    .capabilities          = /*AV_CODEC_CAP_DRAW_HORIZ_BAND | AV_CODEC_CAP_DR1 |*/
                             AV_CODEC_CAP_DELAY | AV_CODEC_CAP_SLICE_THREADS |
                             AV_CODEC_CAP_FRAME_THREADS,
    .hw_configs            = (const AVCodecHWConfigInternal*[]) {
                               NULL
                           },

};
