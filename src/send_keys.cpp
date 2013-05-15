/*
 * 激活窗口 && 发送按键事件
 */

#include "my_xlib.h"

//extern int keys_len;
extern struct process_info cur_process;

//static int exist_flag = 0;
Window match_wid = (Window)0;

/*
 * 执行快捷键指令
 */
void exec_cmd_via_keys(int *keys)
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

/*
 * 设置按键为 Pressed 状态
 */
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

/*
 * 设置按键为 Released 状态
 */
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

/*
 * 分割按键字符串
 * 得到按键的整型值
 */
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
