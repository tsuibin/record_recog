#include <gtk/gtk.h>

int main(int argc, char **argv)
{
	GtkWindow *window = NULL;
	GdkEvent *event = NULL;

	event = gdk_event_new(GDK_KEY_PRESS);
	event->key.window = window->window;
	g_object_ref(event->key.window);

	event->keyval = GDK_F;
	event->state = GDK_CONTROL_MASK;

	gtk_main_do_event(event);
	gdk_event_free(event);

	return 0;
}
