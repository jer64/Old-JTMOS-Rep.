//
#ifdef JTMOSKERNEL
#include "kernel32.h"
#include "filebuf.h"
#include "filedef.h"
#else
#include <stdio.h>
#endif

//
int fclose(FILE *stream)
{
	//
	if(stream==NULL)return EOF;

	// ------------------------------------------------------------
	// FLUSH STREAM IF IT IS NON-READONLY.
	if(!stream->readonly)
	{
		// Flush buffer contents.
		fflush(stream);
	}

	// ------------------------------------------------------------
	// Do close() after fflush, not before.
	if(stream->fd >= 0)
	{
		close(stream->fd);
		stream->fd = -1;
	}

	// ------------------------------------------------------------
	// DEALLOCATION
	//
	// Deallocate the structure's dynamically allocated members.
	// If any needed to.
	if(stream->buf)free(stream->buf);
	setbuffer(stream, NULL,0);

	// Lastly deallocate the stream structure.
	free(stream);

	//
	return 0;
}
