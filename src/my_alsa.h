#ifndef __MY_ALSA_H__
#define __MY_ALSA_H__

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include "my_common.h"
#include <alsa/asoundlib.h>

//int first_record;
//int record_abort;

void MySndOpen (snd_pcm_t **pcm, const char *name, 
		snd_pcm_stream_t stream, int mode);
		
void MySndParams(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);

void MySndReadi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size);

void MySndEnd(snd_pcm_t *pcm);

int MySndRecord(int timenum, const char *output_file);

#endif
