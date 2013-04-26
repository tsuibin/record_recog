#include "parse_command.h"

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

void search_str(char *cmd_buf, char *exec_buf)
{
	FILE *fp;
	int ret;
	int cmd_len = strlen(cmd_buf);
	char read_buf[READ_LINE+1];

	if ( (fp = fopen(CONFIG_FILE, "r")) == NULL ) {
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
