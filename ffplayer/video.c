#include "video.h"
#include "packet.h"
#include "frame.h"
#include "player.h"

#include "SAT070CP50_1024x600.h"


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <poll.h>
#include <poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ENABLE_HDMI		1

#define DISP_DEV        0
#define DISP_LAYER      0
#define DISP_INPUTPORT  0

#define MAINWND_W       1024
#define MAINWND_H       600


static int fdv;
extern AVPacket flush_pkt;


static int queue_picture(player_stat_t *is, AVFrame *src_frame, double pts, double duration, int64_t pos)
{
    frame_t *vp;

    if (!(vp = frame_queue_peek_writable(&is->video_frm_queue)))
        return -1;

    vp->sar = src_frame->sample_aspect_ratio;
    vp->uploaded = 0;

    vp->width = src_frame->width;
    vp->height = src_frame->height;
    vp->format = src_frame->format;

    vp->pts = pts;
    vp->duration = duration;
    vp->pos = pos;
    //vp->serial = serial;

    //set_default_window_size(vp->width, vp->height, vp->sar);
    
    // 将AVFrame拷入队列相应位置
    av_frame_move_ref(vp->frame, src_frame);
	
    //printf("queue frame fomat: %d\n",vp->frame->format);
    // 更新队列计数及写索引
    //printf("before queue ridx: %d,widx: %d,size: %d,maxsize: %d\n ",is->video_frm_queue.rindex,is->video_frm_queue.windex,is->video_frm_queue.size,is->video_frm_queue.max_size);
    frame_queue_push(&is->video_frm_queue);
    //printf("after queue ridx: %d,widx: %d,size: %d,maxsize: %d\n ",is->video_frm_queue.rindex,is->video_frm_queue.windex,is->video_frm_queue.size,is->video_frm_queue.max_size);
    return 0;
}

static int64_t pktpos, pktpts;
static struct timeval tvStart;
static bool decoded;

// 从packet_queue中取一个packet，解码生成frame
static int video_decode_frame(AVCodecContext *p_codec_ctx, packet_queue_t *p_pkt_queue, AVFrame *frame)
{
    int ret;
	
    while (1)
    {
        AVPacket pkt;
	
        while (1)
        {
        	//printf("get in video_decode_frame!\n");
			
            if(p_pkt_queue->abort_request)
            {
                return -1;
            }
            // 3. 从解码器接收frame
            // 3.1 一个视频packet含一个视频frame
            //     解码器缓存一定数量的packet后，才有解码后的frame输出
            //     frame输出顺序是按pts的顺序，如IBBPBBP
            //     frame->pkt_pos变量是此frame对应的packet在视频文件中的偏移地址，值同pkt.pos
            ret = avcodec_receive_frame(p_codec_ctx, frame);
            if (ret < 0)
            {
                if (ret == AVERROR_EOF)
                {
                    av_log(NULL, AV_LOG_INFO, "video avcodec_receive_frame(): the decoder has been fully flushed\n");
                    avcodec_flush_buffers(p_codec_ctx);
                    return 0;
                }
                else if (ret == AVERROR(EAGAIN))
                {
                    //av_log(NULL, AV_LOG_INFO, "video avcodec_receive_frame(): output is not available in this state - "
                    //"user must try to send new input\n");
					//av_log(NULL, AV_LOG_ERROR, "cann't fetch a frame, try again!\n");
                    break;
                }
                else
                {
                    av_log(NULL, AV_LOG_ERROR, "video avcodec_receive_frame(): other errors\n");
                    continue;
                }
            }
            else
            {
                frame->pts = frame->best_effort_timestamp;
				//printf("best_effort_timestamp : %lld.\n", frame->pts);
				#if 0
                //printf("frame pos: %lld\n",frame->pkt_pos);
                if(frame->pkt_pos == pktpos || frame->pts == pktpts)
                {
                    struct timeval tv;
                    int64_t time;
                    
                    gettimeofday(&tv, NULL);
                    time = ((int64_t)tv.tv_sec * 1000000 + tv.tv_usec) - ((int64_t)tvStart.tv_sec * 1000000 + tvStart.tv_usec);
                    
                    printf("fps: %lld\n",time);
                    decoded = 0;
                }
				#endif

				//test yuv data is normal
				//int ysize = frame->width * frame->height;
				//if (frame->pts >= 600 && frame->pts <= 700) {
				//    FILE *fpread = fopen("pframe_1080.yuv", "a+");
            	//    fwrite(frame->data[0], ysize    , 1, fpread);
            	//    fwrite(frame->data[1], ysize / 2, 1, fpread);
            	//    fclose(fpread);
				//}
				
                return 1;   // 成功解码得到一个视频帧或一个音频帧，则返回
            }
        }

        // 1. 取出一个packet。使用pkt对应的serial赋值给d->pkt_serial
        if (packet_queue_get(p_pkt_queue, &pkt, true) < 0)
        {
            printf("packet_queue_get fail\n");
            return -1;
        }

        if (pkt.data == flush_pkt.data)
        {
            // 复位解码器内部状态/刷新内部缓冲区。
            avcodec_flush_buffers(p_codec_ctx);
        }
        else
        {
            // 2. 将packet发送给解码器
            //    发送packet的顺序是按dts递增的顺序，如IPBBPBB
            //    pkt.pos变量可以标识当前packet在视频文件中的地址偏移
			//printf("send packet to decoder!\n");
			#if 0
            //printf("pkt pos: %lld\n",pkt.pos);
            if(!decoded)
            {
                pktpos = pkt.pos;
				pktpts = pkt.pts;
                gettimeofday(&tvStart, NULL);
                decoded = 1;
				//printf("packet info : dts--%d,pts--%d\n", pkt.dts, pkt.pts);
            }
            //calctime = av_gettime_relative();
            #endif
			
            if (avcodec_send_packet(p_codec_ctx, &pkt) == AVERROR(EAGAIN))
            {
                av_log(NULL, AV_LOG_ERROR, "receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
            }
            av_packet_unref(&pkt);
        }
		//printf("exit out video_decode_frame!\n");
    }
}

// 将视频包解码得到视频帧，然后写入picture队列
static int video_decode_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    AVFrame *p_frame = av_frame_alloc();
    double pts;
    double duration;
    int ret;
    int got_picture;
    AVRational tb = is->p_video_stream->time_base;
    AVRational frame_rate = av_guess_frame_rate(is->p_fmt_ctx, is->p_video_stream, NULL);
    
    if (p_frame == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "av_frame_alloc() for p_frame failed\n");
        return AVERROR(ENOMEM);
    }

	printf("video time base : %f ms.\n", 1000 * av_q2d(tb));
	printf("frame rate num : %d. frame rate den : %d.\n", frame_rate.num, frame_rate.den);

	printf("get in video decode thread!\n");
	
    while (1)
    {
        if(is->abort_request)
        {
            printf("video decode thread exit\n");
            break;
        }
        got_picture = video_decode_frame(is->p_vcodec_ctx, &is->video_pkt_queue, p_frame);
        if (got_picture < 0)
        {
            printf("got pic fail\n");
            goto exit;
        }

        duration = (frame_rate.num && frame_rate.den ? av_q2d((AVRational){frame_rate.den, frame_rate.num}) : 0);   // 当前帧播放时长
		pts = (p_frame->pts == AV_NOPTS_VALUE) ? NAN : p_frame->pts * av_q2d(tb);   // 当前帧显示时间戳
	
		//printf("frame duration : %f. video frame clock : %f.\n", duration, pts);
        ret = queue_picture(is, p_frame, pts, duration, p_frame->pkt_pos);   // 将当前帧压入frame_queue
        av_frame_unref(p_frame);

        if (ret < 0)
        {
            goto exit;
        }

    }

exit:
    av_frame_free(&p_frame);

    return 0;
}

// 根据视频时钟与同步时钟(如音频时钟)的差值，校正delay值，使视频时钟追赶或等待同步时钟
// 输入参数delay是上一帧播放时长，即上一帧播放后应延时多长时间后再播放当前帧，通过调节此值来调节当前帧播放快慢
// 返回值delay是将输入参数delay经校正后得到的值
static double compute_target_delay(double delay, player_stat_t *is)
{
    double sync_threshold, diff = 0;

    /* update delay to follow master synchronisation source */

    /* if video is slave, we try to correct big delays by
       duplicating or deleting a frame */
    // 视频时钟与同步时钟(如音频时钟)的差异，时钟值是上一帧pts值(实为：上一帧pts + 上一帧至今流逝的时间差)
    diff = get_clock(&is->video_clk) - get_clock(&is->audio_clk);
    //printf("audio pts: %lf,video pts: %lf\n",is->audio_clk.pts,is->video_clk.pts);
    //printf("audio clock: %lf,video clock: %lf\n",get_clock(&is->audio_clk),get_clock(&is->video_clk));
    //printf("video pts: %lf,lu: %lf,curtime: %lf\n ",is->video_clk.pts,is->video_clk.last_updated,av_gettime_relative() / 1000000.0);
    //printf("audio pts: %lf,lu: %lf,curtime: %lf\n ",is->audio_clk.pts,is->audio_clk.last_updated,av_gettime_relative() / 1000000.0);
    //printf("video diff audio time: %lf\n",diff);
    // delay是上一帧播放时长：当前帧(待播放的帧)播放时间与上一帧播放时间差理论值
    // diff是视频时钟与同步时钟的差值

    /* skip or repeat frame. We take into account the
       delay to compute the threshold. I still don't know
       if it is the best guess */
    // 若delay < AV_SYNC_THRESHOLD_MIN，则同步域值为AV_SYNC_THRESHOLD_MIN
    // 若delay > AV_SYNC_THRESHOLD_MAX，则同步域值为AV_SYNC_THRESHOLD_MAX
    // 若AV_SYNC_THRESHOLD_MIN < delay < AV_SYNC_THRESHOLD_MAX，则同步域值为delay
    sync_threshold = FFMAX(AV_SYNC_THRESHOLD_MIN, FFMIN(AV_SYNC_THRESHOLD_MAX, delay));
    if (!isnan(diff))
    {
        if (diff <= -sync_threshold)        // 视频时钟落后于同步时钟，且超过同步域值
            delay = FFMAX(0, delay + diff); // 当前帧播放时刻落后于同步时钟(delay+diff<0)则delay=0(视频追赶，立即播放)，否则delay=delay+diff
        else if (diff >= sync_threshold && delay > AV_SYNC_FRAMEDUP_THRESHOLD)  // 视频时钟超前于同步时钟，且超过同步域值，但上一帧播放时长超长
            delay = delay + diff;           // 仅仅校正为delay=delay+diff，主要是AV_SYNC_FRAMEDUP_THRESHOLD参数的作用
        else if (diff >= sync_threshold)    // 视频时钟超前于同步时钟，且超过同步域值
            delay = 2 * delay;              // 视频播放要放慢脚步，delay扩大至2倍
    }

    av_log(NULL, AV_LOG_TRACE, "video: delay=%0.3f A-V=%f\n", delay, -diff);
    //printf("video: delay=%0.3f A-V=%f\n", delay, -diff);

    return delay;
}

static double vp_duration(player_stat_t *is, frame_t *vp, frame_t *nextvp) {
    if (vp->serial == nextvp->serial)
    {
        double duration = nextvp->pts - vp->pts;
        if (isnan(duration) || duration <= 0)
            return vp->duration;
        else
            return duration;
    } else {
        return 0.0;
    }
}

static void update_video_pts(player_stat_t *is, double pts, int64_t pos, int serial) {
    /* update current video pts */
    set_clock(&is->video_clk, pts, serial);            // 更新vidclock
    //-sync_clock_to_slave(&is->extclk, &is->vidclk);  // 将extclock同步到vidclock
}

static void video_display(player_stat_t *is)
{
	frame_t *vp;

	vp = frame_queue_peek_last(&is->video_frm_queue);
	//vp->frame->linesize[AV_NUM_DATA_POINTERS] = {vp->frame->width, vp->frame->width, 0, 0, 0, 0, 0, 0};
	//printf("get vp disp ridx: %d,format: %d\n",is->video_frm_queue.rindex,vp->frame->format);

	// 图像转换：p_frm_raw->data ==> p_frm_yuv->data
	// 将源图像中一片连续的区域经过处理后更新到目标图像对应区域，处理的图像区域必须逐行连续
	// plane: 如YUV有Y、U、V三个plane，RGB有R、G、B三个plane
	// slice: 图像中一片连续的行，必须是连续的，顺序由顶部到底部或由底部到顶部
	// stride/pitch: 一行图像所占的字节数，Stride=BytesPerPixel*Width+Padding，注意对齐
	// AVFrame.*data[]: 每个数组元素指向对应plane
	// AVFrame.linesize[]: 每个数组元素表示对应plane中一行图像所占的字节数	
    if (is->decoder_type == SOFT_DECODING)
	    sws_scale(is->img_convert_ctx,                    // sws context
	              (const uint8_t *const *)vp->frame->data,// src slice
	              vp->frame->linesize,                    // src stride
	              0,                                      // src slice y
	              is->p_vcodec_ctx->height,               // src slice height
	              is->p_frm_yuv->data,                    // dst planes
	              is->p_frm_yuv->linesize                 // dst strides
	              );
	
	int ysize, index;
	ysize = vp->frame->width * vp->frame->height;
	//printf("save yuv width: %d,height: %d\n",vp->frame->width,vp->frame->height);

	//put stream to ss disp start
	MI_SYS_BUF_HANDLE hHandle;
	MI_SYS_ChnPort_t  pstSysChnPort;
	MI_SYS_BufConf_t  stBufConf;
	MI_SYS_BufInfo_t  stBufInfo;

	pstSysChnPort.eModId    = E_MI_MODULE_ID_DIVP;//E_MI_MODULE_ID_DISP;
	pstSysChnPort.u32ChnId  = 0;
	pstSysChnPort.u32DevId  = 0;
	pstSysChnPort.u32PortId = 0;

	memset(&stBufInfo , 0 , sizeof(MI_SYS_BufInfo_t));
	memset(&stBufConf , 0 , sizeof(MI_SYS_BufConf_t));

	stBufConf.eBufType             = E_MI_SYS_BUFDATA_FRAME;
	stBufConf.u64TargetPts         = 0;
	stBufConf.stFrameCfg.u16Width  = vp->frame->width;
	stBufConf.stFrameCfg.u16Height = vp->frame->height;
	stBufConf.stFrameCfg.eFormat   = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
	stBufConf.stFrameCfg.eFrameScanMode = E_MI_SYS_FRAME_SCAN_MODE_PROGRESSIVE;

	if(MI_SUCCESS == MI_SYS_ChnInputPortGetBuf(&pstSysChnPort,&stBufConf,&stBufInfo,&hHandle, -1))
	{
	    stBufInfo.stFrameData.eCompressMode = E_MI_SYS_COMPRESS_MODE_NONE;
	    stBufInfo.stFrameData.eFieldType    = E_MI_SYS_FIELDTYPE_NONE;
	    stBufInfo.stFrameData.eTileMode     = E_MI_SYS_FRAME_TILE_MODE_NONE;
	    stBufInfo.bEndOfStream              = FALSE;
        //printf("divp width : %d, height : %d\n", stBufInfo.stFrameData.u16Width, stBufInfo.stFrameData.u16Height);
        if (is->decoder_type == SOFT_DECODING) {
			//向DIVP中填数据时必须按照stride大小填充
			if (stBufInfo.stFrameData.u32Stride[0] == stBufInfo.stFrameData.u16Width) {
		        memcpy(stBufInfo.stFrameData.pVirAddr[0], is->p_frm_yuv->data[0], ysize);
		        memcpy(stBufInfo.stFrameData.pVirAddr[1], is->p_frm_yuv->data[1], ysize / 2);
			} else {
                for (index = 0; index < stBufInfo.stFrameData.u16Height; index ++)
                {
                    memcpy(stBufInfo.stFrameData.pVirAddr[0] + index * stBufInfo.stFrameData.u32Stride[0], 
						   is->p_frm_yuv->data[0] + index * stBufInfo.stFrameData.u16Width, 
						   stBufInfo.stFrameData.u16Width);
                }

				for (index = 0; index < stBufInfo.stFrameData.u16Height / 2; index ++)
				{
                    memcpy(stBufInfo.stFrameData.pVirAddr[1] + index * stBufInfo.stFrameData.u32Stride[1], 
						   is->p_frm_yuv->data[1] + index * stBufInfo.stFrameData.u16Width, 
						   stBufInfo.stFrameData.u16Width);				    
				}
			}
		} else if (is->decoder_type == HARD_DECODING) {
		    memcpy(stBufInfo.stFrameData.pVirAddr[0], vp->frame->data[0], ysize);
		    memcpy(stBufInfo.stFrameData.pVirAddr[1], vp->frame->data[1], ysize / 2);
        }
        //printf("data0: %p,data1: %p,data2: %p\n",vp->frame->data[0],vp->frame->data[1],vp->frame->data[2]);
        #if 0
	    memcpy(stBufInfo.stFrameData.pVirAddr[0],vp->frame->data[0],ysize);
	    memcpy(stBufInfo.stFrameData.pVirAddr[1],vp->frame->data[1],ysize/4);
	    memcpy(stBufInfo.stFrameData.pVirAddr[1]+ysize/4,vp->frame->data[2],ysize/4);
        #endif

		//if (vp->frame->pts >= 600 && vp->frame->pts <= 700) {
		//    FILE *fpread0 = fopen("pic_later.yuv", "a+");
		//    fwrite(vp->frame->data[0], 1, ysize, fpread0);
		//    fwrite(vp->frame->data[1], 1, ysize / 2, fpread0);
		//    fclose(fpread0);
		//}
		
		//FILE *fpread1 = fopen("pic_later.yuv", "a+");
		//fwrite(is->p_frm_yuv->data[0], 1, ysize, fpread1);
		//fwrite(is->p_frm_yuv->data[1], 1, ysize / 2, fpread1);
		//fclose(fpread1);
		
	    MI_SYS_ChnInputPortPutBuf(hHandle ,&stBufInfo , FALSE);
	}
	//put stream to ss disp end
}

/* called to display each frame */
static void video_refresh(void *opaque, double *remaining_time)
{
    player_stat_t *is = (player_stat_t *)opaque;
    double time;
    static bool first_frame = true;
    
retry:
    // 暂停处理：不停播放上一帧图像
    if (is->paused)
        goto display;
        
    if (frame_queue_nb_remaining(&is->video_frm_queue) <= 0)  // 所有帧已显示
    {    
        // nothing to do, no picture to display in the queue
        //printf("already last frame: %d\n",is->video_frm_queue.size);
        //if (is->eof && is->video_pkt_queue.nb_packets == 0)
			//printf("video is palyed done!\n");
        return;
    }
	
    #if 1
	double last_duration, duration, delay;
	frame_t *vp, *lastvp;
	
	/* dequeue the picture */
	lastvp = frame_queue_peek_last(&is->video_frm_queue);     // 上一帧：上次已显示的帧
	vp = frame_queue_peek(&is->video_frm_queue);              // 当前帧：当前待显示的帧
	//printf("refresh ridx: %d,rs:%d,widx: %d,size: %d,maxsize: %d\n",is->video_frm_queue.rindex,is->video_frm_queue.rindex_shown,is->video_frm_queue.windex,is->video_frm_queue.size,is->video_frm_queue.max_size);
	//printf("lastpos: %ld,lastpts: %lf,vppos: %ld,vppts: %lf\n",lastvp->pos,lastvp->pts,vp->pos,vp->pts);
	// lastvp和vp不是同一播放序列(一个seek会开始一个新播放序列)，将frame_timer更新为当前时间
	if (first_frame)
	{
	    is->frame_timer = av_gettime_relative() / 1000000.0;
	    first_frame = false;
	}

	/* compute nominal last_duration */
	last_duration = vp_duration(is, lastvp, vp);        // 上一帧播放时长：vp->pts - lastvp->pts
	delay = compute_target_delay(last_duration, is);    // 根据视频时钟和同步时钟的差值，计算delay值
	//printf("last_duration: %d,delay: %d\n",last_duration,delay);
	time= av_gettime_relative()/1000000.0;
	// 当前帧播放时刻(is->frame_timer+delay)大于当前时刻(time)，表示播放时刻未到
	if (time < is->frame_timer + delay) {
	    // 播放时刻未到，则更新刷新时间remaining_time为当前时刻到下一播放时刻的时间差
	    *remaining_time = FFMIN(is->frame_timer + delay - time, *remaining_time);
	    // 播放时刻未到，则不播放，直接返回
	    //printf("not ready play\n");
	    return;
	}
	//printf("remaining time : %f. duration : %f.\n", *remaining_time, last_duration);
	// 更新frame_timer值
	is->frame_timer += delay;
	// 校正frame_timer值：若frame_timer落后于当前系统时间太久(超过最大同步域值)，则更新为当前系统时间
	if (delay > 0 && time - is->frame_timer > AV_SYNC_THRESHOLD_MAX)
	{
	    is->frame_timer = time;
		printf("###\n");
	}

	pthread_mutex_lock(&is->video_frm_queue.mutex);
	if (!isnan(vp->pts))
	{
	    update_video_pts(is, vp->pts, vp->pos, vp->serial); // 更新视频时钟：时间戳、时钟时间
	}
	pthread_mutex_unlock(&is->video_frm_queue.mutex);

	// 是否要丢弃未能及时播放的视频帧
	if (frame_queue_nb_remaining(&is->video_frm_queue) > 1)  // 队列中未显示帧数>1(只有一帧则不考虑丢帧)
	{         
	    frame_t *nextvp = frame_queue_peek_next(&is->video_frm_queue);  // 下一帧：下一待显示的帧
	    duration = vp_duration(is, vp, nextvp);             // 当前帧vp播放时长 = nextvp->pts - vp->pts
	    // 当前帧vp未能及时播放，即下一帧播放时刻(is->frame_timer+duration)小于当前系统时刻(time)
	    if (time > is->frame_timer + duration)
	    {
	        frame_queue_next(&is->video_frm_queue);   // 删除上一帧已显示帧，即删除lastvp，读指针加1(从lastvp更新到vp)
            printf("!!!\n");
			goto retry;
	    }
	}
	#else
	AVRational frame_rate = av_guess_frame_rate(is->p_fmt_ctx, is->p_video_stream, NULL);
	*remaining_time = av_q2d((AVRational){frame_rate.den, frame_rate.num});    //no sync
	//printf("remaining time : %f.\n", (*remaining_time) * AV_TIME_BASE);
	#endif
	// 删除当前读指针元素，读指针+1。若未丢帧，读指针从lastvp更新到vp；若有丢帧，读指针从vp更新到nextvp
    // 若提前删除当前帧会造成图像撕裂
	//frame_queue_next(&is->video_frm_queue);
    
	if (is->step && !is->paused)
	{
	    stream_toggle_pause(is);
	}

display:
    video_display(is);                      // 取出当前帧vp(若有丢帧是nextvp)进行播放
    if (!is->paused)
	    frame_queue_next(&is->video_frm_queue);

    return 0;
}

static int video_playing_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    double remaining_time = 0.0;
    printf("video_playing_thread in\n");
    while (1)
    {
        if(is->abort_request)
        {
            printf("video play thread exit\n");
            break;
        }
        if (remaining_time > 0.0)
        {
//			printf("delay time: %lf\n",remaining_time);
            av_usleep((unsigned)(remaining_time * 1000000.0));
        }
        remaining_time = REFRESH_RATE;
        // 立即显示当前帧，或延时remaining_time后再显示
        //av_usleep(10*1000);
        video_refresh(is, &remaining_time);
    }

    return 0;
}

static MI_S32 Hdmi_callback_impl(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_EventType_e Event, void *pEventParam, void *pUsrParam)
{
    switch (Event)
    {
        case E_MI_HDMI_EVENT_HOTPLUG:
            printf("E_MI_HDMI_EVENT_HOTPLUG.\n");
            CheckFuncResult(MI_HDMI_Start(eHdmi));
            break;
        case E_MI_HDMI_EVENT_NO_PLUG:
            printf("E_MI_HDMI_EVENT_NO_PLUG.\n");
            CheckFuncResult(MI_HDMI_Stop(eHdmi));
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

    CheckFuncResult(MI_HDMI_Init(&stInitParam));
    CheckFuncResult(MI_HDMI_Open(eHdmi));

	return MI_SUCCESS;
}

static MI_S32 ST_Hdmi_DeInit(MI_HDMI_DeviceId_e eHdmi)
{

    CheckFuncResult(MI_HDMI_Stop(eHdmi));
    CheckFuncResult(MI_HDMI_Close(eHdmi));
    CheckFuncResult(MI_HDMI_DeInit());

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
    CheckFuncResult(MI_HDMI_SetAttr(eHdmi, &stAttr));

    CheckFuncResult(MI_HDMI_Start(eHdmi));

    return MI_SUCCESS;
}

int ss_disp_Deinit(AVCodecContext *pVideoCodeCtx)
{
	ST_Sys_BindInfo_t stBindInfo;
	
	memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));
    stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stBindInfo.stSrcChnPort.u32DevId = 0;
    stBindInfo.stSrcChnPort.u32ChnId = 0;
    stBindInfo.stSrcChnPort.u32PortId = 0;

    stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stBindInfo.stDstChnPort.u32DevId = 0;
    stBindInfo.stDstChnPort.u32ChnId = 0;
    stBindInfo.stDstChnPort.u32PortId = 0;
    stBindInfo.u32SrcFrmrate = 0;
    stBindInfo.u32DstFrmrate = 0;
    ST_Sys_UnBind(&stBindInfo);

	MI_DIVP_StopChn(0);
	MI_DIVP_DestroyChn(0);

    CheckFuncResult(ST_Hdmi_DeInit(0));
	CheckFuncResult(MI_DISP_UnBindVideoLayer(0, 0));
    CheckFuncResult(MI_DISP_DisableInputPort(0 ,0));  
    CheckFuncResult(MI_DISP_DisableVideoLayer(0));
    CheckFuncResult(MI_DISP_Disable(0));
    
    return SUCCESS;
}


int ss_disp_Init(AVCodecContext *pVideoCodeCtx)
{
    MI_DISP_PubAttr_t stPubAttr;
    //MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_RotateConfig_t stRotateConfig;

	//init divp
    MI_DIVP_ChnAttr_t stDivpChnAttr;
    MI_DIVP_OutputPortAttr_t stDivpOutAttr;
    ST_Sys_BindInfo_t stBindInfo;

    //init divp
    memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
    stDivpChnAttr.bHorMirror            = FALSE;
    stDivpChnAttr.bVerMirror            = FALSE;
    stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
    stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
    stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
    stDivpChnAttr.stCropRect.u16X       = 0;
    stDivpChnAttr.stCropRect.u16Y       = 0;
    stDivpChnAttr.stCropRect.u16Width   = pVideoCodeCtx->width;
    stDivpChnAttr.stCropRect.u16Height  = pVideoCodeCtx->height;
    stDivpChnAttr.u32MaxWidth           = pVideoCodeCtx->width;
    stDivpChnAttr.u32MaxHeight          = pVideoCodeCtx->height;

    MI_DIVP_CreateChn(0, &stDivpChnAttr);
    MI_DIVP_StartChn(0);
    
    memset(&stDivpOutAttr, 0, sizeof(stDivpOutAttr));
    stDivpOutAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
    stDivpOutAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
    stDivpOutAttr.u32Width           = pVideoCodeCtx->width;
    stDivpOutAttr.u32Height          = pVideoCodeCtx->height;

    MI_DIVP_SetOutputPortAttr(0, &stDivpOutAttr);    

    //init disp
    memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.u32BgColor = YUYV_BLACK;
    stPubAttr.eIntfSync  = E_MI_DISP_OUTPUT_1080P60;
    stPubAttr.eIntfType  = E_MI_DISP_INTF_HDMI;
    CheckFuncResult(MI_DISP_SetPubAttr(0, &stPubAttr));
    CheckFuncResult(MI_DISP_Enable(0));
    
    CheckFuncResult(MI_DISP_BindVideoLayer(0, 0)); 
    CheckFuncResult(MI_DISP_EnableVideoLayer(0));
    
    //init disp ch
    memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
    CheckFuncResult(MI_DISP_GetInputPortAttr(0, 0, &stInputPortAttr));
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    printf("disp width: %d,heigh: %d\n",pVideoCodeCtx->width,pVideoCodeCtx->height);
    stInputPortAttr.stDispWin.u16Width  = pVideoCodeCtx->width;
    stInputPortAttr.stDispWin.u16Height = pVideoCodeCtx->height;

    stInputPortAttr.u16SrcWidth  = pVideoCodeCtx->width;

    stInputPortAttr.u16SrcHeight = pVideoCodeCtx->height;
    CheckFuncResult(MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr));
    
    stRotateConfig.eRotateMode = E_MI_DISP_ROTATE_NONE;
    CheckFuncResult(MI_DISP_SetVideoLayerRotateMode(0, &stRotateConfig));
    CheckFuncResult(MI_DISP_EnableInputPort(0, 0));
    CheckFuncResult(MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN));

    //init HDMI
    CheckFuncResult(ST_Hdmi_Init());
    CheckFuncResult(ST_Hdmi_Start(E_MI_HDMI_ID_0, E_MI_HDMI_TIMING_1080_60P));

    // bind module
    memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));
    stBindInfo.stSrcChnPort.eModId    = E_MI_MODULE_ID_DIVP;
    stBindInfo.stSrcChnPort.u32DevId  = 0;
    stBindInfo.stSrcChnPort.u32ChnId  = 0;
    stBindInfo.stSrcChnPort.u32PortId = 0;

    stBindInfo.stDstChnPort.eModId    = E_MI_MODULE_ID_DISP;
    stBindInfo.stDstChnPort.u32DevId  = 0;
    stBindInfo.stDstChnPort.u32ChnId  = 0;
    stBindInfo.stDstChnPort.u32PortId = 0;
    stBindInfo.u32SrcFrmrate = 0;
    stBindInfo.u32DstFrmrate = 0;

    ST_Sys_Bind(&stBindInfo);

    return SUCCESS;
}

MI_S32 ST_Sys_Bind(ST_Sys_BindInfo_t *pstBindInfo)
{
    MI_SYS_BindChnPort(&pstBindInfo->stSrcChnPort, &pstBindInfo->stDstChnPort, \
        pstBindInfo->u32SrcFrmrate, pstBindInfo->u32DstFrmrate);

    return MI_SUCCESS;
}

MI_S32 ST_Sys_UnBind(ST_Sys_BindInfo_t *pstBindInfo)
{
    MI_SYS_UnBindChnPort(&pstBindInfo->stSrcChnPort, &pstBindInfo->stDstChnPort);

    return MI_SUCCESS;
}


MI_S32 sstar_enable_display(MI_PANEL_ParamConfig_t *stPanelParam, MI_U16 srcWidth, MI_U16 srcHeight,MI_U16 dstWidth, MI_U16 dstHeight)
{	
    MI_DISP_DEV dispDev = DISP_DEV;
	MI_DISP_LAYER dispLayer = DISP_LAYER;
    MI_U32 u32InputPort = DISP_INPUTPORT;
	MI_DISP_PubAttr_t stPubAttr;
	MI_DISP_VideoLayerAttr_t stLayerAttr;
    MI_DISP_InputPortAttr_t stInputPortAttr;

    //init divp
    MI_DIVP_ChnAttr_t stDivpChnAttr;
    MI_DIVP_OutputPortAttr_t stDivpOutAttr;
    ST_Sys_BindInfo_t stBindInfo;

    memset(&stDivpChnAttr, 0, sizeof(MI_DIVP_ChnAttr_t));
    stDivpChnAttr.bHorMirror            = FALSE;
    stDivpChnAttr.bVerMirror            = FALSE;
    stDivpChnAttr.eDiType               = E_MI_DIVP_DI_TYPE_OFF;
    stDivpChnAttr.eRotateType           = E_MI_SYS_ROTATE_NONE;
    stDivpChnAttr.eTnrLevel             = E_MI_DIVP_TNR_LEVEL_OFF;
    stDivpChnAttr.stCropRect.u16X       = 0;
    stDivpChnAttr.stCropRect.u16Y       = 0;
    stDivpChnAttr.stCropRect.u16Width   = srcWidth;
    stDivpChnAttr.stCropRect.u16Height  = srcHeight;
    stDivpChnAttr.u32MaxWidth           = srcWidth;
    stDivpChnAttr.u32MaxHeight          = srcHeight;

    MI_DIVP_CreateChn(0, &stDivpChnAttr);
    MI_DIVP_StartChn(0);
    
    memset(&stDivpOutAttr, 0, sizeof(stDivpOutAttr));
    stDivpOutAttr.eCompMode          = E_MI_SYS_COMPRESS_MODE_NONE;
    stDivpOutAttr.ePixelFormat       = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
    stDivpOutAttr.u32Width           = dstWidth;
    stDivpOutAttr.u32Height          = dstHeight;

    MI_DIVP_SetOutputPortAttr(0, &stDivpOutAttr);
    
	
	memset(&stPubAttr, 0, sizeof(MI_DISP_PubAttr_t));
    stPubAttr.stSyncInfo.u16Vact = stPanelParam->u16Height;
    stPubAttr.stSyncInfo.u16Vbb  = stPanelParam->u16VSyncBackPorch;
    stPubAttr.stSyncInfo.u16Vfb  = stPanelParam->u16VTotal - (stPanelParam->u16VSyncWidth + stPanelParam->u16Height + stPanelParam->u16VSyncBackPorch);
    stPubAttr.stSyncInfo.u16Hact = stPanelParam->u16Width;
    stPubAttr.stSyncInfo.u16Hbb  = stPanelParam->u16HSyncBackPorch;
    stPubAttr.stSyncInfo.u16Hfb  = stPanelParam->u16HTotal - (stPanelParam->u16HSyncWidth + stPanelParam->u16Width + stPanelParam->u16HSyncBackPorch);
    stPubAttr.stSyncInfo.u16Bvact= 0;
    stPubAttr.stSyncInfo.u16Bvbb = 0;
    stPubAttr.stSyncInfo.u16Bvfb = 0;
    stPubAttr.stSyncInfo.u16Hpw  = stPanelParam->u16HSyncWidth;
    stPubAttr.stSyncInfo.u16Vpw  = stPanelParam->u16VSyncWidth;
    stPubAttr.stSyncInfo.u32FrameRate = stPanelParam->u16DCLK*1000000/(stPanelParam->u16HTotal*stPanelParam->u16VTotal);
    stPubAttr.eIntfSync  = E_MI_DISP_OUTPUT_USER;
    stPubAttr.eIntfType  = E_MI_DISP_INTF_LCD;
    stPubAttr.u32BgColor = YUYV_BLACK;
    MI_DISP_SetPubAttr(dispDev, &stPubAttr);
    MI_DISP_Enable(dispDev);

	stLayerAttr.stVidLayerSize.u16Width     = dstWidth;
	stLayerAttr.stVidLayerSize.u16Height    = dstHeight;
	stLayerAttr.ePixFormat = E_MI_SYS_PIXEL_FRAME_YUV_SEMIPLANAR_420;
	stLayerAttr.stVidLayerDispWin.u16X      = 0;
	stLayerAttr.stVidLayerDispWin.u16Y      = 0;
	stLayerAttr.stVidLayerDispWin.u16Width  = dstWidth;
	stLayerAttr.stVidLayerDispWin.u16Height = dstHeight;
	MI_DISP_SetVideoLayerAttr(dispLayer, &stLayerAttr);
	MI_DISP_BindVideoLayer(dispLayer, dispDev);
	MI_DISP_EnableVideoLayer(dispLayer);

	memset(&stInputPortAttr, 0, sizeof(MI_DISP_InputPortAttr_t));
	MI_DISP_GetInputPortAttr(dispLayer, u32InputPort, &stInputPortAttr);
    stInputPortAttr.stDispWin.u16X      = 0;
    stInputPortAttr.stDispWin.u16Y      = 0;
    stInputPortAttr.stDispWin.u16Width  = dstWidth;
    stInputPortAttr.stDispWin.u16Height = dstHeight;
	stInputPortAttr.u16SrcWidth         = dstWidth;
    stInputPortAttr.u16SrcHeight        = dstHeight;

    MI_DISP_SetInputPortAttr(dispLayer, u32InputPort, &stInputPortAttr);
    MI_DISP_EnableInputPort(dispLayer, u32InputPort);
    MI_DISP_SetInputPortSyncMode(dispLayer, u32InputPort, E_MI_DISP_SYNC_MODE_FREE_RUN);

	MI_PANEL_Init(stPanelParam->eLinkType);
    MI_PANEL_SetPanelParam(stPanelParam);

    memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));
    stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stBindInfo.stSrcChnPort.u32DevId = 0;
    stBindInfo.stSrcChnPort.u32ChnId = 0;
    stBindInfo.stSrcChnPort.u32PortId = 0;

    stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stBindInfo.stDstChnPort.u32DevId = 0;
    stBindInfo.stDstChnPort.u32ChnId = 0;
    stBindInfo.stDstChnPort.u32PortId = 0;
    stBindInfo.u32SrcFrmrate = 0;
    stBindInfo.u32DstFrmrate = 0;

    ST_Sys_Bind(&stBindInfo);

	return 0;
}

void sstar_disable_display(void)
{
	MI_DISP_DEV dispDev = DISP_DEV;
    MI_DISP_LAYER dispLayer = DISP_LAYER;
    MI_U32 u32InputPort = DISP_INPUTPORT;
	
	ST_Sys_BindInfo_t stBindInfo;
	
    MI_PANEL_DeInit();	

	memset(&stBindInfo, 0, sizeof(ST_Sys_BindInfo_t));
    stBindInfo.stSrcChnPort.eModId = E_MI_MODULE_ID_DIVP;
    stBindInfo.stSrcChnPort.u32DevId = 0;
    stBindInfo.stSrcChnPort.u32ChnId = 0;
    stBindInfo.stSrcChnPort.u32PortId = 0;

    stBindInfo.stDstChnPort.eModId = E_MI_MODULE_ID_DISP;
    stBindInfo.stDstChnPort.u32DevId = 0;
    stBindInfo.stDstChnPort.u32ChnId = 0;
    stBindInfo.stDstChnPort.u32PortId = 0;
    stBindInfo.u32SrcFrmrate = 0;
    stBindInfo.u32DstFrmrate = 0;
    ST_Sys_UnBind(&stBindInfo);

	MI_DIVP_StopChn(0);
	MI_DIVP_DestroyChn(0);
	
	MI_DISP_UnBindVideoLayer(dispLayer, dispDev);
	MI_DISP_DisableVideoLayer(dispLayer);
	MI_DISP_DisableInputPort(dispLayer, u32InputPort);
	MI_DISP_Disable(dispDev);
}

static int open_video_playing(void *arg)
{

    player_stat_t *is = (player_stat_t *)arg;
    int ret;
    int buf_size;
    uint8_t* buffer = NULL;
	AVPixFmtDescriptor *desc;
   
    // 为AVFrame.*data[]手工分配缓冲区，用于存储sws_scale()中目的帧视频数据
    buf_size = av_image_get_buffer_size(AV_PIX_FMT_NV12, 
                                        is->p_vcodec_ctx->width, 
                                        is->p_vcodec_ctx->height,
                                        1
                                        );
    //printf("alloc size: %d,width: %d,height: %d\n",buf_size,is->p_vcodec_ctx->width,is->p_vcodec_ctx->height);
    // buffer将作为p_frm_yuv的视频数据缓冲区
    buffer = (uint8_t *)av_malloc(buf_size);
    if (buffer == NULL)
    {
        printf("av_malloc() for buffer failed\n");
        return -1;
    }
    // 使用给定参数设定p_frm_yuv->data和p_frm_yuv->linesize
    ret = av_image_fill_arrays(is->p_frm_yuv->data,     // dst data[]
                               is->p_frm_yuv->linesize, // dst linesize[]
                               buffer,                  // src buffer
                               AV_PIX_FMT_NV12,         // pixel format
                               is->p_vcodec_ctx->width, // width
                               is->p_vcodec_ctx->height,// height
                               1                        // align
                               );
    if (ret < 0)
    {
        printf("av_image_fill_arrays() failed %d\n", ret);
        return -1;;
    }

    // A2. 初始化SWS context，用于后续图像转换
    //     此处第6个参数使用的是FFmpeg中的像素格式，对比参考注释B3
    //     FFmpeg中的像素格式AV_PIX_FMT_YUV420P对应SDL中的像素格式SDL_PIXELFORMAT_IYUV
    //     如果解码后得到图像的不被SDL支持，不进行图像转换的话，SDL是无法正常显示图像的
    //     如果解码后得到图像的能被SDL支持，则不必进行图像转换
    //     这里为了编码简便，统一转换为SDL支持的格式AV_PIX_FMT_YUV420P==>SDL_PIXELFORMAT_IYUV
	desc = av_pix_fmt_desc_get(is->p_vcodec_ctx->pix_fmt);
	printf("video prefix format : %s.\n", desc->name);

    is->img_convert_ctx = sws_getContext(is->p_vcodec_ctx->width,   // src width
                                         is->p_vcodec_ctx->height,  // src height
                                         is->p_vcodec_ctx->pix_fmt, // src format
                                         is->p_vcodec_ctx->width,   // dst width
                                         is->p_vcodec_ctx->height,  // dst height
                                         AV_PIX_FMT_NV12,           // dst format
                                         SWS_POINT,                 // flags
                                         NULL,                      // src filter
                                         NULL,                      // dst filter
                                         NULL                       // param
                                         );
    if (is->img_convert_ctx == NULL)
    {
        printf("sws_getContext() failed\n");
        return -1;
    }

    CheckFuncResult(pthread_create(&is->videoPlay_tid, NULL, video_playing_thread, is));

    return 0;
}

static int open_video_stream(player_stat_t *is)
{
    AVCodecParameters* p_codec_par = NULL;
    AVCodec* p_codec = NULL;
    AVCodecContext* p_codec_ctx = NULL;
    AVStream *p_stream = is->p_video_stream;
    int ret;

    // 1. 为视频流构建解码器AVCodecContext
    // 1.1 获取解码器参数AVCodecParameters
    p_codec_par = p_stream->codecpar;

    // 1.2 获取解码器
    if (is->decoder_type == HARD_DECODING) {
		switch(p_codec_par->codec_id) 
		{
			case AV_CODEC_ID_H264 : 
				p_codec = avcodec_find_decoder_by_name("ssh264"); break;
			
			
			case AV_CODEC_ID_HEVC : 
				p_codec = avcodec_find_decoder_by_name("sshevc"); break;

			default : 
				p_codec = avcodec_find_decoder(p_codec_par->codec_id); 
                is->decoder_type = SOFT_DECODING;
				break;
		}
    } else if (is->decoder_type == SOFT_DECODING) {
        switch(p_codec_par->codec_id) 
		{
			case AV_CODEC_ID_H264 : 
				p_codec = avcodec_find_decoder_by_name("h264"); break;
			
			
			case AV_CODEC_ID_HEVC : 
				p_codec = avcodec_find_decoder_by_name("hevc"); break;

			default : 
				p_codec = avcodec_find_decoder(p_codec_par->codec_id); break;
		}
    }
			
    if (p_codec == NULL)
    {
        printf("Cann't find codec!\n");
        return -1;
    }
    printf("open codec: %s\n",p_codec->name);

    // 1.3 构建解码器AVCodecContext
    // 1.3.1 p_codec_ctx初始化：分配结构体，使用p_codec初始化相应成员为默认值
    p_codec_ctx = avcodec_alloc_context3(p_codec);
    if (p_codec_ctx == NULL)
    {
        printf("avcodec_alloc_context3() failed\n");
        return -1;
    }
    // 1.3.2 p_codec_ctx初始化：p_codec_par ==> p_codec_ctx，初始化相应成员
    ret = avcodec_parameters_to_context(p_codec_ctx, p_codec_par);
    if (ret < 0)
    {
        printf("avcodec_parameters_to_context() failed\n");
        return -1;
    }
    // 1.3.3 p_codec_ctx初始化：使用p_codec初始化p_codec_ctx，初始化完成
    ret = avcodec_open2(p_codec_ctx, p_codec, NULL);
    if (ret < 0)
    {
        printf("avcodec_open2() failed %d\n", ret);
        return -1;
    }

    is->p_vcodec_ctx = p_codec_ctx;
	is->p_vcodec_ctx->debug = true;
    printf("codec width: %d,height: %d\n",is->p_vcodec_ctx->width,is->p_vcodec_ctx->height);
#if ENABLE_HDMI
	// 1.4.1 初始化DISP
	CheckFuncResult(ss_disp_Init(is->p_vcodec_ctx));
#else
	CheckFuncResult(sstar_enable_display(&stPanelParam, is->p_vcodec_ctx->width,is->p_vcodec_ctx->height,MAINWND_W, MAINWND_H));
#endif	
    // 2. 创建视频解码线程
    CheckFuncResult(pthread_create(&is->videoDecode_tid, NULL, video_decode_thread, is));

    return 0;
}

int open_video(player_stat_t *is)
{
    open_video_stream(is);
    sleep(1);
    open_video_playing(is);

    return 0;
}
