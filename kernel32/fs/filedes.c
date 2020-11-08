// ===================================================================
// filedes.c - FILE DESCRIPTOR DATABASE
// (C) 2002-2008 by Jari Tuominen
// ===================================================================
#include "basdef.h"
#include "kernel32.h"
#include "filedes.h"

//
FDDB fddb;

// New FILEDES, returns PTR to a FILEDES structure.
FILEDES *NewFD(void)
{
	int fd;

	//
	fd = new_filedes();
	return get_filedesptr(fd);
}

// Free a file descriptor
int FreeFD(fd)
{
/*	// Flush it's buffers
	FlushFDBuffers(fd);

	//
	fddb.fd[fd]->f = NULL;

	// Free file descriptor
	fddb.fd[fd]->isFree=TRUE;*/

	//
	return 0;
}

void changeFDOwner(int fd, int pid)
{
	//
	if( isValidFD(fd) )
	{
		// Change owner of the fd
		fddb.fd[fd]->op = pid;
	}
}

//
FILEDES *get_filedesptr(int fd)
{
	//
	if( isValidFD(fd) )
	{
		//
		return fddb.fd[fd];
	}
	else	return NULL;
}

// Check that file descriptor is valid.
// 0=invalid, 1=valid
int isValidFD(int fd)
{
	int i,i2,i3,i4;

	// Out of range?
	if(fd>=N_MAX_FD || fd<0)return 0;

        // Freed one?
	if(fddb.fd[fd]->isFree)return 0;

        // Valid.
        return 1;
}

//
void filedesInformation(void)
{
	int i,r,f;

	//
	printk("--: FILEDES information\n");
	printk("Total %d entries reserved:\n", fddb.amount);
	for(i=0,r=0,f=0; i<fddb.amount; i++)
	{
		if(fddb.fd[i]->isFree)
			f++;
			else
			r++;
	}
	printk("%d entries are free\n",		f);
	printk("%d entries are allocated\n", 	r);
}

//
void init_filedes(void)
{
	int i,i2,i3,i4,nullfd;

	//
	fddb.amount =	0;
	fddb.fd =	malloc(8*N_MAX_FD);

	//
	for(i=0; i<N_MAX_FD; i++)
	{
		//
		fddb.fd[i] =		malloc(sizeof(FILEDES));
		memset(fddb.fd[i], 0, sizeof(FILEDES));
		fddb.fd[i]->isFree =	1;
		fddb.fd[i]->fd =	i;
	}

	// Reserve NULL descriptor always!
	nullfd = new_filedes();
}

// deallocate_proc_filedes(pid)
// Deallocates all filedescriptors that
// belong to the specified pid.
// Return value:
// Amount of entries deallocated totally.
int deallocate_proc_filedes(int pid)
{
	int i,i2,i3,i4;

	//
	for(i=0,i2=0; i<N_MAX_FD; i++)
	{
		if(fddb.fd[i]->op==pid)
		{
			fddb.fd[i]->isFree=1;
			i2++;
		}
	}
	return i2;
}


//
int new_filedes(void)
{
	int i,i2,i3,i4,entry;

	// Search for free entry.
	for(i=0,entry=-1; i<fddb.amount; i++)
	{
		// Got free entry?
		if( fddb.fd[i]->isFree )
		{
			// Reset structure
			memset(fddb.fd[i], 0, sizeof(FILEDES));
			// Set FD code
			fddb.fd[i]->fd = i;
			// Set entry as occupied
			fddb.fd[i]->isFree = FALSE;
			// Set owner PID
			fddb.fd[i]->op = GetCurrentThread();
			return i;
		}
	}

	// Already ran out of entries?
	if( (fddb.amount+1)>=N_MAX_FD )
		return 0;

	// Allocate new entry.
	entry=fddb.amount;
	fddb.amount++;
	fddb.fd[entry]->isFree = FALSE;
	fddb.fd[entry]->op = GetCurrentThread();
	fddb.fd[entry]->fd = entry;
	return entry;
}

// Returns 0 if no free entry was found.
// Otherwise returns the entry number.
int NEW_FILE_DESCRIPTOR(void *path, void *p1,void *p2, int v1,int v2)
{
	static int i,i2,i3,i4;

	//
	disable();
	i = new_filedes();
	enable();

	//
	if(!i) return 0;

	//
	return i;
}


