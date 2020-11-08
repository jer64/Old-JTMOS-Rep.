// =========================================================
// File handling functions
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
//
// TODO: buffers.
// =========================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "filebuf.h"
#include "fcntl.h"
// Enable or disable file traceback
//#define TRACE printf
#define TRACE //

// Allocated at the start up
FILE *stdout,*stdin,*stderr;

//
int putc(int c, FILE *stream)
{
        return fputc(c, stream);
}

//
void clearerr(FILE *stream)
{
	return;
}

//
int ferror(FILE *stream)
{
	return 0;
}

//
int fileno(FILE *stream)
{
	return 0;
}

//
void rewind(FILE *f)
{
	fseek(f, 0, SEEK_SET);
}

// fexist
//
// Return values:
// 2:		Directory.
// 1:		File.
// 0,-1:	Not found.
int fexist(const char *pathname)
{
	//
	return __fexist(pathname);
}

// This is unstandard function,
// that's why it has '__' in the name.
int __fexist(const char *pathname)
{
	int fd;

	//
	fd = open(pathname, 0);
	if(fd<0)
		return FALSE;
	close(fd);
	return TRUE;
}

// I had problems with badly formatted strings.
// This is why JTMOS LIBC functions nowadays
// work so that these first checks the file names
// that are passed to them.
//
// Badly terminated strings lead into
// program termination.
//
int StringCheck(const char *s)
{
	int i;
	if(!isValidString(s))
	{
		LIBCERROR("LIBC failure: StringCheck s=non-null terminated string");
	}
	return 0;
}

//---------------------------------------------------------------------------------------------------
// Get file size
int fsizeof(const char *pathname)
{
	int fd,sz;

	//
	fd = open(pathname, 0);
	if(fd<0)
		return -1;
	//
	sz = lseek(fd, 0, SEEK_END);
	//
	close(fd);

	//
	return sz;
}

//---------------------------------------------------------------------------------------------------
// int rename(const char *OLD, const char *NEW);
//
int rename(const char *OldName, const char *NewName)
{
	//
	return scall(SYS_rename, OldName,NewName,0, 0,0,0);
}

// Synonym to remove.
int unlink(const char *pathname)
{
	//
	return remove(pathname);
}

// Remove a file
int remove(const char *pathname)
{
	//
	return scall(SYS_remove, pathname,0,0, 0,0,0);
}

//
FILE *__fnewstream(void)
{
	//
	FILE *new_structure;

	//
	new_structure = NULL;

	// Create & clear new FILE structure
	new_structure = malloc(sizeof(FILE));
	memset(new_structure, 0, sizeof(FILE));

	//
	return new_structure;
}

// Read string from stream to a buffer
int fgets(char *s, int maxv, FILE *stream)
{
	int i,c;

	// Emulate stdin
	if(stream==stdin)
	{
		// ?? TODO ??
		// Shouldn't we have a getsEx with a limiter?
		gets(s);
		return 0;
	}

	//
	for(i=0; i<maxv; i++)
	{
		//
		c = fgetc(stream);
		if(c==EOF)break;
		if(c=='\n')break;
		if(!c)break;
		s[i] = c;
	}
	s[i]=0;

	//
	if(c==EOF)
		return EOF;
	else
		return 0;
}

//
int fputs(const char *s, FILE *fp)
{
	//
	int i,l;

	//
	if(fp->readonly)return EOF;

	// Redirect to fprintf
	l=strlen(s);
	for(i=0; i<l; i++) { fputc(s[i],fp); }

	//
	return 0;
}

// Wrapper.
int read(int fd, void *buf, int count)
{
	//
	return scall(SYS_read, fd,buf,count, 0,0,0);
}

// Wrapper.
int write(int fd, const void *buf, int count)
{
	//
	return scall(SYS_write, fd,buf,count, 0,0,0);
}

// Wrapper.
int open(const char *pathname, int flags, ...)
{
	//
	return scall(SYS_open, pathname,flags,0, 0,0,0);
}

// Wrapper.
int close(int fd)
{
	return scall(SYS_close, fd,0,0, 0,0,0);
}

// Wrapper.
int lseek(int fd, int offset, int whence)
{
	return scall(SYS_lseek, fd,offset,whence, 0,0,0);
}

// RETURN VALUES
//        fread  and fwrite return the number of items successfully read or written (i.e., not the number of charac­
//        ters).  If an error occurs, or the end-of-file is reached, the return value is  a  short  item  count  (or
//        zero).
//
//        fread  does  not  distinguish between end-of-file and error, and callers must use feof(3) and ferror(3) to
//        determine which occurred.
//
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	FILE *f;
	char *tmp;
	int i,i2,i3,i4,l,d;

	//
	if(stream==stdin)
	{
		gets(ptr);
		return strlen(ptr);
	}

	//
	l =	size*nmemb;
	tmp =	ptr;
	for(i=0; i<l; i++)
	{
		d = fgetc(stream);
		if(d==EOF)break;
		tmp[i]=d;
	}

	//
	return i;
}

// Buffered fwrite.
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	int i;
	char *buf;
	char *p;

	// Read only stream?
	if(stream->readonly)return 0;

	// Emulate stderr/stdout
	TRACE("X"); //
	if(stream==stderr || stream==stdout
		|| stream==stdin)
	{
		p = ptr;
		for(i=0; i<(size*nmemb); i++)
		{
			printf("%c", p[i]);
		}
		return size*nmemb;
	}

	//
	StringCheck(stream->tmpfname);

	//
	TRACE("Y"); //
	if(!strlen(stream->tmpfname))
	{
		//
		//LIBCERROR("fwrite: f->tmpfname has length of zero(f=0x%x)", f);
		printf("%s: Warning f->tmpfname has zero length!\n",
			__FUNCTION__);
		return 0;
	}
	
	// Non-stdXXX:
	TRACE("Z"); //
	buf=ptr;

	// Write to stream
	for(i=0; i<(size*nmemb); i++) { fputc(buf[i],stream); }
	return size*nmemb;
}

// Format specified drive.
// (ASCII based drive specification)
// formatDrive("hda");
int formatDrive(const char *drive)
{
	int dnr;

	//
	dnr = getdevicenrbyname(drive);

	// Format now
	return _formatDrive(dnr);
}

// Format drive (number specified argument)
int _formatDrive(int dnr)
{
	//
	scall(SYS_mkfs, dnr,0,0, 0,0,0);
}

//
