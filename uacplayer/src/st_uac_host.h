/*
* XXX.c - Sigmastar
*
* Copyright (c) [2019~2020] SigmaStar Technology.
*
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License version 2 for more details.
*
*/

#ifndef __UAC__
#define __UAC__

#include "common.h"
#include "pcm.h"
#include "mixer.h"

typedef struct {
    unsigned int flags;
    struct pcm_config pcm_config;
} Audio_Info_t;

typedef struct {
    char path[20];
    unsigned int card;
    unsigned int device;
    struct pcm *pcm[2];
    struct mixer *mixer;
    Audio_Info_t audio_info[2];
} Audio_Handle_t;

typedef struct {
    void *buf;
    int length;
} Audio_Buffer_t;

int audio_init(Audio_Handle_t *audio_handle, int mode);

int audio_deinit(Audio_Handle_t *audio_handle, int mode);

int audio_get_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, int mode);

int audio_put_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, int mode);

int audio_enum_standard_control(Audio_Handle_t *audio_handle);

int audio_send_standard_control(Audio_Handle_t *audio_handle, unsigned int id, int *value, int dir);

int audio_dump_buf(Audio_Handle_t *audio_handle, Audio_Buffer_t *audio_buf, char *path, int mode);

#endif
