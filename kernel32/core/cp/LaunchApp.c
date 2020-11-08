// ===============================================================
// LaunchApp.c - Application launcher.
// Try to keep as simple and easy to understand as possible,
// and first of all readable.
// (C) 2002-2005,2017 by Jari Tuominen(jari.t.tuominen@gmail.com).
//
// ERRORS ARE REPORTED USING centralprocess.error -VARIABLE.
// ===============================================================
#include "int.h"
#include "cpu.h"
#include "sysglvar.h"
#include "kernel32.h" // FATALEXCEPTION
#include "delay.h"
#include "threads.h"
#include "centralprocess.h" // central process error defines

//
void Indication(void)
{
	//
	nop();
	nop();
	nop();
	nop();
}

// Launch application with defaults
int LaunchApp(BYTE *abin, int length)
{
	// Defaults to 64K of runtime memory.
	// 16K stack/com area, 48K code area.
	return LaunchAppEx(abin, length, DEF_APP_SPACE_K/64,
		GetCurrentDNR(), GetCurrentDB());
}

//
void AdaptAppPages(int pid, DWORD srcOffset)
{
	DWORD *dst,src,
		i,d1,d2,d3,d4;

	//
	dst = (0x600000+pid*1024*16);

	// First map all pages to zero.
	for(i=0; i<4096; i++)
	{
		dst[i] = 0;
	}

	//
	src = srcOffset/4096;

	// Then copy pages from srcPages.
	for(i=0; i<4096; i++)
	{
		d1 = GetPage(src+i);
		if( d1<8 )
			break;
		dst[i] = d1;
	}	
}

// LaunchAppEx(myproggie,1024,2);
// Will be a system call later on - TODO.
//
// banks = amount of execution memory
// to allocate, in 4K units.
//
// Returns created process ID in UNIPID FORMAT!
int LaunchAppEx(BYTE *abin, int length, int memkb,  int dnr, int db)
{
	int i,i2,i3,i4,l,bytes,flags,unipid;
	WORD selector;
	BYTE *appSpace,*dummy;
	BYTE *appStack;
	int l_appStack;

	//
	if(memkb<16)
	{
		printk("%s: memkb=%d, won't run with this small memory!\n",
			__FUNCTION__,
			memkb);
		return NULL; // min. executable size is 16K.
	}

	//
	flags = get_eflags();
	disable();

	//
	puts("(A) ");

	// Allocate requested memory space + 16K stack.
	bytes = 1024*memkb;
	appSpace = malloc(bytes);
	l_appStack = 1024*16;
	appStack = RTBASE; // 16 KB stack
	if(appSpace==NULL)return NULL;

	// 0x8000 0000 executable binary.
	// Initialize stack area.
	memset(appSpace, 0, 0x4000);

	puts("(B) ");
	// Copy binary.
	l = memkb * 1024;
	for(i=0; i<length; i++)
	{
		if( (i+0x4000)>=l )
		{
			centralprocess.error = CENTRALPROCESS_ERROR_LAUNCHAPPERROR;
			return NULL;
		}
		appSpace[i+0x4000] = abin[i];
	}

	puts("(C) ");
	// Get properties.
	appseg =		0x08; // Temporarily use kernel segment selectors.
	appstackoffs =		appStack+l_appStack-16; // (runtime) Grows backwards!
	appoffset =		RTCODE; // (runtime) JMP address, where the execution begins.
	apprealoffs =		appSpace; // (malloc) The real process memory base.

	puts("(D) ");
	//---------------------------------------------------------------------------
	// Process creation.
	//
	unipid = _createAppThread(appseg, appseg+8, appSpace, appoffset, appstackoffs);

        // Switch PID's memory space to it's own ownership.
        ChangeChunkOwner((void*)appSpace, unipid);

	// Create page table for the new process.
	AdaptAppPages(RPID(unipid), appSpace);

	puts("(E) ");
	// Set path
	SetThreadDNR(unipid, dnr);
	SetThreadDB(unipid, db);
	ReportMessage("%s: Set PID%d DNR/DB to %d/%d\n",
		__FUNCTION__,
		unipid, dnr, db);

	puts("(F) ");
	// Mandatory for deallocation of the thread:
	thread_base[RPID(unipid)] =	appSpace;
	thread_ss[RPID(unipid)] =	selector+8;
	thread_selector[RPID(unipid)] =	selector;
	thread_banks[RPID(unipid)] =	memkb/64;
	thread_memkb[RPID(unipid)] =	memkb;
	SetThreadPriority(unipid, THREAD_PRIORITY_NORMAL);

	puts("(G) ");
	// Define as an application.
	SetThreadType(unipid,
		GetThreadType(unipid) & (~THREAD_TYPE_KERNEL)
			| THREAD_TYPE_APPLICATION);

	// Enable tracking. (**)
	TrackThread(unipid, TRUE);

	//
	ReportMessage("%s: THREAD BASE for PID%d = 0x%x\n",
		__FUNCTION__, unipid, thread_base[RPID(unipid)]);

	puts("(H) ");
	// Indicate that the process have been created successfully
	// (debug functionallity for bochs)
	Indication();

	//
//	ESCLOCK();

	// Restore flags
	set_eflags(flags);

	puts("(I) ");
	// Return unique PID
	return unipid;
}

//


