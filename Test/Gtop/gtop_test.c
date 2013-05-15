#include <unistd.h>
#include <glibtop.h>
#include <glibtop/procstate.h>

void get_name_via_pid(pid_t pid)
{
	glibtop_proc_state buf;

	memset(&buf, 0, sizeof(buf));
	glibtop_get_proc_state( &buf, pid );

	g_print("cmd : %s\n", buf.cmd );
}

int main()
{
	glibtop_init();

	get_name_via_pid(getpid());
	get_name_via_pid(8383);

	return 0;
}
