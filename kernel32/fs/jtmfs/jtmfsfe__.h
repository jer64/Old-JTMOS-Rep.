#ifndef __INCLUDED_JTMFSFE_H__
#define __INCLUDED_JTMFSFE_H__

//
#include "basdef.h"

//
typedef struct
{
        // 32 bytes, so max. length is 31 characters :
        char            name[31];
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

#endif
