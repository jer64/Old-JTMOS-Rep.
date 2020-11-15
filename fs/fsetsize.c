//
#include <stdio.h>

// OBSOLETE FUNCTION
// Fixed thread base.
// Added support for non-kernel memory callers.
int _fsetsize(const char *fname, int length)
{
	int r;
	int ack,rv;

	//
	if(!fname)
		return 0;
	if(!strlen(fname))return 0;

//	rv = setsize(GetCurrentDNR(), length, GetCurrentDB());

	//
	return rv;
}


