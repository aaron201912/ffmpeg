#include "player.h"
#include "packet.h"
#include "frame.h"
#include "audio.h"

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
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>

//sdk audio input/outout param
#define     AUDIO_INPUT_SAMPRATE       48000
#define     AUDIO_INPUT_CHLAYOUT       AV_CH_LAYOUT_MONO
#define     AUDIO_INPUT_SAMPFMT        AV_SAMPLE_FMT_S16

#define     AUDIO_OUTPUT_SAMPRATE       E_MI_AUDIO_SAMPLE_RATE_48000
#define     AUDIO_OUTPUT_CHLAYOUT       E_MI_AUDIO_SOUND_MODE_MONO
#define     AUDIO_OUTPUT_SAMPFMT        E_MI_AUDIO_BIT_WIDTH_16

#define MI_AUDIO_SAMPLE_PER_FRAME 1024


#define MI_AUDIO_MAX_SAMPLES_PER_FRAME     2048
#define MI_AUDIO_MAX_FRAME_NUM             6

#define MI_AO_PCM_BUF_SIZE_BYTE      (MI_AUDIO_MAX_SAMPLES_PER_FRAME * MI_AUDIO_MAX_FRAME_NUM * 2 * 4)

static int fda;
static void sdl_audio_callback(void *opaque, uint8_t *stream, int len);
extern AVPacket flush_pkt;

// 从packet_queue中取一个packet，解码生成frame
static int audio_decode_frame(AVCodecContext *p_codec_ctx, packet_queue_t *p_pkt_queue, AVFrame *frame)
{
    int ret;

    while (1)
    {
        AVPacket pkt;

        while (1)
        {
            //if (d->queue->abort_request)
            //    return -1;

            // 3.2 一个音频packet含一至多个音频frame，每次avcodec_receive_frame()返回一个frame，此函数返回。
            // 下次进来此函数，继续获取一个frame，直到avcodec_receive_frame()返回AVERROR(EAGAIN)，
            // 表示解码器需要填入新的音频packet
            ret = avcodec_receive_frame(p_codec_ctx, frame);
            if (ret >= 0)
            {
                // 时基转换，从d->avctx->pkt_timebase时基转换到1/frame->sample_rate时基
                AVRational tb = (AVRational) { 1, frame->sample_rate };
                if (frame->pts != AV_NOPTS_VALUE)
                {
                    frame->pts = av_rescale_q(frame->pts, p_codec_ctx->pkt_timebase, tb);
                }
                else
                {
                    av_log(NULL, AV_LOG_WARNING, "frame->pts no\n");
                }

                return 1;
            }
            else if (ret == AVERROR_EOF)
            {
                av_log(NULL, AV_LOG_INFO, "audio avcodec_receive_frame(): the decoder has been flushed\n");
                avcodec_flush_buffers(p_codec_ctx);
                return 0;
            }
            else if (ret == AVERROR(EAGAIN))
            {
                //av_log(NULL, AV_LOG_INFO, "audio avcodec_receive_frame(): input is not accepted in the current state\n");
                break;
            }
            else
            {
                av_log(NULL, AV_LOG_ERROR, "audio avcodec_receive_frame(): other errors\n");
                continue;
            }
        }
        //printf("read audio pkt start\n");
        // 1. 取出一个packet。使用pkt对应的serial赋值给d->pkt_serial
        if (packet_queue_get(p_pkt_queue, &pkt, true) < 0)
        {
            printf("audio packet_queue_get fail\n");
            return -1;
        }
        //if(pkt.data)
            //printf("read audio pkt end: %x,%x,%x\n",pkt.data[0],pkt.data[1],pkt.data[2]);
        // packet_queue中第一个总是flush_pkt。每次seek操作会插入flush_pkt，更新serial，开启新的播放序列
        if (pkt.data == flush_pkt.data)
        {
            // 复位解码器内部状态/刷新内部缓冲区。当seek操作或切换流时应调用此函数。
            printf("packet_queue_get null\n");
            avcodec_flush_buffers(p_codec_ctx);
        }
        else
        {
            // 2. 将packet发送给解码器
            //    发送packet的顺序是按dts递增的顺序，如IPBBPBB
            //    pkt.pos变量可以标识当前packet在视频文件中的地址偏移
            if (avcodec_send_packet(p_codec_ctx, &pkt) == AVERROR(EAGAIN))
            {
                av_log(NULL, AV_LOG_ERROR, "receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
            }
            av_packet_unref(&pkt);
        }
    }
}

// 音频解码线程：从音频packet_queue中取数据，解码后放入音频frame_queue
static int audio_decode_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    AVFrame *p_frame = av_frame_alloc();
    frame_t *af;

    int got_frame = 0;
    AVRational tb;
    int ret = 0;

    if (p_frame == NULL)
    {
        return AVERROR(ENOMEM);
    }

    while (1)
    {
        if(is->abort_request)
        {
            printf("audio decode thread exit\n");
            break;
        }
        got_frame = audio_decode_frame(is->p_acodec_ctx, &is->audio_pkt_queue, p_frame);
        if (got_frame < 0)
        {
            goto the_end;
        }
        
        if (got_frame)
        {
            tb = (AVRational) { 1, p_frame->sample_rate };

            if (!(af = frame_queue_peek_writable(&is->audio_frm_queue)))
                goto the_end;
            
            af->pts = (p_frame->pts == AV_NOPTS_VALUE) ? NAN : p_frame->pts * av_q2d(tb);
            
            af->pos = p_frame->pkt_pos;
            //-af->serial = is->auddec.pkt_serial;
            // 当前帧包含的(单个声道)采样数/采样率就是当前帧的播放时长
            af->duration = av_q2d((AVRational) { p_frame->nb_samples, p_frame->sample_rate });

            // 将frame数据拷入af->frame，af->frame指向音频frame队列尾部
            av_frame_move_ref(af->frame, p_frame);
            // 更新音频frame队列大小及写指针
            frame_queue_push(&is->audio_frm_queue);
            //no need unref frame?

        }
    }

the_end:
    av_frame_free(&p_frame);
    return ret;
}

int open_audio_stream(player_stat_t *is)
{
    AVCodecContext *p_codec_ctx;
    AVCodecParameters *p_codec_par = NULL;
    AVCodec* p_codec = NULL;
    int ret;

    // 1. 为音频流构建解码器AVCodecContext

    // 1.1 获取解码器参数AVCodecParameters
    p_codec_par = is->p_audio_stream->codecpar;
    // 1.2 获取解码器
    p_codec = avcodec_find_decoder(p_codec_par->codec_id);
    if (p_codec == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "Cann't find codec!\n");
        return -1;
    }

    // 1.3 构建解码器AVCodecContext
    // 1.3.1 p_codec_ctx初始化：分配结构体，使用p_codec初始化相应成员为默认值
    p_codec_ctx = avcodec_alloc_context3(p_codec);
    if (p_codec_ctx == NULL)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_alloc_context3() failed\n");
        return -1;
    }
    // 1.3.2 p_codec_ctx初始化：p_codec_par ==> p_codec_ctx，初始化相应成员
    ret = avcodec_parameters_to_context(p_codec_ctx, p_codec_par);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_parameters_to_context() failed %d\n", ret);
        return -1;
    }
    // 1.3.3 p_codec_ctx初始化：使用p_codec初始化p_codec_ctx，初始化完成
    ret = avcodec_open2(p_codec_ctx, p_codec, NULL);
    if (ret < 0)
    {
        av_log(NULL, AV_LOG_ERROR, "avcodec_open2() failed %d\n", ret);
        return -1;
    }

    p_codec_ctx->pkt_timebase = is->p_audio_stream->time_base;
    is->p_acodec_ctx = p_codec_ctx;

    // 2. 创建视频解码线程
    CheckFuncResult(pthread_create(&is->audioDecode_tid, NULL, audio_decode_thread, is));

    return 0;
}

static int audio_resample(player_stat_t *is, int64_t audio_callback_time)
{
    int data_size, resampled_data_size;
    int64_t dec_channel_layout;
    av_unused double audio_clock0;
    int wanted_nb_samples;
    frame_t *af;
    
    if (is->paused)
        return -1;

    // 若队列头部可读，则由af指向可读帧
    if (!(af = frame_queue_peek_readable(&is->audio_frm_queue)))
        return -1;

    frame_queue_next(&is->audio_frm_queue);
    // 根据frame中指定的音频参数获取缓冲区的大小
    data_size = av_samples_get_buffer_size(NULL, af->frame->channels,   // 本行两参数：linesize，声道数
        af->frame->nb_samples,       // 本行一参数：本帧中包含的单个声道中的样本数
        af->frame->format, 1);       // 本行两参数：采样格式，不对齐

// 获取声道布局
    dec_channel_layout =
        (af->frame->channel_layout && af->frame->channels == av_get_channel_layout_nb_channels(af->frame->channel_layout)) ?
        af->frame->channel_layout : av_get_default_channel_layout(af->frame->channels);
    wanted_nb_samples = af->frame->nb_samples;
    // is->audio_param_tgt是SDL可接受的音频帧数，是audio_open()中取得的参数
    // 在audio_open()函数中又有“is->audio_src = is->audio_param_tgt”
    // 此处表示：如果frame中的音频参数 == is->audio_src == is->audio_param_tgt，那音频重采样的过程就免了(因此时is->swr_ctr是NULL)
    // 　　　　　否则使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->swr_ctx，并使用frame中的音频参数来赋值is->audio_src
    if (af->frame->format != is->audio_param_src.fmt ||
        dec_channel_layout != is->audio_param_src.channel_layout ||
        af->frame->sample_rate != is->audio_param_src.freq)
    {
        swr_free(&is->audio_swr_ctx);
        //printf("in layout: %lld,format: %d,samrate: %d\n",dec_channel_layout,af->frame->format,af->frame->sample_rate);
        //printf("out layout: %lld,format: %d,samrate: %d\n",is->audio_param_tgt.channel_layout,is->audio_param_tgt.fmt,is->audio_param_tgt.freq);
        
        // 使用frame(源)和is->audio_param_tgt(目标)中的音频参数来设置is->audio_swr_ctx
        is->audio_swr_ctx = swr_alloc_set_opts(NULL,
            is->audio_param_tgt.channel_layout, is->audio_param_tgt.fmt, is->audio_param_tgt.freq,
            dec_channel_layout, af->frame->format, af->frame->sample_rate,
            0, NULL);
        if (!is->audio_swr_ctx || swr_init(is->audio_swr_ctx) < 0)
        {
            av_log(NULL, AV_LOG_ERROR,
                "Cannot create sample rate converter for conversion of %d Hz %s %d channels to %d Hz %s %d channels!\n",
                af->frame->sample_rate, av_get_sample_fmt_name(af->frame->format), af->frame->channels,
                is->audio_param_tgt.freq, av_get_sample_fmt_name(is->audio_param_tgt.fmt), is->audio_param_tgt.channels);
            swr_free(&is->audio_swr_ctx);
            return -1;
        }
        // 使用frame中的参数更新is->audio_src，第一次更新后后面基本不用执行此if分支了，因为一个音频流中各frame通用参数一样
        is->audio_param_src.channel_layout = dec_channel_layout;
        is->audio_param_src.channels = af->frame->channels;
        is->audio_param_src.freq = af->frame->sample_rate;
        is->audio_param_src.fmt = af->frame->format;
        
    }

    if (is->audio_swr_ctx)
    {
        // 重采样输入参数1：输入音频样本数是af->frame->nb_samples
        // 重采样输入参数2：输入音频缓冲区
        const uint8_t **in = (const uint8_t **)af->frame->extended_data;
        // 重采样输出参数1：输出音频缓冲区尺寸
        // 重采样输出参数2：输出音频缓冲区
        uint8_t **out = &is->audio_frm_rwr;
        // 重采样输出参数：输出音频样本数(多加了256个样本)
        int out_count = (int64_t)wanted_nb_samples * is->audio_param_tgt.freq / af->frame->sample_rate + 256;
        // 重采样输出参数：输出音频缓冲区尺寸(以字节为单位)
        int out_size = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, out_count, is->audio_param_tgt.fmt, 0);
        int len2;
        if (out_size < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size() failed\n");
            return -1;
        }
        
        av_fast_malloc(&is->audio_frm_rwr, &is->audio_frm_rwr_size, out_size);
        if (!is->audio_frm_rwr)
        {
            
            return AVERROR(ENOMEM);
        }
        //printf("tgt count: %d,channel: %d,size: %d\n",out_count,is->audio_param_tgt.channels,is->audio_frm_rwr_size);
        //printf("in count: %d,channel: %d\n",af->frame->nb_samples,af->frame->channels);
        
        // 音频重采样：返回值是重采样后得到的音频数据中单个声道的样本数
        len2 = swr_convert(is->audio_swr_ctx, out, out_count, in, af->frame->nb_samples);
        if (len2 < 0)
        {
            av_log(NULL, AV_LOG_ERROR, "swr_convert() failed\n");
            return -1;
        }
        
        if (len2 == out_count)
        {
            av_log(NULL, AV_LOG_WARNING, "audio buffer is probably too small\n");
            if (swr_init(is->audio_swr_ctx) < 0)
                swr_free(&is->audio_swr_ctx);
        }
        
        is->p_audio_frm = is->audio_frm_rwr;
        
        // 重采样返回的一帧音频数据大小(以字节为单位)
        resampled_data_size = len2 * is->audio_param_tgt.channels * av_get_bytes_per_sample(is->audio_param_tgt.fmt);
        
    }
    else
    {
        // 未经重采样，则将指针指向frame中的音频数据
        is->p_audio_frm = af->frame->data[0];
        resampled_data_size = data_size;
    }

    audio_clock0 = is->audio_clock;
    //printf("before audio clock: %lf,pts: %lf,nb: %d,samrate: %d\n",is->audio_clock,af->pts,af->frame->nb_samples,af->frame->sample_rate);
    /* update the audio clock with the pts */
    if (!isnan(af->pts))
    {
        is->audio_clock = af->pts + (double)af->frame->nb_samples / af->frame->sample_rate;
    }
    else
    {
        is->audio_clock = NAN;
    }
    //printf("after pts: %lf,clock: %lf\n",af->pts,is->audio_clock);
    is->audio_clock_serial = af->serial;
    
#ifdef DEBUG
    {
        static double last_clock;
        printf("audio: delay=%0.3f clock=%0.3f clock0=%0.3f\n",
            is->audio_clock - last_clock,
            is->audio_clock, audio_clock0);
        last_clock = is->audio_clock;
    }
#endif
    return resampled_data_size;
}

static int audio_playing_thread(void *arg)
{
    player_stat_t *is = (player_stat_t *)arg;
    int audio_size, len1,len;
    int pause = 0;
    int last_pause =0;

    
    while(1)
    {
        if(is->abort_request)
        {
            printf("audio play thread exit\n");
            break;
        }
        int64_t audio_callback_time = av_gettime_relative();
        
        audio_size = audio_resample(is, audio_callback_time);
        if (audio_size < 0)
        {
            /* if error, just output silence */
            
            pause = 1;
            if(pause != last_pause)
            {

                last_pause = pause;
                MI_AO_PauseChn(0,0);
            }
            is->p_audio_frm = NULL;
            is->audio_frm_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_param_tgt.frame_size * is->audio_param_tgt.frame_size;
        }
        else
        {
            pause = 0;
            if(pause != last_pause)
            {
                last_pause = pause;
                MI_AO_ResumeChn(0,0);
            }
            is->audio_frm_size = audio_size;
        }

        if (is->p_audio_frm != NULL)
        {
            //memcpy(stream, (uint8_t *)is->p_audio_frm + is->audio_cp_index, len1);
            //printf("save audio len: %d\n",is->audio_frm_size);
            //write(fda, (uint8_t *)is->p_audio_frm + is->audio_cp_index, len1);
            //put audio stream to ss player
            MI_AUDIO_Frame_t stAoSendFrame;
            MI_S32 s32RetSendStatus = 0;
            MI_AUDIO_DEV AoDevId = 0;
            MI_AO_CHN AoChn = 0;

            //read data and send to AO module
            stAoSendFrame.u32Len = is->audio_frm_size;
            stAoSendFrame.apVirAddr[0] = is->p_audio_frm;
            stAoSendFrame.apVirAddr[1] = NULL;

            do{
                s32RetSendStatus = MI_AO_SendFrame(AoDevId, AoChn, &stAoSendFrame, 128);
            }while(s32RetSendStatus == MI_AO_ERR_NOBUF);

            if(s32RetSendStatus != MI_SUCCESS)
            {
                printf("[Warning]: MI_AO_SendFrame fail, error is 0x%x: \n",s32RetSendStatus);
            }

            //frame_queue_next(&is->audio_frm_queue);
        }

        // is->audio_write_buf_size是本帧中尚未拷入SDL音频缓冲区的数据量
        is->audio_write_buf_size = is->audio_frm_size - is->audio_cp_index;
        /* Let's assume the audio driver that is used by SDL has two periods. */
        // 3. 更新时钟
        if (!isnan(is->audio_clock))
        {
            // 更新音频时钟，更新时刻：每次往声卡缓冲区拷入数据后
            // 前面audio_decode_frame中更新的is->audio_clock是以音频帧为单位，所以此处第二个参数要减去未拷贝数据量占用的时间
            set_clock_at(&is->audio_clk,
                is->audio_clock - (double)(is->audio_hw_buf_size) / is->audio_param_tgt.bytes_per_sec,
                is->audio_clock_serial,
                audio_callback_time / 1000000.0);
            //printf("audio clk: %lf,curtime: %ld,audio_callback_time: %ld\n",is->audio_clock,is->audio_clock_serial,audio_callback_time);
            //printf("update clk pts: %lf,lud: %lf,dif: %lf\n",is->audio_clk.pts,is->audio_clk.last_updated,is->audio_clk.pts_drift);
        }
    }

    return 0;
}
int ss_ao_Deinit(void)
{
    MI_AUDIO_DEV AoDevId = 0;
    MI_AO_CHN AoChn = 0;

    /* disable ao channel of */
    CheckFuncResult(MI_AO_DisableChn(AoDevId, AoChn));

    /* disable ao device */
    CheckFuncResult(MI_AO_Disable(AoDevId));
    
    return 0;
}


int ss_ao_Init(void)
{
    MI_AUDIO_Attr_t stSetAttr;
    MI_AUDIO_Attr_t stGetAttr;
    MI_AUDIO_DEV AoDevId = 0;
    MI_AO_CHN AoChn = 0;

    MI_S32 s32SetVolumeDb;
    MI_S32 s32GetVolumeDb;

    //set Ao Attr struct
    memset(&stSetAttr, 0, sizeof(MI_AUDIO_Attr_t));
    stSetAttr.eBitwidth = AUDIO_OUTPUT_SAMPFMT;
    stSetAttr.eWorkmode = E_MI_AUDIO_MODE_I2S_MASTER;
    stSetAttr.u32FrmNum = 6;
    stSetAttr.u32PtNumPerFrm = MI_AUDIO_SAMPLE_PER_FRAME;
    stSetAttr.u32ChnCnt = 1;

    if(stSetAttr.u32ChnCnt == 2)
    {
        stSetAttr.eSoundmode = E_MI_AUDIO_SOUND_MODE_STEREO;
    }
    else if(stSetAttr.u32ChnCnt == 1)
    {
        stSetAttr.eSoundmode = AUDIO_OUTPUT_CHLAYOUT;
    }

    stSetAttr.eSamplerate = AUDIO_OUTPUT_SAMPRATE;

    /* set ao public attr*/
    CheckFuncResult(MI_AO_SetPubAttr(AoDevId, &stSetAttr));

    /* get ao device*/
    CheckFuncResult(MI_AO_GetPubAttr(AoDevId, &stGetAttr));

    /* enable ao device */
    CheckFuncResult(MI_AO_Enable(AoDevId));

    /* enable ao channel of device*/
    CheckFuncResult(MI_AO_EnableChn(AoDevId, AoChn));


    /* if test AO Volume */
    s32SetVolumeDb = -20;
    CheckFuncResult(MI_AO_SetVolume(AoDevId, s32SetVolumeDb));
    CheckFuncResult(MI_AO_SetVolume(AoDevId, s32SetVolumeDb));
    /* get AO volume */
    CheckFuncResult(MI_AO_GetVolume(AoDevId, &s32GetVolumeDb));

    return SUCCESS;
}

static int open_audio_playing(void *arg)
{

    player_stat_t *is = (player_stat_t *)arg;

    // 2.2 根据SsPlayer音频参数构建音频重采样参数
    // wanted_spec是期望的参数，actual_spec是实际的参数，wanted_spec和auctual_spec都是SDL中的参数。
    // 此处audio_param是FFmpeg中的参数，此参数应保证是SDL播放支持的参数，后面重采样要用到此参数
    // 音频帧解码后得到的frame中的音频格式未必被SDL支持，比如frame可能是planar格式，但SDL2.0并不支持planar格式，
    // 若将解码后的frame直接送入SDL音频缓冲区，声音将无法正常播放。所以需要先将frame重采样(转换格式)为SDL支持的模式，
    // 然后送再写入SDL音频缓冲区
    
    is->audio_param_tgt.fmt = AUDIO_INPUT_SAMPFMT;
    is->audio_param_tgt.freq = AUDIO_INPUT_SAMPRATE;
    is->audio_param_tgt.channel_layout = AUDIO_INPUT_CHLAYOUT;
    
    is->audio_param_tgt.channels = av_get_channel_layout_nb_channels(is->audio_param_tgt.channel_layout);
        
    is->audio_param_tgt.frame_size = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, 1, is->audio_param_tgt.fmt, 1);
    
    is->audio_param_tgt.bytes_per_sec = av_samples_get_buffer_size(NULL, is->audio_param_tgt.channels, is->audio_param_tgt.freq, is->audio_param_tgt.fmt, 1);
    
    if (is->audio_param_tgt.bytes_per_sec <= 0 || is->audio_param_tgt.frame_size <= 0)
    {
        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
        return -1;
    }
    is->audio_param_src = is->audio_param_tgt;
    is->audio_hw_buf_size = MI_AO_PCM_BUF_SIZE_BYTE;//is->audio_param_tgt.frame_size;   // SDL音频缓冲区大小
    is->audio_frm_size = 0;
    is->audio_cp_index = 0;
    
    CheckFuncResult(pthread_create(&is->audioPlay_tid, NULL, audio_playing_thread, is));

    return 0;
}

// 音频处理回调函数。读队列获取音频包，解码，播放
// 此函数被SDL按需调用，此函数不在用户主线程中，因此数据需要保护
// \param[in]  opaque 用户在注册回调函数时指定的参数
// \param[out] stream 音频数据缓冲区地址，将解码后的音频数据填入此缓冲区
// \param[out] len    音频数据缓冲区大小，单位字节
// 回调函数返回后，stream指向的音频缓冲区将变为无效
// 双声道采样点的顺序为LRLRLR
static void sdl_audio_callback(void *opaque, uint8_t *stream, int len)
{
    player_stat_t *is = (player_stat_t *)opaque;
    int audio_size, len1;

    int64_t audio_callback_time = av_gettime_relative();

    while (len > 0) // 输入参数len等于is->audio_hw_buf_size，是audio_open()中申请到的SDL音频缓冲区大小
    {
        if (is->audio_cp_index >= (int)is->audio_frm_size)
        {
            // 1. 从音频frame队列中取出一个frame，转换为音频设备支持的格式，返回值是重采样音频帧的大小
            audio_size = audio_resample(is, audio_callback_time);
            if (audio_size < 0)
            {
                /* if error, just output silence */
                is->p_audio_frm = NULL;
                is->audio_frm_size = SDL_AUDIO_MIN_BUFFER_SIZE / is->audio_param_tgt.frame_size * is->audio_param_tgt.frame_size;
            }
            else
            {
                is->audio_frm_size = audio_size;
            }
            is->audio_cp_index = 0;
        }
        // 引入is->audio_cp_index的作用：防止一帧音频数据大小超过SDL音频缓冲区大小，这样一帧数据需要经过多次拷贝
        // 用is->audio_cp_index标识重采样帧中已拷入SDL音频缓冲区的数据位置索引，len1表示本次拷贝的数据量
        len1 = is->audio_frm_size - is->audio_cp_index;
        if (len1 > len)
        {
            len1 = len;
        }
        // 2. 将转换后的音频数据拷贝到音频缓冲区stream中，之后的播放就是音频设备驱动程序的工作了
        if (is->p_audio_frm != NULL)
        {
            memcpy(stream, (uint8_t *)is->p_audio_frm + is->audio_cp_index, len1);
        }
        else
        {
            memset(stream, 0, len1);
        }

        len -= len1;
        stream += len1;
        is->audio_cp_index += len1;
    }
    // is->audio_write_buf_size是本帧中尚未拷入SDL音频缓冲区的数据量
    is->audio_write_buf_size = is->audio_frm_size - is->audio_cp_index;
    /* Let's assume the audio driver that is used by SDL has two periods. */
    // 3. 更新时钟
    if (!isnan(is->audio_clock))
    {
        // 更新音频时钟，更新时刻：每次往声卡缓冲区拷入数据后
        // 前面audio_decode_frame中更新的is->audio_clock是以音频帧为单位，所以此处第二个参数要减去未拷贝数据量占用的时间
        set_clock_at(&is->audio_clk,
            is->audio_clock - (double)(2 * is->audio_hw_buf_size + is->audio_write_buf_size) / is->audio_param_tgt.bytes_per_sec,
            is->audio_clock_serial,
            audio_callback_time / 1000000.0);
    }
}

int open_audio(player_stat_t *is)
{
    //init sdk audio
    CheckFuncResult(ss_ao_Init());
    
    open_audio_stream(is);
    
    open_audio_playing(is);
    
    return 0;
}