#ifndef __MY_QTTS_H__
#define __MY_QTTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void MyInit(const char *configs);

void MyTextPut(const char *sessionId, const char *synthText, 
	unsigned int textLen, const char *params);

int SpeechSynth(const char *text, const char *outFile);	
#endif
