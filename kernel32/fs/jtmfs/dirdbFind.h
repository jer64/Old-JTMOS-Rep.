//
#ifndef __INCLUDED_DIRDBFIND_H__
#define __INCLUDED_DIRDBFIND_H__

//
#include "kernel32.h"
#include "dirdb.h"

//
int dirdb_ffindfile(int dnr,
                const char *fname, int db, DIRWALK *dw);
int dirdb_ffindghostfile(int dnr,
                const char *fname, int db, DIRWALK *dw);
int dirdb_ffindfileEx(int dnr,
                const char *fname, int db, DIRWALK *dw,
                int ghost);
int dirdb_ffinddirname(int dnr, const char *name, int db, DIRWALK *dw);
int dirdb_GetDirName(int dnr,int db, char *dest_name);

#endif



