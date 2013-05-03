#include "parse_command.h"

extern struct process_info cur_process;

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
	sys_err("parse record : %s\n", cmd_buf);
	
	return;
}

int is_config_set()
{
	if (cur_process.config[0] == '\0' ) {
		return 0;
	} else {
		return 1;
	}
}

int is_process_open(char *buf)
{
	FILE *fp = NULL;
	char ret_buf[READ_LINE];
	char sys_buf[READ_LINE];

	fprintf(stderr, "is_process_open start...\n" );
	memset(sys_buf, 0, READ_LINE);
	sprintf(sys_buf, "pidof  %s > ./tmp/rows", buf );
	sys_err("sys_buf : %s\n", sys_buf);
	system(sys_buf);

	memset(ret_buf, 0, READ_LINE);
	fp = fopen("./tmp/rows", "r");
	fscanf(fp, "%s", ret_buf);
	fclose(fp);
	
	fprintf(stderr, "is_process_open end...\n" );
	if ( ret_buf[0] != '\0' ) {
		printf("ret : %s\n", ret_buf);
		return 1;
	}
	
	return 0;
}

unsigned long get_pid_via_name( char *process_name )
{
	char buf[1024];
	unsigned long pid = 0;
	FILE *fp = NULL;
	
	printf("get_pid_via_name : %s\n", process_name);
	sprintf( buf, "ps aux | grep %s | sed -n 1p | awk '{print $2}' > ./tmp/pid", 
			process_name );
	system(buf);
	
	if ( (fp = fopen("./tmp/pid", "r")) == NULL ) {
		sys_err("get_pid failed...\n");
		return 0;
	}
	fscanf(fp, "%lud", &pid);
	fclose(fp);
	printf("get_pid_via_name : %lu\n", pid);
	
	return pid;
}
