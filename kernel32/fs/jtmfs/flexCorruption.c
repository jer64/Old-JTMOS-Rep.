// flexCorruption.c
#include "kernel32.h"
#include "jtmfs.h"
#include "jtmfat.h"
#include "flexFat.h"
#include "flexCorruption.h"

//----------------------------------------------------------------
// FlexFAT system report
//
void jtmfatInformation(void)
{
	int dnr;

	// Check HDA
	dnr = driver_getdevicenrbyname("hda");
	jtmfatInformation1(&fle->FAT[dnr], "Hard Disk Drive");

	// Check 
	dnr = driver_getdevicenrbyname("ram");
	jtmfatInformation1(&fle->FAT[dnr], "RAM Disk Drive");
}

// Check FAT structure's status
// FALSE = corrupted
// TRUE = uncorrupted, or information unavailable
int checkFat(FLEFAT *e)
{
	// Skip inactive FATs
	if(e->isOK==FALSE)
		return TRUE;

	// Check guard DWORD
	if(e->guard!=FLFGUARD)
		return FALSE;

/*	// Check buffer #1
	if( fixalCheck(e->buf, e->l_buf)==FALSE )
		return FALSE;

	// Check buffer #2
	if( fixalCheck(e->writemap, e->l_writemap)==FALSE )
		return FALSE;*/

	// OK:
	return TRUE;
}

// FlexFAT general corruption check
int ffCheck(const char *fn, const char *func, int line)
{
	int dnr;
	static char str[256],name[256];
	FLEFAT *e;
	int i;
	static char *drives[]={"ram","hda","*"};
	static int *dnrs[100]={-1};

	//
	if(flexActive!=TRUE)
		return 1;

	//
	return 0;

	//
	for(i=0; strcmp(drives[i],"*"); i++)
	{
		// Check HDA
		if( (dnr=dnrs[i])==-1 )
			dnrs[i] = dnr = driver_getdevicenrbyname(drives[i]);
		if(dnr==-1)
			continue;
		e = &fle->FAT[dnr];
	
		// Check one
//		FLEXCHECK(e);

		// Check two
		if(checkFat(e)==FALSE)
		{
			//
			driver_getdevicename(dnr, name);
			sprintf(str, "error: file %s, function %s, line %d -- FAT failure on device %s",
				fn, func, line,
				name);
			panic(str);
		}
	}

	//
	return 0;
}

// Reporter function
void jtmfatInformation1(FLEFAT *e, const char *name)
{
	int i,i2,i3,i4;

	// Skip inactive FATs
	if(e->isOK==FALSE)
		return;

	// Print descriptions
	printk("-------------------------------------------------------------\n");
	printk("REPORT FOR: %s\n",		name);
	printk("Data PTR    Data length    WMAP    WMAP length    DNR/BNR    GUARD\n");

	// Print values
	printk("%x    %x    %x    %x    %d/%d    %x    amount=%d\n",
		e->buf, e->l_buf, e->writemap,
		e->l_writemap, e->dnr,e->bnr, e->guard,
		e->amount);

	// Check guard DWORD
	if(e->guard!=FLFGUARD)
	{
		//
		printk("WARNING: entry guard variable is incorrect. This most probably indicates a corrupted FAT structure.\n");
		printk("%x != %x\n",
			e->guard, FLFGUARD);
	}

/*	// Check buffer #1
	if( fixalCheck(e->buf, e->l_buf)==FALSE )
	{
		//
		printk("e->buf guard is invalid, this indicates a buffer corruption.\n");
	}

	// Check buffer #2
	if( fixalCheck(e->writemap, e->l_writemap)==FALSE )
	{
		//
		printk("e->writemap guard is invalid, this indicates a buffer corruption.\n");
	}*/
}




