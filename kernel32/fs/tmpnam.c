// tmpnam.c
#include <stdio.h>

//
char *tmpnam(char *s)
{
	int n;

	if(s==NULL)return NULL;

	for(; ;)
	{
		n = rand()&0xFFFFFF;
		sprintf(s, "temp%x.tmp", n);
		if( !exist(s) )break;
	}
	return s;
}

