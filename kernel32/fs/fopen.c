//=====================================================================================
// (C) 2003-2005-2020 by Jari Tuominen.
// 22.09.2003: Added append support.
//=====================================================================================
#include <stdio.h>
#include <fcntl.h>

//
FILE *fopen(const char *path, const char *mode)
{
	//
	return _fopen(path, mode, NULL);
}

// Allocates a FILE structure if a file is found,
// and returns a handle to it.
// TODO: Text or binary files don't differ yet when calling with e.g. "wt" or "rt".
FILE *_fopen(const char *path, const char *mode, FILE *initialPTR)
{
	FILE *ns;
	int i,i2,rv,seek_to;
	int fd,sz;

	//-------------------------------------------------------------------------
	// No seek required as default.
	// (seek_to is required for appending)
	//
	seek_to = -1;

	// Set initial structure PTR.
	ns = initialPTR;

	//-------------------------------------------------------------------------
	// Basic checks
	//
	if(!path || !mode)
		return NULL;
	if(!strlen(path))
		return NULL;

	// Ignore call if both read & write mode are selected.
	if( strchr(mode,'r') && strchr(mode,'w') )
		return NULL;

	//-------------------------------------------------------------------------
	// Create & clear new FILE structure.
	//
	if(ns==NULL)
		ns = malloc(sizeof(FILE));
	memset(ns, 0, sizeof(FILE));

	// Get file size.
	fd = open(path, O_RDONLY);
	ns->fsize = lseek(fd,0,SEEK_END);
	close(fd);

	//-------------------------------------------------------------------------
	// Mode determination
	//
	// Mode "wb" or "w" or "wt" leads into
	// a creation of file.
	// "rb" reads binary.
	// "wb" stands for write binary.
	// "ab" stands for append binary.

	// FILE APPEND MODE
	if( mode[0]=='a' )
	{
		// Append requires file to be created if necessary.
	        fd = open(path, O_RDWR);
	        sz = lseek(fd, 0, SEEK_END);
	        close(fd);
	        if(sz>0)
		{
			//printf("fopen: file %s does not exist, creating it(mode=%s)\n",
			//	path, mode);
			// When file does not exist, create it.
			if( (ns->fd = open(path, O_CREAT))<0 )
				// Creation failed...
				return NULL;

			// Get updated file size.
			fd = open(path, O_RDONLY);
			ns->fsize = lseek(fd,0,SEEK_END);
			close(fd);
		}

		// Get offset for appending.
		fd = open(path, O_RDONLY);
		seek_to = lseek(fd,0,SEEK_END);
		close(fd);
	}
	else
	// FILE WRITE MODE
	if( mode[0]=='w' )
	{
		// Create new file, will truncate old one.
		if( (ns->fd = open(path, O_CREAT))<0 )
			//
			return NULL;

		// Get file size.
		fd = open(path, O_RDONLY);
		ns->fsize = lseek(fd,0,SEEK_END);
		close(fd);
	}
	else
	// FILE READ MODE
	if( mode[0]=='r' )
	{
		//
		if( (ns->fd = open(path, 0))<0 )
			//
			return NULL;

		//
		if( ns->fsize<0 )
		{
			free(ns);
			return NULL;
		}
	}
	else
	{
		//
		free(ns);

		// Unknown mode.
		return NULL;
	}

	// Copy mode string.
	strcpy(ns->strmode, mode);

	// Check mode [READ OR WRITE STREAM].
	if( mode[0]=='w' || mode[0]=='a' ) { ns->readonly = FALSE; }
	else
	if( mode[0]=='r' ) { ns->readonly = TRUE; }
	// When R/W mode, set "read only = FALSE".
	if( strchr(mode, '+') )
		ns->readonly = FALSE;

	// Store file name.
	strcpy(ns->tmpfname, path);

	// Init buffers.
	ns->bufsiz = PRE_BUF_AMOUNT_K*1024;
	ns->buf = malloc(ns->bufsiz);
	if(ns->buf==NULL)
	{
		printf("%s: WARNING! Out of memory.\n",
			__FUNCTION__);
		free(ns);
		return NULL;
	}

	//
	ns->offs=0;
	ns->baseOffs=0;
	ns->writeback = FALSE;
	ns->hasRead = FALSE;

	// End of file = FALSE.
	ns->eof = FALSE;

	//-------------------------------------------------------------------------
	// Load some data from the file into memory.
//	frefresh(ns);

	//-------------------------------------------------------------------------
	// For append:
	//
	if(seek_to>0)
	{
		//
	//	printf("%s: seek_to = %d bytes.\n",
	//		__FUNCTION__, seek_to);
		// Note: fseek will call frefresh on it's own.
		fseek(ns, seek_to, SEEK_SET);
	}

	// Return PTR to the newly allocated FILE structure.
	return ns;
}


