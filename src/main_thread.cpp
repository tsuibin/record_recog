#include "my_alsa.h"
#include "my_init.h"
#include "my_qtts.h"
#include "exec_cmd.h"

#include <signal.h>
#include <sys/select.h>

/* 录音线程 */
void *record_start(void *arg);
/* 读键盘 */
void *read_keyboard(void *arg);

void catch_user1(int signo);
void catch_user2(int signo);

void close_fd();

struct process_info cur_process;

int first_record;
int record_abort = 0;
int keys[KEY_LEN];

int fd = -1;
pthread_t recd_thrd;
struct timeval tvpressed;
struct timeval tvreleased;

int main()
{
	int ret;

	//char tmp[NAME_LEN];
	char kbd_path[NAME_LEN];
	pthread_t read_thrd;
	
	signal(SIGCHLD, SIG_IGN);
	init_deamon();
	
	memset(keys, 0, KEY_LEN * sizeof(int));

	get_dev_path(kbd_path);
	sys_says("kbd_path : %s\n", kbd_path);
	
	//sprintf(tmp, "gksudo chmod 777 %s", kbd_path);
	//sys_says("gksudo : %s\n", tmp);
	//system(tmp);
	
	signal(SIGUSR1, catch_user1);
	signal(SIGUSR2, catch_user2);
	ret = pthread_create(&read_thrd, 0, read_keyboard, kbd_path);
	if ( ret != 0 ) {
		sys_says( "create read keyboard thread failed : %s\n", strerror(errno) );
		exit(-1);
	}
	
	pthread_join(read_thrd, NULL);
	read_wav_from_str("程序退出");
	sys_says("程序退出...\n\n");
	return 0;
}

/* 读键盘 */
void *read_keyboard(void *arg)
{
	int ret;
	fd_set rfds;
	struct timeval tv;
	char *kbd_path = (char*)arg;
	struct input_event myinput;
	
	fd = open(kbd_path, O_RDONLY);
	if ( fd == -1 ) {
		sys_says("open keyboard err : %s\n", strerror(errno) );
		pthread_exit(NULL);
	}
	
	atexit( close_fd );
	while (1) {
		memset(&myinput, 0, sizeof(myinput));

		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		ret = select( fd + 1, &rfds, NULL, NULL, &tv );
		if ( ret == -1 ) {
			sys_says( "select err : %s\n", strerror(errno) );
		} else if ( ret == 0 ) {
			continue;
		} else {
			ret = read(fd, &myinput, sizeof(myinput));
			if ( ret == -1 ) {
				sys_says("read keyboard err : %s\n", strerror(errno));
			} else if (ret == sizeof(myinput)) {
				if (myinput.code == KEY_ESC) {
					close_fd();
					break;
					//pthread_exit(NULL);
				}

				if ( (myinput.code == KEY_LEFTCTRL || 
							myinput.code == KEY_RIGHTCTRL) ) {
					if ( myinput.value == 1 ) {	//按下
						sys_says("KEY_CTRL Pressed\n");
						kill(getpid(), SIGUSR1);
					} else if ( myinput.value == 0 ) {	//松开
						sys_says("KEY_CTRL Released\n");
						kill(getpid(), SIGUSR2);
					}
				}
				
				if ( (myinput.code == KEY_LEFTALT) || 
						(myinput.code == KEY_RIGHTALT) ) {
					if ( myinput.value == 1 ) {	//按下
						read_xsel();
					}
				}
			}
		}
	}
	close_fd();
	
	pthread_exit(NULL);
}

void catch_user1(int signo)
{
	memset(&tvpressed, 0, sizeof(struct timeval));
	gettimeofday( &tvpressed, NULL );
	
	first_record = 1;
	record_abort = 0;
	if ( pthread_create(&recd_thrd, 0, record_start, 0) != 0 ) {
		sys_says("create thrd err...\n");
		return ;
	}
}

/*
 * 录音线程
 */
void *record_start(void *arg)
{
	int ret = 0;
	
	ret = MySndRecord(2, INPUT_FILE);
	if ( ret == -1 ) {
		fprintf(stderr, "录音失败...\n");
		exit(-1);
	}
		
	pthread_exit(NULL);
}

void catch_user2(int signo)
{
	double rtt;
	char cmd_buf[BUF_LEN];
	char type_buf[BUF_LEN];
	char exec_buf[READ_LINE];
	
	memset(cmd_buf, 0, BUF_LEN);
	memset(type_buf, 0, BUF_LEN);
	memset(exec_buf, 0, READ_LINE);
	
	record_abort = 1;
	memset(&tvreleased, 0, sizeof(struct timeval));
	gettimeofday( &tvreleased, NULL );
	
	tv_sub(&tvpressed, &tvreleased);
	rtt = tvreleased.tv_sec * 1000 + tvreleased.tv_usec / 1000;
	sys_says("rtt : %0.1f\n\n", rtt);
	if ( (((int)rtt) / 1000) < 2 ) {
		//按下时间小于2s，不是录音操作，放弃
		return;
	}
	
	pthread_join(recd_thrd, NULL);
	parse_record(cmd_buf);
	if ( cmd_buf[0] == '\0' ) {
		return;
	}
	
	if ( !is_config_set() ) {
		fprintf(stderr, "!is_config_set\n");
		no_set_config(cmd_buf, exec_buf, type_buf);
	} else {
		fprintf(stderr, "is_config_set\n");
		has_set_config(cmd_buf, exec_buf, type_buf);
	}
}

void close_fd()
{
	if ( fd != -1 ) {
		close(fd);
	}
}
