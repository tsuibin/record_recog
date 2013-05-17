#include <unistd.h>
#include <glibtop.h>
#include <glibtop/procstate.h>
#include <glibtop/mountlist.h>

void get_name_via_pid(pid_t pid)
{
	glibtop_proc_state buf;
	glibtop_mountlist map_buf;
	glibtop_mountentry *map_entry;

	memset(&buf, 0, sizeof(buf));
	glibtop_get_proc_state( &buf, pid );

	g_print("cmd : %s\n", buf.cmd );
	
	map_entry = glibtop_get_mountlist(&map_buf, pid);
	g_print( "filename : %s\n", map_entry->devname );
	g_print( "filename : %s\n", map_entry->mountdir );
}

int main()
{
	glibtop_init();

	get_name_via_pid(getpid());
	get_name_via_pid(17876);
	
	glibtop_close();

	return 0;
}
