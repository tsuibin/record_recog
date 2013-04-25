#ifndef __RECORD_RECONG_H__
#define __RECORD_RECONG_H__

#include "my_alsa.h"
#include "my_qisr.h"
#include <signal.h>
#include <pthread.h>

#define INPUT_FILE	"tmp.wav"
#define OUTPUT_FILE	"output"
#define CONFIG_FILE	"command.conf"

void speech_menu();
void catch_signal(int signo);
void parse_chooser(int chooser);

#endif
