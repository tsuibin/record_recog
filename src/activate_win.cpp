#include "my_xlib.h"

extern Window match_wid;
extern struct process_info cur_process;

void set_focus()
{
	Display *disp = NULL;

	setlocale(LC_ALL, "");

	if ( !(disp = XOpenDisplay(NULL)) ) {
		sys_err("Cannot open display...\n");
		return ;
	}
	sys_err("create display ok!\n");
	
	//get_wid(cur_process.name);
	get_wid(cur_process.item);
	if ( match_wid == (Window)0 ) {
		sys_err("match wid failed...\n");
		XCloseDisplay(disp);
		return;
	}
	sys_err("match_wid : %lu\n", match_wid);

	/* 设置指定窗口为焦点 */
	XSetInputFocus(disp, match_wid, RevertToParent, CurrentTime);
	XFlush(disp);
	while ( XPending(disp) ) {}
	
	XCloseDisplay(disp);
	match_wid = (Window)0;
}

void get_wid(char *name)
{
	int flag = 0;
	FILE *fwm = NULL;
	unsigned long int wm = 0;
	char wm_buf[READ_LINE];
	
	if ( name == NULL ) {
		return ;
	}
	
	sys_says("get_wid name : %s\n", name);
	sys_wm_wid();
	
	fwm = fopen("./tmp/wm_wid", "r");
	if ( fwm == NULL ) {
		sys_err("get_wid open file wm_wid failed...\n");
		return ;
	}
	
	memset(wm_buf, 0, READ_LINE);
	while (!feof(fwm)) {
		fgets(wm_buf, READ_LINE, fwm);
		if ( wm_buf == 0 ) {
			continue;
		}
		if ( index_str(name, wm_buf) == 0 ) {
			sscanf(wm_buf, "0x%lx", &wm);
			//sys_says("wm : %lu\n", wm);
			match_wid = (Window)wm;
			break;
		}
	}
	fclose(fwm);
}

void sys_wm_wid()
{
	char buf[] = "wmctrl -lx | sed  \'1,3d\' | awk \'{print $1, $3}\' > \
				  ./tmp/wm_wid";
	
	system(buf);
}

int index_str(char *name, char *src)
{
	int i = 0;
	int flag = 0;
	int src_len = 0;
	int name_len = 0;

	if ( name == NULL || src == NULL ) {
		sys_says("index argument error...\n");
		return -1;
	}

	src_len = strlen(src);
	name_len = strlen(name);
	while ( src[i] != '\0' ) {
		if ( i + name_len > src_len ) {
			break;
		}

		if ( strncmp( name, src + i, name_len ) == 0 ) {
			flag = 1;
			break;
		}
		i++;
	}

	if ( !flag ) {
		return -1;
	}

	return 0;
}
