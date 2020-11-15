//---------------------------------------------------------------------------------------
// lseek.c
//---------------------------------------------------------------------------------------
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "scs.h"

//---------------------------------------------------------------------------------------
// off_t lseek(int fildes, off_t offset, int whence);
int msdos_lseek(SYSCALLPAR)
{
	FILEDES *filedes;
	int whence;
	int offset,fd;
	
	//
	fd = scall_ebx;
	offset = scall_ecx;
	whence = scall_edx;

	//
	if(!isValidFD(fd))
		return -1;

	//
	filedes = get_filedesptr(fd);

	//
	switch(whence)
	{
		//
		case SEEK_CUR:
		filedes->offset += offset;
		break;
	
		//
		case SEEK_SET:
		filedes->offset = offset;
		break;
	
		//
		case SEEK_END:
		fd = open(filedes->name, O_RDONLY);
		filedes->offset = lseek(fd, 0, SEEK_END)+offset;
		close(fd);
		break;

		// Illegal whence.
		default:
		return -1;
	}
	
	//
	return filedes->offset;
}

//

