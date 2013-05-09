#include "my_qtts.h"

int synth_first = 1;

/*
 * 朗读文件中的文本
 */
void read_wav_from_file(char *text)
{
	FILE *fp = NULL;
	char buf[READ_LINE];
	char read_tmp[READ_LINE];
	if ( text == NULL ) {
		sys_says("read_wav_from_file arguments error...\n");
		return ;
	}

	if ( (fp = fopen(text, "r")) == NULL ) {
		sys_says("read_wav_from_file open file failed...\n");
		return;
	}

	while ( !feof(fp) ) {
		memset(buf, 0, READ_LINE);
		fgets(buf, READ_LINE, fp);
		if ( buf == NULL ) {
			continue;
		}

		if ( buf[strlen(buf) - 1] == '\n' ) {
			buf[strlen(buf) - 1] = '\0';
		}

		if ( strlen(buf) < 1 ) {
			continue;
		}
		sys_says("buf : %s\nlen : %d\n", buf, strlen(buf));
		if ( SpeechSynth(buf, READ_WAV) == -1 ) {
			sys_says("SpeechSynth failed...\n");
			continue;
		}
	}
	fclose(fp);

	synth_first = 1;
	memset(read_tmp, 0, READ_LINE);
	sprintf( read_tmp, "aplay -f S16_LE -c 1 -r 16000 %s", READ_WAV );
	system(read_tmp);
}

void read_xsel()
{
	system("xsel > /tmp/sel_out");
	read_wav_from_file("/tmp/sel_out");
}

/*
int main( int argc, char **argv )
{
	if ( argc != 2 ) {
		sys_says("Usage : %s <text name>\n", argv[0]);
		return -1;
	}

	read_wav(argv[1]);

	return 0;
}
*/
