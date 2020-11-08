#ifndef __INCLUDED_FILEFIND_H__
#define __INCLUDED_FILEFIND_H__

#include "basdef.h" // DWORD

typedef struct
{
        DWORD _ff_reserved[5];
        DWORD ff_attrib;
        DWORD ff_ftime;
        DWORD ff_fdate;
        DWORD ff_fsize;
        char ff_name[260];
}FFBLK;

int findfirst(const char *path, FFBLK *ff);
int findnext(int fd, FFBLK *ff);

#endif
