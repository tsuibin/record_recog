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
	ret = SpeechRecog(INPUT_FILE, OUTPUT_FILE);
	if ( ret == -1 ) {
		printf("语音识别失败...\n");
		exit(-1);
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

/*
 * 搜索指定的文件，取得对应的命令
 */
void search_str(char *cmd_buf, char *exec_buf, char *conf_file)
{
	FILE *fp;
	int ret;
	int cmd_len = strlen(cmd_buf);
	char read_buf[READ_LINE+1];

	if ( cmd_len <= 0 ) {
		return;
	}
	
	fprintf(stderr, "cwd : %s\n", get_current_dir_name());
	fprintf(stderr, "search_str file : %s\n",  conf_file);
	if ( (fp = fopen(conf_file, "r")) == NULL ) {
		fprintf(stderr, "search_str open file err %s\n", strerror(errno));
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

/*
 * 搜索 index.conf 文件，
 * 设置进程名，标题，配置文件，命令类型
 */
void search_index(char *cmd_buf)
{
	FILE *fp;
	int ret;
	int cmd_len = strlen(cmd_buf);
	char exec_buf[READ_LINE];
	char read_buf[READ_LINE+1];

	if ( cmd_len <= 0 ) {
		return;
	}
	
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
			
			str_copy_delim(exec_buf);
			fprintf(stderr, "cur_process: %s\n", cur_process.name);
			fprintf(stderr, "cur_config: %s\n", cur_process.config);
			break;
		}
	}
	if ( fp != NULL ) {
		fclose(fp);
	}
}

/*
 * 以指定的字符串分割给出的字符串
 * 得到对应的结果
 */
void str_copy_delim(char *src)
{
	char *p = NULL;
	int len = 0;
	
	memset(&cur_process, 0, sizeof(cur_process));
	if (src == NULL) {
		return ;
	}
	
	p = strtok(src, ":");
	memcpy(cur_process.name, p, strlen(p));
	printf("name : %s\n", cur_process.name);
	
	p = strtok(NULL, ":");
	memcpy(cur_process.item, p, strlen(p));
	printf("item: %s\n", cur_process.item);
	
	p = strtok(NULL, ":");
	memcpy(cur_process.config, p, strlen(p));
	printf("config: %s\n", cur_process.config);
	
	p = strtok(NULL, ":");
	printf("p: %s\n", p);
	memcpy(cur_process.type, p, strlen(p));
	printf("type: %s\n", cur_process.type);
	
	len = strlen(cur_process.type);
	if ( cur_process.type[len -1] == '\n' ) {
		cur_process.type[len -1] = '\0';
	}
}
