//#include "record_recog.h"
#include "parse_command.h"
#include "send_keys.h"

int first_record;
int record_abort;

char cur_process_name[BUF_LEN];
char cur_config_name[READ_LINE];

int main(int argc, char **argv)
{
	int chooser = 0;
	
	signal(SIGINT, catch_signal);
	memset(cur_process_name, 0, BUF_LEN);
	memset(cur_config_name, 0, READ_LINE);
	while (1) {
		speech_menu();
		scanf( "%d", &chooser );
		printf("=======================================================\n\n");
		fflush(stdin);
		
		parse_chooser(chooser);
	}
	
	return 0;
}

void speech_menu()
{
	printf("\n=======================================================\n");
	//printf("请输入你的选择：\n");
	printf("1、录音，按 ctr+c 结束\n");
	printf("2、语音识别\n");
	printf("3、退出程序\n");
	printf("请输入你的选择：");
}

void catch_signal(int signo)
{
	if ( signo == SIGINT ) {
		record_abort = 1;
	}
}

void parse_chooser(int chooser)
{
	int ret = 0;
	int keys[KEY_LEN];
	char cmd_buf[BUF_LEN];
	char exec_buf[READ_LINE];

	memset(keys, 0, KEY_LEN * sizeof(int));
	memset(cmd_buf, 0, BUF_LEN);
	memset(exec_buf, 0, READ_LINE);
	switch(chooser) {
		case 1:
		{
			first_record = 1;
			record_abort = 0;
			
			fprintf(stderr, "开始录音，请输入你的指令：\n");
				
			/* 录音 2s */
			ret = MySndRecord(3, INPUT_FILE);
			if ( ret == -1 ) {
				fprintf(stderr, "录音失败...\n");
				exit(-1);
			}
			/*
			parse_record(cmd_buf);	//获得文本
			if ( cmd_buf[0] == '\0' ) {
				break;
			}
			
			if ( !is_config_set() ) {
				fprintf(stderr, "!is_config_set\n");
				search_index(cmd_buf);
				
				if ( cur_process_name == NULL ) {
					break;
				}
				
				open_process(cmd_buf, exec_buf, cur_process_name);
			} else {
				fprintf(stderr, "is_config_set\n");
				search_str(cmd_buf, exec_buf, cur_config_name);
				if ( exec_buf[0] != '\0' ) {
						//system(exec_buf);
						fprintf(stderr, "exec_buf != NULL\n");
						strtok_num(exec_buf, keys);
						exec_command(keys, get_pid_via_name(cur_process_name));
				} else {
					search_index(cmd_buf);
				
					if ( cur_process_name == NULL ) {
						break;
					}
					open_process(cmd_buf, exec_buf, cur_process_name);
				}
			}*/
			break;
		}
		case 2: {
			parse_record(cmd_buf);
			if ( cmd_buf[0] == '\0' ) {
				break;
			}
			
			if ( !is_config_set() ) {
				fprintf(stderr, "!is_config_set\n");
				search_index(cmd_buf);
				
				if ( cur_process_name == NULL ) {
					break;
				}
				
				open_process(cmd_buf, exec_buf, cur_process_name);
			} else {
				fprintf(stderr, "is_config_set\n");
				search_str(cmd_buf, exec_buf, cur_config_name);
				if ( exec_buf[0] != '\0' ) {
						//system(exec_buf);
						fprintf(stderr, "exec_buf != NULL\n");
						strtok_num(exec_buf, keys);
						exec_command(keys, get_pid_via_name(cur_process_name));
				} else {
					search_index(cmd_buf);
				
					if ( cur_process_name == NULL ) {
						break;
					}
					open_process(cmd_buf, exec_buf, cur_process_name);
				}
			}
			break;
		}
		case 3:
			exit(0);
			break;
		default:
			break;
	}
}

void open_process(char *cmd_buf, char *exec_buf, char *process_name)
{
	if ( !is_process_open(process_name) ) {
		search_str(process_name, exec_buf, OPEN_FILE);
		if ( exec_buf != NULL ) {
			system(exec_buf);
		}
	} else {
		set_focus( get_pid_via_name(process_name) );
	}
}
