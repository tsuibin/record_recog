#include "src/my_common.h"

void get_wid()
{
	int flag = 0;
	FILE *fwm = NULL;
	FILE *fxdot = NULL;
	unsigned long int wm = 0;
	unsigned long int xdot = 0;
	char wm_buf[80];
	char xdot_buf[80];
	
	fwm = fopen("./tmp/wm_wid", "r");
	fxdot = fopen("./tmp/xdot_wid", "r");
	if ( fwm == NULL || fxdot == NULL ) {
		sys_says("get_wid open file failed...\n");
		return ;
	}
	
	memset(wm_buf, 0, 80);
	memset(xdot_buf, 0, 80);
	
	while (!feof(fwm)) {
		fgets(wm_buf, 80, fwm);
		if ( wm_buf == 0 ) {
			continue;
		}
		fscanf(fwm, "0x%lx", &wm);
		//sys_says("wm : %lu\n", wm);
		
		fseek(fxdot, 0, SEEK_SET);
		while(!feof(fxdot)) {
			fgets(xdot_buf, 80, fxdot);
			if ( xdot_buf == 0 ) {
				continue;
			}
			fscanf(fxdot, "%lu", &xdot);
			//sys_says("xdot : %lu\n", xdot);
			
			if ( wm == xdot ) {
				flag = 1;
				//sys_says("wm == xdot\n", xdot);
				break;
			}
		}
		
		if ( flag ) {
			flag = 0;
			break;
		}
	}
	fclose(fwm);
	fclose(fxdot);
}

int main()
{
	get_wid();
	
	return 0;
}
