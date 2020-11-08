#include "basdef.h"
#include "console.h"
#include "file.h"
#include "tmpnam.h"

char *tmpnam(char *s)
{
	int n;

	if(s==NULL)return NULL;

	for(; ;)
	{
		n = rand()&0xFFFFFF;
		sprintf(s, "temp%x.tmp", n);
		if( !fexist(s) )break;
	}
	return s;
}

