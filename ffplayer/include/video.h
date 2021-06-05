#ifndef __VIDEO_H__
#define __VIDEO_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "player.h"

int open_video(player_stat_t *is);
int video_flush_buffer(frame_queue_t *f);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif

