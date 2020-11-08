//
#ifndef __INCLUDED_POSTCOMMANDS_H__
#define __INCLUDED_POSTCOMMANDS_H__

//
#include "basdef.h"
#include "kernel32.h"
#include "postman.h"
#include "systemcalls.h" // FFBLK

//
DWORD post_exist(POSTMANCMD *post, const char *fname);
DWORD post_create(POSTMANCMD *post, const char *fname, int flags); // returns fd
DWORD post_findfirst(POSTMANCMD *post, const char *fname, VFAT_FFBLK *ff); // returns fd
DWORD post_findnext(POSTMANCMD *post, int fd, VFAT_FFBLK *ff);

#endif



