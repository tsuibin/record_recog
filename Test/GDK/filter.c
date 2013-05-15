#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <X11/Xlib.h>

void catch_alrm(int signo);

GdkFilterReturn child_handle_event( void *event1, GdkEvent *event2,
							gpointer userdata )
{
	XEvent *xev = (XEvent*)event1;
	
	if ( xev->type == PropertyNotify ) {
	//if ( xev->type == FocusIn ) {
		g_print("child_handle_event\n");
		kill(getpid(), SIGALRM);
	}
	
	return GDK_FILTER_CONTINUE;
}


int main(int argc, char* argv[])
{
	GdkWindow *window = NULL;
	GdkScreen *screen = NULL;
	
	gtk_init(&argc, &argv);
	signal(SIGALRM, catch_alrm);
	
	screen = gdk_screen_get_default();
	window = gdk_screen_get_root_window(screen);
	//gdk_window_set_events (window, GDK_FOCUS_CHANGE);
	gdk_window_set_events (window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter (window, child_handle_event, NULL);

	gtk_main();
	
	return 0;
}
