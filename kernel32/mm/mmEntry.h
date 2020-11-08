#ifndef __INCLUDED_MMENTRY_H__
#define __INCLUDED_MMENTRY_H__


//
#include <stdio.h>
#include "mm.h"

// Entry allocation function prototypes
MD *mmGiveMD(void *ptr);
MD *mmGetNewEntry(void);
MD *mmNewEntry(void);
MD *mmFindFreeEntry(void);
void *mmAddEntry(int sp, int pages, int bytes);
void mmRemoveEntry(MD *m);
int GetChunkOwner(void *p);
MD *mmDetermineOwner(void *p);


#endif
