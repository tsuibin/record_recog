#ifndef __EXEC_CMD_H__
#define __EXEC_CMD_H__

#include "my_xlib.h"
#include "exec_func.h"
#include "parse_speech.h"

void exec_cmd_via_type(char *exec_buf, char *type_buf);
/* 当前 conf 文件是否设置 */
int is_config_set();
void no_set_config(char *cmd_buf, char *exec_buf, char *type_buf);
void has_set_config(char *cmd_buf, char *exec_buf, char *type_buf);
/* 程序是否打开 */
int is_process_open(char *buf);
void open_process(char *exec_buf, char *type_buf, char *process_name);

#endif
