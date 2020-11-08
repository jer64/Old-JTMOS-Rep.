//
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "filedescriptor.h"

// ssize_t read(int fd, void *buf, size_t count);
int msdos_read(SYSCALLPAR)
{
	int fd,ad;
	FILEDES *filedes;
	BYTE *dbuf;
	int i,i2,d,count;

	//---------------------------------------------------------------------------------------
	//
	fd = scall_ebx;
	//
	ad = A2K(scall_ecx); dbuf=ad;
	//
	count = scall_edx;

	//
	if(!isValidFD(fd))
	{
		printk("%s: INVALID FD: %d\n",
			__FUNCTION__, fd);
		return -1;
	}

	//---------------------------------------------------------------------------------------
	//
	filedes = get_filedesptr(fd);
	if(filedes==NULL)
	{
		printk("%s: INVALID FD: %d\n",
			__FUNCTION__, fd);
		// Oops! NULL PTR...
		return -1;
	}

	//
	if( (filedes->flags & O_DIRECTORY) )
	{
		return _msdos_readdir(fd, dbuf, count);
	}

	//
/*	printf("%s calls readfile name=%s dbuf ptr=0x%x count=%d offset=%d\n", __FUNCTION__,
		filedes->name, dbuf, count, filedes->offset);*/

	_msdos_read(fd, filedes->name, dbuf, count, filedes->offset);
	filedes->offset+=count;

	// Return amount of bytes read.
	return count;
}

//

