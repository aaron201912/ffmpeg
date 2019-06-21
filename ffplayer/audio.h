#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "player.h"

#include "mi_ao.h"


int open_audio(player_stat_t *is);
int ss_ao_Deinit(void);
int ss_ao_Init(void);


#endif
