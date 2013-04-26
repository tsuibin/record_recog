#ifndef __RECORD_RECONG_H__
#define __RECORD_RECONG_H__

#include "my_alsa.h"
#include "my_qisr.h"
#include <signal.h>
#include <pthread.h>

void speech_menu();
void catch_signal(int signo);

void parse_record();
void parse_chooser(int chooser);

#endif
