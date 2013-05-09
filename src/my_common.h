#ifndef __MY_COMMON_H__
#define __MY_COMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_FILE	"/tmp/tmpin.wav"
#define OUTPUT_FILE	"/tmp/output"
#define ROWS_FILE	"/tmp/rows"
#define READ_WAV	"/tmp/read_wav.wav"

#define INDEX_FILE	"conf/index.conf"
#define OPEN_FILE	"conf/open.conf"
#define CMD_FILE	"conf/command.conf"

#define INDEX_JSON	"./conf/index.json"
#define OPEN_JSON	"./conf/open.json"
#define CMD_JSON	"./conf/command.json"

#define sys_err(args ...) {fprintf(stderr, args);}
#define sys_says(args ...) {fprintf(stdout, args);}

#define KEY_LEN	6
#define BUF_LEN	32
#define READ_LINE	1024


struct process_info {
	char name[BUF_LEN];
	char item[BUF_LEN];
	char config[BUF_LEN];
	char type[BUF_LEN];
};

#endif
