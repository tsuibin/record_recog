/*
 * 激活窗口 && 发送按键事件
 */

#include "send_keys.h"

//extern int keys_len;

static int exist_flag = 0;
Window match_wid = (Window)0;

void set_focus(unsigned long pid)
{
	Display *disp = NULL;

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");
	
	get_wid_via_pid(disp, pid);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return;
	}

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}
	
	XCloseDisplay(disp);
}

void exec_command(int *keys, unsigned long pid)
{
	int *tmp_key = keys;
	Display *disp = NULL;
	//Window cur_window = (Window)0x03600006;	//terminal
	//Window cur_window = (Window)0x0380004b;	//qt-creator

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");
	
	get_wid_via_pid(disp, pid);
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
	//send_key_press( disp, match_wid, XK_F1 );	// F1
	//int i = 0;
	//for ( ; i < keys_len; i++ ) {
	for (; *tmp_key != 0; tmp_key++ ) {
		send_key_press( disp, match_wid, *tmp_key  );
		//tmp_key++;
	}

	usleep(500);

	sys_err("set keys release...\n");
	//send_key_release( disp, match_wid, XK_F1 );	// F1
	tmp_key = keys;
	//for ( i = 0; i < keys_len; i++ ) {
	for (; *tmp_key != 0; tmp_key++ ) {
		send_key_release( disp, match_wid, *tmp_key );
		//tmp_key++;
	}

	XCloseDisplay(disp);
}

void get_wid_via_pid( Display *display, unsigned long pid )
{
	Atom atomPID;
	
	// Get the PID property atom.
    atomPID = XInternAtom(display, "_NET_WM_PID", True);
    if(atomPID == None)
    {
    	sys_err( "No such atom" );
    	return;
    }
    
    search_window(display, XDefaultRootWindow(display), atomPID, pid);
}

void search_window(Display *display, Window w, Atom atomPID, unsigned long pid)
{
	// Get the PID for the current Window.
    Atom           type;
    int            format;
    unsigned long  nItems;
    unsigned long  bytesAfter;
    unsigned char *propPID = 0;
    
    if(Success == XGetWindowProperty(display, w, atomPID, 0, 1, False, 
			XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID))
    {
    	if(propPID != 0)
    	{
    		// If the PID matches, add this window to the result set.
    		if(pid == *((unsigned long *)propPID)) {
    			sys_err( "propPID : %lud\n", *(unsigned long *)propPID );
    			exist_flag = 1;
    			match_wid = w;
    			XFree(propPID);
    			return;
			}

    		XFree(propPID);
    	}
    }
    
    // Recurse into child windows.
    Window    wRoot;
    Window    wParent;
    Window   *wChild;
    unsigned  nChildren;
    
    if(0 != XQueryTree(display, w, &wRoot, &wParent, &wChild, &nChildren))
    {
    	for(unsigned i = 0; i < nChildren; i++) {
    		search_window(display, wChild[i], atomPID, pid);
    		if (exist_flag) {
				break;
			}
		}
    }
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
