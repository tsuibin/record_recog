#include "my_alsa.h"
#include "my_qisr.h"

#include <signal.h>
#include <pthread.h>

int first_record;
int record_abort;

void speech_menu();
void catch_signal(int signo);
void *record_thread(void *arg);

int main(int argc, char **argv)
{
	int ret;
	int chooser = 0;
	//pthread_t recd_thrd;
	
	if ( argc != 3 ) {
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		return -1;
	}
	
	signal(SIGINT, catch_signal);
	while (1) {
		speech_menu();
		scanf( "%d", &chooser );
		printf("=======================================================\n\n");
		fflush(stdin);
		
		switch(chooser) {
			case 1:
			{
				first_record = 1;
				record_abort = 0;
				if ( first_record ) {
					fprintf(stderr, "开始录音，最短 3s...\n");
				} else {
					fprintf(stderr, "继续录音中...\n");
				}
				
				/* 录音 3s */
				ret = MySndRecord(3, argv[1]);
				if ( ret == -1 ) {
					fprintf(stderr, "录音失败...\n");
					pthread_exit(NULL);
				}
				break;
			}
			case 2:
				fprintf(stderr, "开始语音识别...\n");
				ret = SpeechRecog(argv[1], argv[2]);
				if ( ret == -1 ) {
					printf("语音识别失败...\n");
					return -1;
				}
				fprintf(stderr, "语音识别结束， 成功！...\n");
				break;
			case 3:
				exit(0);
				break;
			default:
				break;
		}
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

void *record_thread(void *arg)
{
	int ret;
	
	if ( first_record ) {
		fprintf(stderr, "开始录音，最短 3s...\n");
	} else {
		fprintf(stderr, "继续录音中...\n");
	}
	
	/* 录音 3s */
	ret = MySndRecord(3, (char*)arg);
	if ( ret == -1 ) {
		fprintf(stderr, "录音失败...\n");
		pthread_exit(NULL);
	}
	
	pthread_exit(NULL);
}
