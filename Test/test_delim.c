#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define sys_says(arg ...)	{ fprintf(stderr, arg); }

/*
* 以指定的字符串分割给出的字符串
* 得到对应的结果
*/
void str_copy_delim(char *src)
{
	char *p = NULL;
	char *tmp = NULL;
	int len = strlen(src);

	if (src == NULL) {
		return;
	}

	tmp = (char*)calloc(len + 1, sizeof(char));
	if ( tmp == NULL ) {
		sys_says("calloc failed...\n");
		return;
	}
	
	p = strtok(src, ".");
	printf("first : %s\n", p);

	p = strtok(NULL, ".");
	memcpy(tmp, p, strlen(p));
	printf("end: %s\n", tmp);

	memset(src, 0, len + 1);
	memcpy(src, tmp, strlen(tmp));
	free(tmp);
	printf("name: %s\n", src);
	
	if ( *src >= 'A' && *src <= 'Z' ) {
		*src |= 0x20;	//大写转小写
	}
}

int main()
{
	char str[] = "gnome-terminal.Gnome-terminal";
	
	str_copy_delim(str);
	sys_says("str : %s\n", str);
	
	return 0;
}
