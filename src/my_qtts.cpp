#include "my_qtts.h"

extern int synth_first;

void MyInit(const char *configs)
{
	int ret;
	
	ret = QTTSInit(configs);
	if ( ret != 0 ) {
		printf( "QTTSInit failed, error code %d\n", ret );
		exit(-1);
	}
}

void MyTextPut(const char *sessionId, const char *synthText, 
	unsigned int textLen, const char *params)
{
	int ret;
	
	ret  = QTTSTextPut( sessionId, synthText, textLen, params );
	if ( ret != 0 ) {
		printf( "QTTSTextPut failed, error code %d\n", ret );
		char tmp[] = "QTTSTextPut failed!\n"; 
		MyEndSynth(sessionId, tmp);
		exit(-1);
	}
}

/*
 * 语音合成
 */
int SpeechSynth(const char *text, const char *outFile)
{
	const char *configs = NULL;
	const char *session_id = NULL;
	const char *synth_params = NULL;
	unsigned int text_len = 0;
	const void *synth_speech = NULL;
	unsigned int synth_speech_len = 0;

	FILE *f_speech = NULL;
	int synth_status = 0;
	int ret = 0;

	printf("=======================================================\n");
	printf("               Speech synthesis test  \n");
	printf("=======================================================\n");

	/*
	 * 初始化
	 */
	configs = "server_url=http://dev.voicecloud.cn:80/index.htm, \
		appid=51777175,timeout=10000, coding_libs=libspeex.so";
	MyInit(configs);
	//MyInit("appid=51777175");

	/*
	 * 开始会话，使用会话模式
	 */
	synth_params = "ssm=1, auf=audio/L16;rate=16000, aue=speex-wb;7, \
					ent=vivi21, vcn=vinn,tte=UTF8";
	session_id = QTTSSessionBegin( synth_params, &ret );
	if ( ret != 0 ) {
		printf( "QTTSSessionBegin failed, error code %d\n", ret );
		return -1;
	}

	/*
	 * 写入合成文本
	 */
	//synth_text = "讯飞语音云为您提供了最新最好的语音技术体验! 讯飞语音，沟通无限。";
	text_len = strlen( text );
	MyTextPut( session_id, text, text_len, NULL );

	/*
	 * 获取合成的音频
	 */
	if ( synth_first ) {
		synth_first = 0;
		f_speech = fopen(outFile, "wb");
	} else {
		f_speech = fopen(outFile, "ab");
	}
	if ( f_speech == NULL ) {
		printf( "Cannot open file \"synth_speech.pcm\"\n" );
		char tmp[] = "open file failed\n";
		MyEndSynth(session_id, tmp);
		return -1;
	}

	while ( synth_status != TTS_FLAG_DATA_END ) {
		synth_speech = QTTSAudioGet( session_id, &synth_speech_len, 
				&synth_status, &ret );
		if ( ret != 0 ) {
			printf( "QTTSAudioGet failed, error code %d\n", ret );
			break;
		}
		printf( "QTTSAudioGet ok, speech length = %d\n", synth_speech_len );

		if ( (synth_speech != NULL) && (synth_speech_len != 0) ) {
			fwrite(synth_speech, 1, synth_speech_len, f_speech);
		}
	}
	fclose(f_speech);

	{
	char tmp[] = "normal end!\n";
	MyEndSynth(session_id, tmp);
	}

	return 0;
}
