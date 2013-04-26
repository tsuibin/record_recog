#include "parse_command.h"

extern char cur_process_name[BUF_LEN];
extern char cur_config_name[READ_LINE];

void get_cmd_buf(char *buf)
{
	FILE *fp = NULL;
	int ret = 0;
	char *tmp = NULL;

	if ( (fp = fopen(OUTPUT_FILE, "r")) == NULL ) {
		fprintf(stderr, "get_cmd_buf open file err...\n");
		return;
	}

	ret = fread(buf, 1, BUF_LEN, fp);
	if ( ret <= 0 ) {
		fprintf(stderr, "get_cmd_buf read file err...\n" );
		fclose(fp);
		return;
	}

	fprintf(stderr, "buf : %s\nret : %d\n", buf, ret);
	tmp = buf + (ret - 3);
	fprintf(stderr, "tmp : %s\n", tmp);
	clear_punctuation(tmp);
	fprintf(stderr, "cmd_buf : %s\n", buf);
}

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

void search_str(char *cmd_buf, char *exec_buf, char *conf_file)
{
	FILE *fp;
	int ret;
	int cmd_len = strlen(cmd_buf);
	char read_buf[READ_LINE+1];

	if ( (fp = fopen(conf_file, "r")) == NULL ) {
		fprintf(stderr, "search_str open file err...\n");
		return;
	}

	while ( !feof(fp) ) {
		memset(read_buf, 0, READ_LINE+1);
		memset(exec_buf, 0, READ_LINE);
		fgets(read_buf, READ_LINE, fp);
		if ( read_buf == NULL ) {
			fprintf(stderr, "fgets err...\n");
			fclose(fp);
			fp = NULL;
			break;
		}
		ret = strlen(read_buf);
		fprintf(stderr, "read_buf : %s\n", read_buf);

		fprintf(stderr, "cmd_len : %d\n", cmd_len);
		fprintf(stderr, "ret : %d\n", ret);
		if ( strncmp(cmd_buf, read_buf, cmd_len) == 0 ) {
			memcpy(exec_buf, read_buf + cmd_len + 1, ret - cmd_len -1);
			fprintf(stderr, "exec_buf : %s\n", exec_buf);
			fclose(fp);
			fp = NULL;
			//fprintf(stderr, "exec_buf : %s\n", exec_buf);
			break;
		}
	}
	if ( fp != NULL ) {
		fclose(fp);
	}
}

void search_index(char *cmd_buf)
{
	FILE *fp;
	int ret;
	int cmd_len = strlen(cmd_buf);
	char exec_buf[READ_LINE];
	char read_buf[READ_LINE+1];

	if ( (fp = fopen(INDEX_FILE, "r")) == NULL ) {
		fprintf(stderr, "search_str open file err...\n");
		return;
	}

	while ( !feof(fp) ) {
		memset(read_buf, 0, READ_LINE+1);
		memset(exec_buf, 0, READ_LINE);
		fgets(read_buf, READ_LINE, fp);
		if ( read_buf == NULL ) {
			fprintf(stderr, "fgets err...\n");
			fclose(fp);
			fp = NULL;
			break;
		}
		ret = strlen(read_buf);
		fprintf(stderr, "read_buf : %s\n", read_buf);

		fprintf(stderr, "cmd_len : %d\n", cmd_len);
		fprintf(stderr, "ret : %d\n", ret);
		if ( strncmp(cmd_buf, read_buf, cmd_len) == 0 ) {
			memcpy(exec_buf, read_buf + cmd_len + 1, ret - cmd_len -1);
			fprintf(stderr, "exec_buf : %s\n", exec_buf);
			fclose(fp);
			fp = NULL;
			
			str_copy_delim(exec_buf, ':');
			break;
		}
	}
	if ( fp != NULL ) {
		fclose(fp);
	}
}

void str_copy_delim(char *src, char delim)
{
	int i = 0;
	int flag = 0;
	int len = strlen(src);
	char *ptr = src;
	
	memset(cur_process_name, 0, BUF_LEN);
	memset(cur_config_name, 0, READ_LINE);
	if (src == NULL) {
		return ;
	}
	
	for ( i = 0; i < len; i++ ) {
		if ( *ptr == delim ) {
			flag = 1;
			break;;
		}
		ptr++;
	}
	
	if ( flag ) {
		flag = 0;
		memcpy(cur_process_name, src, i);
		memcpy(cur_config_name, ptr + 1, len -1 - i);
	}
}

void parse_record(char *cmd_buf)
{
	int ret;
	//char cmd_buf[BUF_LEN];
	//char exec_buf[READ_LINE];

	memset(cmd_buf, 0, BUF_LEN);
	//memset(exec_buf, 0, BUF_LEN);

	fprintf(stderr, "开始语音识别...\n");
	ret = SpeechRecog(INPUT_FILE, OUTPUT_FILE);
	if ( ret == -1 ) {
		printf("语音识别失败...\n");
		exit(-1);
	}
	fprintf(stderr, "语音识别结束， 成功！...\n");

	get_cmd_buf(cmd_buf);
	/*
	if ( cmd_buf != NULL ) {
		search_str(cmd_buf, exec_buf);
	}
	*/
	/*		
	if (exec_buf != NULL) {
		system(exec_buf);
	}
	*/
	return;
}

int is_config_set()
{
	if (cur_config_name == NULL) {
		return 0;
	} else {
		return 1;
	}
}

int is_process_open(char *buf)
{
	FILE *fp = NULL;
	int i = 0;
	//char tmp[4];
	char sys_buf[READ_LINE];

	memset(sys_buf, 0, READ_LINE);
	sscanf(sys_buf, "ps aux | grep -w %s | wc -l > rows", buf );
	system(sys_buf);

	fp = fopen("rows", "r");
	//fread(tmp, 1, 4, fp);
	fscanf(fp, "%d", &i);
	fclose(fp);
	printf("%d\n", i);
	
	if ( i > 1 ) {
		return 1;
	} else {
		return 0;
	}
}
