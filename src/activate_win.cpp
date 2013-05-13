#include "my_xlib.h"
#include <libwnck/libwnck.h>

extern Window match_wid;
extern struct process_info cur_process;

int activate_win()
{
	WnckScreen *screen = NULL;
	GList *window_l = NULL;
	int flag = 0;
	int pid = get_pid_via_name(cur_process.name);

	gdk_init( NULL, NULL );
	screen = wnck_screen_get_default();
	wnck_screen_force_update(screen);

	window_l = wnck_screen_get_windows(screen);
	for ( ; window_l != NULL; window_l = window_l->next ) {
		WnckWindow *window = WNCK_WINDOW(window_l->data);
		if ( wnck_window_get_window_type(window) != WNCK_WINDOW_NORMAL ) {
			continue;
		}
		if ( pid == wnck_window_get_pid(window) ) {
			match_wid = wnck_window_get_xid(window);
			wnck_window_activate(window, 1);
			flag = 1;
			break;
		}
	}
	
	if ( flag == 0 ) {
		return -1;
	}

	return 0;
}

/*
 * 获取进程 ID
 */
int get_pid_via_name( char *process_name )
{
	char buf[1024];
	int pid = 0;
	FILE *fp = NULL;
	
	printf("get_pid_via_name : %s\n", process_name);
	sprintf( buf, "ps aux | grep %s | sed -n 1p | awk '{print $2}' > /tmp/pid", 
			process_name );
	system(buf);
	
	if ( (fp = fopen("/tmp/pid", "r")) == NULL ) {
		sys_err("get_pid failed...\n");
		return 0;
	}
	fscanf(fp, "%d", &pid);
	fclose(fp);
	printf("get_pid_via_name : %d\n", pid);
	
	return pid;
}

/*
 * 激活窗口
 */
int set_focus()
{
	Display *disp = NULL;

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return -2;
	}
	sys_err("create display ok!\n");
	
	//get_wid(cur_process.name);
	get_wid(cur_process.item);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return -1;
	}
	sys_err("match_wid : %lu\n", match_wid);

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}
	
	XCloseDisplay(disp);
	match_wid = (Window)0;
	
	return 0;
}

/*
 * 获取窗口 ID
 */
void get_wid(char *name)
{
	//int flag = 0;
	FILE *fwm = NULL;
	unsigned long int wm = 0;
	char wm_buf[READ_LINE];
	
	if ( name == NULL ) {
		return ;
	}
	
	sys_says("get_wid name : %s\n", name);
	sys_wm_wid();
	
	fwm = fopen("./tmp/wm_wid", "r");
	if ( fwm == NULL ) {
		sys_err("get_wid open file wm_wid failed...\n");
		return ;
	}
	
	memset(wm_buf, 0, READ_LINE);
	while (!feof(fwm)) {
		fgets(wm_buf, READ_LINE, fwm);
		if ( wm_buf == 0 ) {
			continue;
		}
		if ( index_str(name, wm_buf) == 0 ) {
			sscanf(wm_buf, "0x%lx", &wm);
			//sys_says("wm : %lu\n", wm);
			match_wid = (Window)wm;
			break;
		}
	}
	fclose(fwm);
}

/*
 * 生成窗口 ID 列表
 */
void sys_wm_wid()
{
	char buf[] = "wmctrl -lx | sed  \'1,3d\' | awk \'{print $1, $3}\' > \
				  ./tmp/wm_wid";
	
	system(buf);
}

/*
 * 匹配程序名
 */
int index_str(char *name, char *src)
{
	int i = 0;
	int flag = 0;
	int src_len = 0;
	int name_len = 0;

	if ( name == NULL || src == NULL ) {
		sys_says("index argument error...\n");
		return -1;
	}

	src_len = strlen(src);
	name_len = strlen(name);
	while ( src[i] != '\0' ) {
		if ( i + name_len > src_len ) {
			break;
		}

		if ( strncmp( name, src + i, name_len ) == 0 ) {
			flag = 1;
			break;
		}
		i++;
	}

	if ( !flag ) {
		return -1;
	}

	return 0;
}
