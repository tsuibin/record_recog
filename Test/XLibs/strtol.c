#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void strtok_num(char *exec_buf, int *nums)
{
	char *p = NULL;
	char *tmp = NULL;
	
	p = strtok(exec_buf, " ");
	*nums = strtol(p, &tmp, 16);
	printf("%d\n", *nums);
	
	nums++;
	while ( (p = strtok(NULL, " ")) ) {
		*nums = strtol(p, &tmp, 16);
		printf("%d\n", *nums);
		nums++;
	}
	*nums = 0;
}

int main()
{
	int i = 0;
	char str[] = "0X1234 0Xad34 0X2af4 0Xff";
	int num[10];
	
	strtok_num(str, num);
	
	for (; i < 10; i++ ) {
		if ( num[i] == 0 ) {
			printf("i = %d\n", i);
			break;
		}
		printf("%d\n", num[i]);
	}
	
	return 0;
}
