//
#include <stdio.h>

// Seek to a new position on current stream
int fseek(FILE *stream, int offset, int whence)
{
	int fd;

	//
	stream->eof = 0;

	// Flush if necessary
	//if(!stream->readonly && stream->writeback)
	if(!stream->readonly)
		fflush(stream);

	//
	switch(whence)
	{
		//
		case SEEK_SET:
		stream->baseOffs = offset;
		break;

		//
		case SEEK_CUR:
		stream->baseOffs += offset;
		break;

		//
		case SEEK_END:
		if(offset>=0)
			stream->eof=1;
	        fd = open(stream->tmpfname, O_RDONLY);
	        stream->baseOffs = lseek(fd, 0, SEEK_END)+offset;
	        close(fd);   
		stream->offs = 0;
		break;

		//
		default:
		return EBADF;
	}

	// Refresh, load new location.
	rbuf(stream);
	return 0;
}


