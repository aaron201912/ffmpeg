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
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>



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

pthread_cond_t continue_thread;

static int ss_h264_get_frame(SsH264Context *ssctx, AVFrame *frame)
{
	MI_U32 s32Ret, bufsize;
	MI_SYS_BUF_HANDLE stHandle;
	MI_SYS_BufInfo_t  stBufInfo;
	MI_SYS_ChnPort_t  stChnPort;
	void * pvirFramAddr = NULL;

    stChnPort.eModId    = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId  = 0;
    stChnPort.u32ChnId  = 0;
    stChnPort.u32PortId = 0;
    MI_SYS_SetChnOutputPortDepth(&stChnPort, 5, 10);
	
	stHandle = MI_HANDLE_NULL; 
	memset(&stBufInfo, 0x0, sizeof(MI_SYS_BufInfo_t));
	if (MI_SUCCESS == (s32Ret = MI_SYS_ChnOutputPortGetBuf(&stChnPort, &stBufInfo, &stHandle)))
	{
		// 计算bufsize时注意height是数据对齐后的结果
//		ssctx->f->pts	 = stBufInfo.u64Pts;
		ssctx->f->width  = stBufInfo.stFrameData.u32Stride[0];
		ssctx->f->height = stBufInfo.stFrameData.u16Height;

		// 重新map输出内存的地址
		bufsize = ssctx->f->width * ssctx->f->height * 2;
		if(MI_SUCCESS != MI_SYS_Mmap(stBufInfo.stFrameData.phyAddr[0], bufsize, &pvirFramAddr, TRUE))
			av_log(ssctx, AV_LOG_ERROR, "MI_SYS_Mmap failed!\n");
		stBufInfo.stFrameData.pVirAddr[0] = pvirFramAddr;

		if (stBufInfo.stFrameData.pVirAddr[0])
		{
			memcpy(ssctx->f->data[0],stBufInfo.stFrameData.pVirAddr[0], bufsize / 2);
			memcpy(ssctx->f->data[1],stBufInfo.stFrameData.pVirAddr[1], bufsize / 4);
		}
		
		if (MI_SUCCESS != MI_SYS_ChnOutputPortPutBuf(stHandle)) {
			av_log(ssctx, AV_LOG_ERROR, "vdec output put buf error!\n");
		}	

		// 拷贝数据至输出frame
		if (ssctx->f->buf[0]) {
			av_frame_ref(frame, ssctx->f); 
		}
	} else 
		s32Ret = AVERROR(EAGAIN);

	return s32Ret;
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
    ST_Sys_BindInfo_t stBindInfo;
    SsH264Context *s = avctx->priv_data;
    printf("ss_h264_decode_end\n");
    av_frame_free(&s->f);
	ff_h2645_packet_uninit(&s->pkt);

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
			
//			FILE *fpread = fopen("stream_720.h264", "a+");
//			fwrite(stVdecStream.pu8Addr, 1, stVdecStream.u32Len, fpread);
//			fclose(fpread); 
			
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

//			FILE *fpread = fopen("stream_720.h264", "a+");
//			fwrite(stVdecStream.pu8Addr, 1, stVdecStream.u32Len, fpread);
//			fclose(fpread);
			
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
	AVPixFmtDescriptor *desc;
	
    SsH264Context *s = avctx->priv_data;
    //printf("ss_h264_decode_init width: %d\n",avctx->width);
    s->f = av_frame_alloc();
    if (!s->f) 
        return 0;
    s->f->format = AV_PIX_FMT_NV12;
    s->f->width  = avctx->width;
    s->f->height = avctx->height;

	if (avctx->pix_fmt != AV_PIX_FMT_NONE) {
		desc = av_pix_fmt_desc_get(avctx->pix_fmt);
		printf("video prefix format : %s.\n", desc->name);		
	} else {
		avctx->pix_fmt  = AV_PIX_FMT_NV12;
	}

    s32Ret = av_frame_get_buffer(s->f, 32);
    if (s32Ret < 0) 
    {
        av_frame_free(&s->f);
    }
    if(avctx->width != 0)
    {
        //init vdec
        memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
        stVdecChnAttr.eCodecType =E_MI_VDEC_CODEC_TYPE_H264;
        stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 5;
        stVdecChnAttr.eVideoMode = E_MI_VDEC_VIDEO_MODE_FRAME;
        stVdecChnAttr.u32BufSize = 1 * 1024 * 1024;
        stVdecChnAttr.u32PicWidth  = avctx->width;
        stVdecChnAttr.u32PicHeight = avctx->height;
        stVdecChnAttr.eInplaceMode = E_MI_VDEC_INPLACE_MODE_OFF;
        stVdecChnAttr.u32Priority = 0;
        MI_VDEC_CreateChn(0, &stVdecChnAttr);
        MI_VDEC_StartChn(0);

        MI_VDEC_OutputPortAttr_t stOutputPortAttr;
        stOutputPortAttr.u16Width  = avctx->width;
        stOutputPortAttr.u16Height = avctx->height;
        MI_VDEC_SetOutputPortAttr(0, &stOutputPortAttr);

        memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
        stChnPort.eModId    = E_MI_MODULE_ID_VDEC;
        stChnPort.u32DevId  = 0;
        stChnPort.u32ChnId  = 0;
        stChnPort.u32PortId = 0;

        STCHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 5, 10));
        
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
	int i, ret;
	int64_t pts;
	struct timeval now;
    struct timespec outtime;
    pthread_mutex_t wait_mutex;
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
		if (MI_SUCCESS != (s32Ret = ss_h264_get_frame(s, pFrame))) {
			av_log(avctx, AV_LOG_ERROR, "fetch a frame from buffer failed!\n");
			// 延时10毫秒等待解码
			pthread_mutex_init(&wait_mutex, NULL);		
			pthread_mutex_lock(&wait_mutex);         
			gettimeofday(&now, NULL);       
			outtime.tv_sec  = now.tv_sec;
			outtime.tv_nsec = now.tv_usec * 1000 + 10 * 1000 * 1000;      
			pthread_cond_timedwait(&continue_thread, &wait_mutex, &outtime);	
			pthread_mutex_unlock(&wait_mutex);
			pthread_mutex_destroy(&wait_mutex);	
		}else 
			*got_frame = true;

		// 一个packet中可能包含多个nal数据包	
		ret = ff_h2645_packet_split(&s->pkt, avpkt->data, avpkt->size, avctx, s->is_avc,
                                 s->nal_length_size, avctx->codec_id, 1);
		if (ret < 0) {
	        av_log(avctx, AV_LOG_ERROR, "Error splitting the input into NAL units.\n");
	        return ret;
	    }

		/* decode the NAL units */
	    for (i = 0; i < s->pkt.nb_nals; i++) {
	        H2645NAL *nal = &s->pkt.nals[i];
//			uint8_t *extrabuf = nal->data - sizeof(start_code);
			uint8_t *extrabuf = av_mallocz(nal->size + sizeof(start_code));
			if (!extrabuf)
	        	return AVERROR(ENOMEM);

//			printf("avpckt data addr : %x. nal data addr : %x. tmp buf addr : %x.\n", (uint32_t)avpkt->data, (uint32_t)nal->data, (uint32_t)extrabuf);

			switch (nal->type) {
				case H264_NAL_SLICE:
			    case H264_NAL_DPA:
			    case H264_NAL_DPB:
			    case H264_NAL_DPC:
			    case H264_NAL_IDR_SLICE:
					pts = avpkt->pts;
				break;

			    case H264_NAL_SPS:
			    case H264_NAL_PPS:
			    case H264_NAL_SEI:
					pts = 0;
				break;

				default : break;
	    	}
			
			memcpy(extrabuf, start_code, sizeof(start_code));
			memcpy(extrabuf + sizeof(start_code), nal->data, nal->size);			
	        stVdecStream.pu8Addr = extrabuf;
	        stVdecStream.u32Len  = nal->size + sizeof(start_code);
	        stVdecStream.u64PTS  = pts;
	        stVdecStream.bEndOfFrame  = 1;
	        stVdecStream.bEndOfStream = 0;

//			FILE *fpread = fopen("stream_720.h264", "a+");
//			fwrite(stVdecStream.pu8Addr, 1, stVdecStream.u32Len, fpread);
//			fclose(fpread);

//			printf("size: %d, data: %x,%x,%x,%x.\n",nal->size,extrabuf[4],extrabuf[5],extrabuf[6],extrabuf[7]);
//	        printf("size: %d, data: %x,%x,%x,%x.\n",nal->size,nal->data[0],nal->data[1],nal->data[2],nal->data[3]);
	        if (MI_SUCCESS != MI_VDEC_SendStream(0, &stVdecStream, 20)) {
				av_log(avctx, AV_LOG_ERROR, "MI_VDEC_SendStream fail\n");
				return AVERROR_INVALIDDATA;
	        }
			av_freep(&extrabuf);
	    }
    } else {
        if(!avctx->width)
        {
            int ret;
            int i;
            h264_sps_data_t sps;
            
            ret = ff_h2645_packet_split(&s->pkt, avpkt->data, avpkt->size, avctx, s->is_avc,
                                    s->nal_length_size, avctx->codec_id, avctx->flags2 & AV_CODEC_FLAG2_FAST);
            if (ret < 0) 
            {
                printf("ff_h2645_packet_split fail\n");
                return ret;
            }
            for (i = 0; i < s->pkt.nb_nals; i++) 
            {
                H2645NAL *nal = &s->pkt.nals[i];
                
                switch (nal->type) {
                case H264_NAL_SPS:
                    h264_parse_sps(nal->data,nal->size,&sps);
                    avctx->width = sps.width;
                    avctx->height = sps.height;
                    break;
                default:
                    break;
                }
                if(avctx->width)
                {
                    printf("SPS width: %d,height: %d\n",avctx->width,avctx->height);
                    break;
                }
            }
        }
    }

    if (s32Ret < 0)
        return s32Ret;
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

