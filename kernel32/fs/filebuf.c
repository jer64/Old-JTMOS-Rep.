// =======================================================================
// File buffering functions.
//
// (C) 2002-2005 by Jari Tuominen(jari.tuominen@kanetti.fi).
//
// Some of these buffering functions(3 of these),
// simply implement setvbuffer -function.
// =======================================================================
#include <stdio.h>
#include <fsdos.h>
// Enable or disable file traceback
//#define TRACE printf
#define TRACE //

/** ENABLE OR DISABLE FILEBUF DEBUG MESSAGES **/
// #define FILEBUF_DEBUG
/**********************************************/

#define filebuf_debug //
////#define filebuf_debug printf

// This function is called on buffer refresh need.
// (for read "r" mode only)
int rbuf(FILE *stream)
{
	//
	lseek(stream->fd, stream->baseOffs, SEEK_SET);
	return read(stream->fd, stream->buf, stream->bufsiz);
}

// This function is called on buffer refresh need.
// (for write mode only)
int wbuf(FILE *stream)
{
	//
	stream->writeback = FALSE;
	//
	lseek(stream->fd, stream->baseOffs, SEEK_SET);
	return write(stream->fd, stream->buf, stream->offs);
}

// Enlarge buffer
int __fenlargebuf(FILE *stream, int newsz)
{
	char *tmp;

	//
	if(  newsz >= stream->bufsiz  )
	{
		// Expand buffer when needed.
		filebuf_debug("%s: malloc\n", __FUNCTION__);
		TRACE("<MA %d>", newsz); //
		tmp = malloc(newsz);
		TRACE("</MA>"); //
		if(tmp==NULL)
		{
			panic("__fenlargebuf error");
		}
		filebuf_debug("%s: memcpy\n", __FUNCTION__);
		TRACE("<CPY>"); //
		memcpy(tmp, stream->buf, stream->bufsiz);
		TRACE("</CPY>"); //

		//
		TRACE("<FR>"); //
		free(stream->buf);
		TRACE("</FR>"); //
		stream->buf=tmp;
		stream->bufsiz=newsz;
		filebuf_debug("%s: done\n", __FUNCTION__);
	}
	return 0;
}

// Read next character from stream
int fgetc(FILE *stream)
{
	int d;

#ifndef JTMOSKERNEL
	// == STDIN emulation ==
	if(stream==stdin)
	{
		return getch();
	}
#endif

	// End of file check (TODO).
	if( stream->offs<0 || (stream->offs+stream->baseOffs)>=stream->fsize )
	{
		// End of file reached
	/*	printf("%s: End of file at %d, file size=%d.\n",
			__FUNCTION__,
			stream->offs+stream->baseOffs,
			stream->fsize);*/
		stream->eof=1;
		return EOF;
	}

	// Do we already have some data in the buffer?
	if( !stream->hasRead )
	{
		stream->hasRead=TRUE;
		rbuf(stream);
	}

	// No end of file.
	stream->eof=0;
	// Store byte
	d = stream->buf[stream->offs];
	// Increase offset.
	stream->offs++;

	// Exceeding buffer area?
	if(stream->offs>=stream->bufsiz)
	{
		// Load new area to the buffer
		stream->baseOffs+=stream->bufsiz;
		stream->offs=0;
		rbuf(stream);
	}

	//
	return d;
}

// fputc function
int fputc(int c, FILE *stream)
{
	char *tmp;

	// -------------------------------------------------
	// Standard streams?
#ifndef JTMOSKERNEL
	if(stream==stderr || stream==stdout)
	{
		//
		printf("%c", c);
		return 0;
	}
#endif

	// -------------------------------------------------
	// Buffer PTR / size check
	if(!stream->bufsiz || !stream->buf)
		return EOF;

	// Do we already have some data in the buffer?
	if( !stream->hasRead )
	{
		rbuf(stream);
		stream->hasRead=TRUE;
	}

	// Write byte on buffer.
	stream->buf[stream->offs++] = c;
	// Indicate a need for write back.
	stream->writeback = TRUE;

	// Exceeding buffer area?
	if(stream->offs >= stream->bufsiz)
	{
		// Flush write & re-used buffer
		wbuf(stream);
		stream->baseOffs += stream->bufsiz;
		stream->offs = 0;
	}

	// Determine new file size we're extending the file size.
	if( (stream->offs+stream->baseOffs)>stream->fsize )
	{
		stream->fsize = stream->offs+stream->baseOffs;
	}

	//
	return 0;
}

/*
       The setvbuf function may be used at any time on any open stream to change its buffer.  The mode  parameter
       must be one of the following three macros:
              _IONBF unbuffered
              _IOLBF line buffered
              _IOFBF fully buffered
*/

//
void setbuf(FILE *stream, char *buf)
{
	//
	setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}

//
void setbuffer(FILE *stream, char *buf, size_t size)
{
	//
	setvbuf(stream, buf, buf != NULL ? _IOFBF : _IONBF, size);
}

//
void setlinebuf(FILE *stream)
{
	//
	return setvbuf(stream, NULL, 1, 0);
}

//
int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	//
	if(!stream || !buf)return EOF;

	//
	stream->mode = mode;
	stream->bufsiz = size;
	stream->buf = buf;
	stream->offs = 0;

	//
	return 0;
}

//

