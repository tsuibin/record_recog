#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <shadow.h>
#include <unistd.h>
#include <errno.h>
#include <crypt.h>

#define SPEECH_DBUS_NAME	"com.deepin.speech"
#define SPEECH_DBUS_OBJ		"/com/deepin/speech"
#define SPEECH_DBUS_IFACE	"com.deepin.speech"

//internal functions
static gboolean _retry_registration (gpointer user_data);
static void _on_bus_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data);
static void _on_name_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data);
static void _on_name_lost (GDBusConnection * connection, const gchar * name, gpointer user_data);
static gboolean do_exit(gpointer user_data);

GMainLoop *loop = NULL;

static guint lock_service_owner_id;
static guint lock_service_reg_id;        //used for unregister an object path
static guint retry_reg_timeout_id;   //timer used for retrying dbus name registration.
static GDBusConnection* lock_connection;

static GDBusNodeInfo *      node_info = NULL;
static GDBusInterfaceInfo *interface_info = NULL;
/*static GDBusInterfaceVTable interface_table = {
    method_call:   _bus_method_call,
    get_property:   NULL, // No properties 
    set_property:   NULL  // No properties 
};*/

const char *_lock_dbus_iface_xml = 
"<?xml version=\"1.0\"?>\n"
"<node>\n"
"	<interface name=\""SPEECH_DBUS_IFACE"\">\n"
"		<method name=\"UnlockCheck\">\n"
"			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
"			</arg>\n"
"			<arg name=\"password\" type=\"s\" direction=\"in\">\n"
"			</arg>\n"
"			<arg name=\"succeed\" type=\"b\" direction=\"out\">\n"
"			</arg>\n"
"		</method>\n"
"	</interface>\n"
"</node>\n"
;

int main(int argc, char **argv)
{
	//GDBusInterface *inface = NULL;
	g_type_init();

	loop = g_main_loop_new(NULL, FALSE);

	/*inface = g_dbus_object_get_interface(obj, SPEECH_DBUS_IFACE);
	if ( inface == NULL ) {
		g_debug ("Unable find the interface...\n");
		return -1;
	}*/

	GError *error = NULL;
	node_info = g_dbus_node_info_new_for_xml (_lock_dbus_iface_xml, &error);
	if ( error != NULL ) {
		g_critical("Unable to parse interface xml: %s\n", error->message);
		g_error_free(error);
	}

	interface_info = g_dbus_node_info_lookup_interface(node_info, 
			SPEECH_DBUS_IFACE );
	if ( interface_info == NULL ) {
		g_critical("Unable to find interface '"SPEECH_DBUS_IFACE"'");
	}

	lock_service_owner_id = 0;
	lock_service_reg_id = 0;
	retry_reg_timeout_id = 0;
	_retry_registration(NULL);

	g_timeout_add_seconds( 60, do_exit, NULL );
	g_main_loop_run(loop);

	return 0;
}

static gboolean
do_exit(gpointer user_data)
{
	g_main_loop_quit(loop);

	return FALSE;
}

static gboolean
_retry_registration ( gpointer user_data )
{
	lock_service_owner_id = g_bus_own_name( G_BUS_TYPE_SYSTEM, 
				SPEECH_DBUS_NAME, 
				G_BUS_NAME_WATCHER_FLAGS_NONE, 
				lock_service_reg_id ? NULL : _on_bus_acquired,
				_on_name_acquired,
				_on_name_lost, 
				NULL, 
				NULL);
	return 0;
}

static void
_on_bus_acquired ( GDBusConnection *connection, 
		const gchar *name, 
		gpointer user_data )
{
	g_debug(" on bus acquired ...\n");

	lock_connection = connection;

	//register object
	GError *error = NULL;
	lock_service_reg_id = g_dbus_connection_register_object( connection, 
			SPEECH_DBUS_OBJ, 
			interface_info, 
			//&interface_table, 
			NULL,
			user_data, 
			NULL, 
			&error );

	if ( error != NULL ) {
		g_critical ( "Unable to register object to the dbus: %s\n", 
				error->message );
		g_error_free(error);
		g_bus_unown_name(lock_service_owner_id);
		lock_service_owner_id = 0;
		retry_reg_timeout_id = g_timeout_add_seconds(1, 
				_retry_registration, NULL );
		return;
	}

	return;
}

static void
_on_name_acquired ( GDBusConnection *connection, 
		const gchar* name, 
		gpointer user_data )
{
	g_debug ( "Dbus name acquired ... \n" );
}

static void
_on_name_lost ( GDBusConnection *connection, 
		const gchar *name, 
		gpointer user_data )
{
	if ( connection == NULL ) {
		g_critical ( "Unable to get a connection to DBus...\n" );
	} else {
		g_critical ( "Unable to claim the name %s\n", SPEECH_DBUS_NAME );
	}

	lock_service_owner_id = 0;
}
