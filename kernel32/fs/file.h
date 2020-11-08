//
#ifndef __INCLUDED_FILE_H__
#define __INCLUDED_FILE_H__

//
#include <stdio.h>
#include "filedef.h"
//#include "stdarg.h"
//#include "filedir.h"
//#include "filefind.h"
//#include "stat.h"
//#include "fprintf.h"

//
int _fsetsize(const char *fname, int length);

//
int __fcreate(const char *path);
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *stream);
int remove(const char *pathname);
int putc(int c, FILE *stream);

//
extern FILE *stdout,*stdin,*stderr;

// File descriptor, low level functions
int fexist(const char *pathname);
int __fexist(const char *pathname);
//int open(const char *pathname, int flags, ...);
int close(int fd);

// LIBC functions
int _fgetsize(const char *pathname);
int __faddblocks(const char *pathname,long amount);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
FILE *__fnewstream(void);
int StringCheck(const char *s);
int fgets(char *s, int maxv, FILE *stream);
int rename(const char *OldName, const char *NewName);

//
int feof(FILE *f);
void clearerr(FILE *stream);
int ferror(FILE *stream);
int fileno(FILE *stream);
FILE *_fopen(const char *path, const char *mode, FILE *fff);
FILE *freopen(const  char *path,
        const char *mode, FILE *stream);
int lseek(int fd, int offset, int whence);

//
int formatDrive(const char *name);
int _formatDrive(int dnr);

//
//#include "jtmerrno.h"
//#include "postsup.h"
//#include "tmpnam.h"

//
int unlink(const char *pathname);
int fsizeof(const char *pathname);
void rewind(FILE *f);

// Global variables for file I/O API.
FILE *stdout,*stdin,*stderr;
//
FILE *__fnewstream(void);
int fgets(char *s, int maxv, FILE *stream);
int fputs(const char *s, FILE *fp);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
int putc(int c, FILE *stream);

//
#endif

//

