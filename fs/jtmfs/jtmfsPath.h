#ifndef __INCLUDED_JTMFSPATH_H__
#define __INCLUDED_JTMFSPATH_H__

//
typedef struct
{
	char name[256];
	char path[256];
}RESOLVE;

//
int resolveFileNamePath(const char *filepath,
                        int *dnr,int *db,
                        RESOLVE *r);
int resolvePath(const char *path, int *dnr,int *db);

#endif


