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
#ifndef _ST_UAC_H_
#define _ST_UAC_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"{
#endif

#include "st_uac_datatype.h"

void ST_UAC_Config(ST_UAC_Config_t *uac_config,
			                        uint32_t flags,
			                        uint32_t channels,
			                        uint32_t rate,
			                        uint32_t period_size,
			                        uint32_t period_count,
			                        enum pcm_format format);

int32_t ST_UAC_Init(ST_UAC_Handle_h*, ST_UAC_Config_t*);

int32_t ST_UAC_Uninit(ST_UAC_Handle_h);

int32_t ST_UAC_SendFrame(ST_UAC_Handle_h, const ST_UAC_Frame_t*);

int32_t ST_UAC_GetFrame(ST_UAC_Handle_h, ST_UAC_Frame_t*);

int32_t ST_UAC_GetPcm_BufSize(ST_UAC_Handle_h handle);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif//_ST_UAC_H_
