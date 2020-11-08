//------------------------------------------------------------------------------------------
// Copy a region of memory to another place.
// (delivered from LIBC)
// (C) 2003-2004 by Jari Tuominen.
//------------------------------------------------------------------------------------------
#include "kernel32.h"
#include "memcpy.h"

//----------------------------------------------------------------------------------------------
// Verify address.
//
int illegal(DWORD ad)
{
	//
	if(paging_enabled)
	{
		//
		if( !(GetPage(ad/4096)&1) )
			// Illegal.
			return TRUE;
	}
	else
	{
		//
		if(ad>=0 && ad<=0x3FFF)
			// Illegal.
			return TRUE;
	}

	// Legal.
	return FALSE;
}

//----------------------------------------------------------------------------------------------
//
void *dmemcpy(void *o,void *s,DWORD l,
	char *whos_call)
{
	char str[256];

	//
	if( GetCurrentThread()!=0 && (illegal(o) || illegal(s)) )
	{
		sprintf(str, "Illegal memcpy call: memcpy(%x,%x, %d); called by %s",
			o,s,l, whos_call);
		panic(str);
	}

	//
	return xmemcpy(o,s,l);
}

//----------------------------------------------------------------------------------------------
// The real copy whos_callction.
//
void *xmemcpy(void *_o,void *_s,DWORD length)
{
	DWORD i;
	BYTE *o,*s;

	//
	o = _o;
	s = _s;

	//---------------------------------------------------------
	// Copy memory region.
	for(i=0; i<length; i++) { o[i]=s[i]; }

	//
	return o;
}

//----------------------------------------------------------------------------------------------
//
void *dmemset(char *buf,char ch,int length,
	char *whos_call)
{
	char str[256];

	//
	if( GetCurrentThread()!=0 && illegal(buf) )
	{
		sprintf(str, "Illegal memset call: memset(%x, %d, %d); called by %s",
			buf,ch,length, whos_call);
		panic(str);
	}

	//
	return xmemset(buf,ch,length);
}

//----------------------------------------------------------------------------------------------
// memset(buf,char,length)
//
void *xmemset(char *buf,char ch,int length)
{
	int i; 

	//
	for(i=0; i<length; i++) buf[i] = ch;
	return buf;
}

