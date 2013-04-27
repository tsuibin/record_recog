/*//////////////////////////////////////////
//    Simulated keyboard input program    //
//                by:0x001                //
//////////////////////////////////////////*/
#include "my_common.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/uinput.h>
 
#define die(str, args...) do {\
        perror(str);\
        exit(EXIT_FAILURE);\
    } while(0)
 
int fd;
struct uinput_user_dev uidev; // uInput device structure
struct input_event     ev;  // Input device structure
 
/* Setup the uinput device */
void setup_uinput_device()
{
    int i=0;
    // Open the input device
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open /dev/uinput\n");
 
    memset(&uidev, 0, sizeof(uidev)); // Intialize the uInput device to NULL
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-Openkk");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;
    // Setup the driver I/O channels
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    for (i=0; i < 256; i++) {
        ioctl(fd, UI_SET_KEYBIT, i);
    }
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
 
    /* Create input device into input sub-system */
    write(fd, &uidev, sizeof(uidev));
    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: create uinput device\n");
}
 
// Simulated keyboard input
void simulate_key(int keycode, int keyvalue)
{
    memset(&ev, 0, sizeof(struct input_event));
    gettimeofday(&ev.time, NULL);
    ev.type = EV_KEY;
    ev.code = keycode;
    ev.value = keyvalue;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
    {
            printf("simulate key errorn\n");
            return;
    }else{
        printf("simulate key %d, %d\n", keycode, keyvalue);
    }
 
}
 
void simulate_key_end()
{
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = 0;
    ev.value = 0;
    write(fd, &ev, sizeof(struct input_event));
}
 
//  check the key is control key
int check_valid_control_key(int key)
{
     //int control_key[] = {29,42,54,56,97,100,125,126};
     //int len = sizeof(control_key) / sizeof(control_key[0]);
     int i;
     int ret = 0;
     //for ( i = 0; i < len; i++)
     for ( i = 0; i < 127; i++)
     {
         //if (control_key[i] == key)
         if ( key == i )
         {
             ret = 1;
             break;
         }
     }
     return ret;
}
 
int send_keys(int *p)
{
	if ( p == NULL ) {
		fprintf(stderr, "send_keys args err...\n");
		return -1;
	}
 
    setup_uinput_device();
 
    int i;
    int *tmp = p;
 
    for (i = 0; i < KEY_LEN; i++) {
		if ( *tmp == 0 ) {
			break;
		}
		
        simulate_key(*tmp,1);
        if (check_valid_control_key(*tmp) == 0) {
            simulate_key(*tmp, 0);
        }
        tmp++;
    }

	tmp = p;
    for (i = 0; i < KEY_LEN; i++) {
		if ( *tmp == 0 ) {
			break;
		}
		
        if (check_valid_control_key(*tmp) == 1) {
            simulate_key(*tmp, 0);
        }
        tmp++;
    }
 
    simulate_key_end();
    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl\n");
    close(fd);
 
    return 0;
}

void strtok_num(char *exec_buf, int *nums)
{
	char *p = NULL;
	char *tmp = NULL;
	
	p = strtok(exec_buf, " ");
	*nums = strtol(p, tmp, 16);
	printf("%d\n", *nums);
	
	nums++;
	while ( (p = strtok(NULL, " ")) ) {
		*nums = strtol(p, tmp, 16);
		printf("%d\n", *nums);
		nums++;
	}
	*nums = 0;
}
