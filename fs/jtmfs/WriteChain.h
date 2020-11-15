#ifndef __INCLUDED_WRITECHAIN_H__
#define __INCLUDED_WRITECHAIN_H__

//
int jtmfs_WriteFileChain(BYTE *buf, int dnr, int _d,
                int preciseLength,
                int offset);
int jtmfs_WriteFileChainEx(BYTE *buf, int dnr, int _d,
                int preciseLength, int offset, int useCache);

#endif


