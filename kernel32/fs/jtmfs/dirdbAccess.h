#ifndef __INCLUDED_DIRDBACCESS_H__
#define __INCLUDED_DIRDBACCESS_H__

//
#define DIRDEBUG	dprintk("[%s / %s  Line %d]\n", \
			__FILE__, __FUNCTION__, __LINE__);

//
#include "kernel32.h"
#include "dirdb.h"
//
DIR *touchDir(int dnr, int db, int type);
DIR *dbGiveDir(int dnr, int db, int type);
DIR *copyDirToDB(int dnr, int db, int type);
int readDir(DIR *d);
int countEntries(DIR *d);
int dirdbFlush(void);
void dirdb_clearCacheForDNR(int dnr);
void dirdb_DiskChange(int dnr);

#endif

