#include "my_qtts.h"

int synth_first = 1;

void read_wav_from_str(const char *str)
{
	char read_tmp[READ_LINE];

	if ( str == NULL ) {
		sys_says("read_wav_from_file arguments error...\n");
		return ;
	}

	sys_says("str : %s\nlen : %u\n", str, strlen(str));
	if ( SpeechSynth(str, READ_WAV) == -1 ) {
		sys_says("SpeechSynth failed...\n");
		return;
	}

	sprintf( read_tmp, "aplay -f S16_LE -c 1 -r 16000 %s", READ_WAV );
	system(read_tmp);

	return;
}

/*
 * 朗读文件中的文本
 */
void read_wav_from_file(const char *text)
{
	FILE *fp = NULL;
	int has_synth = 0;
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
		sys_says("buf : %s\nlen : %u\n", buf, strlen(buf));
		if ( SpeechSynth(buf, READ_WAV) == -1 ) {
			sys_says("SpeechSynth failed...\n");
			continue;
		}
		has_synth = 1;
	}
	fclose(fp);

	synth_first = 1;
	if ( has_synth ) {
		has_synth = 0;
		memset(read_tmp, 0, READ_LINE);
		sprintf( read_tmp, "aplay -f S16_LE -c 1 -r 16000 %s", READ_WAV );
		system(read_tmp);
		
		system("rm -f /tmp/read_wav.wav");
		sys_says("echo ...\n");
		system("echo > /tmp/xsel_out");
		sys_says("xsel clear ...\n");
		system("xsel -c");
	}
	
	return;
}

void read_xsel()
{
	system("xsel > /tmp/xsel_out");
	read_wav_from_file(XSEL_OUT);
	
	return;
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
