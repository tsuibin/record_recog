#include <stdio.h>
#include <string.h>
#include <pulse/pulseaudio.h>
#include <assert.h>

pa_stream *s = NULL;

void on_monitor_suspended_callback(pa_stream *p, void *userdata)
{
    if (pa_stream_is_suspended(p)) {
        printf("suspend callback\n");
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
    if (v > 1) v = 1;
    printf("\tread callback peek: %f\n", v);
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
                                   (pa_stream_flags_t) (PA_STREAM_DONT_MOVE |
                                   PA_STREAM_PEAK_DETECT |
                                   PA_STREAM_ADJUST_LATENCY));
    
    if (res < 0) {
        fprintf(stderr, "Failed to connect monitoring stream\n");
        return -3;
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    pa_mainloop *pa_ml = NULL;
    pa_mainloop_api *pa_mlapi = NULL;
    //pa_operation *pa_op = NULL;
    pa_context *pa_ctx = NULL;

    //int pa_ready = 0;
    //int state = 0;

    pa_ml = pa_mainloop_new();
    pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_ctx = pa_context_new(pa_mlapi, "deepin");

    pa_context_connect(pa_ctx, NULL, 0, NULL);
    //pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ready);

    printf("PA_SUBSCRIPTION_EVENT_SINK: %d\n", PA_SUBSCRIPTION_EVENT_SINK);
    printf("PA_SUBSCRIPTION_EVENT_SOURCE: %d\n", PA_SUBSCRIPTION_EVENT_SOURCE);
    printf("PA_SUBSCRIPTION_EVENT_SINK_INPUT: %d\n", PA_SUBSCRIPTION_EVENT_SINK_INPUT);
    printf("PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT: %d\n", PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT);
    printf("PA_SUBSCRIPTION_EVENT_MODULE: %d\n", PA_SUBSCRIPTION_EVENT_MODULE);
    printf("PA_SUBSCRIPTION_EVENT_CLIENT: %d\n", PA_SUBSCRIPTION_EVENT_CLIENT); 
    printf("PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE: %d\n", PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE);
    printf("PA_SUBSCRIPTION_EVENT_SERVER: %d\n", PA_SUBSCRIPTION_EVENT_SERVER);
    printf("PA_SUBSCRIPTION_EVENT_CARD: %d\n", PA_SUBSCRIPTION_EVENT_CARD);
    printf("PA_SUBSCRIPTION_EVENT_FACILITY_MASK: %d\n", PA_SUBSCRIPTION_EVENT_FACILITY_MASK);
    printf("PA_SUBSCRIPTION_EVENT_NEW: %d\n", PA_SUBSCRIPTION_EVENT_NEW);
    printf("PA_SUBSCRIPTION_EVENT_CHANGE: %d\n", PA_SUBSCRIPTION_EVENT_CHANGE);
    printf("PA_SUBSCRIPTION_EVENT_REMOVE: %d\n", PA_SUBSCRIPTION_EVENT_REMOVE);
    printf("PA_SUBSCRIPTION_EVENT_TYPE_MASK: %d\n", PA_SUBSCRIPTION_EVENT_TYPE_MASK);
    printf("------------------------------\n");
    printf("PA_SUBSCRIPTION_MASK_NULL: %d\n", PA_SUBSCRIPTION_MASK_NULL);
    printf("PA_SUBSCRIPTION_MASK_SINK: %d\n", PA_SUBSCRIPTION_MASK_SINK);
    printf("PA_SUBSCRIPTION_MASK_SOURCE: %d\n", PA_SUBSCRIPTION_MASK_SOURCE);
    printf("PA_SUBSCRIPTION_MASK_SINK_INPUT: %d\n", PA_SUBSCRIPTION_MASK_SINK_INPUT);
    printf("PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT: %d\n", PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT);
    printf("PA_SUBSCRIPTION_MASK_MODULE: %d\n", PA_SUBSCRIPTION_MASK_MODULE);
    printf("PA_SUBSCRIPTION_MASK_CLIENT: %d\n", PA_SUBSCRIPTION_MASK_CLIENT);
    printf("PA_SUBSCRIPTION_MASK_SAMPLE_CACHE: %d\n", PA_SUBSCRIPTION_MASK_SAMPLE_CACHE);
    printf("PA_SUBSCRIPTION_MASK_SERVER: %d\n", PA_SUBSCRIPTION_MASK_SERVER);
    printf("PA_SUBSCRIPTION_MASK_CARD: %d\n", PA_SUBSCRIPTION_MASK_CARD);
    printf("PA_SUBSCRIPTION_MASK_ALL: %d\n", PA_SUBSCRIPTION_MASK_ALL);
    printf("------------------------------\n");

    for (;;) {
        if (m_pa_stream_connect(pa_ctx) < 0) {
			return -1;
		}
    }

    return 0;
}
