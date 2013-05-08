#ifndef __PARSE_COMMAND_H__
#define __PARSE_COMMAND_H__

#include "my_qisr.h"

/* 从语音获取文本 */
void parse_record(char *cmd_buf);
void get_cmd_buf(char *buf);
void clear_punctuation(char *punc);	//清理标点

/* 从指定的文件中检索命令 */
void search_str(char *cmd_buf, char *exec_buf, char *conf_file);
/* 检索 index 文件 */
void search_index(char *cmd_buf);
void str_copy_delim(char *src);

/* 使用 json 存储数据 */
int ParseJsonIndex(const char *buf);
int ParseJsonFromFile(const char *buf, char *exec_buf, const char *file);

#endif
