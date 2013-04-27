#ifndef __MY_COMMON_H__
#define __MY_COMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_FILE	"tmp/tmp.wav"
#define OUTPUT_FILE	"tmp/output"
#define ROWS_FILE	"tmp/rows"

#define INDEX_FILE	"conf/index.conf"
#define OPEN_FILE	"conf/open.conf"

#define sys_err(args ...) {fprintf(stderr, args);}

#define KEY_LEN	6
#define BUF_LEN	32
#define READ_LINE	80

#endif
