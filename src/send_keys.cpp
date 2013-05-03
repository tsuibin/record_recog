/*
 * 激活窗口 && 发送按键事件
 */

#include "send_keys.h"

//extern int keys_len;
extern struct process_info cur_process;

static int exist_flag = 0;
Window match_wid = (Window)0;

void set_focus()
{
	Display *disp = NULL;

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");
	
	//get_wid(cur_process.name);
	get_wid(cur_process.item);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return;
	}
	sys_err("match_wid : %lu\n", match_wid);

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}
	
	XCloseDisplay(disp);
	match_wid = (Window)0;
}

void exec_command(int *keys)
{
	int *tmp_key = keys;
	Display *disp = NULL;

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");
	
	//get_wid(cur_process.name);
	get_wid(cur_process.item);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return;
	}

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}

	sys_err("set keys pressed...\n");
	for (; *tmp_key != 0; tmp_key++ ) {
		send_key_press( disp, match_wid, *tmp_key  );
	}

	usleep(500);

	sys_err("set keys release...\n");
	tmp_key = keys;
	for (; *tmp_key != 0; tmp_key++ ) {
		send_key_release( disp, match_wid, *tmp_key );
	}

	XCloseDisplay(disp);
	match_wid = (Window)0;
}

void send_key_press( Display *disp, Window win, int keycode )
{
	XEvent event;

	memset(&event, 0, sizeof(event));
	event.xkey.display		= disp;
	event.xkey.window		= win;
	event.xkey.time			= CurrentTime;
	event.xkey.x			= 1;
	event.xkey.y			= 1;
	event.xkey.x_root		= 1;
	event.xkey.y_root		= 1;
	event.xkey.same_screen	= True;
	event.xkey.keycode		= XKeysymToKeycode(disp, keycode);
	event.xkey.state		= 0;
	//event.xkey.state		= modifiers;
	
	if (event.xkey.keycode) {
		XTestFakeKeyEvent(disp, event.xkey.keycode, True, CurrentTime);
	}

	event.xkey.type = KeyPress;
	if ( XSendEvent(disp, win, True, 0xfff, &event) == 0 ) {
		sys_err("Error with send KeyPress XEvent...\n");
	}
	XFlush(disp);
}

void send_key_release( Display *disp, Window win, int keycode )
{
	XEvent event;

	memset(&event, 0, sizeof(event));
	event.xkey.display		= disp;
	event.xkey.window		=win;
	event.xkey.time			= CurrentTime;
	event.xkey.x			= 1;
	event.xkey.y			= 1;
	event.xkey.x_root		= 1;
	event.xkey.y_root		= 1;
	event.xkey.same_screen	= True;
	event.xkey.keycode		= XKeysymToKeycode(disp, keycode);
	event.xkey.state		= 0;
	//event.xkey.state		= modifiers;

	if (event.xkey.keycode) {
		XTestFakeKeyEvent(disp, event.xkey.keycode, False, CurrentTime);
	}

	event.xkey.type = KeyRelease;
	if ( XSendEvent(disp, win, True, 0xfff, &event) == 0 ) {
		sys_err("Error with send KeyRelease XEvent...\n");
	}
	XFlush(disp);
}

void strtok_num(char *exec_buf, int *nums)
{
	char *p = NULL;
	char *tmp = NULL;
	
	p = strtok(exec_buf, " ");
	*nums = strtol(p, &tmp, 16);
	printf("%d\n", *nums);
	
	nums++;
	//keys_len++;
	while ( (p = strtok(NULL, " ")) ) {
		*nums = strtol(p, &tmp, 16);
		printf("%d\n", *nums);
		nums++;
		//keys_len++;
	}
	*nums = 0;
}

void get_wid(char *name)
{
	int flag = 0;
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

void sys_wm_wid()
{
	char buf[] = "wmctrl -lx | sed  \'1,3d\' | awk \'{print $1, $3}\' > \
				  ./tmp/wm_wid";
	
	system(buf);
}

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
