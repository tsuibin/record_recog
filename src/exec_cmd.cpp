#include "exec_cmd.h"

extern int keys[KEY_LEN];
extern struct process_info cur_process;

void exec_cmd_via_type(char *exec_buf, char *type_buf)
{
	if ( exec_buf == NULL || type_buf == NULL ) {
		return ;
	}
	
	if ( strncmp(type_buf, "cmd", 3 ) == 0 ) {
		system(exec_buf);
	} else if ( strncmp(type_buf, "keys", 4 ) == 0 ) {
		strtok_num(exec_buf, keys);
		exec_cmd_via_keys(keys);
	} else if ( strncmp(type_buf, "func", 4 ) == 0 ) {
		if ( strncmp(cur_process.name, "deepin-music-player", 19 ) == 0 ) {
			music_type_func(exec_buf);
		}
	}
}

/*
 * 判断程序配置文件是否设置
 */
int is_config_set()
{
	if (cur_process.table[0] == '\0' ) {
		return 0;
	} else {
		return 1;
	}
}

/*
 * 程序配置文件未设置的情况
 */
void no_set_config(char *cmd_buf, char *exec_buf, char *type_buf)
{
	//search_index(cmd_buf);
	//ParseJsonIndex(cmd_buf);
	if ( (cmd_buf[0] & 0x80) != 0 ) {
		get_pinyin(cmd_buf);
	}
	search_index_table();
				
	if ( cur_process.name[0] == '\0' ) {
		return;
	}
				
	open_process(exec_buf, type_buf, cur_process.name);
}

/*
 * 程序配置文件设置的情况
 */
void has_set_config(char *cmd_buf, char *exec_buf, char *type_buf)
{
	sys_err("enter has_set_config...\n");
	sys_says("cmd : %s\n", cmd_buf);
	memset(exec_buf, 0, EXEC_BUF);
	//ParseJsonFromFile(cmd_buf, exec_buf, type_buf, cur_process.config);
	if ( (cmd_buf[0] & 0x80) != 0 ) {
		get_pinyin(cmd_buf);
	}
	search_table(cmd_buf, cur_process.table);
	
	sys_says("has_set_config exec : %s\n", exec_buf);
	if ( exec_buf[0] != '\0' ) {
		activate_win();
		exec_cmd_via_type(exec_buf, type_buf);
	} else {
		no_set_config(cmd_buf, exec_buf, type_buf);
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
void open_process(char *exec_buf, char *type_buf, char *process_name)
{
	if ( is_process_open(process_name) == 0 ) {
		// 进程不存在
		//search_str(process_name, exec_buf, OPEN_FILE);
		sys_says("process_name : %s\n", process_name);
		//sys_says("OPEN_JSON : %s\n", OPEN_JSON);
		//ParseJsonFromFile(process_name, exec_buf, type_buf, OPEN_JSON);
		search_table(process_name, OPEN_TABLE);
		sys_says("exec_buf : %s\n", exec_buf);
		exec_cmd_via_type(exec_buf, type_buf);
	} else {
		printf("open process : %s...\n", process_name );
		//if ( set_focus() == -1 ) {
		if ( activate_win() == -1 ) {
			// 进程存在，但未打开窗口
			//search_str(process_name, exec_buf, OPEN_FILE);
			//ParseJsonFromFile(process_name, exec_buf, type_buf, OPEN_JSON);
			search_table(process_name, OPEN_TABLE);
			exec_cmd_via_type(exec_buf, type_buf);
		}
	}
}
