//#include "record_recog.h"
#include "parse_command.h"

int first_record;
int record_abort;

char cur_process_name[BUF_LEN];
char cur_config_name[READ_LINE];

int main(int argc, char **argv)
{
	int chooser = 0;
	
	signal(SIGINT, catch_signal);
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
	char cmd_buf[BUF_LEN];
	char exec_buf[READ_LINE];

	memset(cmd_buf, 0, BUF_LEN);
	memset(exec_buf, 0, READ_LINE);
	switch(chooser) {
		case 1:
		{
			first_record = 1;
			record_abort = 0;
			
			fprintf(stderr, "开始录音，请输入你的指令：\n");
				
			/* 录音 2s */
			ret = MySndRecord(2, INPUT_FILE);
			if ( ret == -1 ) {
				fprintf(stderr, "录音失败...\n");
				exit(-1);
			}

			parse_record(cmd_buf);
			if ( !is_config_set() ) {
				search_index(cmd_buf);
				
				if ( !is_process_open(cur_process_name) ) {
					search_str(cmd_buf, exec_buf, OPEN_FILE);
					if ( exec_buf != NULL ) {
						system(exec_buf);
					}
				}
			} else {
				search_str(cmd_buf, exec_buf, cur_config_name);
				if ( exec_buf != NULL ) {
						system(exec_buf);
				} else {
					search_index(cmd_buf);
				
					if ( !is_process_open(cur_process_name) ) {
						search_str(cmd_buf, exec_buf, OPEN_FILE);
						if ( exec_buf != NULL ) {
							system(exec_buf);
						}
					}
				}
			}
			break;
		}
		case 2:
			parse_record(cmd_buf);
			break;
		case 3:
			exit(0);
			break;
		default:
			break;
	}
}
