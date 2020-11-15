// ------------------------------------------------------------------
// flexFat.c - Flexible FAT system.
// (C) 2002-2020 by Jari Tuominen (jari.t.tuominen@gmail.com).
// ------------------------------------------------------------------
#include "kernel32.h"
#include "jtmfs.h"
#include "jtmfat.h"
#include "flexFat.h"
#include "flexCorruption.h" // corruption checks

//
char flextmp[256];

// Flex FAT main structure
FLEFATSYS *fle;
int flexActive=FALSE;

//
void flexHealthCheck(void)
{
	int dnr,i;
	char *walk[]={
		"hda",
		"ram",
		"*"
		};

	//
	for(i=0; strcmp(walk[i], "*"); i++)
	{
		dnr = driver_getdevicenrbyname(walk[i]);
		if(dnr!=-1)
		{
			AnalyzeFAT(&fle->FAT[dnr]);
		}
	}
}

//----------------------------------------------------------------
// Initialize flex fat system
//
void flexFatInit(void)
{
	int i;
	FLEFAT *f;

	//
	dprintk("%s: FLEX FAT Initializing ..\n", __FUNCTION__);
	//getch();
	dprintk("1 ");
	//getch();
	fle = malloc(sizeof(FLEFATSYS));
	if(fle==NULL)
		panic("Can't allocate fle (PTR) with malloc : NULL ptr case 12");

	// Reset main structure
	dprintk("2 ");
	//getch();
	memset(fle,0,sizeof(FLEFATSYS));

	//
	dprintk("3 ");
	//getch();
	for(i=0; i<N_MAX_FATS; i++)
	{
		//
		f = &fle->FAT[i];

		//
		f->isOK = FALSE;
	}

	//
	dprintk("4 ");
	//getch();
	flexActive = TRUE;
	
	//
	dprintk("Done.\n");
	//getch();

	//
//	addPitHandler(AnalyzeFATs);
}

// Flush contents of FAT cache to disk
void flexFlushFAT(int dnr)
{
	FLEFAT *f;

	//
	DEBUGINFO

	//
	f = &fle->FAT[dnr];
	if(f->isOK)
	{
		//
		printk("%s: WRITING FAT DNR = %d\n",
			__FUNCTION__, f->dnr);
//		waitkey();
		flexRWFAT(f, WRITE);
	}
}

// Clears contents of a FAT
void clearFatContents(FLEFAT *f)
{
	//
	if( illegal(f) )
		panic("NULL PTR case 11");

	//
	printk("f->buf clearing\n");
	memset(f->buf, 0, f->l_buf);
	printk("f->writemap clearing\n");
	memset(f->writemap, 0, f->l_writemap);
}

// Allocate FAT data buffers for drive.
int flexAllocateFat(FLEFAT *f)
{
	static char str[256];

	//
	if( illegal(f) )
		panic("NULL ptr case 10");

	// Buffer
	if(f->buf==NULL)
	{
		//
		f->l_buf =	getsize(f->dnr)*4;
		if(f->l_buf >= MAX_FAT_SIZE)
		{
			// We can't allocate, FAT is too big (TODO !!).
			return 1982;
		}

		f->buf =	procmalloc(f->l_buf, 0, __FUNCTION__);

		if( illegal(f->buf) )
		{
			sprintf(str, "%s: error alloc 1 (illegal PTR: buf = 0x%x, l_buf=%d)",
				__FUNCTION__, f->buf, f->l_buf);
			panic(str);
		}
		memset(f->buf, 0, f->l_buf);
		printk("%s: f->buf = 0x%x\n",
			__FUNCTION__, f->buf);
	}

	// Write map
	if(f->writemap==NULL)
	{
		//
		f->l_writemap =	(getsize(f->dnr)*4/getblocksize(f->dnr));
		if(f->l_writemap >= MAX_FAT_SIZE)
		{
			// We can't allocate, FAT is too big (TODO !!).
			return 1983;
		}

		f->writemap = procmalloc(f->l_writemap, 0, __FUNCTION__);

		if( illegal(f->writemap) )
			panic("error alloc 2");
		memset(f->writemap, 0, f->l_writemap);
		printk("%s: f->writemap = 0x%x\n",
			__FUNCTION__, f->writemap);
	}

	//
	printk("%s: Calling clearFatContents.",
		__FUNCTION__);
	clearFatContents(f);
}

// Analyze all known FATs for any corruption
void AnalyzeFATs(void)
{
	int dnr;

	//
	dnr = driver_getdevicenrbyname("ram");
	AnalyzeFAT( &fle->FAT[dnr] );

	//
	dnr = driver_getdevicenrbyname("hda");
	AnalyzeFAT( &fle->FAT[dnr] );
}

// Analyze FAT table for any errors (integrity check)
void AnalyzeFAT(FLEFAT *f)
{
	int i,errors,sz,conti,maxconti,maxoffs,offs,key;
	DWORD v;
	char str[50];

	//
	return;

	//-----------------------------------------------------------
	// Is this FAT active?
	if( f->isOK==FALSE )
		return;

	//-----------------------------------------------------------
	// Get actual drive size
	sz = getsize(f->dnr);

	//
	for(i=0,errors=0,conti=0,maxconti=0; i<sz; i++)
	{
		//
		v = f->buf[i];
		if(v<0xFFFFFF00)
		{
			//
			if(v >= f->max_val)
			{
				// Show upto 10 errors, then stop showing
				// these to speed up the process...
				if(errors<10)
				{
					//
				//	printk("%c%c%c%c",
				//		v&255, v>>8&255, v>>16&255, v>>24&255);
					printk("reads=%x, max. allowed=%x, location=%d/%d\n",
						v, f->max_val, i,sz);
				}

				//
				errors++;
				if(offs==0)	offs = i;
				conti++;

				// Fix location with SYSTEM RESERVED value.
				f->buf[i] = 0xFFFFFFF7;
			}
			else
			{
				//
				if(conti && conti>maxconti)
				{
					maxconti = conti;
					maxoffs = offs;
				}
				//
				conti=0;
			}
		}
	}

	// If any errors detected
	if(errors)
	{
		// Get user attention 
		Attention();
		// Show user what we got
		driver_getdevicename(f->dnr, str),
		printk("\n* FAT table for drive #%d(%s) is corrupted: table contains unaccepted values (%d errors detected). *\n",
			f->dnr, str, errors);
		printk("Largest contigous corrupted area = %d bytes.\n",
			maxconti*4);

		//
		while(1)
		{
			//
			printk("Run global search(Y/N)?");
			key = getch();
			if(key=='Y')
			{
				// Search whole kernel memory for this pattern.
				GlobalSearch(&f->buf[maxoffs], maxconti*4);
		
				// Wait until user hits key.
				getch();
				break;
			}
			if(key=='N')
				break;

			//
			printk("\nPlease answer Y or N with caps.\n");
		}

		//
	}
}


// Read whole FAT in memory at once.
// [or]
// Write changed FAT areas on drive.
// RW must equal to READ or WRITE.
void flexRWFAT(FLEFAT *f, int rw)
{
	static int i,i2,i3,i4,b,s,e;
	char *p;

	//
	FLEXCHECK(f)

	// Writing?
	if(rw==WRITE)
	{
		// Anything left to write?
		if(f->writeBack==0)
		{
			// Nothing to write.
			return;
		}
		// Flag write
		else f->writeBack=0;
	}

	//
	FLEXCHECK(f)

	//
	DEBUGINFO
	dprintk("%s: DNR=%d\n", f->dnr);

	//
	FLEXCHECK(f)

	// Determine the block we need
	s = f->bb->fat_offs;
	e = f->bb->fat_offs + (getsize(f->dnr)*4/getblocksize(f->dnr)) + 1;
	for(i=s,p=f->buf,i2=0; i<e; i++,p+=getblocksize(f->dnr),i2++)
	{
		//
		if(  (p+getblocksize(f->dnr)) >= (f->buf+f->l_buf)  )
		{
			//
			FLEXER(1100, "flexFat EXCEEDED BUFFER BOUNDER");
		}

		//
		if(rw==READ)
		{
			//
			dprintk("<%s: readblock DNR=%d, BNR=%d, p=0x%x>\n",
				__FUNCTION__, f->dnr, i, p);
			//sleep(1);
			readblock(f->dnr, i, p);
		}
		else
		if(rw==WRITE)
		{
			//
			if( f->writemap[i2] )
			{
				//
				dprintk("<%s: writeblock DNR=%d, BNR=%d, p=0x%x>\n",
					__FUNCTION__, f->dnr, i, p);
				//sleep(1);
				f->writemap[i2]=0;
				writeblock(f->dnr, i, p);
			}
		}
	}

	// Analyze FAT for any integrity errors
	AnalyzeFAT(f);

	//
	FLEXCHECK(f)

	//
	dprintk("\n");
}

// Activate cache
// Return value: (the caller must check the return value upon return!!)
// 0 = Success.
// Non-zero = Error.
int flexActivateFat(int dnr)
{
	FLEFAT *f;

	// Validity check
	if(!isValidDevice(dnr)){ return 1981; }

	//
	f = &fle->FAT[dnr];

	//
	if( (f->bb=jtmfat_getinfoblock(dnr))==NULL || (f->bb && strncmp(f->bb->detstr, "JTMFS", 5)) )
	{
		//
		printk("%s: The drive (%d) is not JTMFS formatted, won't activate FAT.\n",
			__FUNCTION__, dnr);
		return 1980;
	}

	//
	f->status = INITIALIZED;
	f->dnr = dnr;
	f->guard = FLFGUARD;
	f->writeBack = 0;
	f->max_val = getsize(dnr);
	// Define amount of FAT values.
	f->amount = getsize(f->dnr);
	// Perform allocation.
	return flexAllocateFat(f);
}

// Returns amount of FAT entries FAT [DNR] has.
int flexGetFatSize(int dnr)
{
	//
	return fle->FAT[dnr].amount;
}

// ------------------------------------------------------------------
int flexTouchFat(int dnr, int FATbnr)
{
	int i,i2,newBlock,rv;

	// Validity check
	if(!isValidDevice(dnr)){ FLEXER(7, "flexTouchFat: (7) invalid DNR") return 1; }

	//
	DEBUGFUNC

	// 1) Activate FAT if it is not yet activated
	if(fle->FAT[dnr].isOK==FALSE)
	{
		// Get hang of the boot block
		if( (fle->FAT[dnr].bb=jtmfat_getinfoblock(dnr))==NULL )
		{
			//
			FLEXER(8, "flexTouchFat: (8) getinfotblock returned NULL")
			printk("%s: Non-JTMFS drive ?\n",
				__FUNCTION__);
			sleep(1);
			// Can't get hold of boot block on this device!
			return 1;
		}

		// Allocate FAT buffer if neccesary
		// (contains whole FAT at once).
		if(fle->FAT[dnr].buf==NULL)
		{
			//
			if( (rv=flexActivateFat(dnr)) ) 
				return rv;
		}

		// Read FAT
		flexRWFAT(&fle->FAT[dnr], READ);

		// OK now
		fle->FAT[dnr].isOK = TRUE;
	}

	// Activation was successful:
	return 0;
}

//
void flexSetWM(FLEFAT *f, int entry, int what)
{
	//
	f->writemap[entry] = what;
}

//
void flexAffectWM(FLEFAT *f, int entry)
{
	int bs;

	//
	bs = getblocksize(f->dnr);
	if(bs<=0)
		return;

	//
	f->writeBack = 1;
	flexSetWM(f, (entry*4)/bs, 1);
}

// Get block from FAT table
int _flexFatGetBlock(const char *caller, int dnr,int X)
{
	FLEFAT *f;
	int rv;

	// 1A) DNR validity check
	if(!isValidDevice(dnr)){ FLEXER(6, "flexFatGetBlock: (6) invalid DNR") return 0; }

	//
//	dprintk("%s was called by %s\n",
//		__FUNCTION__, caller);

	// 1B) Parameter validity check
	if( X<0 || X>=getsize(dnr) ){ FLEXER(5, "flexFatGetBlock: if( X<0 || X>=getsize(dnr)") return 0; }

	// 2) Get permission(will do writeBack flushes also)
	if( (rv=flexTouchFat(dnr, X)) ){ FLEXER(4, "flexTouchFat(dnr, X)") return 0; }

	// 3) Receive entry
	return FastGetBlock(dnr, X);
}

// Set block
int _FastSetBlock(char *fun, int dnr, int which, DWORD what)
{
	FLEFAT *f;
	char str[256];

	//
	if( flexTouchFat(dnr, which) ){ FLEXER(1, "flexFatSetBlock: touch failed") return 0; }

	// 1C Get PTR
	f = &fle->FAT[dnr];

	// 3) Change entry
	if((what & 0xFFFFFFF0) == 0xFFFFFFF0 || what < f->max_val)
	{
		//
		f->buf[which] = what;
	}
	else
	{
		// Illegal value!
		sprintf(str, "Function %s is attempting to set illegal FAT value(%x, max=%x) on FAT table.",
			fun, what, f->max_val);
		panic(str);
	}
	

	// 4) Declare writeback need on writeback map
	flexAffectWM(f, which);
	
	//
	return 0;
}

// Set block
int FastSetBlocks(int dnr, int x1, int x2, int what)
{
	FLEFAT *f;
	int x,bs;

	//
	if( flexTouchFat(dnr, x1) ){ FLEXER(1, "flexFatSetBlock: touch failed") return 0; }

	// 1C Get PTR
	f = &fle->FAT[dnr];

	// 3) Change entry
	for(x=x1; x<x2; x++)
		f->buf[x] = what;

	// 4) Declare writeback need
	bs = getblocksize(f->dnr);
	if(bs<=0)	return 1;
	for(x=(x1*4)/bs; x<(((x2*4)/bs)+1); x++)
		flexSetWM(f, x, 1);
	return 0;
}

// Get block
int _FastGetBlock(char *fun, int dnr, int which)
{
	FLEFAT *f;

	//
	if( flexTouchFat(dnr, which) ){ FLEXER(1, "flexFatSetBlock: touch failed") return 0; }

	// 1C Get PTR
	f = &fle->FAT[dnr];

	// 3) Change entry
	return f->buf[which];
}

// Set block on FAT table
int _flexFatSetBlock(const char *caller, int dnr,int X,int VALUE)
{
	FLEFAT *f;

	//
//	dprintk("%s was called by %s\n",
//		__FUNCTION__, caller);

	// 1A) DNR validity check
	if(!isValidDevice(dnr)){ FLEXER(3, "flexFatSetBlock: invalid dnr") return 0; }

	// 1B) Parameter validity check
	if( X<0 || X>=getsize(dnr) ){ FLEXER(2, "invalid requested block #") return 0; }

	// 2) Get permission(will do writeBack flushes also)
	if( flexTouchFat(dnr, X) ){ FLEXER(1, "flexFatSetBlock: touch failed") return 0; }

	//
	return FastSetBlock(dnr, X, VALUE);
}

// This function is called upon disk changes
void flexDiskChange(int dnr)
{
	FLEFAT *f;
	char str[256];

	// Get FAT PTR
	f = &fle->FAT[dnr];

	//
	if(f->status!=INITIALIZED)
	{
		return;
		panic("flexDiskChange call on non-initialized FAT");
	}

	// Eject FAT cache for this device
	f->isOK = 0;
	driver_getdevicename(dnr, str);
	printk("%s: Calling clearFatContents (device=%s).\n",
		__FUNCTION__, str);
	clearFatContents(f);
}

//


