//-----------------------------------------------------------------------
// These functions send postman packets.
//-----------------------------------------------------------------------
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "postcommands.h"

// ==========================================================
DWORD post_exist(POSTMANCMD *post,const char *fname)
{
	//
	if(!XON_POSTMAN)	return 0;

	//
	if( !(fname && isValidString(fname) && strlen(fname)) )
		return 0;

	//
	post->id =		PCMD_EXIST;
	post->par1 = 		fname;
	post->acknowledged =	0;
	return scall(SYS_POSTCMD, post,0,0, 0,0,0);
}

// Returns fd
DWORD post_findfirst(POSTMANCMD *post, const char *fname, VFAT_FFBLK *ff)
{
	int r;

	//
	if( !(fname && isValidString(fname) && strlen(fname)) )
		return 0;

	//
	printk("post_findfirst\n");
	post->id =		PCMD_FINDFIRST;
	post->par1 = 		fname;
	post->par2 =		ff;
	post->acknowledged =	0;
	return scall(SYS_POSTCMD, post,0,0, 0,0,0);
}

//
DWORD post_findnext(POSTMANCMD *post, int fd, VFAT_FFBLK *ff)
{
	//
	post->id =		PCMD_FINDNEXT;
	post->v1 = 		fd;
	post->par1 =		ff;
	post->acknowledged =	0;
	return scall(SYS_POSTCMD, post,0,0, 0,0,0);
}

//

