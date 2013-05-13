#include <stdio.h>

int main()
{
	char buf[]="我 test 结束";

	int i = 0;
	while ( buf[i] != '\0' ) {
		printf("%c\t0X%x\t%d\n", buf[i], buf[i], buf[i]&0X80);
		i++;
	}
	printf("\n");

	return 0;
}
