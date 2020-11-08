//
#ifndef __INCLUDED_MULTIACCESS_H__
#define __INCLUDED_MULTIACCESS_H__

//
#define JTMOS_EXE_FMT   ".bin"
#define JTMOS_PATH      "/ram/bin;/bin;/usr/bin;ram;"

//
#define MADEBUG        dprintk("[%s / %s  Line %d]\n", \
                        __FILE__, __FUNCTION__, __LINE__);
//
#include "kernel32.h"
#include "cmos.h"
#include "rtc.h"

//
int globalcopy(int srcPID, DWORD srcOffs,
        int dstPID, DWORD dstOffs,
        int amount);
int SearchThroughPath(char *path, char *name, char *found);
// open, close, read, write, lseek
int registerNewDevice(const char *name,
        void *call,int cs,
        int pid,int type,
        int alias);
void startMessageBox(void);

           struct dirent {
               int          d_ino;       /* inode number */
               int          d_off;       /* offset to the next dirent */
               unsigned short d_reclen;    /* length of this record */
               unsigned char  d_type;      /* type of file; not supported
                                              by all file system types */
               char           d_name[256]; /* filename */
           };

#endif


