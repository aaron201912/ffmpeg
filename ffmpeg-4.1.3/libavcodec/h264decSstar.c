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

static av_cold int ss_h264_decode_end(AVCodecContext *avctx)
{
    ST_Sys_BindInfo_t stBindInfo;
    SsH264Context *s = avctx->priv_data;
    printf("ss_h264_decode_end\n");
    av_frame_free(&s->f);

    STCHECKRESULT(MI_VDEC_StopChn(0));
    STCHECKRESULT(MI_VDEC_DestroyChn(0));
    return 0;
}


static int ss_h264_decode_extradata(const uint8_t *data, int size,
                             int *is_avc, int *nal_length_size,
                             int err_recognition, void *logctx)
{
    int ret;
    int j;
    MI_VDEC_VideoStream_t stVdecStream;
    uint8_t *extradata_buf;
    char start_code[]={0,0,0,1};
    
    if (!data || size <= 0)
        return -1;

    if (data[0] == 1) {
        int i, cnt, nalsize;
        const uint8_t *p = data;

        *is_avc = 1;

        if (size < 7) {
            av_log(logctx, AV_LOG_ERROR, "avcC %d too short\n", size);
            return AVERROR_INVALIDDATA;
        }

        // Decode sps from avcC
        cnt = *(p + 5) & 0x1f; // Number of sps
        p  += 6;
        for (i = 0; i < cnt; i++) {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - data))
                return AVERROR_INVALIDDATA;
            printf("\n");
            printf("SPS: ");
            for(j = 0;j < nalsize;j++)
            {
                printf("%x,",*(p+j));
            }
            printf("\n");
            extradata_buf = av_mallocz(nalsize+2);
            if (!extradata_buf)
                return AVERROR(ENOMEM);
            memcpy(extradata_buf,start_code,sizeof(start_code));
            memcpy(extradata_buf+sizeof(start_code),p+2,nalsize-2);
            stVdecStream.pu8Addr = extradata_buf;
            stVdecStream.u32Len = nalsize + 2;
            stVdecStream.u64PTS = 0;
            stVdecStream.bEndOfFrame = 1;
            stVdecStream.bEndOfStream = 0;
            
            ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
            if(0 != ret)
            {
                printf("MI_VDEC_SendStream fail\n");
                return AVERROR_INVALIDDATA;
            }
            av_freep(&extradata_buf);

            p += nalsize;
        }
        // Decode pps from avcC
        cnt = *(p++); // Number of pps
        for (i = 0; i < cnt; i++) {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - data))
                return AVERROR_INVALIDDATA;
            printf("\n");
            printf("PPS: ");
            for(j = 0;j < nalsize;j++)
            {
                printf("%x,",*(p+j));
            }
            printf("\n");
            extradata_buf = av_mallocz(nalsize+2);
            if (!extradata_buf)
                return AVERROR(ENOMEM);
            memcpy(extradata_buf,start_code,sizeof(start_code));
            memcpy(extradata_buf+sizeof(start_code),p+2,nalsize-2);
            stVdecStream.pu8Addr = extradata_buf;
            stVdecStream.u32Len = nalsize + 2;
            stVdecStream.u64PTS = 0;
            stVdecStream.bEndOfFrame = 1;
            stVdecStream.bEndOfStream = 0;
            
            ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
            if(0 != ret)
            {
                printf("MI_VDEC_SendStream fail\n");
                return AVERROR_INVALIDDATA;
            }
            av_freep(&extradata_buf);
            p += nalsize;
        }
        // Store right nal length size that will be used to parse all other nals
        *nal_length_size = (data[4] & 0x03) + 1;
    }
    return size;
}



static av_cold int ss_h264_decode_init(AVCodecContext *avctx)
{
    MI_VDEC_ChnAttr_t stVdecChnAttr;
    MI_S32 s32Ret;
    MI_SYS_ChnPort_t stChnPort;
    MI_SYS_ChnPort_t stSrcChnPort;
    MI_SYS_ChnPort_t stDstChnPort;

    SsH264Context *s = avctx->priv_data;

    s->f = av_frame_alloc();
    if (!s->f) 
        return 0;
    s->f->format = AV_PIX_FMT_YUV420P;
    s->f->width = 640;
    s->f->height = 352;

    s32Ret = av_frame_get_buffer(s->f, 32);
    if (s32Ret < 0) 
    {
        av_frame_free(&s->f);
    }

    //init vdec
    memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
    stVdecChnAttr.eCodecType =E_MI_VDEC_CODEC_TYPE_H264;
    stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 5;
    stVdecChnAttr.eVideoMode = E_MI_VDEC_VIDEO_MODE_FRAME;
    stVdecChnAttr.u32BufSize = 1 * 1024 * 1024;
    stVdecChnAttr.u32PicWidth = 1920;//avctx->width;
    stVdecChnAttr.u32PicHeight = 1080;//avctx->height;
    stVdecChnAttr.eInplaceMode = E_MI_VDEC_INPLACE_MODE_OFF;
    stVdecChnAttr.u32Priority = 0;
    MI_VDEC_CreateChn(0, &stVdecChnAttr);
    MI_VDEC_StartChn(0);

    MI_VDEC_OutputPortAttr_t stOutputPortAttr;
    stOutputPortAttr.u16Width = 640;
    stOutputPortAttr.u16Height = 352;
    MI_VDEC_SetOutputPortAttr(0, &stOutputPortAttr);

    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;

    STCHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 0, 6));
    
    //check h264 or avc1
    if (avctx->extradata_size > 0 && avctx->extradata) 
    {
        s32Ret = ss_h264_decode_extradata(avctx->extradata, avctx->extradata_size,
                                       &s->is_avc, &s->nal_length_size,
                                       avctx->err_recognition, avctx);
        if (s32Ret < 0) {
            ss_h264_decode_end(avctx);
            return s32Ret;
        }
    }

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

static int is_extra(const uint8_t *buf, int buf_size)
{
    int cnt= buf[5]&0x1f;
    const uint8_t *p= buf+6;
    if (!cnt)
        return 0;
    while(cnt--){
        int nalsize= AV_RB16(p) + 2;
        if(nalsize > buf_size - (p-buf) || (p[2] & 0x9F) != 7)
            return 0;
        p += nalsize;
    }
    cnt = *(p++);
    if(!cnt)
        return 0;
    while(cnt--){
        int nalsize= AV_RB16(p) + 2;
        if(nalsize > buf_size - (p-buf) || (p[2] & 0x9F) != 8)
            return 0;
        p += nalsize;
    }
    return 1;
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
    char start_code[] = {0,0,0,1};
    
    SsH264Context *s = avctx->priv_data;
    pFrame = (AVFrame *)data;
    
    /* end of stream */
    if(!avpkt->size)
    {
        printf("packet size is 0!!\n");
        return AVERROR_EOF;
    }

    if(avctx->debug)
    {
        stVdecStream.pu8Addr = avpkt->data;
        stVdecStream.u32Len = avpkt->size;
        stVdecStream.u64PTS = avpkt->pts;
        stVdecStream.bEndOfFrame = 1;
        stVdecStream.bEndOfStream = 0;

        //printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",avpkt->size,stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
                //stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
        s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
        if(MI_SUCCESS != s32Ret)
        {
            printf("MI_VDEC_SendStream fail\n");
            return AVERROR_INVALIDDATA;
        }
    }

    s32Ret = av_frame_ref(pFrame, s->f);
    if (s32Ret < 0)
        return s32Ret;
    *got_frame = true;
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

