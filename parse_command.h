#ifndef __PARSE_COMMAND_H__
#define __PARSE_COMMAND_H__

#include <string.h>
#include <stdlib.h>
#include "record_recog.h"

#define BUF_LEN	20
#define READ_LINE	80

//char cmd_buf[BUF_LEN];

void get_cmd_buf(char *buf);
void clear_punctuation(char *punc);	//清理标点

void search_str(char *cmd_buf, char *exec_buf);

#endif
