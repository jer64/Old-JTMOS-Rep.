#ifndef __INCLUDED_DIRDBWALK_H__
#define __INCLUDED_DIRDBWALK_H__

#include "kernel32.h"
#include "jtmfs.h"
#include "dirdb.h"

int dirdb_stepover(int device_nr,DIRWALK *dw);
int dirdb_fnext(int device_nr,DIRWALK *dw);
int dirdb_ffirst(int device_nr,
                int dirblock, DIRWALK *dw);
int dirdb_flushRequirement(DIRWALK *dw);

#endif
