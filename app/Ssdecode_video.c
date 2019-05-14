/**
 * @file
 * video decoding with libavcodec API example
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdbool.h>

//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//frame格式转换
#include "libswscale/swscale.h"



int main(int argc, char **argv)
{
    const char *filename, *outfilename;

    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(0);
    }
    filename    = argv[1];
    outfilename = argv[2];

    //1.注册所有组件
    av_register_all();

    //封装格式上下文，统领全局的结构体，保存视频文件封装格式相关信息
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //2.打开输入视频文件
    if(avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0)
    {
        printf("open %s failed\n",filename);
        return 1;
    }

    //3.获取视频文件信息
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        printf("avformat_find_stream_info fail\n");
        return 1;
    }

    //获取视频流的索引位置
    //遍历所有类型的流(音频流，视频流，字幕流)，找到视频流
    int v_stream_idx = -1;
    int i = 0;

    //number of streams
    for(; i < pFormatCtx->nb_streams; i++)
    {
        //流类型
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            v_stream_idx = i;
            break;
        }
    }

    if(v_stream_idx == -1)
    {
        printf("can't find video stream!\n");
        return 1;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;

    //4.根据编解码上下文中的编码id查找对应的解码
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL)
    {
        printf("can't find codec: %d\n",pCodecCtx->codec_id);
        return 1;
    }

    //5.打开解码器
    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("avcodec_open2 fail\n");
        return 1;
    }

    //输出视频信息
    printf("video format: %s\n",pFormatCtx->iformat->name);
    printf("video duration: %d\n",(pFormatCtx->duration)/1000000);
    printf("video width: %d,height: %d\n",pCodecCtx->width,pCodecCtx->height);
    printf("video decode name: %s\n",pCodec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的编码数据(H264)
    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //只分配了结构体空间，并没有分配实际的数据缓冲区间
    AVFrame *pFrame = av_frame_alloc();

    AVFrame *pFrameYUV = av_frame_alloc();

    //只有指定了AVFrame的像素格式，画面大小才能真正分配内存
    //开始真正分配缓冲区间内存
    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));

    //初始化缓冲区
    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

    //用于转码(缩放)的参数，转之前的宽高，转之后的宽高，格式等
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    int got_picture, ret;

    FILE *fp_yuv = fopen(outfilename, "wb+");

    int frame_count = 0;

    //6.一帧一帧的读取压缩数据
    while(av_read_frame(pFormatCtx, packet) >= 0)
    {
        //只要视频压缩数据(根据流的索引位置判断)
        if(packet->stream_index == v_stream_idx)
        {
            //7.解码一帧视频压缩数据
            //ret = avcodec_decode_video2(pFormatCtx, pFrame, &got_picture, packet);

            ret = avcodec_send_packet(pCodecCtx, packet);
            if(ret < 0)
            {
                printf("avcodec_send_packet fail!\n");
                return 1;
            }

            ret = avcodec_receive_frame(pCodecCtx, pFrame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                return;
            else if (ret < 0)
            {
                fprintf(stderr, "Error during decoding\n");
                exit(1);
            }

            //printf("saving frame %3d\n", pCodecCtx->frame_number);
            fflush(stdout);
        
            //if(got_picture)
            {
                //AVFrame转为像素格式YUV420
                //参数2,6为输入，输出数据
                //参数3,7为输入，输出画面一行的数据大小
                //参数4为输入数据第一列要转码的位置从0开始
                //参数5为输入画面的高度
                sws_scale(sws_ctx, pFrame, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV, pFrameYUV->linesize);

                //保存到YUV文件
                int y_size = pCodecCtx->width * pCodecCtx->height;

                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);
                frame_count++;
                //printf("Decoded %d frames\n",frame_count);
            }
        }
        
        av_free_packet(packet);
    }

    fclose(fp_yuv);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_free_context(pFormatCtx);

    return 0;

}
