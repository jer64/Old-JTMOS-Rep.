//
#include <stdio.h>
#include "file.h"

// Allocated at the start up
FILE *stdout,*stdin,*stderr;

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

//
int putc(int c, FILE *stream)
{
        return fputc(c, stream);
}
