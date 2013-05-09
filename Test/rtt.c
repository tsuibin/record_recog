#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct timeval tvrecv;
struct timeval tvsend;

/* 计算往返时间 */
void tv_sub( struct timeval *recvtime, struct timeval *sendtime )
{
	long sec  = recvtime->tv_sec - sendtime->tv_sec;
	long usec = recvtime->tv_usec - sendtime->tv_usec;

	if ( usec >= 0 ) {
		recvtime->tv_sec  = sec;
		recvtime->tv_usec = usec;
	} else {
		recvtime->tv_sec  = sec - 1;
		recvtime->tv_usec = -usec;
	}
}

int main()
{
	double rtt;
	
	gettimeofday( &tvsend, NULL );
	sleep(1);
	gettimeofday( &tvrecv, NULL );
	
	tv_sub( &tvrecv, &tvsend );
	rtt = tvrecv.tv_sec * 1000 + tvrecv.tv_usec / 1000;
	
	int t = ((int)rtt) / 1000;
	printf("time : %.1f\nt : %d\n", rtt, t);
	
	return 0;
}
