#ifndef __MY_QTTS_H__
#define __MY_QTTS_H__

#include "my_common.h"
#include "qtts.h"

inline void MyEndSynth(const char *sessionId, char *reason)
{
	int ret;

	ret = QTTSSessionEnd(sessionId, reason);
	if ( ret != 0 ) {
		printf("QTTSSessionEnd failed, error code is %d\n", ret);
	}
	sessionId = NULL;

	ret = QTTSFini();
	if ( ret != 0 ) {
		printf("QTTSFini failed, error code is %d\n", ret);
	}
}

int MyInitTTS(const char *configs);

void MyTextPut(const char *sessionId, const char *synthText, 
	unsigned int textLen, const char *params);

int SpeechSynth(const char *text, const char *outFile);	

void read_wav_from_file(const char *text);
void read_xsel();
#endif
