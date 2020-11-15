//
#include <stdio.h>

// Get file size
int msdos_fgetsize(SYSCALLPAR)
{
	// I first forgot static here.
	int actualsize,ack,rv;
	char *pathname;

	//
	pathname = scall_ebx;

	//
	if(!pathname)
		return 0;

	//
	if(!strlen(pathname))return 0;

	//
	rv = _msdos_getfilesize(GetCurrentDNR(), pathname, GetCurrentDB());

	//
	return rv;
}

//
int _msdos_getfilesize(int dnr, const char *pathname, int db)
{
	//
	
}
