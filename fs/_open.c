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

// O_CREAT, O_EXCL, O_NOCTTY, O_TRUNC, O_APPEND, O_NONBLOCK, O_NDELAY,
// O_SYNC, O_NOFOLLOW, O_DIRECTORY, O_LARGEFILE.
// int open(const char *pathname, int flags);
// scall_ebx	pathname
// scall_ecx	flags
int msdos_open(SYSCALLPAR)
{
	int fd,ad,flags,created;
	void *name;
	char str[256],ac[256];
	FILE *f;
	FILEDES *filedes;

	// Get name PTR.
	ad = A2K(scall_ebx); name=ad;
	flags = scall_ecx;

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

	//----------------------------------------------------------------------
	// TCP socket open.
	//
	if(!strncmp(name, "/tcp/", 5))
	{
		// TCP/IP: socket open
		strcpy(str, name+5);
		fd = _opensocket(str);
		return fd;
	}

	//----------------------------------------------------------------------
	// File open.
	//

	// File already created?
	created = FALSE;

	// EXCL case.
	fd = open(name, O_RDONLY);
	if(fd>=0) { close(fd); }
	if(flags&O_EXCL && flags&O_CREAT && fd>=0)
		return -EEXIST;

	// If using O_TRUNC mode:
	// Remove target file & recreate it 
	if(flags & O_TRUNC)
	{
		// Does old exist?

		fd = open(name, O_RDONLY);
		if(fd>=0) {
			//
			close(fd);
			// Remove old.
			unlink(name);
			// Recreate the file.
			fd = creat(name, 0);
			if(fd>=0) {
				close(fd); 
				created = TRUE;
			}
		}

		// HMM, I think this should not return an error...
		// Therefore I removed it.
	/*	else
			// Error!
			return -ENOENT; */
	}

	// If file is not yet created, we check O_CREAT to do it.
	// Require O_CREAT to create it if it does not exist.
	if(!created && flags&O_CREAT)
	{
		// Create new file.
		fd = creat(name, 0);
		if(fd>=0) {
			close(fd); 
			created = TRUE;
		}
	}

	// If not yet created and it does not exist even,
	// then return "No Entry Found".
	fd = open(name, O_RDONLY);

	// Implementation of opening of a directory.
	if(fd>=0) {
		if(flags&O_DIRECTORY)
		{
			//
			filedes = get_filedesptr(fd);
			filedes->dnr = GetCurrentDNR();
			filedes->db = GetCurrentDB();
			filedes->flags = flags;
			_msdos_open(filedes, name, flags);
			return fd;
		}
		close(fd);
	}
	// Not created yet, called without O_CREAT.
	if(!created && fd<0)
		return -ENOENT;

	//----------------------------------------------------------------------------------
	// Here we should have a valid target file.
	//

	// Create new file descriptor.
	filedes = NewFD();

	// Store flags.
	filedes->flags = flags;

	// No bytes written as default.
	filedes->written = FALSE;

	// How far we've been at?
	filedes->max_offset_peek = 0;

	// Store name.
	strcpy(filedes->name, name);

	// Set start offset.
	if(flags&O_APPEND)
	{
		// Append.
		fd = open(name, O_RDWR);
		lseek(fd, 0, SEEK_END);
		if(fd>=0) {
			close(fd); 
			created = TRUE;
		}
		fd = open(name, O_RDONLY);
		if(fd>=0) {
			filedes->offset = lseek(fd, 0, SEEK_END);
			close(fd);
		}
	}
	else
		// Non-append.
		filedes->offset = 0;

	// Thread-safe DNR/DB specification of where
	// we are in the virtual file system.
	filedes->dnr = GetCurrentDNR();
	filedes->db = GetCurrentDB();
	_msdos_open(filedes, name, flags);

	// Return fd ID.
	return filedes->fd;

}

//


