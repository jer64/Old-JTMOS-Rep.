// ================================================================
// centralprocess.c - The process creator & Debug window view & Recycler
// (C) 2002-2005,2017 by Jari Tuominen(jari.t.tuominen@gmail.com).
// ================================================================
/*
 * How much to allocate memory for each of the processes?
 *
 * Check out file 'LaunchApp.h' for the amount of memory
 * the kernel will spend on each of the application
 * that runs inside it's own memory space.
 * For instance centralprocess process will be creating processes
 * that are depending on the setting of LaunchApp.h.
 */
#include "kernel32.h" // STATIC_APP_MEM_SIZE
#include "threads.h"
#include "centralprocess.h"
#include "centralprocessInit.h"
#include "sysglvar.h"
#include "keyb.h"
#include "syssh.h"	// ShellMain
#include "driver_api.h"	// driver_getLock
#include "centralprocessMain.h"

//#define centralprocess_DEBUG

//
VERBATIM centralprocess;
//
int turn=1;

// This function's purpose is to explain the happended error
// in human friendly English language, so that the user
// can understand the error. This information is used in
// debugging.
void centralprocessExplainError(void)
{
	//
	switch(centralprocess.error)
	{
		// No error
		case 0:
		break;
		//
		case CENTRALPROCESS_ERROR_LAUNCHAPPERROR:
		printk("CENTRALPROCESS: Application launch error.\n");
		break;
		//
		case CENTRALPROCESS_ERROR_FILENOTFOUND:
		printk("CENTRALPROCESS: Executable binary file not found.\n");
		break;
		//
		case CENTRALPROCESS_ERROR_CORRUPTEDEXE:
		printk("CENTRALPROCESS: Corrupted or non-JTMOS executable binary file.\n");
		break;
		//
		default:
		printk("CENTRALPROCESS: Unknown error occured while creating a process.\n");
		break;
	}
}

//
void trap1(void)
{
}

// fname = file name, ends at the first space(space not included)
// path = whole command line with arguments
int centralprocess_exec(const char *fname, const char *path, int dnr, int db,
		const char *runPath,
		int terID)
{
	//
	return centralprocessExecEx(fname, path, terID,
		dnr, db, runPath);
}

// FILE NAME, PATH, TERMINAL ID
//
// dnr,db =	location where the EXE binary resides
// fname =	file name of the EXE binary file
// runPath =	the path where program will run/work at
//		(aka work path, e.g. '/tmp')
//
int centralprocessExecEx(const char *fname, const char *path, int terminalID,
		int dnr, int db, const char *runPath)
{
	DWORD flags;

	// Return PID 0 if error
	if(fname==NULL)return 0;
	//
	flags=get_eflags(); disable();
	while(centralprocess.isBusy) { SwitchToThread(); }

	// Set situation
	centralprocess.isBusy=1;
	centralprocess.appPID=0;
	launchapp_cleartogo=0;
	// Set parameters
	centralprocess.terminalID = terminalID;

	// Set file name
	strcpy(centralprocess.fname, fname);
	// Set path
	strcpy(centralprocess.path, path);
	// Add .bin extension if needed
	if( strstr(centralprocess.fname, ".")==NULL )
	{
		strcat(centralprocess.fname, ".bin");
	}

	// Set run path
	strcpy(centralprocess.runPath, runPath);
	centralprocess.dnr = dnr;
	centralprocess.db = db;

	//
	while(!launchapp_cleartogo) { SwitchToThread(); }
	while(centralprocess.isBusy) { SwitchToThread(); }
	if(centralprocess.error)return 0;

	//
	set_eflags(flags);
	return centralprocess.appPID;
}

//
void centralprocess_wait_moment(void)
{
	int i;

	//
	for(i=0; i<10; i++)
		SwitchToThread();
}

// Called by centralprocess main porcess to load up the exe
int centralprocess_loadEXE(const char *fname)
{
	static int sz;
	int NeedToLoad;
	char path[256],found[256];
	int fd;

	//
	NeedToLoad=1;

	// -----------------------------------------------
	// INITIALIZE
	// Set error
	centralprocess.error=0;
	// Set device
	SetThreadDNR(GetCurrentThread(), centralprocess.dnr);
	SetThreadDB(GetCurrentThread(), centralprocess.db);

	// -----------------------------------------------
	// LOAD EXECUTABLE FILE INTO centralprocess.bin -BUFFER

	// Add .bin extension if one not added yet.
	if(strstr(fname, ".")==NULL)
		strcat(fname, ".bin");

	// INTERNAL APPLICATION
	if(!strcmp(fname, ":install.bin"))
	{
		sz = 65536;
		//memcpy(centralprocess.bin, testapp, 65536);
		NeedToLoad=0;
		goto past11;
	}

	// INTERNAL DRIVER
	if(!strcmp(fname, ":hd.bin"))
	{
		sz = 65536;
	//	memcpy(centralprocess.bin, testdrv, 65536);
		NeedToLoad=0;
		goto past11;
	}

	// Get path variable.
	// (TODO: Use process variable database if one exists.)
	strcpy(path, "");
	if( kgetenv(kenv, "PATH",path) )
	{
		printk("Error: System environment variable PATH is not set or cannot be accessed.\n");
		goto error;
	}

	//
	strcpy(found, "");
	// File already got a path?
	if( strstr(fname, "/")!=NULL )
	{
		//
		strcpy(found, fname);
	}
	else
	{
		// Look for file from common paths.

		// Search through all specified locations for the executable.
		if( !SearchThroughPath(path, fname, found) )
		{
			//
			printk("Path search did not find any matching files.\n");
			// File not found.
			goto error;
		}
		printk("Path search found match at %s.\n",
			found);
	}


	//-----------------------------------------------------------------------
	fd = fopen(found, O_RDONLY);
	sz = lseek(fd, 0, SEEK_END);
	if(fd>=0) {
		close(fd);
	} else {
		sz = 0;
	}
past11:
	centralprocess.exesz = sz;
	printf("FILE '%s' SIZE = %d bytes (%x), maxsz = %d\n",
		fname, sz,sz, centralprocess.l_bin);
//	getch();
	if(sz>=centralprocess.l_bin)	goto error;
	if(sz<=0)
	{
		printk("centralprocess: File '%s'(%s) not found, sz=%d(0x%x)\n",
			fname, found, sz, sz);
error:
		disable();
		centralprocess.error = CENTRALPROCESS_ERROR_FILENOTFOUND; // 1 = file not found
		centralprocess.appPID=0;
		launchapp_cleartogo=3;
		centralprocess.isBusy=0;
		goto past;
	}

	// 1. Read exe binary
	if(NeedToLoad)
	{
		//
		printk("%s: Reading file '%s', size=%dK bytes, Buffer* = 0x%x.\n",
			__FUNCTION__, found, sz/1024+1,  centralprocess.bin);
		fd = open(found, O_RDONLY);
		read(fd, centralprocess.bin, sz);
		close(fd);
	}
dontload:

	// 2. Confirm format
	if( isExeBinary(centralprocess.bin, sz)==FALSE )
	{
error2:
		// Non-JTMOS EXE
		disable();
		centralprocess.appPID=0;
		centralprocess.error = CENTRALPROCESS_ERROR_CORRUPTEDEXE;
		launchapp_cleartogo=3;
		centralprocess.isBusy=0;
		goto past;
	}

	// 3. Get memory requirement
	centralprocess.memrq = getExeMemReq(centralprocess.bin, sz);
	printk("Memory requirement = %dK\n", centralprocess.memrq);

	// 4. Report
	printk("Read complete, launching up application at directory '%s'\n",
		centralprocess.runPath);
past:
	// --------------------------------------------------------------
	return centralprocess.error;
}

// Called by process creator to summon up a new process.
int centralprocess_job(void)
{
	//
	// void centralprocess_createproc(const char *fname,
	// void *appcode,int appsize)
	return centralprocess_createproc(centralprocess.fname, centralprocess_runexe1, 1024*512);
}

// Called by centralprocess_job.
int centralprocess_createproc(const char *fname,
		void *appcode, int appsize)
{
	// ------------------------------------------------------------------
	static char *appstack;
	static int unipid,pr,key;
	static char str[256];
	static int pid,memkb;

	// ========== DISABLE INTERRUPTS ====================================
	// - Change to work directory(where the program will work).
	chdir(centralprocess.runPath);
	printk("%s: DNR/DB=%d/%d (chdir to '%s')\n",
		__FUNCTION__, GetCurrentDNR(), GetCurrentDB(),
		centralprocess.runPath);

	//
	disable();
	// Get default memory requirement.
	memkb = STATIC_APP_MEM_SIZE;
	// Use custom memory requirement if one defined.
	if(centralprocess.memrq)
		memkb = centralprocess.memrq;
	// - Launch up application.
	centralprocess.isBusy=0;
	unipid = LaunchAppEx(centralprocess.bin, centralprocess.exesz, memkb,
				GetCurrentDNR(), GetCurrentDB());
	launchapp_cleartogo=1;

	// Set command line.
	SetThreadCmdLine(unipid, centralprocess.path);

	// Give it a proper name.
	IdentifyThread(unipid, centralprocess.fname);

	// Set terminal.
	thread_terminal[RPID(unipid)] = centralprocess.terminalID;

	// No termination -flag set at the beginning.
	syskey_terminate=0;
	pr=1;

	// Store the PID.
	centralprocess.appPID =	unipid;

	// ========== ENABLE INTERRUPTS ===================================
	enable();

	// Schedule.
	SwitchToThread();
	return unipid;
}

//
void centralprocess_runexe1(void)
{
	//
	centralprocess_runexe(centralprocess.fname);
}

// centralprocess Run Exe
// Does not return any value.
int centralprocess_runexe(const char *fname)
{
	int memkb;

	// - Change to work directory(where the program will work)
	chdir(centralprocess.runPath);

	// Get default memory requirement
	memkb = STATIC_APP_MEM_SIZE;
	// Use custom memory requirement if one defined
	if(centralprocess.memrq)
		memkb = centralprocess.memrq;

	// - Launch up application
	centralprocess.isBusy=0;
	centralprocess.appPID = LaunchAppEx(centralprocess.bin, centralprocess.exesz, memkb/64,
				GetCurrentDNR(), GetCurrentDB());
	launchapp_cleartogo=1;
}

//
void centralprocess_HelpPage(void)
{
	//
	printk("'k'=Kill process  [Space]=List processes  's'=Launch new shell\n");
	printk("'l'=Show DRIVER API lock PID  'h'=Show this help page\n");
	printk("'u'=Force unlock DRIVER API lock  'y'=Show last function for lock owner\n");
	printk("'d'=Directory cache info  'f'=FILEDES info  'F'=JTMFAT info\n");
	printk("'D'=List devices  'v'=toggle verbose debugging\n");
}

//


