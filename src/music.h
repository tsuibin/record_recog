#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <stdio.h>
#include <stdlib.h>

int MusicPlayPause();
int MusicForward();
int MusicPrev();
int MusicNext();
int MusicStop();
int MusicQuit();
int MusicRewind();

//int MusicChangeVolume(float value);
int MusicChangeVolume(char *value);

#endif
