#include "my_alsa.h"
#include "exec_cmd.h"
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define NAME_LEN 80
//#define sys_says(arg ...)	{ fprintf(stderr, arg); }

void get_dev_id(char *id_name);
void *record_start(void *arg);

struct process_info cur_process;

int first_record;
int record_abort = 0;

#include <signal.h>
#include <sys/stat.h>
//#include <sys/param.h>

/* 守护进程 */
void init_deamon();

int main()
{
	int fd;
	int flag;
	char name[NAME_LEN];
	char kbd_path[NAME_LEN];
	struct input_event myinput;
	
	int keys[KEY_LEN];
	char cmd_buf[BUF_LEN];
	char exec_buf[READ_LINE];
	
	init_deamon();
	
	memset(keys, 0, KEY_LEN * sizeof(int));
	memset(cmd_buf, 0, BUF_LEN);
	memset(exec_buf, 0, READ_LINE);

	memset(name, 0, NAME_LEN);
	get_dev_id(name);
	if ( name[0] == '\0' ) {
		sys_says("get_dev_id failed...\n");
		exit(-1);
	}

	sprintf( kbd_path, "/dev/input/%s", name );
	sys_says("kbd_path : %s\n", kbd_path);
	fd = open(kbd_path, O_RDONLY);
	if ( fd == -1 ) {
		sys_says("open keyboard err : %s\n", strerror(errno) );
		return -1;
	}

	//pthread_t recd_start;

	//init_deamon();
	while (1) {
		memset(&myinput, 0, sizeof(myinput));
		flag = read(fd, &myinput, sizeof(myinput));
		if ( flag == -1 ) {
			sys_says("read keyboard err : %s\n", strerror(errno));
		} else if (flag == sizeof(myinput)) {
			if (myinput.code == KEY_ESC) {
				break;
			}

			if ( (myinput.code == KEY_LEFTCTRL || 
						myinput.code == KEY_RIGHTCTRL) && 
					(myinput.value == 1) ) {
				sys_says("KEY_CTRL Pressed\n");
				
				first_record = 1;
				record_abort = 1;
				/*if ( pthread_create(&recd_start, 0, record_start, 0) != 0 ) {
					sys_says("create thrd err...\n");
					return -1;
				}*/
			}
			
			if ( (myinput.code == KEY_LEFTCTRL || 
						myinput.code == KEY_RIGHTCTRL) && 
					(myinput.value == 0) ) {
				sys_says("KEY_CTRL Released\n");
				record_abort = 0;
				
				//pthread_join(recd_start, NULL);
				parse_record(cmd_buf);
				if ( cmd_buf[0] == '\0' ) {
					continue;
				}
				
				if ( !is_config_set() ) {
					fprintf(stderr, "!is_config_set\n");
					no_set_config(cmd_buf, exec_buf);
				} else {
					fprintf(stderr, "is_config_set\n");
					has_set_config(cmd_buf, exec_buf, keys);
				}
			}
		}
	}
	close(fd);

	return 0;
}

/*
 * 录音线程
 */
void *record_start(void *arg)
{
	int ret = 0;
	
	//while (record_abort) {
		// 录音 2s
		ret = MySndRecord(2, INPUT_FILE);
		if ( ret == -1 ) {
			fprintf(stderr, "录音失败...\n");
			exit(-1);
		}
	//}
	pthread_exit(NULL);
}

/*
 * 获取键盘设备名
 */
void get_dev_id(char *id_name)
{
	FILE *fp = NULL;
	char buf[] = "ls -l /dev/input/by-path/ | awk \'{print $9, $NF}\' | \
			  grep 0-event-kbd | awk \'{print $NF}\' | cut -c4- > /tmp/keys";

	system(buf);
	if ( (fp = fopen("/tmp/keys", "r")) == NULL ) {
		sys_says("fopen err : %s\n", strerror(errno));
		exit(-1);
	}

	fscanf(fp, "%s", id_name);
	fclose(fp);

	sys_says("id_name : %s\n", id_name);
}

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

	// 第一子进程，后台继续执行
	setsid();	//第一子进程成为新的会话组长和进程组长

	// 禁止进程重新打开控制终端
	/*pid = fork();
	if ( pid == -1 ) {
		sys_says("child fork failed : %s\n", strerror(errno));
		exit(-1);
	} else if ( pid > 0 ) {
		exit(0);	//结束第一子进程
	}*/

	//第二子进程不再是会话组长
	//关闭已打开的文件描述符
	/*for ( i = 0; i < NOFILE; i++ ) {
		close(i);
	}*/

	chdir("/home/iwen/Yunio/Deepin/record_speech");	//改变工作目录到 /tmp
	umask(0);	//重设文件创建掩码

	signal(SIGCHLD, SIG_IGN);

	return ;
}
