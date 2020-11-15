//
#include <stdio.h>

// fflush - flush a stream
// The  function  fflush  forces  a  write of all buffered data for
// the given output or update stream via the stream's underlying write function.
// The open status of the stream is unaffected.
int fflush(FILE *stream)
{
	//
	if(stream==NULL)
		return EBADF;

	// Stream was a write one?
	if(!stream->readonly)
	{
		// Write to the current ftell() location.
		// Write buffer directly to file.
		wbuf(stream);
	}
	else return EBADF;

	//
	return 0;
}


