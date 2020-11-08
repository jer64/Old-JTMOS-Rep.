//
#ifndef __INCLUDED_FILEDEF_H__
#define __INCLUDED_FILEDEF_H__

/* The possibilities for the third argument to `setvbuf'.  */
#define _IOFBF	0x1		/* Full buffering.  */
#define _IOLBF	0x2		/* Line buffering.  */
#define _IONBF	0x4		/* No buffering.  */

//
#define	SEEK_SET	0	/* Seek from beginning of file.  */
#define	SEEK_CUR	1	/* Seek from current position.  */
#define	SEEK_END	2	/* Seek from end of file.  */

//
#ifndef EOF
#define EOF (-1)
#endif
#ifndef BUFSIZ
#define BUFSIZ 1024
#endif

//
#ifndef size_t
#define size_t unsigned long int
#endif

//
typedef struct
{
	//
	int fd;
	// from libc
	char	*ptr;
	int	cnt;
	char	*base;
	int	flag;
	int	file;
	int	charbuf;
	char	tmpfname[256];
	char	strmode[10];
	// buffering
	int	mode;
	unsigned char	*buf;		// all buffers defined
	int	bufsiz;		// number of buffers currently allocated
	int	fsize;
	int	offs;		// current offset
	int	baseOffs;	// current base offset
	char	readonly; // if set, stream is readonly(f.e. "rb")
	char	hasRead; // for fgetc
	char	eof; // End of file? TRUE or FALSE.
	char	writeback; // Indicates that a write flush must be done when closing the file.
}FILE;

//
typedef FILE _IO_FILE;

/* Error Codes */
#define EZERO           0
#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define ENOTBLK         15
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define ETXTBSY         26
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EUCLEAN         35
#define EDEADLOCK       36

//
#include "filebuf.h"

#endif


