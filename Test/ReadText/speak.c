#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define READ_LINE 1024

struct speech_type {
	int zh_flag;
	char text[READ_LINE];
};

int i = 0;
struct speech_type speech_text[10];

char *copy_asc(char *dest, char *src)
{
	if ( src == NULL || strlen(src) < 1 ) {
		return NULL;
	}

	printf("copy_asc...\n");
	while ( *src != '\0' ) {
		if ( (*src & 0X80) == 0 ) { 
			*(dest++) = *(src++);
		} else {
			break;
		}
	}

	return src;
}

char *copy_zh(char *dest, char *src)
{
	if ( src == NULL || strlen(src) < 1 ) {
		return NULL;
	}

	printf("copy_zh...\n");
	while ( *src != '\0' ) {
		if ( (*src & 0X80) == 0 ) { 
			break;
		} else {
			memcpy(dest, src, 3);
			//printf("dest : %s\n", dest);
			src += 3;
			dest += 3;
		}
	}

	return src;
}

void split_zh(char *text)
{
	int cnt = 1;
	int first = 1;
	/*struct speech_type **p = (struct speech_type**)calloc(1, 
			sizeof(struct speech_type*));*/

	printf("split_zh...\n");
	while ( *text != '\0' ) {
		if ( (*text & 0X80) == 0 ) { 
			speech_text[i].zh_flag = 0;
			text = copy_asc(speech_text[i].text, text);
			i++;
		} else {
			speech_text[i].zh_flag = 1;
			text = copy_zh(speech_text[i].text, text);
			i++;
		}
		//text++;
	}
}

int main()
{
	int j = 0;
	char tmp[READ_LINE];
	char buf[] = "Linux 我是谁？ Windows 设计有以下的多字符组串行的特质 What do you doing? 请回答。Where are you going ? 结束。";

	memset(&speech_text, 0, 10 * sizeof(struct speech_type));
	split_zh(buf);

	printf("output: \n");
	while ( j < i ) {
		memset(tmp, 0, READ_LINE);
		printf("flag : %d\n", speech_text[j].zh_flag);
		if ( speech_text[j].zh_flag ) {
			sprintf(tmp, "espeak -vzh+f2 \"%s\"", speech_text[j].text);
			//printf("tmp : %s\n\n", tmp);
			system(tmp);
		} else {
			sprintf(tmp, "espeak -ven+f2  \"%s\"", speech_text[j].text);
			//printf("tmp : %s\n\n", tmp);
			system(tmp);
		}
		j++;
	}

	return 0;
}
