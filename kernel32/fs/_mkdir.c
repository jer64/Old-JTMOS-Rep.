// open.c
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "_opensocket.h"
#include "filedescriptor.h"

/*
	FROM LINUX PROGRAMMERS MANUAL:
       The  open() system call is used to convert a pathname into
       a file descriptor (a small, non-negative integer  for  use
       in  subsequent  I/O  as with read, write, etc.).  When the
       call is successful, the file descriptor returned  will  be
       the lowest file descriptor not currently open for the pro­
       cess.  This call creates a new open file, not shared  with
       any  other  process.  (But shared open files may arise via
       the fork(2) system call.)  The new file descriptor is  set
       to  remain open across exec functions (see fcntl(2)).  The
       file offset is set to the beginning of the file.
*/

// Creating Directories
//
// int mkdir(pathname, mode_t mode);
// scall_ebx	pathname
// scall_ecx	mode
//
// Directories are created with the mkdir function. (There is also a shell command mkdir which does the same thing.)
//
// Function: int mkdir (const char *filename, mode_t mode)
int msdos_mkdir	(SYSCALLPAR)
{
	int fd,ad,mode,created;
	void *name;
	char str[256],ac[256];
	FILE *f;
	FILEDES *filedes;

	// Get name PTR.
	ad = A2K(scall_ebx); name=ad;
	mode = scall_ecx;

	//----------------------------------------------------------------------
	// Check for invalid parameters.
	//
	if(name==NULL && !isValidString(name) && strlen(name)<=0 )
	{
		//
		printk("%s: Invalid parameters.\n",
			__FUNCTION__);
		return -1;
	}


	//
	return mkdir(name, mode);
}

//


