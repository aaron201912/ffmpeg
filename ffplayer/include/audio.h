#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "player.h"


#ifdef SUPPORT_HDMI
#define AUDIO_DEV           3
#else
#define AUDIO_DEV           0
#endif

#define AUDIO_CHN           0

#define MIN_AO_VOLUME           -60
#define MAX_AO_VOLUME           30
#define MIN_ADJUST_AO_VOLUME    -10
#define MAX_ADJUST_AO_VOLUME    20

extern uint64_t g_audio_chlayout;
extern int audio_dev;

int open_audio(player_stat_t *is);
int sstar_audio_deinit(void);
int sstar_audio_init(int nAoDevId);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif
