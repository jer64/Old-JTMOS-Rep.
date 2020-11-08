#ifndef __INCLUDED_FSWALK_H__
#define __INCLUDED_FSWALK_H__

// Use DIRDB functions
#define MODERN_WALK

//
#include "jtmfsfe.h" // JTMFS_FILE_ENTRY, DIRWALK
#include "dirdb.h" // DIR

//
int jtmfs_stepover(int device_nr,DIRWALK *dw);
int jtmfs_fnext(int device_nr,DIRWALK *dw);
void jtmfs_fend(DIRWALK *dw);
int jtmfs_ffirst(int device_nr,
		int dirblock, DIRWALK *dw);

#endif

