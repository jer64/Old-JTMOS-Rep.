//
#ifndef __INCLUDED_STAT_H__
#define __INCLUDED_STAT_H__

//
#include "statext.h"

//
#define dev_t long
#define ino_t long
#define mode_t long
#define nlink_t long
#define uid_t long
#define gid_t long
#define dev_t long
#define off_t long
//
#define time_t long

//
struct stat
{
    dev_t         st_dev;      /* device */
    ino_t         st_ino;      /* inode */
    mode_t        st_mode;     /* protection */
    nlink_t       st_nlink;    /* number of hard links */
    uid_t         st_uid;      /* user ID of owner */
    gid_t         st_gid;      /* group ID of owner */
    dev_t         st_rdev;     /* device type (if inode device) */
    off_t         st_size;     /* total size, in bytes */
    unsigned long st_blksize;  /* blocksize for filesystem I/O */
    unsigned long st_blocks;   /* number of blocks allocated */
    time_t        st_atime;    /* time of last access */
    time_t        st_mtime;    /* time of last modification */
    time_t        st_ctime;    /* time of last change */
};

//
int msdos_stat(SYSCALLPAR);
int _msdos_stat(const char *name, struct stat *s);
int msdos_fstat(SYSCALLPAR);

#endif




