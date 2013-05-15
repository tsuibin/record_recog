#include "my_init.h"
#include "my_qtts.h"

int main()
{
	int fd;
	int flag;
	char tmp[NAME_LEN];
	char kbd_path[NAME_LEN];
	struct input_event myinput;
	
	//double rtt;
	//struct timeval tvpressed;
	//struct timeval tvreleased;

	get_dev_path(kbd_path);
	sys_says("kbd_path : %s\n", kbd_path);
	
	sprintf(tmp, "gksudo chmod 777 %s", kbd_path);
	sys_says("gksudo : %s\n", tmp);
	system(tmp);
	
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
			if (myinput.code == KEY_ESC) {
				break;
			}

			if ( (myinput.code == KEY_LEFTALT) || 
					(myinput.code == KEY_RIGHTALT) ) {
				if ( myinput.value == 1 ) {	//按下
					read_xsel();
				}
			}
		}
	}
	close(fd);

	return 0;
}
