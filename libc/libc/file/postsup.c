// ===================================================
// postsup.c - LIBC POSTMAN SUPPORT.
// (C) 2002-2005 by Jari Tuominen(ehc50@kanetti.fi).
// ===================================================
#include "basdef.h"
#include "file.h"
#include "postman.h"
#include "postsup.h"

//
int PostmanPost(POSTMANCMD *p)
{
	//
	printf("%s: FEATURE TEMPORARILY DISABLED.\n",
		__FUNCTION__);
	abort();

	//
	p->magic = POSTMAN_MAGIC;

	//
	return scall(SYS_POSTCMD, p,0,0, 0,0,0);
}

// OBSOLETE FUNCTION:
/*// PostmanWaitAck
// Will return the proper result.
int PostmanWaitAck(POSTMANCMD *p)
{
	int i;

	//
	for(i=0; ; i++)
	{
		//
		if(*p->acknowledged)break;
		SwitchToThread();
	}

	//
	return *p->rv;
}*/



