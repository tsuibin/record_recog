#ifndef __SEND_KEYS_H__
#define __SEND_KEYS_H__

#include "my_common.h"
#include <locale.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/extensions/XTest.h>

#define MAX_PROPERTY_VALUE_LEN 4096

/* 激活窗口 */
void activate_window(unsigned long pid);
void set_focus(unsigned long pid);
/* 执行指令 */
void exec_command(int *keys, unsigned long pid);
void strtok_num(char *exec_buf, int *nums);

void send_key_press( Display *disp, Window win, int keycode );
void send_key_release( Display *disp, Window win, int keycode );

//void get_wid_via_pid( char *name );
void get_wid_via_pid( Display *display, unsigned long pid );
//void search_window(Display *display, Window w, unsigned long pid);
void search_window(Display *display, Window w, Atom atomPID, unsigned long pid);

void get_wid(char *name);
void sys_wm_wid();
void sys_xdot_wid(char *name);

#endif
