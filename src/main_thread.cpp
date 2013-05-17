#include "my_alsa.h"
#include "my_init.h"
#include "my_qtts.h"
#include "exec_cmd.h"
#include "my_xlib.h"

#include <signal.h>
#include <sys/select.h>

/* 录音线程 */
void *record_start(void *arg);
/* 读键盘 */
void *read_keyboard(void *arg);
void *listen_window(void *arg);
GdkFilterReturn child_handle_event( void *event1, GdkEvent *event2, 
					gpointer userdata );
void str_copy_delim(char *src);

void catch_user1(int signo);
void catch_user2(int signo);
void catch_alrm(int signo);

void close_fd();

struct process_info cur_process;

int first_record;
int record_abort = 0;
int keys[KEY_LEN];

int fd = -1;
pthread_t recd_thrd;
struct timeval tvpressed;
struct timeval tvreleased;

int main(int argc, char* argv[])
{
	int ret;

	//char tmp[NAME_LEN];
	char kbd_path[NAME_LEN];
	pthread_t read_thrd;
	//pthread_t listen_win_thrd;
	
	//signal(SIGCHLD, SIG_IGN);
	//init_deamon();
	
	//gtk_init(&argc, &argv);
	memset(keys, 0, KEY_LEN * sizeof(int));
	memset(&cur_process, 0, sizeof(cur_process));

	get_dev_path(kbd_path);
	sys_says("kbd_path : %s\n", kbd_path);
	
	//sprintf(tmp, "gksudo chmod 777 %s", kbd_path);
	//sys_says("gksudo : %s\n", tmp);
	//system(tmp);
	
	signal(SIGUSR1, catch_user1);
	signal(SIGUSR2, catch_user2);
	signal(SIGALRM, catch_alrm);
	/*
	ret = pthread_create(&listen_win_thrd, 0, listen_window, 0);
	if ( ret != 0 ) {
		sys_says( "create listen window thread failed : %s\n", strerror(errno) );
		exit(-1);
	}
	*/
	ret = pthread_create(&read_thrd, 0, read_keyboard, kbd_path);
	if ( ret != 0 ) {
		sys_says( "create read keyboard thread failed : %s\n", strerror(errno) );
		exit(-1);
	}
	
	GdkWindow *window = NULL;
	GdkScreen *screen = NULL;
	
	gtk_init(NULL, NULL);
	screen = gdk_screen_get_default();
	window = gdk_screen_get_root_window(screen);
	gdk_window_set_events (window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter (window, child_handle_event, NULL);

	gtk_main();
	
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
					gtk_main_quit ();
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
	//pthread_t play_thrd;
	
	record_abort = 1;
	memset(&tvreleased, 0, sizeof(struct timeval));
	gettimeofday( &tvreleased, NULL );
	
	tv_sub(&tvpressed, &tvreleased);
	rtt = tvreleased.tv_sec * 1000 + tvreleased.tv_usec / 1000;
	sys_says("rtt : %0.1f\n\n", rtt);
	if ( ((int)rtt) < 1500 ) {
		//按下时间小于2s，不是录音操作，放弃
		return;
	}
	
	system(" espeak -vzh+f2 \"您输入的语音为\"");
	system("aplay -f S16_LE -c 1 -r 16000  /tmp/tmpin.wav");
	pthread_join(recd_thrd, NULL);
	parse_record(cur_process.cmd_buf);
	if ( cur_process.cmd_buf[0] == '\0' ) {
		return;
	}
	
	if ( !is_config_set() ) {
		fprintf(stderr, "!is_config_set\n");
		no_set_config(cur_process.cmd_buf, cur_process.exec_buf, 
					cur_process.type);
	} else {
		fprintf(stderr, "is_config_set\n");
		has_set_config(cur_process.cmd_buf, cur_process.exec_buf, 
					cur_process.type);
	}
}

GdkFilterReturn child_handle_event( void *event1, GdkEvent *event2,
							gpointer userdata )
{
	pthread_detach(pthread_self());
	XEvent *xev = (XEvent*)event1;
	
	if ( xev->type == PropertyNotify ) {
	//if ( xev->type == FocusIn ) {
		//g_print("child_handle_event\n");
		kill(getpid(), SIGALRM);
		//g_print("child handle event kill SIGALRM...\n");
	}
	
	return GDK_FILTER_CONTINUE;
}

void *listen_window(void *arg)
{
	GdkWindow *window = NULL;
	GdkScreen *screen = NULL;
	
	gtk_init(NULL, NULL);
	screen = gdk_screen_get_default();
	window = gdk_screen_get_root_window(screen);
	gdk_window_set_events (window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter (window, child_handle_event, NULL);

	gtk_main();
	
	pthread_exit(NULL);
}

void catch_alrm(int signo)
{
	char window_class[EXEC_BUF];
	gchar *tmp;
	
	tmp = get_window_class(get_active_window());
	memset(window_class, 0, EXEC_BUF);
	memcpy(window_class, tmp, strlen(tmp));
	g_free(tmp);
	sys_says("window class name : %s\n", window_class);
	
	str_copy_delim(window_class);
	memset(cur_process.name, 0, BUF_LEN);
	memcpy(cur_process.name, window_class, strlen(window_class));
	sys_says("cur_name : %s\n", cur_process.name);
	
	int i = 0;
	for ( ; i < (int)strlen(window_class); i++ ) {
		if ( window_class[i] == '-' ) {
			window_class[i] = '_';
		}
	}
	memset(cur_process.table, 0, BUF_LEN);
	memcpy(cur_process.table, window_class, strlen(window_class));
	sys_says("cur_table : %s\n", cur_process.table);
}

/*
* 以指定的字符串分割给出的字符串
* 得到对应的结果
*/
void str_copy_delim(char *src)
{
	char *p = NULL;
	char tmp[32];
	int len = strlen(src);

	if (src == NULL) {
		return;
	}

	memset(tmp, 0, 32);
	if ( tmp == NULL ) {
		sys_says("calloc failed...\n");
		return;
	}
	
	p = strtok(src, ".");
	//printf("first : %s\n", p);

	p = strtok(NULL, ".");
	memcpy(tmp, p, strlen(p));
	//printf("end: %s\n", tmp);

	memset(src, 0, len + 1);
	memcpy(src, tmp, strlen(tmp));
	printf("name: %s\n", src);
	
	if ( *src >= 'A' && *src <= 'Z' ) {
		*src |= 0x20;	//大写转小写
	}
}

void close_fd()
{
	if ( fd != -1 ) {
		close(fd);
	}
}
