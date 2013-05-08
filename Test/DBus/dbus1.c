#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

#define sys_says(args ...) { fprintf(stderr, args); }

void parse_dbus_err(DBusError *err);
void my_dbus_init(DBusConnection *conn);
void my_dbus_get(DBusConnection *conn, DBusError *err);
int call_dmusic_method(DBusConnection *conn, char *method, char *param);

int main()
{
	DBusConnection *conn = NULL;

	my_dbus_init(conn);
	call_dmusic_method(conn, "PlayPause", NULL);

	dbus_connection_close(conn);
}

void parse_dbus_err(DBusError *err)
{
	if ( dbus_error_is_set(err) ) {
		sys_says("Connection Error (%s)\n", err->message);
		dbus_error_free(err);
	}
}

void my_dbus_get(DBusConnection *conn, DBusError *err)
{
	/* connect to the dbus */
	conn = dbus_bus_get(DBUS_BUS_SESSION, err);
	parse_dbus_err(err);
	if ( conn == NULL ) {
		sys_says("connection failed...\n");
		exit(-1);
	}
	sys_says("connection OK...\n");
}

//void my_dbus_request()

void my_dbus_init(DBusConnection *conn)
{
	DBusError err;
	int ret;

	dbus_error_init(&err);	//initialize the errors

	//my_dbus_get(conn, &err);
	/* connect to the dbus */
	conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
	parse_dbus_err(&err);
	if ( conn == NULL ) {
		sys_says("connection failed...\n");
		exit(-1);
	}
	
	sys_says("request connection...\n");
	/* request a name on the bus */
	ret = dbus_bus_request_name(conn, "com.linuxdeepin.DMusic", 
			DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	parse_dbus_err(&err);
	if ( ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER ) {
		sys_says("connection close...\n");
		dbus_connection_close(conn);
		exit(-1);
	}

	//dbus_connection_close(conn);
}

int call_dmusic_method(DBusConnection *conn, char *method, char *param)
{
	DBusMessage *msg = NULL;
	DBusMessageIter msg_iter;
	DBusPendingCall *pending = NULL;
	dbus_bool_t stat;
	dbus_uint32_t level;

	if ( method == NULL ) {
		return -1;
	}

	sys_says("method call...\n");
	msg = dbus_message_new_method_call("com.linuxdeepin.DMusic", 
			"/com/linuxdeepin/DMusic", "com.linuxdeepin.DMusic", 
			method );
	if ( msg == NULL ) {
		sys_says("Message NULL\n");
		return -1;
	}

	if ( param != NULL ) {
	/* append msg_iter */
	dbus_message_iter_init_append(msg, &msg_iter);
	if ( !dbus_message_iter_append_basic(&msg_iter, 
				DBUS_TYPE_STRING, &param) ) {
		sys_says("Out Of Memory!\n");
		return -1;
	}
	}

	/*
	 * send message and get a handle for a reply
	 * -1 default timeout
	 */
	if ( !dbus_connection_send_with_reply(conn, msg, &pending, -1) ) {
		sys_says("Out Of Memory!\n");
		return -1;
	}
	dbus_connection_flush(conn);
	dbus_message_unref(msg);	// free message

	// block until we receive a reply
	dbus_pending_call_block(pending);

	// get the reply message
	msg = dbus_pending_call_steal_reply(pending);
	if ( msg == NULL ) {
		sys_says("Reply NULL\n");
		return -1;
	}

	dbus_pending_call_unref(pending);

	// read the parameters
	if ( !dbus_message_iter_init(msg, &msg_iter) ) {
		sys_says("Message has no msg_iter...\n");
	} else if ( dbus_message_iter_get_arg_type(&msg_iter) != 
			DBUS_TYPE_BOOLEAN ) {
		sys_says("Arguments is not boolean!...\n");
	} else {
		dbus_message_iter_get_basic(&msg_iter, &stat);
	}

	if ( !dbus_message_iter_next(&msg_iter) ) {
		sys_says("Message has no msg_iter...\n");
	} else if ( dbus_message_iter_get_arg_type(&msg_iter) != 
			DBUS_TYPE_UINT32 ) {
		sys_says("Arguments is not int!...\n");
	} else {
		dbus_message_iter_get_basic(&msg_iter, &level);
	}

	sys_says("Got Reply: %d, %d\n", stat, level);

	// free reply and close connection
	dbus_message_unref(msg);

	return 0;
}
