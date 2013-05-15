#ifndef __MY_INIT_H__
#define __MY_INIT_H__

#include "my_common.h"
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include <signal.h>
#include <sys/stat.h>

#ifndef NOFILE
#define NOFILE 3
#endif

#define NAME_LEN 80

/* 获得键盘设备文件 */
void get_dev_path(char *path);
/* 计算时间差 */
void tv_sub( struct timeval *presse_time, struct timeval *release_time );

#endif
