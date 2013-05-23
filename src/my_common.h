#ifndef __MY_COMMON_H__
#define __MY_COMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATABASE_PATH	"./database/DeepinSpeech.db"
#define INDEX_TABLE		"SpeechIndex"
#define OPEN_TABLE		"OpenCommand"
#define COMMON_TABLE	"CommonIndex"

#define INPUT_FILE		"/tmp/tmpin.wav"
#define OUTPUT_FILE		"/tmp/output"
#define ROWS_FILE		"/tmp/rows"
#define READ_WAV		"/tmp/read_wav.wav"
#define XSEL_OUT		"/tmp/xsel_out"

#define sys_err(args ...) {fprintf(stderr, args);}
#define sys_says(args ...) {fprintf(stdout, args);}

#define KEY_LEN		6
#define BUF_LEN		20
#define EXEC_BUF 	80
#define READ_LINE	1024


struct process_info {
	char name[BUF_LEN];		//进程名
	char table[BUF_LEN];	//数据表名
	
	char cmd_buf[READ_LINE];
	char exec_buf[EXEC_BUF];
	char type[BUF_LEN];
};

#endif
