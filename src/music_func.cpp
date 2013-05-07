#include "exec_func.h"

void music_type_func(char *exec_buf)
{
	//sys_err("music func\n");
	if ( strncmp(exec_buf, "PlayPause", 9) == 0 ) {
		MusicPlayPause();
		return;
	}
	
	if ( strncmp(exec_buf, "Next", 4) == 0 ) {
		MusicNext();
		return;
	}
	
	if ( strncmp(exec_buf, "Prev", 4) == 0 ) {
		MusicPrev();
		return;
	}
	
	if ( strncmp(exec_buf, "Rewind", 6) == 0 ) {
		MusicRewind();
		return;
	}
	
	if ( strncmp(exec_buf, "Forward", 7) == 0 ) {
		MusicForward();
		return;
	}
	
	if ( strncmp(exec_buf, "Stop", 4) == 0 ) {
		MusicStop();
		return;
	}
	
	if ( strncmp(exec_buf, "Quit", 4) == 0 ) {
		MusicQuit();
		return;
	}
}

int MusicPlayPause()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.PlayPause";
	
	if ( (system(buf)) == -1 ) {
		printf("PlayPause failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicForward()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Forward";
	
	if ( (system(buf)) == -1 ) {
		printf("Forward failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicPrev()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Prev";
	
	if ( (system(buf)) == -1 ) {
		printf("Prev failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicNext()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Next";
	
	if ( (system(buf)) == -1 ) {
		printf("Next failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicStop()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Stop";
	
	if ( (system(buf)) == -1 ) {
		printf("Stop failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicQuit()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Quit";
	
	if ( (system(buf)) == -1 ) {
		printf("Quit failed...\n");
		return -1;
	} 
	
	return 0;
}

int MusicRewind()
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.Rewind";
	
	if ( (system(buf)) == -1 ) {
		printf("Rewind failed...\n");
		return -1;
	} 
	
	return 0;
}

//int MusicChangeVolume(float value)
int MusicChangeVolume(char *value)
{
	char buf[] = "dbus-send --print-reply --type=method_call \
	--dest=com.linuxdeepin.DMusic /com/linuxdeepin/DMusic \
	com.linuxdeepin.DMusic.ChangeVolume double:";
	
	sprintf(buf, "%s%s", buf, value);
	//printf("%s\n", buf);
	if ( (system(buf)) == -1 ) {
		printf("ChangeVolume failed...\n");
		return -1;
	} 
	
	return 0;
}
