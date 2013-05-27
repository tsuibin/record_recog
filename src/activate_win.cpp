#include "my_xlib.h"

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#undef  WNCK_I_KNOW_THIS_IS_UNSTABLE

extern Window match_wid;
extern struct process_info cur_process;

Window get_active_window()
{
	Window active_xid = 0;
	WnckScreen *screen = NULL;
	WnckWindow *active_win = NULL;
	GList *window_l = NULL;

	gdk_init( NULL, NULL );
	screen = wnck_screen_get_default();
	wnck_screen_force_update(screen);
	active_win = wnck_screen_get_active_window(screen);

	window_l = wnck_screen_get_windows(screen);
	for ( ; window_l != NULL; window_l = window_l->next ) {
		WnckWindow *window = WNCK_WINDOW(window_l->data);
		/*if ( wnck_window_get_window_type(window) != WNCK_WINDOW_NORMAL ) {
			continue;
		}*/
		if ( window == active_win ) {
			active_xid = wnck_window_get_xid(window);
			sys_says("active xid : 0x%x\n", active_xid);
			break;
		}
	}
	
	return active_xid;
}

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
			sys_says("match_wid xid : 0x%x\n", match_wid);
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
	if ( memcmp(process_name, "wpp", 3) == 0 ) {
		sprintf( buf, "ps aux | grep wps-office | sed -n 1p | awk '{print $2}' \
				> /tmp/pid" );
	} else {
		sprintf( buf, "ps aux | grep %s | sed -n 1p | awk '{print $2}' \
				> /tmp/pid", process_name );
	}
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
