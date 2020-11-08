// stat.c
// TODO: Some functions are not ready yet.
#include <stdio.h>
#include "stat.h"

// Get file status.
int stat(const char *file_name, struct stat *buf)
{
	//
	return scall(SYS_stat, file_name,buf,0, 0,0,0);
}

// Same as stat but uses a file descriptor.
int fstat(int filedes, struct stat *buf)
{
	//
	return scall(SYS_fstat, filedes,buf,0, 0,0,0);
}

// Same as stat.
int lstat(const char *file_name, struct stat *buf)
{
	//
	return stat(file_name, buf);
}

//
int chmod(const char *path, mode_t mode)
{
	return 0;
}

//
int fchmod(int fildes, mode_t mode)
{
	return 0;
}

// isatty
// Returns 1 if desc is an open descriptor connected to a terminal and 0 else.
// (Linux programmer's manual)
int isatty(int desc)
{
	return 0;
}

//
int chown(const char *path, uid_t owner, gid_t group)
{
	return 0;
}

//
int fchown(int fd, uid_t owner, gid_t group)
{
	return 0;
}

//
int lchown(const char *path, uid_t owner, gid_t group)
{
	return 0;
}



