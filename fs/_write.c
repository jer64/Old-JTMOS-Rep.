//
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "filedescriptor.h"

// ssize_t write(int fd, void *buf, size_t count);
int msdos_write(SYSCALLPAR)
{
	POSTMANCMD *post;
	int fd,ad;
	FILEDES *filedes;
	BYTE *dbuf;
	int i,i2,d,count;

	//
	fd = scall_ebx;
	//
	ad = A2K(scall_ecx); dbuf=ad;
	//
	count = scall_edx;

	//
	if(!isValidFD(fd))
	{
		print("scall_write: Invalid file descriptor.\n");
		return -1;
	}

	//
	filedes = get_filedesptr(fd);
	if(filedes==NULL)
	{
		print("scall_write: Invalid file descriptor.\n");
		return -1;
	}

	//

	_msdos_write(fd, filedes->name, dbuf, count, filedes->offset);
	filedes->offset+=count;

	//
	if(filedes->offset > filedes->max_offset_peek)
		filedes->max_offset_peek = filedes->offset;

	// Return amount of bytes written.
	return count;
}

//
