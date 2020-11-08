#ifndef __INCLUDED_FDCCACHE_H__
#define __INCLUDED_FDCCACHE_H__

#include "kernel32.h"

void fdcClearCache(void);
BYTE *giveBlock(int which, int readIt);
int fd_read_block(int block, BYTE *buf);
int fd_write_block(int block, BYTE *buf);
void fdcFlushCache(void);
void fdcFlushCacheEnt(FDCENT *ent);

#endif


