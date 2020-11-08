// ================================================
// Direct Disk access functions.
// Non-portable, for internal use.
// (C) 2002-2003 by Jari Tuominen.
// ================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "directdisk.h"

//
DWORD getdevnrbyname(const char *name)
{
        //
	if(!name)
		return 0;
	StringCheck(name);
	if(!strlen(name))return 0;

	//
	return scall(SYS_getdevnrbyname, name,0,0, 0,0,0);
}

//
DWORD readblock(int devnr, int block, char *buf)
{
	static POSTMANCMD post;
	int ack;

	//
	post.id=PCMD_READBLOCK;
	post.v1=devnr;
	post.v2=block;
	post.par1=buf;

	//
	return PostmanPost(&post);
}

//
DWORD writeblock(int devnr, int block, char *buf)
{
	static POSTMANCMD post;
	int ack;

	//
	post.id=PCMD_WRITEBLOCK;
	post.v1=devnr;
	post.v2=block;
	post.par1=buf;

	//
	return PostmanPost(&post);
}


