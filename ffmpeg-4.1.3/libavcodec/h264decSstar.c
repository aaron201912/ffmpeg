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
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>


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
#include "decode.h"

#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))

#define VDEC_CHN_ID     0


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

#if 0
typedef struct pts_list {
    int64_t pts;
    struct pts_list *next;
}pts_list_t;


typedef struct pts_queue{
    struct pts_list *first, *last;	
    uint8_t idx;
}pts_queue_t;

static void pts_queue_init(pts_queue_t *ptr)
{
    memset(ptr, 0, sizeof(pts_queue_t));
}

static int pts_queue_put(pts_queue_t *q, int64_t value)
{
    pts_list_t *pts_list;

    if (NULL == (pts_list = av_malloc(sizeof(pts_list_t))))
    {
        av_log(NULL, AV_LOG_ERROR, "malloc pts list failed!\n");
        return -1;
    }

    pts_list->pts  = value;       //strore value to queue
    pts_list->next = NULL;        

    if (!q->first)
    {
        q->first = pts_list;      //queue is null 
    }
    else
    {
        q->last->next = pts_list; //queue is not null 
    }

    q->last = pts_list;           //add new list to queue tail
    q->idx ++;                    //num of queue self-adding

    return 0;
}

static int pts_queue_get(pts_queue_t *q, int64_t *value)
{
    pts_list_t *pts_head;

    pts_head = q->first;
    if (pts_head)             // queue is not null
    {
        q->first = pts_head->next;
        if (!q->first)        // queue is the last
        {
            q->last = NULL;
        }
        q->idx --;
        *value = pts_head->pts;
        av_free(pts_head);
    } else {
        av_log(NULL, AV_LOG_INFO, "pts queue is null!\n");
    }

    return 0;
}

static int pts_queue_destroy(pts_queue_t *q)
{
    pts_list_t *tmp, *tmp1;

    for (tmp = q->first; tmp; tmp = tmp1) {
        tmp1 = tmp->next;
        av_freep(&tmp);
    }
    q->last = NULL;
    q->first = NULL;
    q->idx = 0;

    return 0;
}

pts_queue_t h264_pts;
#endif

pthread_cond_t h264_thread;
FILE *h264_fd;
/****************************************************************************************************/
// 此函数用于获取带B帧图像
static int ss_h264_inject_frame(SsH264Context *ssctx, AVFrame *frame)
{
    MI_S32 ret, ysize, totalsize;

    MI_SYS_ChnPort_t  stVdecChnPort;
    MI_SYS_BUF_HANDLE stVdecHandle;
    MI_SYS_BufInfo_t  stVdecBufInfo;

    mi_vdec_DispFrame_t *pstVdecInfo = NULL;

    stVdecHandle = MI_HANDLE_NULL; 
    memset(&stVdecBufInfo, 0x0, sizeof(MI_SYS_BufInfo_t));

    memset(&stVdecChnPort, 0, sizeof(MI_SYS_ChnPort_t));
    stVdecChnPort.eModId      = E_MI_MODULE_ID_VDEC;
    stVdecChnPort.u32DevId    = 0;
    stVdecChnPort.u32ChnId    = VDEC_CHN_ID;
    stVdecChnPort.u32PortId   = 0;
    MI_SYS_SetChnOutputPortDepth(&stVdecChnPort, 2, 4);

    if (MI_SUCCESS == (ret = MI_SYS_ChnOutputPortGetBuf(&stVdecChnPort, &stVdecBufInfo, &stVdecHandle)))
    {
        void *vdec_vir_addr = NULL;
        pstVdecInfo = (mi_vdec_DispFrame_t *)stVdecBufInfo.stMetaData.pVirAddr;

        frame->width  = pstVdecInfo->stFrmInfo.u16Stride;
        frame->height = pstVdecInfo->stFrmInfo.u16Height;
        frame->format = ssctx->f->format;

        ret = av_frame_get_buffer(frame, 32);
        if (ret < 0 || !frame->width || !frame->height)
        {
            av_frame_unref(frame);
            ret = AVERROR(ENOMEM);
        }
        else
        {
            //get frame pts from vdec
            frame->pts = stVdecBufInfo.u64Pts;
            //pts_queue_get(&h264_pts, &frame->pts);
            //printf("[%s %d]get frame pts : %lld\n", __FUNCTION__, __LINE__, stVdecBufInfo.u64Pts);
            ysize     = frame->width * frame->height;
            totalsize = ysize + ysize / 2;
            //printf("vdec output buf width : %d, height : %d, total size : %d\n", frame->width, frame->height, totalsize);
            //使用Map地址与大小必须4K对齐
            //printf("phyLumaAddr : 0x%llx, phyChromaAddr : 0x%llx\n", pstVdecInfo->stFrmInfo.phyLumaAddr, pstVdecInfo->stFrmInfo.phyChromaAddr);
            MI_SYS_Mmap(pstVdecInfo->stFrmInfo.phyLumaAddr, ALIGN_UP(totalsize, 4096), &vdec_vir_addr, FALSE);
            //复制图像信息到frame
            if (frame->buf[0])
            {
                memcpy(frame->data[0], vdec_vir_addr , ysize);
                memcpy(frame->data[1], vdec_vir_addr + ysize, ysize / 2);
            }

            //FILE *fpread = fopen("/mnt/h264_output.yuv", "a+");
            //fwrite(vdec_vir_addr , ysize, 1, fpread);
            //fwrite(vdec_vir_addr + ysize, ysize / 2, 1, fpread);
            //fclose(fpread);

            MI_SYS_Munmap(vdec_vir_addr, ALIGN_UP(totalsize, 4096));
        }

        if (MI_SUCCESS != MI_SYS_ChnOutputPortPutBuf(stVdecHandle)) {
            av_log(ssctx, AV_LOG_ERROR, "vdec output put buf error!\n");
        }
    } 
    else 
        ret = AVERROR(EAGAIN);

    return ret;
}

// 此函数用于获取不带B帧图像
static int ss_h264_get_frame(SsH264Context *ssctx, AVFrame *frame)
{
    MI_S32 ret, ysize;
    MI_SYS_BUF_HANDLE stVdecHandle;
    MI_SYS_BufInfo_t  stVdecBufInfo;
    MI_SYS_ChnPort_t  stVdecChnPort;

    stVdecHandle = MI_HANDLE_NULL; 
    memset(&stVdecBufInfo, 0x0, sizeof(MI_SYS_BufInfo_t));
    memset(&stVdecChnPort, 0, sizeof(MI_SYS_ChnPort_t));
    stVdecChnPort.eModId      = E_MI_MODULE_ID_VDEC;
    stVdecChnPort.u32DevId    = 0;
    stVdecChnPort.u32ChnId    = VDEC_CHN_ID;
    stVdecChnPort.u32PortId   = 0;
    MI_SYS_SetChnOutputPortDepth(&stVdecChnPort, 2, 4);
    
    if (MI_SUCCESS == (ret = MI_SYS_ChnOutputPortGetBuf(&stVdecChnPort, &stVdecBufInfo, &stVdecHandle)))
    {
        frame->width  = stVdecBufInfo.stFrameData.u32Stride[0];
        frame->height = stVdecBufInfo.stFrameData.u16Height;
        frame->format = ssctx->f->format;

        ret = av_frame_get_buffer(frame, 32);
        if (ret < 0 || !frame->width || !frame->height) {
            av_frame_unref(frame);
            ret = AVERROR(ENOMEM);
        }
        else
        {
            //get frame pts from vdec
            frame->pts = stVdecBufInfo.u64Pts;
            //pts_queue_get(&h264_pts, &frame->pts);
            //printf("[%s %d]get frame pts : %lld\n", __FUNCTION__, __LINE__, stVdecBufInfo.u64Pts);
            ysize  = frame->width * frame->height;
            //printf("vdec output buf width : %d, height : %d\n", frame->width, frame->height);
            if (frame->buf[0]) {
                memcpy(frame->data[0], stVdecBufInfo.stFrameData.pVirAddr[0], ysize);
                memcpy(frame->data[1], stVdecBufInfo.stFrameData.pVirAddr[1], ysize / 2);
            }
        }
        
        if (MI_SUCCESS != MI_SYS_ChnOutputPortPutBuf(stVdecHandle)) {
            av_log(ssctx, AV_LOG_ERROR, "vdec output put buf error!\n");
        }
    } 
    else 
        ret = AVERROR(EAGAIN);

    return ret;
}

static MI_S32 ss_h264_send_stream(MI_U8 *data, MI_U32 size, int64_t pts)
{
    MI_VDEC_VideoStream_t stVdecStream;
    MI_S32 ret;

    if (0x12 != data[4]) {

        stVdecStream.pu8Addr      = data;
        stVdecStream.u32Len       = size;
        stVdecStream.u64PTS       = pts;
        stVdecStream.bEndOfFrame  = 1;
        stVdecStream.bEndOfStream = 0;

        //printf("size : %d. data : %x,%x,%x,%x,%x,%x,%x,%x.\n", stVdecStream.u32Len, stVdecStream.pu8Addr[0],
        //stVdecStream.pu8Addr[1], stVdecStream.pu8Addr[2], stVdecStream.pu8Addr[3], stVdecStream.pu8Addr[4],
        //stVdecStream.pu8Addr[5], stVdecStream.pu8Addr[6], stVdecStream.pu8Addr[7]);

        //fwrite(stVdecStream.pu8Addr, stVdecStream.u32Len, 1, h264_fd);
        //printf("[%s %d]send to stream pts : %lld\n",  __FUNCTION__, __LINE__, stVdecStream.u64PTS);
        if(MI_SUCCESS != (ret = MI_VDEC_SendStream(0, &stVdecStream, 20)))
        {
            av_log(NULL, AV_LOG_ERROR, "[%s %d]MI_VDEC_SendStream failed!\n", __FUNCTION__, __LINE__);
            return AVERROR_INVALIDDATA;
        } 
        //printf("[%s %d]MI_VDEC_SendStream success!.\n", __FUNCTION__, __LINE__);
    }

    return ret;
}	

int h264_parse_sps(const uint8_t *buf, int len, h264_sps_data_t *sps) {
 
    //find sps
    bool findSPS = false;
 
    if (buf[2] == 0) {
        if ((buf[4]&0x1f) == 7) { //start code 0 0 0 1
            len -= 5;
            buf += 5;
            findSPS = true;
        }
    } else if (buf[2] == 1) {//start code 0 0 1
        if ((buf[3]&0x1f) == 7) {
            len -= 4;
            buf += 4;
            findSPS = true;
        }
    } else {
        if ((buf[0]&0x1f) == 7) { //no start code 0x67 ��ͷ
            len -= 1;
            buf += 1;
            findSPS = true;
        }
    }
 
 
    br_state br = BR_INIT(buf, len);
    int profile_idc, pic_order_cnt_type;
    int frame_mbs_only;
    int i, j;
 
    profile_idc = br_get_u8(&br);
    sps->profile = profile_idc;
    printf("H.264 SPS: profile_idc %d", profile_idc);
    /* constraint_set0_flag = br_get_bit(br);    */
    /* constraint_set1_flag = br_get_bit(br);    */
    /* constraint_set2_flag = br_get_bit(br);    */
    /* constraint_set3_flag = br_get_bit(br);    */
    /* reserved             = br_get_bits(br,4); */
    sps->level = br_get_u8(&br);
    br_skip_bits(&br, 8);
    br_skip_ue_golomb(&br);   /* seq_parameter_set_id */
    if (profile_idc >= 100) {
 
        if (br_get_ue_golomb(&br) == 3) /* chroma_format_idc      */
            br_skip_bit(&br);     /* residual_colour_transform_flag */
        br_skip_ue_golomb(&br); /* bit_depth_luma - 8             */
        br_skip_ue_golomb(&br); /* bit_depth_chroma - 8           */
        br_skip_bit(&br);       /* transform_bypass               */
        if (br_get_bit(&br))    /* seq_scaling_matrix_present     */
            for (i = 0; i < 8; i++)
                if (br_get_bit(&br)) {
                    /* seq_scaling_list_present    */
                    int last = 8, next = 8, size = (i<6) ? 16 : 64;
                    for (j = 0; j < size; j++) {
 
                        if (next)
                            next = (last + br_get_se_golomb(&br)) & 0xff;
                        last = next ? next: last;
 
                    }
 
                }
 
    }
 
    br_skip_ue_golomb(&br);      /* log2_max_frame_num - 4 */
    pic_order_cnt_type = br_get_ue_golomb(&br);
    if (pic_order_cnt_type == 0)
        br_skip_ue_golomb(&br);    /* log2_max_poc_lsb - 4 */
    else if (pic_order_cnt_type == 1) {
 
        br_skip_bit(&br);          /* delta_pic_order_always_zero     */
        br_skip_se_golomb(&br);    /* offset_for_non_ref_pic          */
        br_skip_se_golomb(&br);    /* offset_for_top_to_bottom_field  */
        j = br_get_ue_golomb(&br); /* num_ref_frames_in_pic_order_cnt_cycle */
        for (i = 0; i < j; i++)
            br_skip_se_golomb(&br);  /* offset_for_ref_frame[i]         */
 
    }
    br_skip_ue_golomb(&br);      /* ref_frames                      */
    br_skip_bit(&br);            /* gaps_in_frame_num_allowed       */
    sps->width  /* mbs */ = br_get_ue_golomb(&br) + 1;
    sps->height /* mbs */ = br_get_ue_golomb(&br) + 1;
    frame_mbs_only        = br_get_bit(&br);
    printf("H.264 SPS: pic_width:  %u mbs", (unsigned) sps->width);
    printf("H.264 SPS: pic_height: %u mbs", (unsigned) sps->height);
    printf("H.264 SPS: frame only flag: %d", frame_mbs_only);
 
    sps->width  *= 16;
    sps->height *= 16 * (2-frame_mbs_only);
 
    if (!frame_mbs_only)
        if (br_get_bit(&br)) /* mb_adaptive_frame_field_flag */
            printf("H.264 SPS: MBAFF");
    br_skip_bit(&br);      /* direct_8x8_inference_flag    */
    if (br_get_bit(&br)) {
        /* frame_cropping_flag */
        uint32_t crop_left   = br_get_ue_golomb(&br);
        uint32_t crop_right  = br_get_ue_golomb(&br);
        uint32_t crop_top    = br_get_ue_golomb(&br);
        uint32_t crop_bottom = br_get_ue_golomb(&br);
        printf("H.264 SPS: cropping %d %d %d %d",
               crop_left, crop_top, crop_right, crop_bottom);
 
        sps->width -= 2*(crop_left + crop_right);
        if (frame_mbs_only)
            sps->height -= 2*(crop_top + crop_bottom);
        else
            sps->height -= 4*(crop_top + crop_bottom);
 
    }
 
    /* VUI parameters */
    sps->pixel_aspect.num = 0;
    if (br_get_bit(&br)) {
        /* vui_parameters_present flag */
        if (br_get_bit(&br)) {
            /* aspect_ratio_info_present */
            uint32_t aspect_ratio_idc = br_get_u8(&br);
            printf("H.264 SPS: aspect_ratio_idc %d", aspect_ratio_idc);
 
            if (aspect_ratio_idc == 255 /* Extended_SAR */) {
 
                sps->pixel_aspect.num = br_get_u16(&br); /* sar_width */
                sps->pixel_aspect.den = br_get_u16(&br); /* sar_height */
                printf("H.264 SPS: -> sar %dx%d", sps->pixel_aspect.num, sps->pixel_aspect.den);
 
            } else {
 
                static const mpeg_rational_t aspect_ratios[] =
                {
                /* page 213: */
                /* 0: unknown */
                    {
                    0, 1
                    },
                /* 1...16: */
                    {
                    1,  1
                    }, {
                        12, 11
                    }, {
                        10, 11
                    }, {
                        16, 11
                    }, {
                        40, 33
                    }, {
                        24, 11
                    }, {
                        20, 11
                    }, {
                        32, 11
                    }, 
                    {
                    80, 33
                    }, {
                        18, 11
                    }, {
                        15, 11
                    }, {
                        64, 33
                    }, {
                        160, 99
                    }, {
                        4,  3
                    }, {
                        3,  2
                    }, {
                        2,  1
                    }
 
                };
 
                if (aspect_ratio_idc < sizeof(aspect_ratios)/sizeof(aspect_ratios[0])) {
 
                    memcpy(&sps->pixel_aspect, &aspect_ratios[aspect_ratio_idc], sizeof(mpeg_rational_t));
                    printf("H.264 SPS: -> aspect ratio %d / %d", sps->pixel_aspect.num, sps->pixel_aspect.den);
 
                } else {
 
                    printf("H.264 SPS: aspect_ratio_idc out of range !");
 
                }
 
            }
 
        }
 
    }
 
 
    printf("H.264 SPS: -> video size %dx%d, aspect %d:%d",
           sps->width, sps->height, sps->pixel_aspect.num, sps->pixel_aspect.den);

    return 1;
}

static av_cold int ss_h264_decode_end(AVCodecContext *avctx)
{
    SsH264Context *s = (SsH264Context *)avctx->priv_data;

    av_frame_free(&s->f);
    av_freep(&s->extradata);
    ff_h2645_packet_uninit(&s->pkt);
    //pts_queue_destroy(&h264_pts);
    //fclose(h264_fd);

    STCHECKRESULT(MI_VDEC_StopChn(0));
    STCHECKRESULT(MI_VDEC_DestroyChn(0));
    STCHECKRESULT(MI_VDEC_DeInitDev());
    av_log(avctx, AV_LOG_INFO, "ss_h264_decode_end successful!\n");
    return 0;
}

static int ss_h264_decode_extradata(const uint8_t *data, int size,
                             int *is_avc, int *nal_length_size, void *logctx)
{
    int j;
    SsH264Context *ssctx = (SsH264Context *)logctx;
    const uint8_t start_code[4]={0,0,0,1};
    
    if (!data || size <= 0)
        return -1;

    if (data[0] == 1) {
        int i, cnt, nalsize;
        const uint8_t *p = data;

        *is_avc = 1;

        if (size < 7) {
            av_log(ssctx, AV_LOG_ERROR, "avcC %d too short\n", size);
            return AVERROR_INVALIDDATA;
        }

        // Decode sps from avcC
        cnt = *(p + 5) & 0x1f; // Number of sps
        p  += 6;
        for (i = 0; i < cnt; i++) {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - data))
                return AVERROR_INVALIDDATA;
            printf("\nSPS: ");
            for(j = 0;j < nalsize;j++)
            {
                printf("%x,",*(p+j));
            }
            printf("\n");

            // copy sps data to extradata
            memcpy(ssctx->extradata + ssctx->extradata_size, start_code, sizeof(start_code));
            memcpy(ssctx->extradata + ssctx->extradata_size + sizeof(start_code), p + 2, nalsize - 2);
            ssctx->extradata[ssctx->extradata_size + 3] = 1;
            ssctx->extradata_size += (nalsize - 2 + sizeof(start_code));
            p += nalsize;
        }
        // Decode pps from avcC
        cnt = *(p++); // Number of pps
        for (i = 0; i < cnt; i++) {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - data))
                return AVERROR_INVALIDDATA;
            printf("\nPPS: ");
            for(j = 0;j < nalsize;j++)
            {
                printf("%x,",*(p+j));
            }
            printf("\n");

            // copy pps data to extradata
            memcpy(ssctx->extradata + ssctx->extradata_size, start_code, sizeof(start_code));
            memcpy(ssctx->extradata + ssctx->extradata_size + sizeof(start_code), p + 2, nalsize - 2);
            ssctx->extradata[ssctx->extradata_size + 3] = 1;
            ssctx->extradata_size += (nalsize - 2 + sizeof(start_code));
            p += nalsize;
        }
        // Store right nal length size that will be used to parse all other nals
        *nal_length_size = (data[4] & 0x03) + 1;
    }
    return size;
}

static MI_U32 ss_h264_vdec_init(AVCodecContext *avctx)
{
    MI_VDEC_ChnAttr_t stVdecChnAttr;
    MI_VDEC_OutputPortAttr_t stOutputPortAttr;
    MI_VDEC_CHN stVdecChn = VDEC_CHN_ID;
    MI_VDEC_InitParam_t stVdecInitParam;

    av_log(avctx, AV_LOG_WARNING, "h264 has b frames : %d\n", avctx->has_b_frames);
    memset(&stVdecInitParam, 0, sizeof(MI_VDEC_InitParam_t));
    if (avctx->has_b_frames > 0)
        stVdecInitParam.bDisableLowLatency = true;
    else
        stVdecInitParam.bDisableLowLatency = false;
    STCHECKRESULT(MI_VDEC_InitDev(&stVdecInitParam));

    memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
    stVdecChnAttr.eCodecType    = E_MI_VDEC_CODEC_TYPE_H264;
    stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 16;
    stVdecChnAttr.eVideoMode    = E_MI_VDEC_VIDEO_MODE_FRAME;
    stVdecChnAttr.u32BufSize    = 1 * 1920 * 1080;
    stVdecChnAttr.u32PicWidth   = avctx->width;
    stVdecChnAttr.u32PicHeight  = avctx->height;
    stVdecChnAttr.eDpbBufMode   = E_MI_VDEC_DPB_MODE_NORMAL;
    stVdecChnAttr.u32Priority   = 0;

    STCHECKRESULT(MI_VDEC_CreateChn(stVdecChn, &stVdecChnAttr));
    STCHECKRESULT(MI_VDEC_StartChn(stVdecChn));

    memset(&stOutputPortAttr, 0, sizeof(MI_VDEC_OutputPortAttr_t));
    stOutputPortAttr.u16Width   = (avctx->flags  > 0) ? avctx->flags  : avctx->width;
    stOutputPortAttr.u16Height  = (avctx->flags2 > 0) ? avctx->flags2 : avctx->height;
    STCHECKRESULT(MI_VDEC_SetOutputPortAttr(0, &stOutputPortAttr));

    printf("ss_h264_vdec_init successful!\n");

    return MI_SUCCESS;
}


static av_cold int ss_h264_decode_init(AVCodecContext *avctx)
{
    MI_S32 ret;
    const AVPixFmtDescriptor *desc;
    SsH264Context *s = (SsH264Context *)avctx->priv_data;

    s->f = av_frame_alloc();
    if (!s->f)
    {
        av_log(avctx, AV_LOG_ERROR, "ssh264 malloc for frame failed!\n");
        return 0;
    }

    s->avctx     = avctx;
    s->f->format = AV_PIX_FMT_NV12;
    s->f->width  = avctx->flags;
    s->f->height = avctx->flags2;

    s->extradata_size     = 0;
    s->max_extradata_size = 256;
    s->extradata = av_mallocz(s->max_extradata_size);
    if (!s->extradata)
    {
        av_log(avctx, AV_LOG_ERROR, "ssh264 malloc for extra data failed!\n");
        return 0;
    }

    //printf("ss_h264_decode_init width: %d, height : %d\n", avctx->width, avctx->height);
    //av_log(avctx, AV_LOG_INFO, "frame width : %d, height : %d.\n", s->f->width, s->f->height);

    if (avctx->pix_fmt != AV_PIX_FMT_NONE) {
        desc = av_pix_fmt_desc_get(avctx->pix_fmt);
        av_log(avctx, AV_LOG_INFO, "video prefix format : %s.\n", desc->name);
    } else {
        avctx->pix_fmt  = AV_PIX_FMT_NV12;
    }

    // Init pts output
    //pts_queue_init(&h264_pts);
    //h264_fd = fopen("/mnt/pstream_h264.es", "a+");

    // Init vdec module
    if (MI_SUCCESS != (ret = ss_h264_vdec_init(avctx)))
    {
        av_log(avctx, AV_LOG_ERROR, "ss_h264_vdec_init failed!\n");
    }

    //check h264 or avc1
    if (avctx->extradata_size > 0 && avctx->extradata) 
    {
        ret = ss_h264_decode_extradata(avctx->extradata, avctx->extradata_size,
                                       &s->is_avc, &s->nal_length_size, s);
        if (ret < 0 || s->extradata_size > s->max_extradata_size) {
            av_log(avctx, AV_LOG_ERROR, "ss_h264_decode_extradata failed!\n");
            ss_h264_decode_end(avctx);
            return ret;
        }
    }

    av_log(avctx, AV_LOG_INFO, "ss_h264_decode_init successful!\n");
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

static int64_t ss_h264_guess_correct_pts(AVCodecContext *ctx, int64_t reordered_pts, int64_t dts)
{
    int64_t pts = AV_NOPTS_VALUE;

    if (dts != AV_NOPTS_VALUE) {
        ctx->pts_correction_num_faulty_dts += dts <= ctx->pts_correction_last_dts;
        ctx->pts_correction_last_dts = dts;
    } else if (reordered_pts != AV_NOPTS_VALUE)
        ctx->pts_correction_last_dts = reordered_pts;

    if (reordered_pts != AV_NOPTS_VALUE) {
        ctx->pts_correction_num_faulty_pts += reordered_pts <= ctx->pts_correction_last_pts;
        ctx->pts_correction_last_pts = reordered_pts;
    } else if(dts != AV_NOPTS_VALUE)
        ctx->pts_correction_last_pts = dts;

    if ((ctx->pts_correction_num_faulty_pts<=ctx->pts_correction_num_faulty_dts || dts == AV_NOPTS_VALUE)
       && reordered_pts != AV_NOPTS_VALUE)
        pts = reordered_pts;
    else
        pts = dts;

    return pts;
}

static int ss_h264_decode_nalu(SsH264Context *s, AVPacket *avpkt)
{
    int ret, i, data_idx;
    const uint8_t start_code[4] = {0,0,0,1};
    uint8_t *extrabuf;

    ret = ss_h2645_packet_split(&s->pkt, avpkt->data, avpkt->size, s->avctx, s->is_avc,
                             s->nal_length_size, s->avctx->codec_id, 1);
    if (ret < 0) {
        av_log(s->avctx, AV_LOG_ERROR, "Error splitting the input into NAL units.\n");
        return ret;
    }
    //printf("avpkt size : %d, pkt.nb_nals : %d\n", avpkt->size, s->pkt.nb_nals);
    /* decode the NAL units */
    extrabuf = av_mallocz(avpkt->size + s->max_extradata_size);
    if (!extrabuf)
        return AVERROR(ENOMEM);
    data_idx = 0;
    for (i = 0; i < s->pkt.nb_nals; i++)
    {
        H2645NAL *nal = &s->pkt.nals[i];
        switch (nal->type) {
            case H264_NAL_IDR_SLICE:
                if (data_idx == 0)
                {
                    s->avctx->flags &= ~(1 << 7);
                    memcpy(extrabuf, s->extradata, s->extradata_size);
                    data_idx = s->extradata_size;
                }
            case H264_NAL_SLICE:
            case H264_NAL_DPA:
            case H264_NAL_DPB:
            case H264_NAL_DPC:
            case H264_NAL_SPS:
            case H264_NAL_PPS:
                //if (h264_pts.idx > 10)
                //    pts_queue_get(&h264_pts, &ret);
                //pts_queue_put(&h264_pts, avpkt->pts);
                //printf("pps,sps,sei dts : %lld, pts : %lld\n", avpkt->dts, avpkt->pts);
                if (!(s->avctx->flags & (1 << 7)))
                {
                    //add data head to nal
                    memcpy(extrabuf + data_idx, start_code, sizeof(start_code));
                    memcpy(extrabuf + data_idx + sizeof(start_code), nal->data, nal->size);
                    extrabuf[data_idx + 3] = 1;
                    data_idx += (nal->size + sizeof(start_code));
                    //printf("extra size : %d, nal size : %d, nal data : %x,%x,%x,%x,%x,%x\n", data_idx, nal->size + 4, extrabuf[data_idx + 2], 
                    //extrabuf[data_idx + 3], extrabuf[data_idx + 4], extrabuf[data_idx + 5], extrabuf[data_idx + 6], extrabuf[data_idx + 7]);
                }
                break;
            default : break;
        }
    }
    //send nal data to vdec
    if (!(s->avctx->flags & (1 << 7)))
    {
        avpkt->pts = ss_h264_guess_correct_pts(s->avctx, avpkt->pts, avpkt->dts);
        ret = ss_h264_send_stream(extrabuf, data_idx, avpkt->pts);
    }
    av_freep(&extrabuf);

    ff_h2645_packet_uninit(&s->pkt);

    return ret;
}

static int ss_h264_decode_frame(AVCodecContext *avctx, void *data,
                             int *got_frame, AVPacket *avpkt)
{
    int ret;

    SsH264Context *s = avctx->priv_data;

    //av_log(avctx, AV_LOG_INFO, "get in ss_h264_decode_frame\n");

    if(avctx->debug)
    {
        /* end of stream and vdec buf is null*/
        if (!avpkt->size && ret < 0) {
            av_log(avctx, AV_LOG_INFO, "packet size is 0!!\n");
            return AVERROR_EOF;
        } else {
            //stream is not at the end, continue to parse nal data
            ret = ss_h264_decode_nalu(s, avpkt);
            if (ret < 0)
                av_log(avctx, AV_LOG_ERROR, "ss_h264_decode_nalu failed!\n");
        }
    } 

    if (ret < 0)
        return ret;
    return avpkt->size;
}

static int ss_h264_receive_frame(AVCodecContext *avctx, AVFrame *frame)
{
    int ret, got_frame;
    SsH264Context        *s = avctx->priv_data; 
    AVCodecInternal   *avci = avctx->internal;
    DecodeSimpleContext *ds = &avci->ds;
    AVPacket         *avpkt = ds->in_pkt;

    struct timeval now;
    struct timespec outtime;
    pthread_mutex_t wait_mutex;

    //av_log(avctx, AV_LOG_INFO, "get in ss_h264_receive_frame\n");
    
    if (true == avctx->debug) 
    {
        while (!frame->buf[0])
        {
            if (!avpkt->data && !avci->draining && !(avctx->flags & (1 << 5)))
            {
                av_packet_unref(avpkt);
                ret = ff_decode_get_packet(avctx, avpkt);
                if (ret < 0 && ret != AVERROR_EOF) 
                    return AVERROR(EAGAIN);
                else 
                    return ret;
            }

            got_frame = 0;
            if (avctx->flags & (1 << 6))
                ret = ss_h264_inject_frame(s, frame);
            else
                ret = ss_h264_get_frame(s, frame);

            if (MI_SUCCESS != ret)
            {
                //av_log(avctx, AV_LOG_ERROR, "ss_h264 fetch frame from buffer failed!\n");
                // vdec wait for 10ms and continue to send stream
                pthread_mutex_init(&wait_mutex, NULL);
                pthread_mutex_lock(&wait_mutex);
                gettimeofday(&now, NULL);
                outtime.tv_sec  = now.tv_sec;
                outtime.tv_nsec = now.tv_usec * 1000 + 10 * 1000 * 1000;
                pthread_cond_timedwait(&h264_thread, &wait_mutex, &outtime);
                pthread_mutex_unlock(&wait_mutex);
                pthread_mutex_destroy(&wait_mutex);
            }
            else
            {
                got_frame = 1;
                frame->best_effort_timestamp = frame->pts;
            }

            if (avpkt->data)
            {
                ret = ss_h264_decode_nalu(s, avpkt);
                if (ret < 0)
                   av_log(avctx, AV_LOG_ERROR, "ss_h264_decode_nalu failed!\n");

                if (!(avctx->codec->caps_internal & FF_CODEC_CAP_SETS_PKT_DTS))
                    frame->pkt_dts = avpkt->dts;
                if (avctx->codec->type == AVMEDIA_TYPE_VIDEO && !avctx->has_b_frames) {
                    frame->pkt_pos = avpkt->pos;
                }

                if (!got_frame)
                    av_frame_unref(frame);
                
                if (ret >= 0 && avctx->codec->type == AVMEDIA_TYPE_VIDEO && !(avctx->flags & AV_CODEC_FLAG_TRUNCATED))
                    ret = avpkt->size;
                avci->compat_decode_consumed += ret;

                if (ret >= avpkt->size || ret < 0) {
                    av_packet_unref(avpkt);
                } else {
                    int consumed = ret;

                    avpkt->data                += consumed;
                    avpkt->size                -= consumed;
                    avci->last_pkt_props->size -= consumed; // See extract_packet_props() comment.
                    avpkt->pts                  = AV_NOPTS_VALUE;
                    avpkt->dts                  = AV_NOPTS_VALUE;
                    avci->last_pkt_props->pts = AV_NOPTS_VALUE;
                    avci->last_pkt_props->dts = AV_NOPTS_VALUE;
                }

                if (got_frame)
                    av_assert0(frame->buf[0]);
            }

            if (ret < 0)
                return ret;
        }
    }

    return avpkt->size;
}

static void ss_h264_decode_flush(AVCodecContext *avctx)
{
    if (avctx->flags & (1 << 7))
    {
        MI_VDEC_FlushChn(VDEC_CHN_ID);
        //av_log(avctx, AV_LOG_INFO, "ss_h264_decode_flush done!\n");
    }
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
    .flush                 = ss_h264_decode_flush,
    .capabilities          = /*AV_CODEC_CAP_DRAW_HORIZ_BAND | AV_CODEC_CAP_DR1 |*/
                             AV_CODEC_CAP_DELAY | AV_CODEC_CAP_SLICE_THREADS |
                             AV_CODEC_CAP_FRAME_THREADS,
    .hw_configs            = (const AVCodecHWConfigInternal*[]) {
                               NULL
                           },

};

