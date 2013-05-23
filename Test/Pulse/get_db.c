/* Copyright (C) 2011 ~ 2012 Deepin, Inc.
 *               2011 ~ 2012 Long Changjin
 * 
 * Author:     Long Changjin <admin@longchangjin.cn>
 * Maintainer: Long Changjin <admin@longchangjin.cn>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "record.h"
#include <pulse/pulseaudio.h>
#include <pulse/glib-mainloop.h>
#include <assert.h>

void pa_state_cb(pa_context *c, void *userdata);
void on_monitor_read_callback(pa_stream *p, size_t length, void *userdata);
void on_monitor_suspended_callback(pa_stream *p, void *userdata);
static int m_pa_stream_connect(pa_context *pa_ctx);

double ret = 0;
pa_stream *s = NULL;
GMainLoop *mainloop = NULL;

double get_db()
{
    pa_glib_mainloop *pa_ml = NULL;
    pa_mainloop_api *pa_mlapi = NULL;
    //pa_operation *pa_op = NULL;
    pa_context *pa_ctx = NULL;

    GMainContext *mctx = NULL;

    pa_ml = pa_glib_mainloop_new(g_main_context_default());
    pa_mlapi = pa_glib_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new(pa_mlapi, "deepin");

    pa_context_connect(pa_ctx, NULL, 0, NULL);
    pa_context_set_state_callback(pa_ctx, pa_state_cb, NULL);

    mctx = g_main_context_default();
    mainloop = g_main_loop_new(mctx, FALSE);
    g_main_loop_run(mainloop);
    
    return ret;
}

void pa_state_cb(pa_context *c, void *userdata)
{
        pa_context_state_t state;

        state = pa_context_get_state(c);
        switch  (state) {
                // There are just here for reference
                case PA_CONTEXT_UNCONNECTED:
                case PA_CONTEXT_CONNECTING:
                case PA_CONTEXT_AUTHORIZING:
                case PA_CONTEXT_SETTING_NAME:
                        break;
                case PA_CONTEXT_FAILED:
                case PA_CONTEXT_TERMINATED:
                        exit(-1);
                        break;
                case PA_CONTEXT_READY:
                    if (m_pa_stream_connect(c) < 0) {
                        exit(-1);
                    }
                        break;
        }
}

void on_monitor_read_callback(pa_stream *p, size_t length, void *userdata)
{
    const void *data;
    double v;

    printf("read callback length: %d\n", length);
    printf("\tget_device_index: %d\n", pa_stream_get_device_index(p));
    printf("\tget_device_name: %s\n", pa_stream_get_device_name(p));
    printf("\tget_monitor_stream: %d\n", pa_stream_get_monitor_stream(p));
    if (pa_stream_peek(p, &data, &length) < 0) {
        printf("Failed to read data from stream\n");
        return;
    }
    
    assert(length > 0);
    assert(length % sizeof(float) == 0);

    v = ((const float*) data)[length / sizeof(float) -1];

    pa_stream_drop(p);

    if (v < 0) v = 0;
    //if (v > 1) v = 1;
    printf("\tread callback peek: %f\n", v);
    ret = v;
    g_main_loop_quit(mainloop);
}

void on_monitor_suspended_callback(pa_stream *p, void *userdata)
{
    if (pa_stream_is_suspended(p)) {
        printf("suspend callback\n");
    }
}

static int m_pa_stream_connect(pa_context *pa_ctx)
{
    if (pa_context_get_server_protocol_version (pa_ctx) < 13) {
            return -1;
    }
    printf("server version: %d\n", pa_context_get_server_protocol_version(pa_ctx));
    if (s) {
        pa_stream_disconnect(s);
        pa_stream_unref(s);
    }

    pa_proplist  *proplist;

    pa_buffer_attr attr;
    pa_sample_spec ss;

    int res;
    //char dev_name[40];

    // pa_sample_spec
    ss.channels = 1;
    ss.format = PA_SAMPLE_FLOAT32;
    ss.rate = 25;

    // pa_buffer_attr
    memset(&attr, 0, sizeof(attr));
    attr.fragsize = sizeof(float);
    attr.maxlength = (uint32_t) -1;

    // pa_proplist
    proplist = pa_proplist_new ();
    pa_proplist_sets (proplist, PA_PROP_APPLICATION_ID, "Deepin Sound Settings");

    // create new stream
    if (!(s = pa_stream_new_with_proplist(pa_ctx, "Deepin Sound Settings", &ss, NULL, proplist))) {
        fprintf(stderr, "pa_stream_new error\n");
        return -2;
    }
    pa_proplist_free(proplist);

    pa_stream_set_read_callback(s, on_monitor_read_callback, NULL);
    pa_stream_set_suspended_callback(s, on_monitor_suspended_callback, NULL);

    res = pa_stream_connect_record(s, NULL, &attr, 
                                   (pa_stream_flags_t) (PA_STREAM_DONT_MOVE
                                                        |PA_STREAM_PEAK_DETECT
                                                        |PA_STREAM_ADJUST_LATENCY));
    
    if (res < 0) {
        fprintf(stderr, "Failed to connect monitoring stream\n");
        return -3;
    }
    return 0;
}
