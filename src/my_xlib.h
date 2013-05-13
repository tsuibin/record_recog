#ifndef __MY_XLIB_H__
#define __MY_XLIB_H__

#include "my_common.h"
#include <locale.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <X11/extensions/XTest.h>

/* 执行指令 */
void exec_command(int *keys);
void strtok_num(char *exec_buf, int *nums);

void send_key_press( Display *disp, Window win, int keycode );
void send_key_release( Display *disp, Window win, int keycode );

/* 激活窗口 */
int set_focus();

void get_wid(char *name);
int index_str(char *name, char *src);
void sys_wm_wid();

/* libwnck */
int activate_win();
int get_pid_via_name( char *process_name );

#endif
