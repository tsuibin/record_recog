#include "my_qisr.h"

void MyInit(const char *configs)
{
	int ret;
	
	ret = QISRInit(configs);
	if ( ret != 0 ) {
		printf( "QISRInit failed, error code %d\n", ret );
		exit(-1);
	}
}

void MyAudioWrite(const char *sessionId, const void *recogAudio, unsigned 
	int audioLen, int audioStatus, int *epStatus, int *recogStatus)
{
	int ret;
	
	ret  = QISRAudioWrite( sessionId, recogAudio, audioLen, audioStatus, 
		epStatus, recogStatus );
	if ( ret != 0 ) {
		printf( "QISRAudioWrite failed, error code %d\n", ret );
		*recogStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
		return;
	}
}

/*
 * 语音识别
 */
int SpeechRecog(const char *inFile, const char *outFile)
{
	const char *configs = NULL;
	const char *sessionId = NULL;
	const char *recogParams = NULL;
	char		recogAudio[BLOCK_LEN];
	FILE		*fSpeech = NULL;
	FILE		*fOut = NULL;
	int			audioStatus = 0;
	int			epStatus = 0;
	//int			recStatus = 0;
	int			rsltStatus = 0;
	const char *recResult = NULL;
	unsigned int audioLen = 0;
	int			ret = 0;

	printf("=======================================================\n");
	printf("               Speech recog running  \n");
	printf("=======================================================\n");

	configs = "server_url=http://dev.voicecloud.cn/index.htm, appid=51777175, \
			   coding_libs=libspeex.so, vad_enable=0";
	MyInit(configs);
	//MyInit("appid=51777175,vad_enable=0");

	/*
	 * 开始会话，使用会话模式
	 * 16k audio
	 */
	recogParams = "ssm=1,sub=iat,auf=audio/L16;rate=16000,aue=raw,ent=sms16k,\
		rst=plain,rse=utf8";
	//8k
	//recogParams = "ssm=1,sub=iat,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,\
		rst=plain,rse=utf8";
	sessionId = QISRSessionBegin("", recogParams, &ret);
	if ( ret != 0 ) {
		printf("QISRSessionBegin failed, error code is %d\n", ret);
		return -1;
	}

	/*
	 * 打开用来进行识别的语音文件
	 */
	fSpeech = fopen(inFile, "rb");
	if ( fSpeech == NULL ) {
		printf("open file failed...\n");
		char tmp[] = "open file failed!\n";
		MyEndRecog(sessionId, tmp);
		return -1;
	}
	
	fOut = fopen(outFile, "wb+");
	if ( fOut == NULL ) {
		printf("open file output failed...\n");
		char tmp[] = "open file failed!\n";
		MyEndRecog(sessionId, tmp);
		return -1;
	}

	fprintf(stderr, "语音识别中...\n");
	/*
	 * 发送音频数据，获取语音听写结果
	 */
	while( audioStatus != ISR_AUDIO_SAMPLE_LAST ) {
		audioLen = fread(recogAudio, 1, BLOCK_LEN, fSpeech);
		audioStatus = (audioLen == BLOCK_LEN) ? 
			ISR_AUDIO_SAMPLE_CONTINUE:ISR_AUDIO_SAMPLE_LAST;

		ret  = QISRAudioWrite(sessionId, recogAudio, audioLen, audioStatus, 
				&epStatus, &rsltStatus);
		if ( ret != 0 ) {
			printf( "QISRAudioWrite failed, error code %d\n", ret );
			rsltStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
			break;
		}
		//printf("write audio data ok!\n"
		//		"len = %d, status = %d\n", audioLen, audioStatus );

		/*  已经有结果缓存在 MSC 中了，可以获取了 */
		if ( rsltStatus == ISR_REC_STATUS_SUCCESS ) {
			//printf("已经有结果缓存在 MSC 中了，可以获取了\n");
			recResult = QISRGetResult(sessionId, &rsltStatus, 5000, &ret);
			if ( ret != 0 ) {
				printf( "QISRGetResult failed, error code is %d\n", ret );
				rsltStatus = ISR_REC_STATUS_SPEECH_COMPLETE;
				break;
			}

			if ( recResult != NULL ) {
				printf("got a result: %s\n", recResult);
				fwrite(recResult, 1, strlen(recResult), fOut);
			}

			/* 全部结果已经取完 */
			if ( rsltStatus == ISR_REC_STATUS_SPEECH_COMPLETE ) {
				printf("the result has been got completely!\n");
				break;
			}
		}

		/* 检测到音频后端点，结束音频发送 */
		if ( epStatus == ISR_EP_AFTER_SPEECH ) {
			printf( "end point of speech has been detected!\n" );
			break;
		}

		//sleep(120);
		usleep(200000);
	}
	fclose(fSpeech);

	/* 获取余下的识别结果 */
	while ( rsltStatus != ISR_REC_STATUS_SPEECH_COMPLETE ) {
		//printf("获取余下的识别结果\n");
		recResult = QISRGetResult(sessionId, &rsltStatus, 5000, &ret);
		if ( ret != 0 ) {
			printf( "QISRGetResult failed, error code is %d\n", ret );
			break;
		}

		if ( recResult != NULL ) {
			printf( "got a result: %s\n", recResult );
			fwrite(recResult, 1, strlen(recResult), fOut);
		}

		/* 睡眠防止 MSC 段无缓存的识别浪费 CPU 资源 */
		usleep(400000);
	}
	fclose(fOut);

	{
	char tmp[] = "normal end\n";
	MyEndRecog(sessionId, tmp);
	}

	return 0;
}
