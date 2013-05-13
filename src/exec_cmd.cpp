#include "exec_cmd.h"

extern struct process_info cur_process;

/*
 * 判断程序配置文件是否设置
 */
int is_config_set()
{
	if (cur_process.config[0] == '\0' ) {
		return 0;
	} else {
		return 1;
	}
}

/*
 * 程序配置文件未设置的情况
 */
void no_set_config(char *cmd_buf, char *exec_buf)
{
	//search_index(cmd_buf);
	ParseJsonIndex(cmd_buf);
				
	if ( cur_process.name[0] == '\0' ) {
		return;
	}
				
	open_process(cmd_buf, exec_buf, cur_process.name);
}

/*
 * 程序配置文件设置的情况
 */
void has_set_config(char *cmd_buf, char *exec_buf, int *keys)
{
	sys_err("enter has_set_config...\n");
	//search_str(cmd_buf, exec_buf, cur_process.config);
	ParseJsonFromFile(cmd_buf, exec_buf, cur_process.config);
	
	if ( exec_buf[0] != '\0' ) {
		//system(exec_buf);
		// 根据命令类型来执行相关的操作
		sys_says("-----type : %s\n", cur_process.type);
		if ( (strncmp(cur_process.type, "keys", 4 ) == 0) ||
				(strncmp(cur_process.type, "multi", 5 ) == 0) ) {
			// 执行快捷键
			fprintf(stderr, "exec_buf != NULL\n");
			strtok_num(exec_buf, keys);
			exec_command(keys);
		} else if ( strncmp(cur_process.type, "func", 4 ) == 0 ) {
			// 调用函数
			set_focus();
			//sys_err("enter music...\n");
			if ( strncmp(cur_process.name, "deepin-music-player", 19 ) == 0 ) {
				music_type_func(exec_buf);
			}
		} else if ( strncmp(cur_process.type, "cmd", 3 ) == 0 ) {
			//search_str(cmd_buf, exec_buf, OPEN_FILE);
			// 执行命令
			if (exec_buf[0] != '\0' ) {
				system(exec_buf);
			}
		}
	} else {
		if ( strncmp(cur_process.type, "multi", 5 ) == 0 ) {
			// 多样的命令类型
			memset(exec_buf, 0, READ_LINE);
			//search_str(cmd_buf, exec_buf, CMD_FILE);
			ParseJsonFromFile(cmd_buf, exec_buf, CMD_JSON);
			// 执行其他的命令
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

/*
 * 判断进程是否打开
 */
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
	
	sys_says("proceess not exist...\n");
	return 0;
}

/*
 * 打开进程
 */
void open_process(char *cmd_buf, char *exec_buf, char *process_name)
{
	if ( is_process_open(process_name) == 0 ) {
		// 进程不存在
		//search_str(process_name, exec_buf, OPEN_FILE);
		sys_says("process_name : %s\n", process_name);
		sys_says("OPEN_JSON : %s\n", OPEN_JSON);
		ParseJsonFromFile(process_name, exec_buf, OPEN_JSON);
		sys_says("exec_buf : %s\n", exec_buf);
		if ( exec_buf != NULL ) {
			system(exec_buf);
		}
	} else {
		printf("open process : %s...\n", process_name );
		//if ( set_focus() == -1 ) {
		if ( activate_win() == -1 ) {
			// 进程存在，但为打开窗口
			//search_str(process_name, exec_buf, OPEN_FILE);
			ParseJsonFromFile(process_name, exec_buf, OPEN_JSON);
			if ( exec_buf != NULL ) {
				system(exec_buf);
			}
		}
	}
}
