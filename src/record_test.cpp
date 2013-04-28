#include "parse_command.h"
#include "send_keys.h"


int first_record;
int record_abort;

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
	
	switch(chooser) {
		case 1:
		{
			first_record = 1;
			record_abort = 0;
			
			fprintf(stderr, "开始录音，请输入你的指令：\n");
			ret = MySndRecord(3, "input.wav");	// 录音 3s
			if ( ret == -1 ) {
				fprintf(stderr, "录音失败...\n");
				exit(-1);
			}
			break;
		}
		case 2: {
			ret = SpeechRecog("input.wav", "output");
			if ( ret == -1 ) {
				printf("语音识别失败...\n");
				exit(-1);
			}
			fprintf(stderr, "语音识别结束， 成功！...\n");
			break;
		}
		case 3:
			exit(0);
			break;
		default:
			break;
	}
}
