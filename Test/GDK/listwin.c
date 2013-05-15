#include <libwnck/libwnck.h>

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
		g_print("window id : 0x%x\tpid : %d\n\n", wnck_window_get_xid(window), 
				wnck_window_get_pid(window) );
		if ( 4921 == wnck_window_get_pid(window) ) {
			wnck_window_activate(window, 1);
			break;
		}
	}

	return 0;
}
