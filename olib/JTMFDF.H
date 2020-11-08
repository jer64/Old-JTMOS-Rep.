/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/**FROM STDIO, and converted to JTMOS - Probally most needed stuff..**/
#define _IOFBF    	00001
#define _IONBF    	00002
#define _IOLBF    	00004

#define BUFSIZ		16384
#define EOF		(-1)
#define FILENAME_MAX	260
#define FOPEN_MAX	20
#define L_tmpnam	260
#define NULL		0
#define TMP_MAX		999999

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

typedef struct {
  int   _cnt;
  char *_ptr;
  char *_base;
  int   _bufsiz;
  int   _flag;
  int   _file;
  char *_name_to_remove;
} FILE;

/**************** DIR.H ****************/
struct ffblk {
  char lfn_magic[6] __attribute__((packed));			/* LFN */
  short lfn_handle __attribute__((packed));			/* LFN */
  unsigned short lfn_ctime __attribute__((packed));		/* LFN */
  unsigned short lfn_cdate __attribute__((packed));		/* LFN */
  unsigned short lfn_atime __attribute__((packed));		/* LFN */
  unsigned short lfn_adate __attribute__((packed));		/* LFN */
  char _ff_reserved[5] __attribute__((packed));
  unsigned char  ff_attrib __attribute__((packed));
  unsigned short ff_ftime __attribute__((packed));
  unsigned short ff_fdate __attribute__((packed));
  unsigned long  ff_fsize __attribute__((packed));
  char ff_name[260] __attribute__((packed));
};

#define FA_RDONLY       1
#define FA_HIDDEN       2
#define FA_SYSTEM       4
#define FA_LABEL        8
#define FA_DIREC        16
#define FA_ARCH         32

