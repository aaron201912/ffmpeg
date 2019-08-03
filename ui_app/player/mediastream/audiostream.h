#ifndef __AUDIO_STREAM_H__
#define __AUDIO_STREAM_H__

#include "player.h"

#include "mi_ao.h"
#include "mi_ao_datatype.h"


#define AUDIO_DEV       0
#define AUDIO_CHN       0

#define MIN_AO_VOLUME           -60
#define MAX_AO_VOLUME           30
#define MIN_ADJUST_AO_VOLUME    -10
#define MAX_ADJUST_AO_VOLUME    20

#define VOLUME_RANGE            100


int open_audio(player_stat_t *is);
MI_S32 sstar_ao_init(MI_S32 volume);
void sstar_ao_deinit(void);


#endif
