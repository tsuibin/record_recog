#ifndef __PARSE_COMMAND_H__
#define __PARSE_COMMAND_H__

#include "my_qisr.h"

/* 从语音获取文本 */
void parse_record(char *cmd_buf);
void get_cmd_buf(char *buf);
void clear_punctuation(char *punc);	//清理标点

/* 使用 json 存储数据 */
int ParseJsonIndex(const char *buf);
int ParseJsonFromFile(const char *buf, char *exec_buf, 
				char *type_buf, const char *file);

/* 使用 sqlite 存储数据 */
void search_table(char *cmd_buf, const char *table_name);
void search_index_table();

#endif
