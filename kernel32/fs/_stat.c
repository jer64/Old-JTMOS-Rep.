// stat.c - get file status.
#include "kernel32.h"
#include "_stat.h"
#include "threads.h"
#include "scs.h"
#include "_opensocket.h"
#include "filedescriptor.h"

// We aim to simulate a UNIX stat system call.
// We use some JTMOS kernel system call functions
// to do some research on the file and
// fill the proper values in the info structure.
// int msdos_stat(const char *name, struct stat *s)
int msdos_stat(SYSCALLPAR)
{
	char *name,*s;

	//-----------------------------------------------------------------------------
	// Get parameters.
	name = A2K(scall_ebx);
	s = A2K(scall_ecx);

	//-----------------------------------------------------------------------------
	//
	if(name==NULL && !isValidString(name) && strlen(name)<=0 )
		// Error.
		return -1;

	//-----------------------------------------------------------------------------
	return _msdos_stat(name, s);
}

// Stat on a file descriptor.
// int msdos_fstat(int fd, const char *buf)
int msdos_fstat(SYSCALLPAR)
{
	const char *name;
	struct stat *s;
	int type,fd;
	FILEDES *f;

	//-----------------------------------------------------------------------------
	// Get parameters.
	fd = scall_ebx;
	s = A2K(scall_ecx);

	//-----------------------------------------------------------------------------
	//
	f = get_filedesptr(fd);
	if(f==NULL)
		// Invalid fd.
		return -1;

	//-----------------------------------------------------------------------------
	return _msdos_stat(name, s);
}


// The stat function itself.
// int _msdos_stat(const char *name, struct stat *s)
int _msdos_stat(const char *name, struct stat *s)
{
	int type,fd;

	//
	fd = open(name, O_RDONLY);
	if(fd>=0) {
		close(fd);
		type = 1; // only files accepted on open function.
	}
	if(type<=0)
		// File not found.
		return -1;

	// Initialize structure
	memset(s, 0, sizeof(struct stat));

	//
	if(type==1)
		// File.
		s->st_mode |= S_IFREG;
	if(type==2)
		// Directory.
		s->st_mode |= S_IFDIR;

	// Get file size.
	s->st_size = _msdos_get_file_size(name);

	// Simulate "amount of blocks allocated for this file".
	// (TODO)
	s->st_blocks = s->st_size/512;

	// Default block size.
	s->st_blksize = 512;

	//
	return 0;
}

//


//
int _msdos_get_file_size(const char *path)
{
        //
        printk("%s/%s: Not yet implemented.",
                __FUNCTION__, __FILE__);
	return 0;
}

