// ======================================================================
// Enhanced directory cache database system,
//		aka EDCDS system.
// (C) 2002-2005 by Jari Tuominen.
//
// This file contains only the init routine
// and some few functions.
// Other functions are located in seperate
// dirdb* files.
// ======================================================================
#include "kernel32.h"
#include "dirdb.h"		// DIRDB main header file
#include "dirdbAccess.h"	// DIRDB access functions
#include "dirdbWalk.h"		// DIRDB dir walking functions
#include "writeDir.h"
//
DIRDB dirdb;

//
int dirdb_inited=0;

//
void pause(const char *s)
{
	printk(s);
	ESCLOCK();
}

// Initialise DIRDB
int dirdb_init(void)
{
	// --------------------------------------
	int i,i2,i3,i4;

	//
	if(dirdb_inited)return 1;
	dirdb_inited = 1;

	// --------------------------------------
	// Clear structure
	memset(&dirdb, 0, sizeof(DIRDB));

	// Set amount of directories in cache
	dirdb.total =		MAX_DIRS_CACHED;

	// Set entries amount to zero
	dirdb.n_entries =	0;

	// Set state to free(for concurrency handling)
	dirdb.state =		0;

	// Set all directory entries free
	for(i=0; i<dirdb.total; i++)
	{
		// Set guard variable
		dirdb.dir[i].guard =	DIRDB_GUARD;
		// Set entry free
		dirdb.dir[i].isfree =	1;
		// Allocate data for dir data buffer
		dirdb.dir[i].buf =	malloc(MAX_BYTES_PER_DIR);
		dirdb.dir[i].l_buf =	MAX_BYTES_PER_DIR;
		// Set type to directory
		dirdb.dir[i].type =	ID_DIRECTORY;
	}

	// No cache flush request at the start up.
	dirdb.cacheFlushRec = 0;

	//
	return 0;
}


