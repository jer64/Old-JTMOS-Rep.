//
#ifndef __INCLUDED_JTMFSFE_H__
#define __INCLUDED_JTMFSFE_H__

//
#include "basdef.h"

//====================================================================================
//
// DIRECTORY (ACTUAL DIRECTORY/CACHE ENTRY) -STRUCTURE DEFINITION.
//
typedef struct
{
	// Guard variable
	DWORD guard;

	// Set to non-zero value if this entry is allocated,
	// zero will indicate that the entry is free.
	char isfree;

	// Some data changed since last flush.
	// This variable is mostly useful for the dirdb
	// cache flush process.
	char flushRequired;

	// Running number that is used for checking
	// that the directory is the same as we were
	// accessing before.
	// E.g. cache changes might happend in middle.
	int run;

	// Type of item
	int type;

	// Number of entries currently exists on this dir
	int n_entries;
	// Buffer where to store the directory
	BYTE *buf;
	int l_buf;

	// -1 = UNKNOWN
	// Other values point to the entry # of an empty entry
	int empty_entry;
	int blocks;

	// Directory Block #, Device #
	int db,dnr;

	// Block offset
	struct
	{
		int s,e;
	}offs;
}DIR;


// 50 bytes
typedef struct
{
        // 33 characters max. per file name + zero char.
        char            name[34];
        // Health indicator.
        DWORD           magicNumber;    // 0xFCE2E37B
        // File type.
        WORD            type;
        // Length of file, -1 means deleted entry(aka ghost).
        int             length;
        // First data block(FAT chain).
        DWORD           FirstDataBlock;
        // Last data block(FAT chain).
        DWORD           LastDataBlock;
}JTMFS_FILE_ENTRY;

// This structure is used to make easier to read dirs generally
typedef struct
{
	//
	int current_block;
	int start_block,end_block;
	// Device # & directory block #
	int device_nr,db;
	// Pointer to the directory
	DIR *d;
	// Entry #
	int entry;

	//
	int isWalking;
	int loaded_block;
	int current_offset;
	BYTE buf[16384]; // enlarge for bigger chunk sizes
	int buf_length;
	JTMFS_FILE_ENTRY        *fe;
}DIRWALK;

// !: Some parameters not implemented at this time.
/*	// File protection
//	DWORD           protection;
	// Owner
//	BYTE            owner;
	// Creation date
//	DWORD           creationdate;
	// Last access date
//	DWORD           lastaccessdate;
	// Last change date
//	DWORD           lastchangedate;*/

/*
//
typedef struct
{
        // 32 bytes, so max. length is 31 characters :
	char            name[20];
//	DWORD		nameChecksum32; // used for hash table
	WORD            type;
	DWORD           length;
	DWORD           protection;
	BYTE            owner;
	DWORD           creationdate;
	DWORD           lastaccessdate;
	DWORD           lastchangedate;
	DWORD           FirstDataBlock;
	DWORD           LastDataBlock;
	BYTE            unused;
}JTMFS_FILE_ENTRY;
*/

#endif


