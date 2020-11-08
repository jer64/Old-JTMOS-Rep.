// close.c - Close file descriptor / file.
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "filedescriptor.h"

//
// Free the descriptor.
// This will also cause automatic
// flushing of all the buffers, etc.
// Deallocation of the allocate data
// on buffers, if any, and so on.
//
int msdos_close(SYSCALLPAR)
{
	POSTMANCMD *post;
	FILEDES *f;
	int fd;

	//-------------------------------------------------------
	fd = scall_ebx;

	// Check file descriptor validity
	if(!isValidFD(fd))
		return -1;

	//-------------------------------------------------------
	f = get_filedesptr(fd);
	if(f==NULL)
	{
		dprintk("scall_close: Invalid fd=%d.\n",
			fd);
		return -1;
	}

	// Close socket.
	if(f->socketHandle)
	{
		// Close socket
		scall(SYS_closesocket, f->socketHandle,0,0, 0,0,0);
	}

	//
	dprintk("scall_close: Closing fd %d.\n",
		fd);

	// Got a new larger file size? Set new file size.
	if( f->max_offset_peek > _msdos_setsize(f->name) )
	{
		// Set new size.
		_msdos_setsize(f->name, f->max_offset_peek);
	}

	//
        _msdos_close(f);

	// Free file descriptor
	FreeFD(fd);

	//
	return 0;
}

//
int _msdos_setsize(const char *pathname, int offsetti)
{
	//

}

//

