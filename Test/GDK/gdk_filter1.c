#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <X11/Xlib.h>

GdkFilterReturn child_handle_event( void *event1, GdkEvent *event2,
gpointer userdata )
{
	XEvent *xev = (XEvent*)event1;
	//GdkEvent *xev = (GdkEvent*)event1;
	
	if ( xev->type == FocusIn || xev->type == PropertyNotify ) {
	//if ( xev->type == GDK_FOCUS_CHANGE ) {
		g_print("child_handle_event\n");
	}
	
	return GDK_FILTER_CONTINUE;
}


int main(int argc, char* argv[])
{

	//GdkWindow *parent, *child;
	//GdkWindowAttr attr;

	gtk_init(&argc, &argv);
	/*
	attr.event_mask = GDK_VISIBILITY_NOTIFY_MASK | GDK_EXPOSURE_MASK |
	GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK |
GDK_BUTTON_PRESS_MASK |
	GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
	GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK ;
	attr.x = 100;
	attr.y = 100;
	attr.width = 800;
	attr.height = 600;
	attr.window_type = GDK_WINDOW_TOPLEVEL;
	attr.wclass = GDK_INPUT_OUTPUT;
	parent = gdk_window_new (NULL, &attr, GDK_WA_X | GDK_WA_Y);

	attr.event_mask = GDK_VISIBILITY_NOTIFY_MASK | GDK_EXPOSURE_MASK |
	GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK |
GDK_BUTTON_PRESS_MASK |
	GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
	GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK;
	attr.x = 200;
	attr.y = 200;
	attr.width = 400;
	attr.height = 300;
	attr.window_type = GDK_WINDOW_CHILD;
	attr.wclass = GDK_INPUT_OUTPUT;
	child = gdk_window_new (parent, &attr, GDK_WA_X | GDK_WA_Y);
	*/
	
	GdkWindow *window = NULL;
	GdkScreen *screen = NULL;
	
	screen = gdk_screen_get_default();
	window = gdk_screen_get_root_window(screen);
	gdk_window_set_events (window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter (window, child_handle_event, NULL);

	//gdk_window_show (child);
	//gdk_window_show (parent);

	gtk_main();
	
	return 0;
}
