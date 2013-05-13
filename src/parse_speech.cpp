#include "parse_speech.h"

extern struct process_info cur_process;

/*
 * 处理音频文件
 */
void parse_record(char *cmd_buf)
{
	int ret;

	memset(cmd_buf, 0, BUF_LEN);

	fprintf(stderr, "开始语音识别...\n");
	sys_err("input : %s\n", INPUT_FILE);
	sys_err("ouput : %s\n", OUTPUT_FILE);
	ret = SpeechRecog(INPUT_FILE, OUTPUT_FILE);
	if ( ret == -1 ) {
		printf("语音识别失败...\n");
		//exit(-1);
		return;
	}
	fprintf(stderr, "语音识别结束， 成功！...\n");

	get_cmd_buf(cmd_buf);
	if ( cmd_buf[0] == '\0' ) {
		return;
	}
	sys_err("parse record : %s\n", cmd_buf);
	
	return;
}

/*
 * 获取语音输入的命令
 */
void get_cmd_buf(char *buf)
{
	FILE *fp = NULL;
	int ret = 0;
	char *tmp = NULL;

	if ( (fp = fopen(OUTPUT_FILE, "r")) == NULL ) {
		fprintf(stderr, "get_cmd_buf open file err: %s\n", strerror(errno));
		return;
	}

	ret = fread(buf, 1, BUF_LEN, fp);
	if ( ret <= 0 ) {
		fprintf(stderr, "get_cmd_buf read file err: %s\n", strerror(errno) );
		fclose(fp);
		return;
	}

	fprintf(stderr, "buf : %s\nret : %d\n", buf, ret);
	tmp = buf + (ret - 3);
	fprintf(stderr, "tmp : %s\n", tmp);
	clear_punctuation(tmp);
	fprintf(stderr, "cmd_buf : %s\n", buf);
}

/*
 * 去掉多余的符号
 */
void clear_punctuation(char *punc)
{
	if ( strncmp(punc, "。", 3) == 0 ) {
		memset(punc, 0, 3);
	} else if ( strncmp(punc, "？", 3) == 0 ) {
		memset(punc, 0, 3);
	} else if ( strncmp(punc, "，", 3) == 0 ) {
		memset(punc, 0, 3);
	}
}
