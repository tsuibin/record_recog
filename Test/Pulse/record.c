/***
  This file is part of PulseAudio.

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#include "record.h"

#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/volume.h>

#define BUFSIZE 1024
int first_db = 0;

/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(FILE *fp, const void*data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = fwrite(data, 1, size, fp)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}

int quit_flag = 0;
double db;

void *func_thread(void *arg)
{
	while ( 1 ) {
		db = get_db();
		sleep(1);
		if ( !first_db ) {
			first_db = 1;
			continue;
		}
		if ( db < 0.500001 ) {
			quit_flag = 1;
			break;
		}
	}
	
	pthread_exit(NULL);
}

int main(int argc, char*argv[]) {
    /* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 16000,
        .channels = 1
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;
    FILE *fp = NULL;
    pthread_t db_thrd;

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        goto finish;
    }
    
    if ( (fp = fopen("tmp.wav", "w")) == NULL ) {
		printf( "fopen file failed...\n" );
		return -1;
	}

	if ( (pthread_create(&db_thrd, 0, func_thread, 0)) != 0 ) {
		printf( "create thread failed...\n" );
		return -1;
	}
	
    for (;;) {
        uint8_t buf[BUFSIZE];

		if ( quit_flag ) {
			break;
		}
		
        /* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            goto finish;
        }

        /* And write it to STDOUT */
        if (loop_write(fp, buf, sizeof(buf)) != sizeof(buf)) {
            fprintf(stderr, __FILE__": write() failed: %s\n", strerror(errno));
            goto finish;
        }
    }
    fclose(fp);
    pthread_join(db_thrd, NULL);

    ret = 0;

finish:

    if (s)
        pa_simple_free(s);

    return ret;
}
