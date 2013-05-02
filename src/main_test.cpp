#include "parse_command.h"
#include "send_keys.h"

int test_flag = 0;
int keys_len = 0;

int first_record;
int record_abort;

struct process_info cur_process;

void open_process(char *cmd_buf, char *exec_buf, char *process_name)
{
	//sys_err("cmd : %s\n", cmd_buf);
	if ( is_process_open(process_name) != 1 ) {
		search_str(process_name, exec_buf, OPEN_FILE);
		if ( exec_buf != NULL ) {
			system(exec_buf);
		}
	} else {
		set_focus( get_pid_via_name(process_name) );
	}
}
/*
void catch_signal(int signo)
{
	if ( signo == SIGINT ) {
		test_flag = 1;
	}
}
*/
int main()
{
	int ret = 0;
	//int keys_len = 0;
	int keys[KEY_LEN];
	char cmd_buf[BUF_LEN];
	char exec_buf[READ_LINE];

	//signal(SIGINT, catch_signal);
	while (1) {
		memset(keys, 0, KEY_LEN * sizeof(int));
		memset(cmd_buf, 0, BUF_LEN);
		memset(exec_buf, 0, READ_LINE);
		
		sys_err("请选择要进入的应用程序(输入quit退出)：");
		scanf("%s", cmd_buf);
		fflush(stdin);
		sys_err("cmd : %s\n", cmd_buf);
		if (strncmp(cmd_buf, "quit", 4) == 0) {
			break;
		}
		
		search_index(cmd_buf);
		if ( cur_process.name == NULL ) {
			break;
		}			
		open_process(cmd_buf, exec_buf, cur_process.name);
		
		while (!test_flag) {
			memset(cmd_buf, 0, BUF_LEN);
			memset(exec_buf, 0, READ_LINE);
			sys_err("请选择要进入行的操作(输入back返回上一层)：");
			memset(cmd_buf, 0, BUF_LEN);
			scanf("%s", cmd_buf);
			fflush(stdin);
			sys_err("cmd : %s\n", cmd_buf);
			if (strncmp(cmd_buf, "back", 4) == 0) {
				break;
			}
			
			search_str(cmd_buf, exec_buf, cur_process.name);
			if ( exec_buf[0] != '\0' ) {
				//system(exec_buf);
				fprintf(stderr, "exec_buf != NULL\n");
				strtok_num(exec_buf, keys);
				exec_command(keys, get_pid_via_name(cur_process.name));
			}
			//sleep(1);
		}
	}
	
	return 0;
}
