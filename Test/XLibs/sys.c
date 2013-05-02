#include <stdio.h>
#include <stdlib.h>

int main()
{
	system("pgrep -f  gedit | wc -l > rows");

	return 0;
}
