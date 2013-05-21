#include <unistd.h>
#include <glibtop.h>
#include <glibtop/procstate.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#undef WNCK_I_KNOW_THIS_IS_UNSTABLE

void get_name_via_pid(pid_t pid)
{
	glibtop_proc_state buf;

	memset(&buf, 0, sizeof(buf));
	glibtop_get_proc_state( &buf, pid );

	g_print("cmd : %s\n\n", buf.cmd );
}

int main(int argc, char **argv)
{
	WnckScreen *screen = NULL;
	WnckWindow *active_win = NULL;
	GList *window_l = NULL;

	gdk_init( &argc, &argv );
	screen = wnck_screen_get_default();
	wnck_screen_force_update(screen);
	active_win = wnck_screen_get_active_window(screen);

	window_l = wnck_screen_get_windows(screen);
	for ( ; window_l != NULL; window_l = window_l->next ) {
		WnckWindow *window = WNCK_WINDOW(window_l->data);
		if ( wnck_window_get_window_type(window) != WNCK_WINDOW_NORMAL ) {
			continue;
		}
		g_print( "%s%s\n", wnck_window_get_name(window), 
				window == active_win ? " (active)" : "" );
		g_print("window id : 0x%x\tpid : %d\n", wnck_window_get_xid(window), 
				wnck_window_get_pid(window) );
				
		get_name_via_pid( wnck_window_get_pid(window) );
	}

	return 0;
}
