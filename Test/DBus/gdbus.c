#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib/giochannel.h>

#define SMSS_DEBUG

static void lose (const char *str, ...)
{
    va_list args;
    va_start (args, str);
    vfprintf (stderr, str, args);
    printf("\n");
    va_end (args);
    exit (1);
}

static void lose_gerror (const char *prefix, GError *error)
{
    if (error) {
        lose ("%s: %s", prefix, error->message);
    }
    else {
        lose ("%s", prefix);
    }
}

static void shell_help(void)
{
    printf( "/ts/tsend signal/n"
        "/tq/tQuit/n"
        );
}
/*
void emit_signal(GsmSms *obj)
{
    GHashTable *features = sms_create_features("ucs2", 3, 1);
    gsm_sms_emit_incoming_message(obj, "12345678901", "hello signal!", features);
    sms_release_features(features);
}
*/
#define STDIN_BUF_SIZE    1024
static gboolean channel_cb(GIOChannel *source, GIOCondition condition, gpointer data)
{
    int rc;
    char buf[STDIN_BUF_SIZE+1];
    //GsmSms *obj = (GsmSms *)data;

    if (condition != G_IO_IN) {
        return TRUE;
    }

    /* we've received something on stdin.    */
    printf("# ");
    rc = fscanf(stdin, "%s", buf);
    if (rc <= 0) {
        printf("NULL/n");
        return TRUE;
    }

    if (!strcmp(buf, "h")) {
        shell_help();
    } else if (!strcmp(buf, "?")) {
        shell_help();
    } else if (!strcmp(buf, "s")) {
        //emit_signal(obj);
    } else if (!strcmp(buf, "q")) {
        exit(0);
    } else {
        printf("Unknown command `%s'/n", buf);
    }
    return TRUE;
}

int main (int argc, char **argv)
{
    DBusGConnection *bus;
    DBusGProxy *bus_proxy;
    GError *error = NULL;
    char *obj;
    GMainLoop *mainloop;
    guint request_name_result;
    GIOChannel *chan;

#ifdef SMSS_DEBUG
    g_slice_set_config(G_SLICE_CONFIG_ALWAYS_MALLOC, TRUE);
#endif
    g_type_init ();

    //dbus_g_object_type_install_info (GSM_SMS_TYPE, &	dbus_glib_gsm_sms_object_info);

    mainloop = g_main_loop_new (NULL, FALSE);

    bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
    if (!bus)
        lose_gerror ("Couldn't connect to system bus", error);

    bus_proxy = dbus_g_proxy_new_for_name (bus, "org.freedesktop.DBus",
        "/", "org.freedesktop.DBus");

    if (!dbus_g_proxy_call (bus_proxy, "RequestName", &error,
        G_TYPE_STRING, "org.freesmartphone.ogsmd",
        G_TYPE_UINT, 0,
        G_TYPE_INVALID,
        G_TYPE_UINT, &request_name_result,
        G_TYPE_INVALID))
        lose_gerror ("Failed to acquire org.freesmartphone.ogsmd", error);

    obj = g_object_new (G_TYPE_STRING, NULL);
    dbus_g_connection_register_g_object (bus, "/org/freesmartphone/GSM/Device", G_OBJECT (obj));

    printf ("service is running/n");
    chan = g_io_channel_unix_new(0);
    g_io_add_watch(chan, G_IO_IN, channel_cb, obj);
    g_main_loop_run (mainloop);

    exit (0);
}
