#include "my_common.h"

int main()
{
	FILE *fp = NULL;
	char buf[READ_LINE];
	
	if ( (fp = fopen("tt", "r")) == NULL ) {
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
	}
	fclose(fp);
	
	return 0;
}
