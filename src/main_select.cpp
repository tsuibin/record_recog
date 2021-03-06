#include "my_alsa.h"
#include "my_init.h"
#include "my_qtts.h"
#include "exec_cmd.h"

#include <sys/select.h>

/* 录音线程 */
void *record_start(void *arg);

void close_fd();

struct process_info cur_process;

int first_record;
int record_abort = 0;
int keys[KEY_LEN];

int fd = -1;

int main()
{
	int flag;

	int ret;
	fd_set rfds;
	struct timeval tv;

	char tmp[NAME_LEN];
	char kbd_path[NAME_LEN];
	char cmd_buf[BUF_LEN];
	char type_buf[BUF_LEN];
	char exec_buf[READ_LINE];
	
	double rtt;
	struct timeval tvpressed;
	struct timeval tvreleased;
	
	struct input_event myinput;
	
	atexit( close_fd );
	//signal(SIGCHLD, SIG_IGN);
	//init_deamon();
	
	memset(keys, 0, KEY_LEN * sizeof(int));
	memset(cmd_buf, 0, BUF_LEN);
	memset(type_buf, 0, BUF_LEN);
	memset(exec_buf, 0, READ_LINE);

	get_dev_path(kbd_path);
	sys_says("kbd_path : %s\n", kbd_path);
	
	sprintf(tmp, "gksudo chmod 777 %s", kbd_path);
	sys_says("gksudo : %s\n", tmp);
	system(tmp);
	
	fd = open(kbd_path, O_RDONLY);
	if ( fd == -1 ) {
		sys_says("open keyboard err : %s\n", strerror(errno) );
		return -1;
	}

	//pthread_t recd_start;

	//init_deamon();
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
			//if ( FD_ISSET(fd, &rfds) ) {
			flag = read(fd, &myinput, sizeof(myinput));
			if ( flag == -1 ) {
				sys_says("read keyboard err : %s\n", strerror(errno));
			} else if (flag == sizeof(myinput)) {
				if (myinput.code == KEY_ESC) {
					//close(fd);
					//fd = -1;
					break;
				}

				if ( (myinput.code == KEY_LEFTCTRL || 
							myinput.code == KEY_RIGHTCTRL) ) {
					if ( myinput.value == 1 ) {	//按下
						memset(&tvpressed, 0, sizeof(struct timeval));
						gettimeofday( &tvpressed, NULL );
						sys_says("KEY_CTRL Pressed\n");
					
						first_record = 1;
						record_abort = 1;
						/*if ( pthread_create(&recd_start, 0, 
											record_start, 0) != 0 ) {
							sys_says("create thrd err...\n");
							return -1;
						}*/
					} else if ( myinput.value == 0 ) {	//松开
						memset(&tvreleased, 0, sizeof(struct timeval));
						gettimeofday( &tvreleased, NULL );
						sys_says("KEY_CTRL Released\n");
						record_abort = 0;
					
						//pthread_join(recd_start, NULL);
						
						tv_sub(&tvpressed, &tvreleased);
						rtt = tvreleased.tv_sec * 1000 + tvreleased.tv_usec / 1000;
						sys_says("rtt : %0.1f\n\n", rtt);
						if ( (((int)rtt) / 1000) < 2 ) {
							//按下时间小于2s，不是录音操作，放弃
							continue;
						}
						parse_record(cmd_buf);
						if ( cmd_buf[0] == '\0' ) {
							continue;
						}
					
						if ( !is_config_set() ) {
							fprintf(stderr, "!is_config_set\n");
							no_set_config(cmd_buf, exec_buf, type_buf);
						} else {
							fprintf(stderr, "is_config_set\n");
							has_set_config(cmd_buf, exec_buf, type_buf);
						}
					}
				}
				
				if ( (myinput.code == KEY_LEFTALT) || 
						(myinput.code == KEY_RIGHTALT) ) {
					if ( myinput.value == 1 ) {	//按下
						read_xsel();
					}
				}
			}
			//}
		}
	}
	/*
	if ( fd != -1 ) {
		close(fd);
	}
	*/
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

void close_fd()
{
	if ( fd != -1 ) {
		close(fd);
	}
}
