// =============================================================================
// File handling functions.
// (C) 2002-2005 by Jari Tuominen(ehc50@kanetti.fi).
// =============================================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "filefind.h"

// Returns file descriptor.
int findfirst(const char *path, FFBLK *ff)
{
	//
	return scall(SYS_findfirst, path,ff,0, 0,0,0);
}

//
int findnext(int fd, FFBLK *ff)
{
	//
	return scall(SYS_findnext, fd,ff,0, 0,0,0);
}

