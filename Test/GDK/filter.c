#include "my_xlib.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#undef  WNCK_I_KNOW_THIS_IS_UNSTABLE

void catch_alrm(int signo);
void *filter_thread(void *arg);

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
			break;
		}
	}
	
	return active_xid;
}

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
	int ret;
	pthread_t filter_thrd;
	
	gtk_init(&argc, &argv);
	signal(SIGALRM, catch_alrm);
	
	ret = pthread_create(&filter_thrd, 0, filter_thread, 0);
	if ( ret != 0 ) {
		g_print("create thread failed...\n");
		return -1;
	}
	
	pthread_exit(NULL);
	
	return 0;
}

void *filter_thread(void *arg)
{
	GdkWindow *window = NULL;
	GdkScreen *screen = NULL;
	
	screen = gdk_screen_get_default();
	window = gdk_screen_get_root_window(screen);
	//gdk_window_set_events (window, GDK_FOCUS_CHANGE);
	gdk_window_set_events (window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter (window, child_handle_event, NULL);

	gtk_main();
	
	return 0;
}

void catch_alrm(int signo)
{
	char window_class[BUF_LEN];
	gchar *tmp;
	
	tmp = get_window_class(get_active_window());
	memset(window_class, 0, BUF_LEN);
	memcpy(window_class, tmp, strlen(tmp));
	g_free(tmp);
	sys_says("window class name : %s\n", window_class);
}

#define p_verbose(...) { fprintf(stderr, __VA_ARGS__); }
#define MAX_PROPERTY_VALUE_LEN 4096

static gchar *get_property (Display *disp, Window win, 
        Atom xa_prop_type, gchar *prop_name, unsigned long *size);
gchar *get_window_class (Window win);

gchar *get_window_class (Window win) {
    gchar *class_utf8;
    gchar *wm_class;
    unsigned long size;
    Display *disp;
    
    if ( win == 0 ) {
		return NULL;
	}
    /* necessary to make g_get_charset() and g_locale_*() work */
    setlocale(LC_ALL, "");
    
    if (! (disp = XOpenDisplay(NULL))) {
        fputs("Cannot open display.\n", stderr);
        return NULL;
    }

    wm_class = get_property(disp, win, XA_STRING, "WM_CLASS", &size);
    XCloseDisplay(disp);
    if (wm_class) {
        gchar *p_0 = strchr(wm_class, '\0');
        if (wm_class + size - 1 > p_0) {
            *(p_0) = '.';
        }
        class_utf8 = g_locale_to_utf8(wm_class, -1, NULL, NULL, NULL);
    }
    else {
        class_utf8 = NULL;
    }

    g_free(wm_class);
    
    return class_utf8;
}

static gchar *get_property (Display *disp, Window win, 
        Atom xa_prop_type, gchar *prop_name, unsigned long *size) {
    Atom xa_prop_name;
    Atom xa_ret_type;
    int ret_format;
    unsigned long ret_nitems;
    unsigned long ret_bytes_after;
    unsigned long tmp_size;
    unsigned char *ret_prop;
    gchar *ret;
    
    xa_prop_name = XInternAtom(disp, prop_name, False);
    
    /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
     *
     * long_length = Specifies the length in 32-bit multiples of the
     *               data to be retrieved.
     */
    if (XGetWindowProperty(disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
            xa_prop_type, &xa_ret_type, &ret_format,     
            &ret_nitems, &ret_bytes_after, &ret_prop) != Success) {
        p_verbose("Cannot get %s property.\n", prop_name);
        return NULL;
    }
  
    if (xa_ret_type != xa_prop_type) {
        //p_verbose("Invalid type of %s property.\n", prop_name);
        XFree(ret_prop);
        return NULL;
    }

    /* null terminate the result to make string handling easier */
    tmp_size = (ret_format / 8) * ret_nitems;
    ret = (gchar*)g_malloc(tmp_size + 1);
    memcpy(ret, ret_prop, tmp_size);
    ret[tmp_size] = '\0';

    if (size) {
        *size = tmp_size;
    }
    
    XFree(ret_prop);
    return ret;
}
