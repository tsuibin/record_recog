#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/param.h>

#define sys_says(args ...)	{ fprintf(stderr, args); }

void init_deamon()
{
	int i;
	pid_t pid;

	pid = fork();
	if ( pid == -1 ) {
		sys_says("fork failed : %s\n", strerror(errno));
		exit(-1);
	} else if ( pid > 0 ) {
		exit(0);	//结束父进程
	}

	// 第一子进程，后台继续执行
	setsid();	//第一子进程成为新的会话组长和进程组长

	//与控制终端脱离
	pid = fork();
	if ( pid == -1 ) {
		sys_says("child fork failed : %s\n", strerror(errno));
		exit(-1);
	} else if ( pid > 0 ) {
		exit(0);	//结束第一子进程
	}

	//第二子进程不再是会话组长
	//关闭已打开的文件描述符
	for ( i = 0; i < NOFILE; i++ ) {
		close(i);
	}

	chdir("/home/iwen/Yunio/Deepin/record_speech/conf");	//改变工作目录到 /tmp
	umask(0);	//重设文件创建掩码

	signal(SIGCHLD, SIG_IGN);

	return ;
}

/*
 * 测试程序
 */

#include <time.h>

int main()
{
	FILE *fp = NULL;
	time_t tm;

	init_deamon();
	while (1) {
		sleep(60);

		if ( (fp = fopen("test.log", "a")) != NULL ) {
			tm = time(0);
			fprintf(fp, "Im here at %s", asctime(localtime(&tm)));
			fclose(fp);
		}
	}

	return 0;
}
