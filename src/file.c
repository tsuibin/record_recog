#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_exec_file()
{
	FILE *fp = NULL;
	int i = 0;
	char tmp[4];
	char buf[] = "ps aux | grep -w deepin-music-player | wc -l > rows";

	system(buf);

	fp = fopen("rows", "r");
	//fread(tmp, 1, 4, fp);
	fscanf(fp, "%d", &i);
	fclose(fp);
	printf("%d\n", i);
}

void get_exec_pipe()
{
	char buf_ps[1024];
	char ret[1024];
	FILE *ptr = NULL;
	char cmd[]="ps aux | grep -w deepin-music-player | wc -l";

	ptr = popen(cmd, "r");
	while (fgets(buf_ps, 1024, ptr) != NULL ) {
		strcat(ret, buf_ps);
		if (strlen(ret) > 1024 ) {
			break;
		}
	}
	pclose(ptr);
	ptr = NULL;

	printf("ret : %s\n", ret);
}

int main()
{
	get_exec_file();

	return 0;
}
