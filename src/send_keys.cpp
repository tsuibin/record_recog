/*
 * 激活窗口 && 发送按键事件
 */

#include "send_keys.h"

//extern int keys_len;
extern struct process_info cur_process;

static int exist_flag = 0;
Window match_wid = (Window)0;

void activate_window(unsigned long pid)
{
	//Display *disp = NULL;
	char buf[80];

	//setlocale(LC_ALL, "");

	/*if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");*/
	
	//get_wid_via_pid(disp, pid);
	//get_wid_via_pid(cur_process.name);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		//XCloseDisplay(disp);
		return;
	}
	sys_err("match_wid : %lu\n", match_wid);
	
	/* 设置指定窗口为焦点 */
	sprintf(buf, "wmctrl -i -a %lu", match_wid);
	system(buf);
}

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
	//get_wid_via_pid(cur_process.name);
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
	//XMapRaised(disp, match_wid);
	
	XCloseDisplay(disp);
	match_wid = (Window)0;
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
	//get_wid_via_pid(cur_process.name);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return;
	}

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}
	//XMapRaised(disp, match_wid);

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

void get_wid_via_pid( Display *display, unsigned long pid )
//void get_wid_via_pid( char *name )
{
	Atom atomPID;
	
	// Get the PID property atom.
    atomPID = XInternAtom(display, "_NET_WM_PID", False);
    if(atomPID == None)
    {
    	sys_err( "No such atom\n" );
    	return;
    }
    
    sys_err( "atom pid : %lu\n",  atomPID);
    //search_window(display, XDefaultRootWindow(display), pid);
    search_window(display, XDefaultRootWindow(display), atomPID, pid);
    XDeleteProperty(display, XDefaultRootWindow(display), atomPID);
    sys_err( "delete atom\n" );
    
    /*
    //xdotool
    unsigned long int wid = 0;
    FILE *fp = NULL;
    char buf[80];
    
    sprintf(buf, "xdotool search --name %s | tail -1 > xdot", name);
    system(buf);
    
    fp = fopen("xdot", "r");
    if ( fp == NULL ) {
		fprintf(stderr, "get wid failed...\n");
		return ;
	}
	fscanf(fp, "%lu", &wid);
	fclose(fp);
	fp = NULL;
	sys_err("name : %s\n", name);
	sys_err("wid : %lu\n", wid);
	match_wid = (Window)wid;
    */
    return;
}

//void search_window(Display *display, Window w, unsigned long pid)
void search_window(Display *display, Window w, Atom atomPID, unsigned long pid)
{
	// Get the PID for the current Window.
    Atom           type;
    int            format;
    unsigned long  nItems;
    unsigned long  bytesAfter;
    unsigned char *propPID = 0;
    
    /*Atom atomPID;
	
	// Get the PID property atom.
    atomPID = XInternAtom(display, "_NET_WM_PID", False);
    if(atomPID == None)
    {
    	sys_err( "No such atom\n" );
    	return;
    }*/
    
    sys_err("XGetWindowProperty...\n");
    if(Success == XGetWindowProperty(display, w, atomPID, 0, 
		MAX_PROPERTY_VALUE_LEN / 4, False, XA_CARDINAL, &type, &format, 
		&nItems, &bytesAfter, &propPID))
    {
    	if(propPID != 0)
    	{
			//sys_err( "propPID : %lu\n", *(unsigned long *)propPID );
    		// If the PID matches, add this window to the result set.
    		if(pid == *((unsigned long *)propPID)) {
    			sys_err( "match pid : %lu\n", *(unsigned long *)propPID );
    			exist_flag = 1;
    			match_wid = w;
    			XFree(propPID);
    			propPID = NULL;
    			return;
			}

    		XFree(propPID);
    		propPID = NULL;
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
    		//search_window(display, wChild[i], pid);
    		search_window(display, wChild[i], atomPID, pid);
    		if (exist_flag) {
				break;
			}
		}
    }
}
/*
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
    			sys_err( "propPID : %lu\n", *(unsigned long *)propPID );
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
