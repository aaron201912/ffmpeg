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

#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK MAKE_YUYV_VALUE(0,128,128)
#define YUYV_WHITE MAKE_YUYV_VALUE(255,128,128)
#define YUYV_RED MAKE_YUYV_VALUE(76,84,255)
#define YUYV_GREEN MAKE_YUYV_VALUE(149,43,21)
#define YUYV_BLUE MAKE_YUYV_VALUE(29,225,107)


#define SAVE_YUV_TO_FILE 0

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

static MI_S32 Hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
{
    switch (Event)
    {
        case E_MI_HDMI_EVENT_HOTPLUG:
            printf("E_MI_HDMI_EVENT_HOTPLUG.\n");
            STCHECKRESULT(MI_HDMI_Start(eHdmi));
            break;
        case E_MI_HDMI_EVENT_NO_PLUG:
            printf("E_MI_HDMI_EVENT_NO_PLUG.\n");
            STCHECKRESULT(MI_HDMI_Stop(eHdmi));
            break;
        default:
            printf("Unsupport event.\n");
            break;
    }

    return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_Init(void)
{
    MI_HDMI_InitParam_t stInitParam;
    MI_HDMI_DeviceId_e eHdmi = E_MI_HDMI_ID_0;

    stInitParam.pCallBackArgs = NULL;
    stInitParam.pfnHdmiEventCallback = Hdmi_callback_impl;

    STCHECKRESULT(MI_HDMI_Init(&stInitParam));
    STCHECKRESULT(MI_HDMI_Open(eHdmi));

	return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_DeInit(MI_HDMI_DeviceId_e eHdmi)
{

    STCHECKRESULT(MI_HDMI_Stop(eHdmi));
    STCHECKRESULT(MI_HDMI_Close(eHdmi));
    STCHECKRESULT(MI_HDMI_DeInit());

    return MI_SUCCESS;
}

/*
 * Default: HDMI MODE, YUV444, NoID(color depth)
 */
static MI_S32 ST_Hdmi_Start(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_TimingType_e eTimingType)
{
    MI_HDMI_Attr_t stAttr;

    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    stAttr.stEnInfoFrame.bEnableAudInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame  = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame  = FALSE;
    stAttr.stAudioAttr.bEnableAudio = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 0;
    stAttr.stAudioAttr.eBitDepth = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo = TRUE;
    stAttr.stVideoAttr.eColorType = E_MI_HDMI_COLOR_TYPE_RGB444;//default color type
    stAttr.stVideoAttr.eDeepColorMode = E_MI_HDMI_DEEP_COLOR_MAX;
    stAttr.stVideoAttr.eTimingType = eTimingType;
    stAttr.stVideoAttr.eOutputMode = E_MI_HDMI_OUTPUT_MODE_HDMI;
    STCHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr));

    STCHECKRESULT(MI_HDMI_Start(eHdmi));

    return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_SetAttr(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_TimingType_e eTimingType)
{
    MI_S32 s32Ret = MI_SUCCESS;
    MI_HDMI_Attr_t stAttr;

    STCHECKRESULT(MI_HDMI_SetAvMute(eHdmi, TRUE));
    STCHECKRESULT(MI_HDMI_GetAttr(eHdmi, &stAttr));

    stAttr.stVideoAttr.eTimingType = eTimingType;

    STCHECKRESULT(MI_HDMI_SetAttr(eHdmi, &stAttr));
    STCHECKRESULT(MI_HDMI_SetAvMute(eHdmi, FALSE));


    return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_GetEdid(MI_HDMI_DeviceId_e eHdmi, MI_U8 *pu8Data, MI_U8 *u8Len)
{
    MI_HDMI_Edid_t stEdid;
    MI_S32 i = 0;

    STCHECKRESULT(MI_HDMI_ForceGetEdid(eHdmi, &stEdid));
    for (i = 0; i < stEdid.u32Edidlength; i++)
    {
        printf("[%x] ", stEdid.au8Edid[i]);
    }
    printf("\n");
    memcpy(pu8Data, &stEdid.au8Edid, stEdid.u32Edidlength);
    *u8Len = stEdid.u32Edidlength;

    return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_GetSinkInfo(MI_HDMI_DeviceId_e eHdmi)
{
    MI_HDMI_SinkInfo_t stSinkInfo;
    MI_S32 s32Ret = MI_SUCCESS;

    STCHECKRESULT(MI_HDMI_GetSinkInfo(eHdmi, &stSinkInfo));

    return MI_SUCCESS;
}


static av_cold int ss_h264_decode_end(AVCodecContext *avctx)
{
    ST_Sys_BindInfo_t stBindInfo;
    SsH264Context *s = avctx->priv_data;

    if(!bExit)
    {
        ST_Hdmi_DeInit(0);
        MI_DISP_DisableInputPort(0 ,0);
        
        MI_DISP_DisableVideoLayer(0);
        MI_DISP_UnBindVideoLayer(0, 0);

         MI_DISP_Disable(0);
         
        memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));

        stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
        stBindInfo.stSrcChnPort.u32DevId = 0;
        stBindInfo.stSrcChnPort.u32ChnId = s->channel;
        stBindInfo.stSrcChnPort.u32PortId = 0;

        stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
        stBindInfo.stDstChnPort.u32DevId = 0;
        stBindInfo.stDstChnPort.u32ChnId = s->channel;
        stBindInfo.stDstChnPort.u32PortId = 0;
        
        STCHECKRESULT(ST_Sys_UnBind(&stBindInfo));
        
        STCHECKRESULT(MI_VDEC_StopChn(s->channel));
        STCHECKRESULT(MI_VDEC_DestroyChn(s->channel));

        //STCHECKRESULT(MI_DIVP_StopChn(s->channel));
        //STCHECKRESULT(MI_DIVP_DestroyChn(s->channel));
        
        STCHECKRESULT(MI_SYS_Exit());
        bExit = 1;
    }
    return 0;
}

static MI_S32 CreateDispDev(SsCropContext *pCropC)
{
    MI_DISP_PubAttr_t stPubAttr;
    //MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_RotateConfig_t stRotateConfig;

    //init disp
    memset(&stPubAttr, 0, sizeof(stPubAttr));
    stPubAttr.u32BgColor = YUYV_BLACK;
    stPubAttr.eIntfSync = E_MI_DISP_OUTPUT_1080P60;
    stPubAttr.eIntfType = E_MI_DISP_INTF_HDMI;
    STCHECKRESULT(MI_DISP_SetPubAttr(0, &stPubAttr));
    STCHECKRESULT(MI_DISP_Enable(0));
#if 0    //i2m unneeded
    memset(&stLayerAttr, 0, sizeof(stLayerAttr));

    stLayerAttr.stVidLayerSize.u16Width  = pCropC->cropwidth;
    stLayerAttr.stVidLayerSize.u16Height = pCropC->cropheight;


    stLayerAttr.ePixFormat = E_MI_SYS_PIXEL_FRAME_YUV_MST_420;
    stLayerAttr.stVidLayerDispWin.u16X      = pCropC->x;
    stLayerAttr.stVidLayerDispWin.u16Y      = pCropC->y;
    stLayerAttr.stVidLayerDispWin.u16Width  = pCropC->cropwidth;
    stLayerAttr.stVidLayerDispWin.u16Height = pCropC->cropheight;
#endif
    STCHECKRESULT(MI_DISP_BindVideoLayer(0, 0));
    //STCHECKRESULT(MI_DISP_SetVideoLayerAttr(0, &stLayerAttr));
    //STCHECKRESULT(MI_DISP_GetVideoLayerAttr(0, &stLayerAttr));
    
    STCHECKRESULT(MI_DISP_EnableVideoLayer(0));
    
    //init disp ch
    memset(&stInputPortAttr, 0, sizeof(stInputPortAttr));
    STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;

    stInputPortAttr.stDispWin.u16Width  = 640;//pCropC->cropwidth;
    stInputPortAttr.stDispWin.u16Height = 352;//pCropC->cropheight;

    stInputPortAttr.u16SrcWidth = 640;//pCropC->cropwidth;

    stInputPortAttr.u16SrcHeight = 352;//pCropC->cropheight;
    STCHECKRESULT(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    //STCHECKRESULT(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    STCHECKRESULT(MI_DISP_SetVideoLayerRotateMode(0, &stRotateConfig));
    STCHECKRESULT(MI_DISP_EnableInputPort(0, 0));
    STCHECKRESULT(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));
    
    //init HDMI
    STCHECKRESULT(ST_Hdmi_Init());
    STCHECKRESULT(ST_Hdmi_Start(E_MI_HDMI_ID_0, E_MI_HDMI_TIMING_1080_60P));

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
            
            //printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",stVdecStream.u32Len,stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
                    //stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
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
    SsCropContext *sCrop = avctx->priv_data_ss;
    //printf("ss_h264_decode_init pix_fmt: %d,cropwidth: %d,cropheight: %d\n",avctx->pix_fmt,sCrop->cropwidth,sCrop->cropheight);
    s->channel = 0;
    bExit = 0;
    s->f = av_frame_alloc();
    if (!s->f) 
        return 0;
    s->f->format = AV_PIX_FMT_YUV420P;
    if(0)//(sCrop->cropwidth)
    {
        s->f->width = sCrop->cropwidth;
        s->f->height = sCrop->cropheight;
    }
    else
    {
        s->f->width = 640;
        s->f->height = 352;
    }
    s32Ret = av_frame_get_buffer(s->f, 32);
    if (s32Ret < 0) 
    {
        av_frame_free(&s->f);
    }

    ST_Sys_Init();

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
    stOutputPortAttr.u16Width = s->f->width;
    stOutputPortAttr.u16Height = s->f->height;
    MI_VDEC_SetOutputPortAttr(0, &stOutputPortAttr);

    memset(&stChnPort, 0x0, sizeof(MI_SYS_ChnPort_t));
    stChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stChnPort.u32DevId = 0;
    stChnPort.u32ChnId = 0;
    stChnPort.u32PortId = 0;

    STCHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 0, 6));

    //init disp
    CreateDispDev(sCrop);
    usleep(30*1000);

    //bind vdec to disp
    stSrcChnPort.eModId = E_MI_MODULE_ID_VDEC;
    stSrcChnPort.u32DevId = 0;
    stSrcChnPort.u32ChnId = 0;
    stSrcChnPort.u32PortId = 0;

    stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stDstChnPort.u32DevId = 0;
    stDstChnPort.u32ChnId = 0;
    stDstChnPort.u32PortId = 0;
    MI_SYS_BindChnPort(&stSrcChnPort, &stDstChnPort, 30, 30);


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
    
    SsH264Context *s = avctx->priv_data;
    pFrame = (AVFrame *)data;
    
    /* end of stream */
    if(!avpkt->size)
    {
        printf("packet size is 0!!\n");
        return AVERROR_EOF;
    }
#if 1
    stVdecStream.pu8Addr = avpkt->data;
    stVdecStream.u32Len = avpkt->size;
    stVdecStream.u64PTS = avpkt->pts;
    stVdecStream.bEndOfFrame = 1;
    stVdecStream.bEndOfStream = 0;

    usleep(40*1000);
    //printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",avpkt->size,stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
            //stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
    s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
    if(MI_SUCCESS != s32Ret)
    {
        printf("MI_VDEC_SendStream fail\n");
        
        //return AVERROR_INVALIDDATA;
    }

#else
    //printf("avc: %d,nal_length_size: %d\n",s->is_avc,s->nal_length_size);
    //printf("ss_h264_decode_frame: %x,%x,%x,%x,%x\n",avpkt->data[0],avpkt->data[1],avpkt->data[2],avpkt->data[3],avpkt->data[4]);
    #if 0
    if (s->is_avc && av_packet_get_side_data(avpkt, AV_PKT_DATA_NEW_EXTRADATA, NULL)) 
    {
        int side_size;
        uint8_t *side = av_packet_get_side_data(avpkt, AV_PKT_DATA_NEW_EXTRADATA, &side_size);
        if (is_extra(side, side_size))
            ff_h264_decode_extradata(side, side_size,
                                     &s->ps, &s->is_avc, &s->nal_length_size,
                                     avctx->err_recognition, avctx);
    }
    if (s->is_avc && avpkt->size >= 9 && avpkt->data[0]==1 && avpkt->data[2]==0 && (avpkt->data[4]&0xFC)==0xFC) 
    {
        if (is_extra(avpkt->data, avpkt->size))
            return ff_h264_decode_extradata(avpkt->data, avpkt->size,
                                            &s->ps, &s->is_avc, &s->nal_length_size,
                                            avctx->err_recognition, avctx);
    }

    if (s->nal_length_size == 4) {
    if (avpkt->size > 8 && AV_RB32(avpkt->data) == 1 && AV_RB32(avpkt->data+5) > (unsigned)avpkt->size) {
        s->is_avc = 0;
    }else if(avpkt->size > 3 && AV_RB32(avpkt->data) > 1 && AV_RB32(avpkt->data) <= (unsigned)avpkt->size)
        s->is_avc = 1;
    }

    s32Ret = ff_h2645_packet_split(&s->pkt, avpkt->data, avpkt->size, avctx, s->is_avc,
                                s->nal_length_size, avctx->codec_id, avctx->flags2 & AV_CODEC_FLAG2_FAST);
    if (s32Ret < 0) {
        av_log(avctx, AV_LOG_ERROR,
               "Error splitting the input into NAL units.\n");
        return s32Ret;
    }

    printf("nalnum: %d\n",s->pkt.nb_nals);
    usleep(40*1000);
    for(int i = 0; i < s->pkt.nb_nals; i++)
    {
        H2645NAL *nal = &s->pkt.nals[i];

        stVdecStream.pu8Addr = nal->data;
        stVdecStream.u32Len = nal->size;
        stVdecStream.u64PTS = avpkt->pts;
        stVdecStream.bEndOfFrame = 1;
        stVdecStream.bEndOfStream = 0;
        
        printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",nal->size,stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
                stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
        s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
        if(MI_SUCCESS != s32Ret)
        {
            printf("MI_VDEC_SendStream fail\n");
            return AVERROR_INVALIDDATA;
        }
    #endif

    #if 0                
        //printf("naldata: %x,%x,%x,%x,%x,%x,%x,%x\n",nal->data[0],nal->data[1],nal->data[2],nal->data[3],nal->data[4],nal->data[5],nal->data[6],nal->data[7]);
        switch (nal->type) 
        {
        case H264_NAL_IDR_SLICE:
        case H264_NAL_SLICE:
        case H264_NAL_SPS:
        case H264_NAL_PPS:

        case H264_NAL_DPA:
        case H264_NAL_DPB:
        case H264_NAL_DPC:

        case H264_NAL_SEI:
        case H264_NAL_AUD:
        case H264_NAL_END_SEQUENCE:
        case H264_NAL_END_STREAM:
        case H264_NAL_FILLER_DATA:
        case H264_NAL_SPS_EXT:
        case H264_NAL_AUXILIARY_SLICE:
           
            {
                stVdecStream.pu8Addr = nal->data;
                stVdecStream.u32Len = nal->size;
                stVdecStream.u64PTS = avpkt->pts;
                stVdecStream.bEndOfFrame = 1;
                stVdecStream.bEndOfStream = 0;
                usleep(40*1000);
                printf("size: %d,data: %x,%x,%x,%x,%x,%x,%x,%x\n",nal->size,stVdecStream.pu8Addr[0],stVdecStream.pu8Addr[1],\
                        stVdecStream.pu8Addr[2],stVdecStream.pu8Addr[3],stVdecStream.pu8Addr[4],stVdecStream.pu8Addr[5],stVdecStream.pu8Addr[6],stVdecStream.pu8Addr[7]);
                s32Ret = MI_VDEC_SendStream(0, &stVdecStream, 20);
                if(MI_SUCCESS != s32Ret)
                {
                    printf("MI_VDEC_SendStream fail\n");
                    
                    //return AVERROR_INVALIDDATA;
                }
                break;
            }
        

        default:
            printf("Unknown NAL code: %d (%d bits)\n",nal->type, nal->size_bits);
        }
    


    }
    #endif
#endif
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
