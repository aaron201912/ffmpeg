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
#include <stdlib.h>
#include <string.h>

#include "st_uac_datatype.h"

#define DEVICE_CHECK_INITED(pdev) \
    do{ \
       if (!pdev || (GET_MOD(pdev->magic) != UAC_MOD_MAGIC)){ \
           fprintf(stderr, "%s: magic: %x %x\n", __func__, GET_MOD(pdev->magic), UAC_MOD_MAGIC); \
           return -1; \
        }\
      } while(0)

static int param_check(const ST_UAC_Config_t *uac_config)
{
    return 0;
}

void ST_UAC_Config(ST_UAC_Config_t *uac_config,
                        uint32_t flags,
                        uint32_t channels,
                        uint32_t rate,
                        uint32_t period_size,
                        uint32_t period_count,
                        enum pcm_format format)
{
    uac_config->card = 0;
    uac_config->device = 0;
    uac_config->flags = flags;
    uac_config->pcm_config.channels= channels;
    uac_config->pcm_config.rate= rate;
    uac_config->pcm_config.period_size= period_size;
    uac_config->pcm_config.period_count= period_count;
    uac_config->pcm_config.format= format;
    uac_config->pcm_config.start_threshold= period_count * period_size;
    uac_config->pcm_config.stop_threshold= period_count * period_size;
    uac_config->pcm_config.silence_threshold= 0;
}

int32_t ST_UAC_GetPcm_BufSize(ST_UAC_Handle_h handle)
{
    ST_UAC_Device_t * pdev = handle;

    DEVICE_CHECK_INITED(pdev);

    return pcm_get_buffer_size(pdev->pcm);
}

int32_t ST_UAC_Init(ST_UAC_Handle_h * phandle,
                ST_UAC_Config_t *uac_config)
{
    ST_UAC_Device_t * pdev = NULL;

    pdev = *phandle;

    if (pdev && (GET_MOD(pdev->magic) == UAC_MOD_MAGIC))
    {
        fprintf(stderr, "%s already Init\n", __func__);
        return ST_UAC_SUCCESS;
    }

    pdev = (ST_UAC_Device_t *)calloc(1, sizeof(ST_UAC_Device_t));
    pdev->config = (ST_UAC_Config_t*)malloc(sizeof(ST_UAC_Config_t));
    if (!pdev || !pdev->config)
    {
        fprintf(stderr, "%s Failed: No mem\n", __func__);
        return -1;
    }

    pdev->magic = UAC_MKMAGIC(pdev->config->card, pdev->config->device);
    memcpy(pdev->config, uac_config, sizeof(ST_UAC_Config_t));

    /* param setting */
    if (param_check(uac_config))
        goto err;

    pdev->pcm = pcm_open(pdev->config->card,
                         pdev->config->device,
                         pdev->config->flags,
                         &uac_config->pcm_config);
    if (!pdev-> pcm || !pcm_is_ready(pdev->pcm))
    {
        printf("%s: %s \n", __func__, pcm_get_error(pdev->pcm));
        goto err;
    }

    *phandle = pdev;
    return 0;
err:
    free(pdev->config);
    free(pdev);
    return -1;
}

int32_t ST_UAC_Uninit(ST_UAC_Handle_h handle)
{
    ST_UAC_Device_t * pdev = handle;

    DEVICE_CHECK_INITED(pdev);

    if (!pcm_is_ready(pdev->pcm))
        return -1;

    pcm_close(pdev->pcm);
    free(pdev->config);
    free(pdev);
    return 0;
}

int32_t ST_UAC_SendFrame(ST_UAC_Handle_h handle, const ST_UAC_Frame_t * pFrame)
{
    ST_UAC_Device_t * pdev = handle;

    DEVICE_CHECK_INITED(pdev);

    if (pcm_writei(pdev->pcm,
           pFrame->data,
           pcm_bytes_to_frames(pdev->pcm, pFrame->length)) < 0)
    {
        return -1;
    }
    return 0;
}


int32_t ST_UAC_GetFrame(ST_UAC_Handle_h handle, ST_UAC_Frame_t * pFrame)
{
    ST_UAC_Device_t * pdev = handle;

    DEVICE_CHECK_INITED(pdev);

    int ret = pcm_readi(pdev->pcm, pFrame->data,
            pcm_bytes_to_frames(pdev->pcm, pcm_get_buffer_size(pdev->pcm)));
    if (ret < 0)
    {
        return -1;
    }

    pFrame->length = pcm_frames_to_bytes(pdev->pcm, ret);
    return 0;
}
