//=======================================================================
// JTMOS executable file identification.
// (C) 2003-2005 by Jari Tuominen.
//=======================================================================
#include "kernel32.h"
#include "exeidentificationmodule.h"

// Can run anything?
int exeIdentAnythingGoes = FALSE;

// Returns -1 if no MemReq string was found
int getExeMemReq(BYTE *buf, int len)
{
	// identification string
	static char *ids="SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=";
	static int i,i2,i3,i4,ok,l;
	static char *where,*p;

	// Look for identification string
	for(i=0,ok=0,i2=0,where=NULL; i<len; i++)
	{
		//
		if(ids[i2]==0){ok=1; break;}

		//
		if(buf[i]==ids[i2])
		{
			if(i2==0)
				where = buf+i;
			i2++;
		}
		else
		{
			i2=0;
			where=NULL;
		}
	}

	//
	if(ok && where)
	{
		// Found ID string
		l = strlen(where);
		for(i=0,p=where; i<l; i++,p++)
		{
			// Reached "=" mark?
			if(*p=='=')
			{
				// Skip "="
				p++;
				break;
			}
		}

		//
		printk("%s: size string = '%s'\n",
			__FUNCTION__, p);
		// Get size
		return dec2int(p);
	}

	// As default, return zero:
	return 0;
}

// Determine whether if the binary in buffer ptr is JTMOS EXE or not?
int isExeBinary(BYTE *buf, int l)
{
	static char *det="[JTMOS LIBC (C) 2002 Jari Tuominen]";
	static char *tail="[VALID JTMOS EXECUTABLE BINARY FILE FCE2E37BE38BA000]";
	static int i,i2,i3,i4,ok;

	// If allowing to run anything, then let's do so !:
	if(exeIdentAnythingGoes)
		return TRUE;

	// Find first string
	for(i=0,ok=0,i2=0; i<200; i++)
	{
		if(det[i2]==0){ok=1; break;}
		if(buf[i]==det[i2])
		{
			i2++;
		}
		else i2=0;
	}
	if(!ok)
	{
		printk("%s: First EXE identification string not found.\n",
			__FUNCTION__);
		return FALSE;
	}

	// Find second string(tail).
	for(i=0,i2=0,ok=0; i<(l+1); i++)
	{
		if(tail[i2]==0){ok=1; break;}
		if(buf[i]==tail[i2])
		{
			i2++;
		}
		else i2=0;
	}
	if(!ok)	
	{
		printk("%s: Second EXE identification string not found.\n",
			__FUNCTION__);
		return FALSE;
	}

	// Return that it is EXE
	return TRUE;
}


