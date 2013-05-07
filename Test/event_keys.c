#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define READ_LINE 80
#define sys_says(arg ...)	{ fprintf(stderr, arg); }

void get_dev_id(char *id_name);

int main()
{
	int fd;
	int flag;
	char name[READ_LINE];
	char kbd_path[READ_LINE];
	struct input_event myinput;

	memset(name, 0, READ_LINE);
	get_dev_id(name);
	if ( name[0] == '\0' ) {
		sys_says("get_dev_id failed...\n");
		exit(-1);
	}

	sprintf( kbd_path, "/dev/input/%s", name );
	sys_says("kbd_path : %s\n", kbd_path);
	fd = open(kbd_path, O_RDONLY);
	if ( fd == -1 ) {
		sys_says("open keyboard err : %s\n", strerror(errno) );
		return -1;
	}

	while (1) {
		memset(&myinput, 0, sizeof(myinput));
		flag = read(fd, &myinput, sizeof(myinput));
		if ( flag == -1 ) {
			sys_says("read keyboard err : %s\n", strerror(errno));
		} else if (flag == sizeof(myinput)) {
			//sys_says("=========================================\n");
			//sys_says("type:%d\n", myinput.type);
			//sys_says("code:%d\n", myinput.code);
			//sys_says("value:%d\n", myinput.value);
			//sys_says("key %d %s\n", myinput.code, 
			//		(myinput.value) ? "Pressed" : "Released" );
			//sys_says("=========================================\n");
			if (myinput.code == KEY_ESC) {
				break;
			}

			if ( (myinput.code == KEY_LEFTCTRL || 
						myinput.code == KEY_RIGHTCTRL) && 
					(myinput.value == 1) ) {
				sys_says("KEY_CTRL Pressed\n");
			}
			if ( (myinput.code == KEY_LEFTCTRL || 
						myinput.code == KEY_RIGHTCTRL) && 
					(myinput.value == 0) ) {
				sys_says("KEY_CTRL Released\n");
			}
		}
	}
	close(fd);

	return 0;
}

void get_dev_id(char *id_name)
{
	FILE *fp = NULL;
	char buf[] = "ls -l /dev/input/by-path/ | awk \'{print $9, $NF}\' | \
			  grep 0-event-kbd | awk \'{print $NF}\' | cut -c4- > /tmp/keys";

	system(buf);
	if ( (fp = fopen("/tmp/keys", "r")) == NULL ) {
		sys_says("fopen err : %s\n", strerror(errno));
		exit(-1);
	}

	fscanf(fp, "%s", id_name);
	fclose(fp);

	sys_says("id_name : %s\n", id_name);
}
