#include "my_init.h"

/*
 * 初始化守护进程
 */
void init_deamon()
{
	//int i;
	pid_t pid;

	pid = fork();
	if ( pid == -1 ) {
		sys_says("fork failed : %s\n", strerror(errno));
		exit(-1);
	} else if ( pid > 0 ) {
		exit(0);	//结束父进程
	}
	//sleep(1);

	// 第一子进程，后台继续执行
	setsid();	//第一子进程成为新的会话组长和进程组长

	// 禁止进程重新打开控制终端
	pid = fork();
	if ( pid == -1 ) {
		sys_says("child fork failed : %s\n", strerror(errno));
		exit(-1);
	} else if ( pid > 0 ) {
		exit(0);	//结束第一子进程
	}
	//sleep(1);

	//第二子进程不再是会话组长
	//关闭已打开的文件描述符
	/*for ( i = 0; i < NOFILE; i++ ) {
		close(i);
	}*/

	chdir("/home/iwen/Yunio/Deepin/SpeechCtrl");	//改变工作目录到 /tmp
	umask(0);	//重设文件创建掩码

	return ;
}

/*
 * 获取键盘设备名
 */
void get_dev_path(char *path)
{
	FILE *fp = NULL;
	char tmp[NAME_LEN];
	char buf[] = "ls -l /dev/input/by-path/ | awk \'{print $9, $NF}\' | \
			  grep 0-event-kbd | awk \'{print $NF}\' | cut -c4- > /tmp/keys";

	system(buf);
	memset(tmp, 0, NAME_LEN);
	if ( (fp = fopen("/tmp/keys", "r")) == NULL ) {
		sys_says("fopen err : %s\n", strerror(errno));
		exit(-1);
	}

	fscanf(fp, "%s", tmp);
	fclose(fp);
	
	sys_says("tmp : %s\n", tmp);
	if ( tmp[0] == '\0' ) {
		sys_says("get_dev_id failed...\n");
		exit(-1);
	}

	memset(path, 0, NAME_LEN);
	sprintf( path, "/dev/input/%s", tmp );
	//sys_says("path : %s\n", path);
}

/* 计算按键按下松开之间的时间 */
void tv_sub( struct timeval *presse_time, struct timeval *release_time )
{
	long sec  = release_time->tv_sec - presse_time->tv_sec;
	long usec = release_time->tv_usec - presse_time->tv_usec;

	if ( usec >= 0 ) {
		release_time->tv_sec  = sec;
		release_time->tv_usec = usec;
	} else {
		release_time->tv_sec  = sec - 1;
		release_time->tv_usec = -usec;
	}
}
