#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/extensions/XTest.h>

#define sys_err(args ...) { fprintf(stderr, args); }

void send_key_press( Display *disp, Window win, int keycode );
void send_key_release( Display *disp, Window win, int keycode );

int main(int argc, char **argv)
{
	Display *disp = NULL;
	Window cur_window = (Window)0x03600006;	//terminal
	//Window cur_window = (Window)0x0380004b;	//qt-creator

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return -1;
	}
	sys_err("create display ok!\n");

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, cur_window, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}

	int i = 5;
	while (i) {
		i--;
		getchar();
	//sys_err("set keys pressed...\n");
	//send_key_press( disp, cur_window, XK_F1 );	// F1
	send_key_press( disp, cur_window, XK_Shift_L );	// shift
	send_key_press( disp, cur_window, XK_Control_L );	//ctrl
	send_key_press( disp, cur_window, XK_T );	// t
	//send_key_press( disp, cur_window, 49 );	// n

	usleep(500);

	sys_err("set keys release...\n");
	//send_key_release( disp, cur_window, XK_F1 );	// F1
	send_key_release( disp, cur_window, XK_Shift_L );
	send_key_release( disp, cur_window, XK_Control_L );
	send_key_release( disp, cur_window, XK_T );
	//send_key_release( disp, cur_window, 49 );
}

	XCloseDisplay(disp);

	return 0;
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
