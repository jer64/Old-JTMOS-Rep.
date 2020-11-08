#ifndef __INCLUDED_DIRDB_H__
#define __INCLUDED_DIRDB_H__

//
#include "jtmfsfe.h"; // DIR, DIRWALK, file entry structure definition, and so on.

//
#define DIRDB_GUARD	0xFCE2E38B

//
#define RV_DIRFULL	-1984

// Types
#define ID_DIRECTORY	0x10
#define ID_FILE		0x20

//
#include "kernel32.h"

//====================================================================================
// >> Preferences for directory database system
#define MAX_CACHED_FILE_SIZE	(1024*1024*2) // 2 megabytes at most
#define MAX_DIRS_CACHED		400
#define MAX_ENTRIES_PER_DIR	200
#define MAX_BYTES_PER_DIR	(sizeof(JTMFS_FILE_ENTRY)*MAX_ENTRIES_PER_DIR)
//
//#define FILE DIR

// DIRECTORY DATABASE MAIN STRUCTURE
typedef struct
{
	// 0 =	DB is free for access
	// 1 =	DB is reserved
	// 2 =	DB is free for database flush operation
	//	and is reserved for all other operations,
	//	after flush has been done state is set to l_state.
	int state;
	// Temporary variable
	int l_state;

	// Structures for cached directories
	DIR dir[(MAX_DIRS_CACHED+10)];
	// Number of directories currently registered in database
	int n_entries;
	// Number of total directories in database
	int total;

	// Set to non-zero when cache flush is recommended
	int cacheFlushRec;
}DIRDB;

//
extern DIRDB dirdb;

//
void pause(const char *s);

//
#include "dirdbCorruption.h"

#endif


