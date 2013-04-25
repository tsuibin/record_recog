#ifndef __MY_QTTS_H__
#define __MY_QTTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "qisr.h"

#define BLOCK_LEN	5*1024

inline void MyEndRecog(const char *sessionId, char *reason)
{
	int ret;

	ret = QISRSessionEnd(sessionId, reason);
	if ( ret != 0 ) {
		printf("QTTSSessionEnd failed, error code is %d\n", ret);
	}
	sessionId = NULL;

	ret = QISRFini();
	if ( ret != 0 ) {
		printf("QTTSFini failed, error code is %d\n", ret);
	}
}

void MyInit(const char *configs);

void MyAudioWrite(const char *sessionId, const void *recogAudio, unsigned 
	int audioLen, int audioStatus, int *epStatus, int *recogStatus);
	
int SpeechRecog(const char *inFile, const char *outFile);
	
#endif
