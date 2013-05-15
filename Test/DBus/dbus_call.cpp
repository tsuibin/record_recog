#include <stdio.h>
#include <stdlib.h>

int MusicPlayPause();
int MusicForward();
int MusicPrev();
int MusicNext();
int MusicStop();
int MusicQuit();
int MusicRewind();
//int MusicChangeVolume(float value);
int MusicChangeVolume(char *value);

int main()
{
	MusicChangeVolume("0.6");
	
	return 0;
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
