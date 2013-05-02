//#include "record_recog.h"
#include "parse_command.h"
#include "send_keys.h"
#include "music.h"

struct process_info cur_process;

int first_record;
int record_abort;

int main(int argc, char **argv)
{
	int chooser = 0;
	
	signal(SIGINT, catch_signal);
	memset(&cur_process, 0, sizeof(cur_process));
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
			break;
		}
		case 2: {
			parse_record(cmd_buf);
			if ( cmd_buf[0] == '\0' ) {
				break;
			}
			
			if ( !is_config_set() ) {
				fprintf(stderr, "!is_config_set\n");
				no_set_config(cmd_buf, exec_buf);
			} else {
				fprintf(stderr, "is_config_set\n");
				has_set_config(cmd_buf, exec_buf, keys);
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

void no_set_config(char *cmd_buf, char *exec_buf)
{
	search_index(cmd_buf);
				
	if ( cur_process.name == NULL ) {
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
			
		}
	} else {
		no_set_config(cmd_buf, exec_buf);
	}
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

void music_type_func(char *exec_buf)
{
	//sys_err("music func\n");
	if ( strncmp(exec_buf, "PlayPause", 9) == 0 ) {
		MusicPlayPause();
		return;
	}
	
	if ( strncmp(exec_buf, "Next", 4) == 0 ) {
		MusicNext();
		return;
	}
	
	if ( strncmp(exec_buf, "Prev", 4) == 0 ) {
		MusicPrev();
		return;
	}
	
	if ( strncmp(exec_buf, "Rewind", 6) == 0 ) {
		MusicRewind();
		return;
	}
	
	if ( strncmp(exec_buf, "Forward", 7) == 0 ) {
		MusicForward();
		return;
	}
	
	if ( strncmp(exec_buf, "Stop", 4) == 0 ) {
		MusicStop();
		return;
	}
	
	if ( strncmp(exec_buf, "Quit", 4) == 0 ) {
		MusicQuit();
		return;
	}
}
