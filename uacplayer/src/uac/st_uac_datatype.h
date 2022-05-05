/* SigmaStar trade secret */
/* Copyright (c) [2019~2020] SigmaStar Technology.
All rights reserved.

Unless otherwise stipulated in writing, any and all information contained
herein regardless in any format shall remain the sole proprietary of
SigmaStar and be kept in strict confidence
(SigmaStar Confidential Information) by the recipient.
Any unauthorized act including without limitation unauthorized disclosure,
copying, use, reproduction, sale, distribution, modification, disassembling,
reverse engineering and compiling of the contents of SigmaStar Confidential
Information is unlawful and strictly prohibited. SigmaStar hereby reserves the
rights to any and all damages, losses, costs and expenses resulting therefrom.
*/
#ifndef _ST_UAC_DATATYPE_H_
#define _ST_UAC_DATATYPE_H_

#include <linux/usb/ch9.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pcm.h"

typedef enum
{
    E_AUDIO_BIT_WIDTH_16 =16,/* 16bit width */
    E_AUDIO_BIT_WIDTH_24 =24,/* 24bit width */
    E_AUDIO_BIT_WIDTH_MAX,
} ST_AUDIO_BitWidth_e;

typedef struct ST_UAC_Frame_s {
    ST_AUDIO_BitWidth_e bitwith;
    void *data;
    uint32_t length;
} ST_UAC_Frame_t;

typedef struct ST_UAC_Config_s {
    uint8_t card;
    uint8_t device;
    uint32_t flags;
    struct pcm_config pcm_config;
} ST_UAC_Config_t;

typedef struct ST_UAC_Device_s {
    int32_t magic; 
#define UAC_MOD_MAGIC 0xEFDA0000
#define UAC_MOD_MAGIC_MASK 0xFFFF0000
#define UAC_MKMAGIC(card, device) (UAC_MOD_MAGIC | ((card & 0xFF)<<8) | (device & 0xFF))
#define GET_MOD(nod) (nod & UAC_MOD_MAGIC_MASK)
#define GET_CRAD_NUM(magic) ((magic & 0xFF00)>>8)
#define GET_DEVICE_NUM(magic) (magic & 0xFF)
    ST_UAC_Config_t *config;
    struct pcm *pcm;
} ST_UAC_Device_t;

typedef void* ST_UAC_Handle_h;
#define ST_UAC_SUCCESS 0;
#endif//_ST_UAC_DATATYPE_H_
