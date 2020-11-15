//
#include <stdio.h>

// Get file size
int _fgetsize(const char *pathname)
{
	// I first forgot static here.
	int actualsize,ack,rv;

	//
	if(pathname==NULL)
		return 0;

	//
	if(!strlen(pathname))return 0;
	
	//
	rv = jtmfs_GetFileSize(GetCurrentDNR(), pathname, GetCurrentDB());
	
	//
	return rv;
}
