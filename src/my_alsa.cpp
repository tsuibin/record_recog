#include "my_alsa.h"

extern int first_record;
extern int record_abort;

void MySndOpen (snd_pcm_t **pcm, const char *name, 
		snd_pcm_stream_t stream, int mode)
{
	int ret = 0;
	
	ret = snd_pcm_open(pcm, name, stream, mode);
	if (ret < 0) {
		fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(ret));
		exit(1);
	}
}

void MySndParams(snd_pcm_t *pcm, snd_pcm_hw_params_t *params)
{
	int ret = 0;
	
	ret = snd_pcm_hw_params(pcm, params);
	if (ret != 0) {
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(ret));
		exit(1);
	}
}

void MySndReadi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size)
{
	snd_pcm_sframes_t ret;
	
	ret = snd_pcm_readi(pcm, buffer, size);
	if (ret == -EPIPE) {
		/* EPIPE means overrun */
		fprintf(stderr, "overrun occurred\n");
		snd_pcm_prepare(pcm);
	} else if (ret < 0) {
		fprintf(stderr, "error from read: %s\n", snd_strerror(ret));
	} else if (ret != (int)size) {
		fprintf(stderr, "short read, read %ld frames\n", ret);
	}
}

void MySndEnd(snd_pcm_t *pcm)
{
	int ret;
	
	ret = snd_pcm_drain(pcm);
	if (ret != 0) {
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(ret));
		exit(1);
	}
	
	ret = snd_pcm_close(pcm);
	if (ret != 0) {
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(ret));
		exit(1);
	}
}

int MySndRecord(int timenum, const char *output_file)
{
	long loops;	//录音的秒数
	int rc;		//保存函数返回值，检测函数是否执行成功
	int size;	//每帧数据大小
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	char *buffer = NULL;	//存放音频数据
	FILE *fp_out = NULL;	//音频数据保存文件

	/* 以采集方式打开默认设备 */
	MySndOpen(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);

	/* Allocate a hardware parameters object. */
	snd_pcm_hw_params_alloca(&params);

	/* Fill it in with default values. */
	snd_pcm_hw_params_any(handle, params);

	/*
	 * Set the desired hardware parameters.
	 */

	/* Interleaved mode */
	snd_pcm_hw_params_set_access(handle, params,
						  SND_PCM_ACCESS_RW_INTERLEAVED);

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params,
								  SND_PCM_FORMAT_S16_LE);

	/* Two channels (stereo) */
	snd_pcm_hw_params_set_channels(handle, params, 1);

	/* 16000 bits/second sampling rate (CD quality) */
	val = 16000;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

	/* Set period size to 32 frames. */
	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	/* Write the parameters to the driver */
	MySndParams(handle, params);

	/* Use a buffer large enough to hold one period */
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	size = frames * 2; /* 2 bytes/sample, 1 channels */
	buffer = (char *) malloc(size);

	/* We want to loop for 3 seconds */
	snd_pcm_hw_params_get_period_time(params,  &val, &dir);
	loops = (timenum * 1000000) / val;

	if ( first_record ) {
		first_record = 0;
		fp_out = fopen(output_file, "wb+");
	} else {
		fp_out = fopen(output_file, "ab+");
	}
	if ( fp_out == NULL ) {
		fprintf( stderr, "open file err...\n" );
		MySndEnd(handle);
		free(buffer);
		return -1;
	}
	
	fprintf(stderr, "录音中...\n");
	while (loops > 0) {
		loops--;
		if ( loops == 0 ) {
			if (record_abort) {
				loops = 2;
			}
		}
		MySndReadi(handle, buffer, frames);
		
		//写入到文件
		rc = fwrite(buffer, 1, size, fp_out);
		if (rc != size) {
		  fprintf(stderr, "short write: wrote %d bytes\n", rc);
	    }
	}

	fclose(fp_out);
	MySndEnd(handle);
	free(buffer);
	fprintf(stderr, "录音完毕...\n");

	return 0;
}
