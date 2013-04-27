#ifndef __PARSE_COMMAND_H__
#define __PARSE_COMMAND_H__

#include "record_recog.h"

//char cmd_buf[BUF_LEN];

/* 当前 conf 文件是否设置 */
int is_config_set();

/* 从语音获取文本 */
void parse_record(char *cmd_buf);
void get_cmd_buf(char *buf);
void clear_punctuation(char *punc);	//清理标点

/* 从指定的文件中检索命令 */
void search_str(char *cmd_buf, char *exec_buf, char *conf_file);
/* 检索 index 文件 */
void search_index(char *cmd_buf);
void str_copy_delim(char *src, char delim);

/* 程序是否打开 */
int is_process_open(char *buf);

unsigned long get_pid_via_name( char *process_name );

#endif
