/**
 * @file
 * H.265/HEVC codec.
 * @author jeffrey.wu <jeffrey.wu@sigmastar.com.cn>
 */

#include <stdio.h>
#include <stdbool.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

#include "libavutil/avassert.h"
#include "libavutil/display.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/stereo3d.h"
#include "libavutil/time.h"
#include "internal.h"
#include "bytestream.h"
#include "cabac.h"
#include "cabac_functions.h"
#include "error_resilience.h"
#include "avcodec.h"
#include "golomb.h"
#include "hwaccel.h"
#include "mathops.h"
#include "me_cmp.h"
#include "mpegutils.h"
#include "profiles.h"
#include "rectangle.h"
#include "thread.h"

#include "hevc.h"
#include "hevc_data.h"
#include "hevc_parse.h"
#include "hevcdecSstar.h"
#include "h2645_parse.h"


#define MI_U32VALUE(data, idx) ((data[idx]<<24)|(data[idx + 1]<<16)|(data[idx + 2]<<8)|(data[idx + 3]))
#define MI_U16VALUE(data, idx) ((data[idx]<<8)|(data[idx + 1]))
#define ALIGN_UP(x, align) (((x) + ((align) - 1)) & ~((align) - 1))


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

#define VDEC_CHN_ID  0

typedef struct pts_list {
    int64_t pts;
	struct pts_list *next;
}pts_list_t;
	

typedef struct pts_queue{
    struct pts_list *first, *last;	
	uint8_t idx;
}pts_queue_t;


pts_queue_t v_pts;
MI_SYS_ChnPort_t  stChnPort;
pthread_cond_t continue_thread;

/**************************************************************************/
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

	pts_list->pts  = value;
    pts_list->next = NULL;        

    if (!q->first)
    {
        q->first = pts_list;
    }
    else
    {
        q->last->next = pts_list;
    }
	
    q->last = pts_list;
    q->idx ++;

	return 0;
}

static int pts_queue_get(pts_queue_t *q, int64_t *value)
{
	pts_list_t *pts_head;

    pts_head = q->first;
	if (pts_head)
	{
 	    q->first = pts_head->next;
 	    if (!q->first)
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

static int ss_hevc_get_frame(SsHevcContext *ssctx, AVFrame *frame)
{
	MI_U32 s32Ret, ysize, uvsize;
	MI_SYS_BUF_HANDLE stHandle;
	MI_SYS_BufInfo_t  stBufInfo;
	//void * pvirFramAddr = NULL;

	//uint8_t *in_data[4] = {NULL};
	//int  in_linesize[4] = {ssctx->frame->width, ssctx->frame->width, 0, 0};

	//printf("get in ss_hevc_get_frame!\n");
	
	stHandle = MI_HANDLE_NULL; 
	memset(&stBufInfo, 0x0, sizeof(MI_SYS_BufInfo_t));
	if (MI_SUCCESS == (s32Ret = MI_SYS_ChnOutputPortGetBuf(&stChnPort, &stBufInfo, &stHandle)))
	{
		//ssctx->frame->pts	   = stBufInfo.u64Pts;
		pts_queue_get(&v_pts, &ssctx->frame->pts);
		
		ssctx->frame->width  = stBufInfo.stFrameData.u32Stride[0];
		ssctx->frame->height = stBufInfo.stFrameData.u16Height;
		//ssctx->frame->height = ALIGN_UP(stBufInfo.stFrameData.u16Height, 32);
        
		// get image data form vdec module 
		ysize  = ssctx->frame->width * ssctx->frame->height;
		//uvsize = stBufInfo.stFrameData.u32BufSize - ysize;
		//printf("width : %d, height : %d, ysize : %d, uvsize : %d\n", ssctx->frame->width, ssctx->frame->height, ysize, uvsize);

		//if(MI_SUCCESS != MI_SYS_Mmap(stBufInfo.stFrameData.phyAddr[0], bufsize * 2, &pvirFramAddr, TRUE))
		//	av_log(ssctx->avctx, AV_LOG_ERROR, "MI_SYS_Mmap failed!\n");
		//stBufInfo.stFrameData.pVirAddr[0] = pvirFramAddr;

        #if 0
		in_data[0] = (uint8_t *)av_mallocz(ysize);
		in_data[1] = (uint8_t *)av_mallocz(ysize / 2);
		memcpy(in_data[0],stBufInfo.stFrameData.pVirAddr[0], ysize);
		memcpy(in_data[1],stBufInfo.stFrameData.pVirAddr[1], ysize / 2);
		
		// init image format convert context
		sws_scale(ssctx->img_ctx,                    // sws context
		          (const uint8_t *const *)in_data,   // src slice
		          in_linesize,                       // src stride
		          0,                                 // src slice y
		          ssctx->frame->height,              // src slice height
		          ssctx->frame->data,                // dst planes
		          ssctx->frame->linesize             // dst strides
	         	  );
		
		av_freep(&in_data[0]);
		av_freep(&in_data[1]);
        #else
		memcpy(ssctx->frame->data[0],stBufInfo.stFrameData.pVirAddr[0], ysize);
		memcpy(ssctx->frame->data[1],stBufInfo.stFrameData.pVirAddr[1], ysize / 2);
		#endif
		
		if (MI_SUCCESS != MI_SYS_ChnOutputPortPutBuf(stHandle)) {
			av_log(ssctx->avctx, AV_LOG_ERROR, "vdec output put buf error!\n");
		} 
		// copy valid frame to out frame
		if (ssctx->frame->buf[0]) {
			av_frame_ref(frame, ssctx->frame); 
			//printf("read a frame from sys module success!\n");
		}
	}else {
		s32Ret = AVERROR(EAGAIN);
	}

    //printf("exit out ss_hevc_get_frame!\n");
	
	return s32Ret;
}

static MI_U32 ss_hevc_vdec_init(AVCodecContext *avctx)
{
	MI_VDEC_ChnAttr_t stVdecChnAttr;
	MI_VDEC_OutputPortAttr_t stOutputPortAttr;
	MI_VDEC_CHN stVdecChn = VDEC_CHN_ID;

	memset(&stVdecChnAttr, 0, sizeof(MI_VDEC_ChnAttr_t));
	stVdecChnAttr.eCodecType   = E_MI_VDEC_CODEC_TYPE_H265;
	stVdecChnAttr.stVdecVideoAttr.u32RefFrameNum = 5;
	stVdecChnAttr.eVideoMode   = E_MI_VDEC_VIDEO_MODE_FRAME;
	stVdecChnAttr.u32BufSize   = 1 * 1920 * 1080;
	stVdecChnAttr.u32PicWidth  = avctx->width;
	stVdecChnAttr.u32PicHeight = avctx->height;
	stVdecChnAttr.eDpbBufMode  = E_MI_VDEC_DPB_MODE_NORMAL;
	stVdecChnAttr.u32Priority  = 0;

	STCHECKRESULT(MI_VDEC_CreateChn(stVdecChn, &stVdecChnAttr));
	STCHECKRESULT(MI_VDEC_StartChn(stVdecChn));

	memset(&stOutputPortAttr, 0, sizeof(MI_VDEC_OutputPortAttr_t));
	stOutputPortAttr.u16Width  = avctx->width;
	stOutputPortAttr.u16Height = avctx->height;
	STCHECKRESULT(MI_VDEC_SetOutputPortAttr(0, &stOutputPortAttr));

	memset(&stChnPort, 0, sizeof(MI_SYS_ChnPort_t));
	stChnPort.eModId	= E_MI_MODULE_ID_VDEC;
	stChnPort.u32DevId	= 0;
	stChnPort.u32ChnId	= stVdecChn;
	stChnPort.u32PortId = 0;

	STCHECKRESULT(MI_SYS_SetChnOutputPortDepth(&stChnPort, 5, 10));

	printf("ss_hevc_vdec_init successful!\n");

	return MI_SUCCESS;
}

static MI_U32 ss_hevc_send_stream(MI_U8 *data, MI_U32 size, int64_t pts)
{
	MI_VDEC_VideoStream_t stVdecStream;
	MI_U32 s32Ret;
	MI_VDEC_CHN stVdecChn = VDEC_CHN_ID;

	if (0x12 != data[4]) {
		
		stVdecStream.pu8Addr      = data;
		stVdecStream.u32Len       = size;
		stVdecStream.u64PTS       = pts;
		stVdecStream.bEndOfFrame  = 1;
		stVdecStream.bEndOfStream = 0;

		//printf("size : %d. data : %x,%x,%x,%x,%x,%x,%x,%x.\n", stVdecStream.u32Len, stVdecStream.pu8Addr[0],
		//stVdecStream.pu8Addr[1], stVdecStream.pu8Addr[2], stVdecStream.pu8Addr[3], stVdecStream.pu8Addr[4],
		//stVdecStream.pu8Addr[5], stVdecStream.pu8Addr[6], stVdecStream.pu8Addr[7]);

		//FILE *fpread = fopen("pstream.h265", "a+");
		//fwrite(stVdecStream.pu8Addr, stVdecStream.u32Len, 1, fpread);
		//fclose(fpread);		

		if(MI_SUCCESS != (s32Ret = MI_VDEC_SendStream(stVdecChn, &stVdecStream, 20)))
		{
			av_log(NULL, AV_LOG_ERROR, "[%s %d]MI_VDEC_SendStream failed!\n", __FUNCTION__, __LINE__);
			return AVERROR_INVALIDDATA;
		} 	
		//printf("[%s %d]MI_VDEC_SendStream success!.\n", __FUNCTION__, __LINE__);
	}
	
	return s32Ret;
}

static int ss_hevc_decode_nalu(SsHevcContext *s, AVPacket *avpkt)
{
	int eos_at_start = 1, i, ret;
	MI_U8  start_code[4] = {0,0,0,1}; 

	s->last_eos = s->eos;
    s->eos = 0;

	ret = ff_h2645_packet_split(&s->pkt, avpkt->data, avpkt->size, s->avctx, s->is_nalff,
                                 s->nal_length_size, s->avctx->codec_id, 1);
	if (ret < 0) {
        av_log(s->avctx, AV_LOG_ERROR,
			"Error splitting the input into NAL units.\n");
        return ret;
    }
	
	for (i = 0; i < s->pkt.nb_nals; i++) {
        if (s->pkt.nals[i].type == HEVC_NAL_EOB_NUT ||
            s->pkt.nals[i].type == HEVC_NAL_EOS_NUT) {
            if (eos_at_start) {
                s->last_eos = 1;
            } else {
                s->eos = 1;
            }
        } else {
            eos_at_start = 0;
        }
    }

	/* decode the NAL units */
    for (i = 0; i < s->pkt.nb_nals; i++) {
        H2645NAL *nal = &s->pkt.nals[i];
		MI_U8 * extradata_buf = av_mallocz(nal->size + sizeof(start_code));
		if (!extradata_buf)
        	return AVERROR(ENOMEM);
		
        switch (nal->type)
        {
	        case HEVC_NAL_VPS:		
			case HEVC_NAL_SPS:
			case HEVC_NAL_PPS:
			case HEVC_NAL_SEI_PREFIX:
	        case HEVC_NAL_SEI_SUFFIX:
				memcpy(extradata_buf, start_code, sizeof(start_code));
				memcpy(extradata_buf + sizeof(start_code), nal->data, nal->size);
				ss_hevc_send_stream(extradata_buf, nal->size + 4, 0);
			break;
		
			case HEVC_NAL_TRAIL_R:
			case HEVC_NAL_TRAIL_N:
			case HEVC_NAL_TSA_N:
			case HEVC_NAL_TSA_R:
			case HEVC_NAL_STSA_N:
			case HEVC_NAL_STSA_R:
			case HEVC_NAL_BLA_W_LP:
			case HEVC_NAL_BLA_W_RADL:
			case HEVC_NAL_BLA_N_LP:
			case HEVC_NAL_IDR_W_RADL:
			case HEVC_NAL_IDR_N_LP:
			case HEVC_NAL_CRA_NUT:
			case HEVC_NAL_RADL_N:
			case HEVC_NAL_RADL_R:
			case HEVC_NAL_RASL_N:
			case HEVC_NAL_RASL_R:				
				memcpy(extradata_buf, start_code, sizeof(start_code));
				memcpy(extradata_buf + sizeof(start_code), nal->data, nal->size);
			    //printf("packet pts : %lld.\n", avpkt->pts);
			    if (v_pts.idx >= 10)
					pts_queue_get(&v_pts, &ret);
			    pts_queue_put(&v_pts, avpkt->pts);
				ss_hevc_send_stream(extradata_buf, nal->size + 4, avpkt->pts);
			break;

			case HEVC_NAL_EOS_NUT:
    		case HEVC_NAL_EOB_NUT:
        		s->seq_decode = (s->seq_decode + 1) & 0xff;
        		s->max_ra     = INT_MAX;
        	break;
    		case HEVC_NAL_AUD:
    		case HEVC_NAL_FD_NUT:
       		break;
    		default:
        		av_log(s->avctx, AV_LOG_INFO, "Skipping NAL unit %d\n", s->nal_unit_type);
			break;
        }
		av_freep(&extradata_buf);
    }

	return MI_SUCCESS;
}


static int ss_hevc_parser_nalu(const uint8_t *buf, int buf_size, HEVCParamSets *ps,
                                 HEVCSEI *sei, int is_nalff, int nal_length_size,
                                 int err_recognition, int apply_defdispwin, void *logctx)
{
	int ret, i;
	MI_U8  start_code[4] = {0,0,0,1}; 
	H2645Packet pkt = { 0 };

	ret = ff_h2645_packet_split(&pkt, buf, buf_size, logctx, is_nalff, nal_length_size, AV_CODEC_ID_HEVC, 1);
    if (ret < 0) {
        return ret;
    }

    for (i = 0; i < pkt.nb_nals; i++) {
        H2645NAL *nal = &pkt.nals[i];
		MI_U8 * extradata_buf = av_mallocz(nal->size + sizeof(start_code));
		if (!extradata_buf)
        	return AVERROR(ENOMEM);

		switch (nal->type)
		{
	    	/* ignore everything except parameter sets and VCL NALUs */
	   	    case HEVC_NAL_VPS:		
			case HEVC_NAL_SPS:
			case HEVC_NAL_PPS:
			case HEVC_NAL_SEI_PREFIX:
	        case HEVC_NAL_SEI_SUFFIX:
				printf("type of nal : %x. len : %d.\r\n", nal->type, nal->size);
				printf("data : %x,%x,%x,%x.\r\n", nal->data[0],nal->data[1],nal->data[2],nal->data[3]);		
				// VPS/SPS/PPS				
				memcpy(extradata_buf, start_code, sizeof(start_code));
				memcpy(extradata_buf+sizeof(start_code), nal->data,nal->size);
				ss_hevc_send_stream(extradata_buf, nal->size + 4, 0);
			break;

			default : break;
		}
			
		av_freep(&extradata_buf);
    }	
	return ret;
}

static av_cold int ss_hevc_decode_free(AVCodecContext *avctx)
{
	MI_VDEC_CHN stVdecChn = VDEC_CHN_ID;
	SsHevcContext *s = avctx->priv_data;
		
	av_frame_free(&s->frame);
	ff_h2645_packet_uninit(&s->pkt);
	//sws_freeContext(s->img_ctx);
    pts_queue_destroy(&v_pts);
	decoder_type = DEFAULT_DECODING;
	
	STCHECKRESULT(MI_VDEC_StopChn(stVdecChn));
	STCHECKRESULT(MI_VDEC_DestroyChn(stVdecChn));

	printf("ss_hevc_decode_free!\n");
	
	return 0;
}

 
static int ss_hevc_decode_extradata(SsHevcContext *s, uint8_t *buf, int length, int first)
{
	int ret = 0;
    GetByteContext gb;

    bytestream2_init(&gb, buf, length);

    if (length > 3 && (buf[0] || buf[1] || buf[2] > 1)) {
        /* It seems the extradata is encoded as hvcC format.
         * Temporarily, we support configurationVersion==0 until 14496-15 3rd
         * is finalized. When finalized, configurationVersion will be 1 and we
         * can recognize hvcC by checking if avctx->extradata[0]==1 or not. */
        int i, j, num_arrays, nal_len_size;

        s->is_nalff = 1;

        bytestream2_skip(&gb, 21);
        nal_len_size = (bytestream2_get_byte(&gb) & 3) + 1;
        num_arrays   = bytestream2_get_byte(&gb);

        /* nal units in the hvcC always have length coded with 2 bytes,
         * so put a fake nal_length_size = 2 while parsing them */
        s->nal_length_size = 2;

        /* Decode nal units from hvcC. */
        for (i = 0; i < num_arrays; i++) {
            int type = bytestream2_get_byte(&gb) & 0x3f;
            int cnt  = bytestream2_get_be16(&gb);

            for (j = 0; j < cnt; j++) {
                // +2 for the nal size field
                int nalsize = bytestream2_peek_be16(&gb) + 2;
                if (bytestream2_get_bytes_left(&gb) < nalsize) {
                    av_log(s->avctx, AV_LOG_ERROR,
                           "Invalid NAL unit size in extradata.\n");
                    return AVERROR_INVALIDDATA;
                }

                ret = ss_hevc_parser_nalu(gb.buffer, nalsize, &s->ps, &s->sei, s->is_nalff,
                                            s->nal_length_size, s->avctx->err_recognition, s->apply_defdispwin,
                                            s->avctx);
                if (ret < 0) {
                    av_log(s->avctx, AV_LOG_ERROR,
                           "Decoding nal unit %d %d from hvcC failed\n",
                           type, i);
                    return ret;
                }
                bytestream2_skip(&gb, nalsize);
            }
        }

        /* Now store right nal length size, that will be used to parse
         * all other nals */
        s->nal_length_size = nal_len_size;
    } else {
        s->is_nalff = 0;
        ret = ss_hevc_parser_nalu(buf, length, &s->ps, &s->sei, s->is_nalff, s->nal_length_size,
                                    s->avctx->err_recognition, s->apply_defdispwin, s->avctx);
        if (ret < 0)
            return ret;
    }

    return ret;
}


static av_cold int ss_hevc_decode_init(AVCodecContext *avctx)
{	
	MI_S32 s32Ret;
	const AVPixFmtDescriptor *desc;
	SsHevcContext *s = avctx->priv_data;  

    s->avctx = avctx;

    ff_bswapdsp_init(&s->bdsp);

    s->context_initialized = 1;
    s->eos = 0;

    ff_hevc_reset_sei(&s->sei);

	s->frame = av_frame_alloc();
	if (!s->frame) {
		av_log(avctx, AV_LOG_ERROR, "sshevc malloc frame error!\n");
		return 0;
	}
	s->frame->format = AV_PIX_FMT_NV12;
	s->frame->width  = ALIGN_UP(avctx->width, 32);
	s->frame->height = avctx->height;
	
	if (avctx->pix_fmt != AV_PIX_FMT_NONE) {
		desc = av_pix_fmt_desc_get(avctx->pix_fmt);
		av_log(avctx, AV_LOG_INFO, "video prefix format : %s.\n", desc->name);		
	} else {
		avctx->pix_fmt  = AV_PIX_FMT_NV12;
	}

	s32Ret = av_frame_get_buffer(s->frame, 32);
	if (s32Ret < 0) 
	{
		av_frame_free(&s->frame);
		av_log(avctx, AV_LOG_ERROR, "sshevc malloc frame buf error!\n");
	}

	pts_queue_init(&v_pts);
	decoder_type = HARD_DECODING;

	#if 0
 	// vdec解码之后输出的图像格式为NV12,转换为420P	
    s->img_ctx = sws_getContext( avctx->width,   			// src width
                                 avctx->height,  			// src height
                                 AV_PIX_FMT_NV12, 			// src format
                                 avctx->width,   			// dst width
                                 avctx->height,  			// dst height
                                 avctx->pix_fmt,            // dst format
                                 SWS_POINT,                 // 转换算法
                                 NULL,                      // src filter
                                 NULL,                      // dst filter
                                 NULL                       // param
                               );
    if (s->img_ctx == NULL)
    {
        av_log(avctx, AV_LOG_ERROR, "sws_getContext() failed\n");
        return -1;
    }	
	#endif

	// Init vdec module
	if (MI_SUCCESS != (s32Ret = ss_hevc_vdec_init(avctx))){
		printf("MI Vdec Init Fail!\n");
	}
	
	// parse nal data from packet
	if (avctx->extradata_size > 0 && avctx->extradata) {	
        s32Ret = ss_hevc_decode_extradata(s, avctx->extradata, avctx->extradata_size, 1);
        if (s32Ret < 0) {
            ss_hevc_decode_free(avctx);
            return s32Ret;
        }
	}

	printf("sshevc_decode_init success!\r\n");

	return 0;
}



static int ss_hevc_decode_frame(AVCodecContext *avctx, void *data,
						  			  int *got_frame, AVPacket *avpkt)
{
	MI_S32 s32Ret;
	struct timeval now;
    struct timespec outtime;
    pthread_mutex_t wait_mutex;
	SsHevcContext   *s = avctx->priv_data; 

	//printf("get in ss_hevc_decode_frame!\n");
	
	if (true == avctx->debug)
	{		
		// fetch frame buffer from vdec
		if (MI_SUCCESS != (s32Ret = ss_hevc_get_frame(s, (AVFrame *)data))) {
			//av_log(avctx, AV_LOG_INFO, "ss_hevc_get_frame failed!\n");
			// if not a frame wait for 10ms
			pthread_mutex_init(&wait_mutex, NULL);		
			pthread_mutex_lock(&wait_mutex);         
			gettimeofday(&now, NULL);       
			outtime.tv_sec  = now.tv_sec;
			outtime.tv_nsec = now.tv_usec * 1000 + 10 * 1000 * 1000;      
			pthread_cond_timedwait(&continue_thread, &wait_mutex, &outtime);	
			pthread_mutex_unlock(&wait_mutex);
			pthread_mutex_destroy(&wait_mutex);	
		} else {
			*got_frame = true;
		}
        //end of stream and vdec buf is null
		if (!avpkt->size && s32Ret < 0) {
		    av_log(avctx, AV_LOG_INFO, "packet size is 0!!\n");
		    return AVERROR_EOF;
		} else {
		    // continue to send stream to vdec
		    if (MI_SUCCESS != ss_hevc_decode_nalu(s, avpkt))
			    av_log(avctx, AV_LOG_ERROR, "ss_hevc_decode_nalu failed!\n");
		}	
	}

	//printf("exit out ss_hevc_decode_frame!\n");

	if (s32Ret < 0)
		return s32Ret;
	return avpkt->size;
}



AVCodec ff_sshevc_decoder = {
    .name                  = "sshevc",
    .long_name             = NULL_IF_CONFIG_SMALL("HEVC (High Efficiency Video Coding)"),
    .type                  = AVMEDIA_TYPE_VIDEO,
    .id                    = AV_CODEC_ID_HEVC,
    .priv_data_size        = sizeof(SsHevcContext),
    .init                  = ss_hevc_decode_init,
    .close                 = ss_hevc_decode_free,
    .decode                = ss_hevc_decode_frame,
    .capabilities          = /*AV_CODEC_CAP_DR1 |*/ AV_CODEC_CAP_DELAY |
                             AV_CODEC_CAP_SLICE_THREADS | AV_CODEC_CAP_FRAME_THREADS,
    .hw_configs            = (const AVCodecHWConfigInternal*[]) {
                               NULL
                           },

};



