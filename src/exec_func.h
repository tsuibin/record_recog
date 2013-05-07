#ifndef __EXEC_FUNC_H__
#define __EXEC_FUNC_H__

#include "my_common.h"

/* deepin-music-player */
int MusicPlayPause();
int MusicForward();
int MusicPrev();
int MusicNext();
int MusicStop();
int MusicQuit();
int MusicRewind();
//int MusicChangeVolume(float value);
int MusicChangeVolume(char *value);

void music_type_func(char *exec_buf);
/* end */

#endif
