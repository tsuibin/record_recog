#include "exec_cmd.h"

extern struct process_info cur_process;

int is_config_set()
{
	if (cur_process.config[0] == '\0' ) {
		return 0;
	} else {
		return 1;
	}
}


void no_set_config(char *cmd_buf, char *exec_buf)
{
	search_index(cmd_buf);
				
	if ( cur_process.name[0] == '\0' ) {
		return;
	}
				
	open_process(cmd_buf, exec_buf, cur_process.name);
}

void has_set_config(char *cmd_buf, char *exec_buf, int *keys)
{
	sys_err("enter has_set_config...\n");
	search_str(cmd_buf, exec_buf, cur_process.config);
	if ( exec_buf[0] != '\0' ) {
		//system(exec_buf);
		if ( strncmp(cur_process.type, "keys", 4 ) == 0 ) {
			fprintf(stderr, "exec_buf != NULL\n");
			strtok_num(exec_buf, keys);
			exec_command(keys);
		} else if ( strncmp(cur_process.type, "func", 4 ) == 0 ) {
			set_focus();
			
			//sys_err("enter music...\n");
			if ( strncmp(cur_process.name, "deepin-music-player", 19 ) == 0 ) {
				music_type_func(exec_buf);
			}
		} else if ( strncmp(cur_process.type, "cmd", 3 ) == 0 ) {
			//search_str(cmd_buf, exec_buf, OPEN_FILE);
			if (exec_buf[0] != '\0' ) {
				system(exec_buf);
			}
		}
	} else {
		if ( strncmp(cur_process.type, "multi", 5 ) == 0 ) {
			memset(exec_buf, 0, READ_LINE);
			search_str(cmd_buf, exec_buf, CMD_FILE);
			if (exec_buf[0] != '\0' ) {
				system(exec_buf);
			} else {
				no_set_config(cmd_buf, exec_buf);
			}
		} else {
			no_set_config(cmd_buf, exec_buf);
		}
	}
}

int is_process_open(char *buf)
{
	FILE *fp = NULL;
	char ret_buf[READ_LINE];
	char sys_buf[READ_LINE];

	fprintf(stderr, "is_process_open start...\n" );
	system("ps a -U $USER | awk \'{print $5,$NF}\' > /tmp/ps_out");
	memset(sys_buf, 0, READ_LINE);
	sprintf(sys_buf, "grep  %s /tmp/ps_out > /tmp/grep_out", buf );
	sys_err("sys_buf : %s\n", sys_buf);
	system(sys_buf);

	memset(ret_buf, 0, READ_LINE);
	fp = fopen("/tmp/grep_out", "r");
	fscanf(fp, "%s", ret_buf);
	fclose(fp);
	
	fprintf(stderr, "is_process_open end...\n" );
	if ( ret_buf[0] != '\0' ) {
		printf("ret : %s\n", ret_buf);
		return 1;
	}
	
	return 0;
}

void open_process(char *cmd_buf, char *exec_buf, char *process_name)
{
	if ( is_process_open(process_name) == 0 ) {
		search_str(process_name, exec_buf, OPEN_FILE);
		if ( exec_buf != NULL ) {
			system(exec_buf);
		}
	} else {
		printf("open process : %s...\n", process_name );
		set_focus();
	}
}

unsigned long get_pid_via_name( char *process_name )
{
	char buf[1024];
	unsigned long pid = 0;
	FILE *fp = NULL;
	
	printf("get_pid_via_name : %s\n", process_name);
	sprintf( buf, "ps aux | grep %s | sed -n 1p | awk '{print $2}' > /tmp/pid", 
			process_name );
	system(buf);
	
	if ( (fp = fopen("/tmp/pid", "r")) == NULL ) {
		sys_err("get_pid failed...\n");
		return 0;
	}
	fscanf(fp, "%lud", &pid);
	fclose(fp);
	printf("get_pid_via_name : %lu\n", pid);
	
	return pid;
}
