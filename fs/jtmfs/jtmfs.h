#ifndef __INCLUDED_JTMFS_H__
#define __INCLUDED_JTMFS_H__

//
#include "basdef.h"
#include "jtmfat.h"
#include "devapi.h"
#include "api.h"
#include "jtmfsPath.h"
#include "flexFat.h"
#include "dirtype.h"

//
#define DEBUGLINE         dprintk("[%s / %s  Line %d]\n", \
                        __FILE__, __FUNCTION__, __LINE__);

//
#define JTMFSDEBUG
//#define __JTMFS_DEBUG__

//
#define ENTRYLMT        (0x200)

//
typedef struct
{
	char line[256];
	char function[256];
	char file[256];
}DEBUGSTR;
extern DEBUGSTR debugstr;

#define DEBUGINFO	dprintk("Function=%s, Line=%s, File=%s\n",	\
				debugstr.function,			\
				debugstr.line,				\
				debugstr.file);


//
#define DEBUGFUNC	sprintf(debugstr.line, "%d", __LINE__);			\
			strcpy(debugstr.function, __FUNCTION__);	\
			strcpy(debugstr.file, __FILE__);

//
/*#define DExpandChain(x,y)	DEBUGFUNC \
				jtmfs_expandchain(x,y)*/
#define DExpandChain jtmfs_expandchain

//
int jtmfs_expandchain(int device_nr,int amount);

// JTMFS_FILE_ENTRY
#include "jtmfsfe.h"

// DIRWALK structure
#include "fswalk.h"

//
#include "jtmfsAccess.h"

//
int jtmfs_deletechain(int device_nr,int blocknr);
void jtmfs_inc_try(void);
void jtmfs_init_try(void);
void jtmfs_ffupdatefe(DIRWALK *dw);

int jtmfs_fentry(       JTMFS_FILE_ENTRY *e,
                        const char *name,
                        unsigned short int      type,
                        unsigned int       length,
                        unsigned char           protection,
                        unsigned char           owner,
                        unsigned int       creationdate,
                        unsigned int       lastaccessdate,
                        unsigned int       lastchangedate,
                        unsigned int       FirstDataBlock,
                        unsigned int       LastDataBlock);

int jtmfs_ffindfile(int device_nr,const char *fname,int block_nr,DIRWALK *dw);
int jtmfs_cedir(JTMFS_FILE_ENTRY *e,const char *name,
        int FirstDataBlock,int LastDataBlock);
void jtmfs_cedirend(JTMFS_FILE_ENTRY *);
int jtmfs_createdirectory(int device_nr,int dirblock, int backdb,
		const char *name);
int jtmfs_getrootdir(int device_nr);
int jtmfs_createrootdir(int device_nr);
int jtmfs_formatdrive(int device_nr);
int jtmfs_dirroot(int device_nr);
int jtmfs_ffIsLast(int device_nr,DIRWALK *);
int jtmfs_clearwbuf(int device_nr,DIRWALK *);
int jtmfs_ffreadlocation(int device_nr,DIRWALK *);
int jtmfs_ffwritelocation(int device_nr,DIRWALK *);
int jtmfs_addentry(int device_nr,
		int,JTMFS_FILE_ENTRY *);
void EndDirWalk(DIRWALK *dw);
int jtmfs_getfilesize(int device_nr,const char *fname,int dblock);
int jtmfs_getfileblocksize(int device_nr,const char *fname,int dblock);
void InitDW(DIRWALK *dw);
int jtmfs_fexist(int dnr,char *fname,int dblock);
int jtmfs_renamefile(int device_nr,
                const char *OldName, const char *NewName,
                        int dirblock);
int jtmfs_setsize(int device_nr,
                const char *OldName, int newSize,
                        int dirblock);
int jtmfs_entryIsDeleted(JTMFS_FILE_ENTRY *fe);

//
#include "dir.h"

#endif

