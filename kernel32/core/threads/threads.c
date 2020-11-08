//--------------------------------------------------------------
// JTMOS THREADS - JTMOS THREAD MANAGEMENT SYSTEM
//
// This is the actual thread management code, this code still
// does not do any low level multitasking, it just manipulates
// the multithread engine(llint.asm).
//
// New: Added support for deallocation of all memory allocated
//      with malloc -function by a specific process.
//      From now on, on a malloc call, the malloc also
//      registers PID. Function void malloc_freeprocmem(int pid)
//      from malloc.h will be used in the deallocation process.
//
// (C) 2001-2005 by Jari Tuominen(jari@vunet.org).
//
// For more information, see README.
//--------------------------------------------------------------
//
// Some new functions:
// create_app_thread, RunApplication
//
#include "kernel32.h"
#include "rtc.h"
#include "int.h"
#include "threads.h" // Much of system threads, contains also thread_shell_ptr array.
#include "dezombie.h"
#include "hexdump.h"
#include "vmware.h"
#include "tss.h" // LOW LEVEL TASK SEGMENTATION

//
extern DWORD thread_shell_pointer[N_MAX_THREADS];

//---------------------------------------------------
// Multitasking enabled yet?
int Multitasking=0;
int threadMagic=0xFCE2E37B;
// Unique PID generation walking number
static int uniwalk=0;

//
int kill(pid_t pid, int sig)
{
	KillThread(pid);
}

//
void abort(void)
{
	//
	ExitThread(0);
}

// Get process memory load
// (total number of kilobytes allocated by the process).
int GetThreadML(int pid)
{
	//
	return thread_ml[pid];
}

//
void SetThreadML(int pid, int load)
{
	//
	thread_ml[pid] = load;
}

// Track or untrack thread system calls.
void TrackThread(int pid, char val)
{
	if(val)
		SetThreadType(pid,
			GetThreadType(pid) | THREAD_TYPE_SYSCALL_TRACE);
	else
		SetThreadType(pid,
			GetThreadType(pid) & (~THREAD_TYPE_SYSCALL_TRACE));
}

// Get the count of task switches occured since start of the threads engine
int GetSwitchCount(void)
{
	return taskSwitchNr;
}

//---------------------------------------------------
// Same as new_thread, but returns UNIPID
//
DWORD newUniqueThread(void)
{
	int key,uni;

	//
	key = new_thread();
	uniwalk++;
#ifdef USE_UNIQUE_PIDS
	uni = uniwalk;
#else
	uni = 0;
#endif
	thread_uniqid[key] = uni<<UNIQSHIFT;
	return thread_uniqid[key] | (key&UNIQMASK);
}

//---------------------------------------------------
// Unique ID comparison for the PID pointed by UNIPID
//
int unif(int x)
{
	return (thread_uniqid[RPID(x)]&UNIQMASK2)==(x&UNIQMASK2);
}

//---------------------------------------------------
// Find a process named by PTR name
//
int GetThreadPID(const char *name)
{
	int i;

	//
	for(i=0; i<nr_threads; i++)
	{
		//
		if(!strcmp(thread_name[i], name))
		{
			// Found
			return i | thread_uniqid[i];
		}
	}

	// Not found
	return -1;
}

// Get thread TSS segment structure PTR.
TSSEG *GetThreadTSS(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	return &tasks->tss[RPID(unipid)];
}

// Set current thread to sleep for X number of turns.
void SetToSleep(int x)
{
	// Set thread to idle abit
	thread_idling[nr_curthread] = x;
}

// Skip over a process that wants to sleep abit now.
void schedSleepSwitch(void)
{
	//
	if(Multitasking)
	{
		//
		SetToSleep(4);

		// Switch to next thread
		forceSwitch();
	}
}

// Called by PIT timer handler when a thread has
// exceeded it's run-time.
void ThreadTimeOut(void)
{
	// Increase spending for this thread
	thread_CPUspending[nr_curthread]++;
}

// Called on task switch by
// PIT handler / schedule switch (SwitchToThread).
void scheduleNextThread(void)
{
	// Switch to next thread
	forceSwitch();
}

// Used for sleeping
void Schedule(void)
{
	//
	SwitchToThread();
}

// Tick on!
int ThreadGiveMagic(void)
{
	threadMagic++;
	return threadMagic;
}

//---------------------------------------------------------------
// Waits until specified process exists.
void WaitProcessTermination(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	while(1)
	{
		//
		if( GetThreadPriority(unipid)==0 )break;
		SwitchToThread();
	}
}

// Cause immediate task switch.
int SwitchToThread(void)
{
	//
	if(Multitasking)
	{
		//
		SetToSleep(2);
		//
		idle_moment();
	}
}

// SwitchToThreadEx
// Wakes up specified process and transfers
// CPU to it immediatly.
int SwitchToThreadEx(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 1;

	// Reset ticking, get max. time.
	thread_tick[RPID(unipid)]=0;
	// Specify switch PID.
	force_switch_thread_nr = RPID(unipid);
	// Do the switch.
	SwitchToThread();
	return 0;
}

// Set thread command line
int SetThreadCmdLine(int unipid, const char *CmdLine)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 1;

	// Allocate command line if neccessary.
	if( thread_cmdline[RPID(unipid)]==NULL )
	{
		thread_cmdline[RPID(unipid)] = malloc(256);
		ChangeChunkOwner(thread_cmdline[RPID(unipid)], 0); // Change to PID0 ownership (a fix).
		SetMemoryName(thread_cmdline[RPID(unipid)], "ThreadCmdLine");
	}

	// Store command line string.
	strcpy(thread_cmdline[RPID(unipid)], CmdLine);
	return 0;
}

// Get currently being used terminal
int GetCurrentTerminal(void)
{
	//
	if(Multitasking!=TRUE) { return 0; }

	//
	return GetThreadTerminal(GetCurrentThread());
}

//
int GetThreadTerminal(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return thread_terminal[RPID(unipid)];
}

//
void SetThreadTerminal(int unipid, int ter)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	thread_terminal[RPID(unipid)] = ter;
}

// Get oointer to thread's shell structure.
void *GetThreadShellPTR(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	return thread_shell_pointer[RPID(unipid)];
}

// Set oointer to thread's shell structure.
void SetThreadShellPTR(int unipid, void *_syssh)
{
}


//
BYTE GetThreadPriority(DWORD unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return get_prio(unipid);
}

//
void SetThreadPriority(DWORD unipid, BYTE prio)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	set_prio(unipid, prio);
}

// Get current thread PID(wrapper)
int getpid(void)
{
	//
	return GetCurrentThread();
}

// Get current thread UNIPID (unique PID)
int GetCurrentThread(void)
{
	//
	if(Multitasking!=TRUE) { return 0; }

	//
	return nr_curthread | thread_uniqid[nr_curthread];
}

// Get thread count
int GetThreadCount(void)
{
	//
	return nr_threads;
}

// Give thread a name
void IdentifyThread(DWORD unipid, const char *_ident)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	_IdentifyThread(RPID(unipid), _ident);
}

// --
void _IdentifyThread(DWORD pid, const char *_ident)
{
	//
	if(_ident==NULL || strlen(_ident)>38)return;

	//
	if(thread_name[pid]==NULL)
	{
		thread_name[pid] = malloc(256);
		ChangeChunkOwner(thread_name[pid], 0); // Change to PID0 ownership (a fix).
		SetMemoryName(thread_name[pid], "ThreadName");
	}

	//
	strcpy(thread_name[pid], _ident);
}

//---------------------------------------------------------------------
//
void ThreadCreateStack(THREAD *t ,int _stacksize)
{
	t->stack = malloc(_stacksize);
	SetMemoryName(t->stack, "ThreadStack");
	t->l_stack =_stacksize;
}

//
void ThreadDestroyStack(THREAD *t)
{
	if(t->stack)	free(t->stack);
	t->stack=NULL;
	t->l_stack=0;
}

// Loop & spare CPU time to others
void infLoop(void)
{
	while(1)
	{
	//	printk("HELLO WORLD - THIS TSS THREAD WORKS !\N");
		SwitchToThread();
	}
}

// Make process immortal.
void Immortal(int unipid)
{
	//
	SetThreadType(unipid, GetThreadType(unipid) | THREAD_TYPE_IMMORTAL);
}

// Initialize multitasking scheduler system
void init_threads(void)
{
	int i;
	static int flags;
	char *name,*p;

	//
	disable();
	printk("Initializing threads ...\n");

	//
	Multitasking=FALSE;

	//
	nr_threads=1;
	nr_curthread=0;

	//
	name = malloc(N_MAX_THREADS*100);

	//
	for(i=0,p=name; i<N_MAX_THREADS; i++,p+=100)
	{
		//
		thread_special[i]=0; // 0 = non-zombie
		thread_states[i]=0;
		thread_ds[i]=0x10; // default => kernel data segment
		thread_es[i]=0x10;
		thread_fs[i]=0x10;
		thread_gs[i]=0x10;
		thread_ss[i]=0x10;
		thread_type[i]=0;
		thread_esps[i]=0;
		thread_tick[i]=0;
		thread_base[i]=0;
		thread_selector[i]=0;
		thread_banks[i]=0;
		thread_terminal[i]=0;
		thread_cmdline[i]=NULL;
		thread_name[i] = p;
		strcpy(thread_name[i], "UNKNOWN");
		thread_uniqid[i]=0;
		thread_shell_pointer[i]=0;

		// Default name
	//	thread_name[i]=malloc(100);
	//	_IdentifyThread(i, "[NONE]");
	}

	// Init thread descriptors:
	memset(thread_codedescs, 0, N_MAX_THREADS*8);
	memset(thread_datadescs, 0, N_MAX_THREADS*8);

	// Enable first thread(init):
	thread_states[0]=1;

	// This function creates entries inside the GDT which are
	// tasks, each task contain own registers such as
	// a real processor would have(i386),
	// that includes f.e. : eax,ebx,ecx,edx,esp,eip and so on:
	// Initialize TSS system.
	printk("  - TSS init\n");
	InitTSS();

	// Create TSS for first thread:
	printk("  PASS 1\n");
	NewTSS(&tasks->tss[0], infLoop, SEG_NO_TSS, GetCurrentThread());

	// We are at the 0x100:
	printk("  PASS 2\n");
	LOADTR(TSSSEL_OFFS);
	nr_curthread=0;

	// Make process 0 (init process) immortal.
	printk("  PASS 3\n");
	Immortal(0);

	//
	printk("  - Done.\n");

	//
	printk("  - IdleThread init\n");
	init_IdleThread();
	//
	printk("  - Done.\n");

	//
//	if(vmware)
//	{
		printk("  - Enabling ACPI sleep support...\n");
		vmware_sleep_enabled = TRUE;
		vmware = TRUE;
		printk("  - Done.\n");
//	}

	//
	printk("Multitasking=TRUE :\n");
	sleep(1);
	//
	Multitasking=TRUE;
	//
	printk("OK.\n");
}

// set_thread pid esp ss cs ds pc
//
// set_thread for KERNEL MODE THREADS
// which can access all memory available,
// aka non-application threads
//
// Renders threads so that it is runnable
//
// Special note:
// even 16-bit segment registers are stored
// at the 32-bit DWORD sized variables
// because of logicallity and sympathy
// for the assembly programs.
//
// KERNEL THREAD:
// set_thread(key, _pr_stack+(_stacksize-1),
//                GIMMIE_DS(),GIMMIE_CS(),GIMMIE_DS(),
//                _pr_pc);
//
// APPLICATION THREAD:
// set_thread(key, _real_esp,
//                0x38,0x30,0x38,
//                _pr_pc);
//
void set_thread(
	DWORD unipid, DWORD _esp,
	DWORD _ss,DWORD _cs,
	DWORD _ds,DWORD _pc)
{
	// Memory based variable
	static int prid;
	DWORD *eip,*eflags;
	WORD *cs;
	char *ren;

	// copy to safety
	prid = RPID(unipid);

	//
	if(prid>=nr_threads)
	{
		panic("set_thread: illegal PID");
	}

	//
	thread_ds[prid]=	_ds;
	thread_es[prid]=	_ds;
	thread_fs[prid]=	_ds;
	thread_gs[prid]=	_ds;
	//
	thread_ss[prid]=	_ss;
	thread_esps[prid]=	_esp;

	// reset tick, so that execution
	// priority gets right
	thread_tick[prid]=0;

	// And as a last thing we do the renderation:
	//
	thread_esps[prid] = RenderStack(_esp+GetThreadBase(unipid),_pc,unipid,
					_ss,_cs,_ds) - GetThreadBase(unipid);
}

// create_thread
// -------------
//
// Creates a thread that can be executed.
// note: stack must be pre-allocated before
// the function call, this function will
// not allocate the stack, the stack size
// must be also reported properly, otherwise
// the thread will not run correctly.
//
// Thread running is set to default priority.
// 
// NEW NOTE:
// The stack must be allocated with kernel malloc,
// or else memory corruption will occur.
// System relies on dynamical memory allocation.
//
// New:
// Now stack is automatically placed in ownership
// of the thread. And is deallocated on thread termination.
//
// example:
// create_thread(stack_offs,1024*16,_my_thread_program);
//
DWORD create_thread(DWORD _pr_stack, DWORD _stacksize, DWORD _pr_pc)
{
	//
	return create_threadEx(_pr_stack, _stacksize, _pr_pc,
		0x08, 0x10); // code & data segment
}

//
DWORD CreateThreadA(DWORD _pr_stack, DWORD _stacksize, DWORD _pr_pc)
{

	//
	return create_threadEx(_pr_stack, _stacksize, _pr_pc,
		kappCodeSeg, kappDataSeg); // code & data segment
		// |1 = privilege level 1(0-2)
}

//	return create_threadEx(_pr_stack, _stacksize, _pr_pc,
//		kappCodeSeg, kappDataSeg); // code & data segment

// The real create thread function.
// Returns unipids since 07/2003.
DWORD create_threadEx(DWORD _pr_stack, DWORD _stacksize, DWORD _pr_pc,
	WORD codeSeg, WORD dataSeg)
{
	DWORD key, flags;
	int dnr,db,i,unipid;
	char *ren;
	char str[256];
	TSSEG *ts;



	//
	flags = get_eflags();
	disable();

	// First create the thread, set stack.
	// (will skip all zombies, won't try to re-use
	//   zombies before dezombie safely deallocates these..)
	unipid = newUniqueThread();

	// Change the stack to thread's ownership.
	ChangeChunkOwner((void*)_pr_stack, unipid);

	// Set CPU spending counter to zero.
	thread_CPUspending[RPID(unipid)] = 0;

	// Create TSS entry.
	ts = &tasks->tss[RPID(unipid)];
	NewTSS(ts, _pr_pc, SEG_NO_TSS, unipid);
	// Set main stack.
	ts->esp = _pr_stack+(_stacksize-16);

	// Kernel memory base
	thread_base[RPID(unipid)] = 0;
	// Preferences
	thread_special[RPID(unipid)] = 0; // 0 = non-zombie
	thread_magic[RPID(unipid)] = ThreadGiveMagic();

	//-----------------------------------------------------------------
	// Note: Thread base must be set before this function is called.
	ren = _pr_stack+(_stacksize-THREADS_STACKROLLBACK);
/*	set_thread(unipid, ren,
			dataSeg,codeSeg,dataSeg,
			_pr_pc);*/

	//
	SetThreadPriority(unipid, DEFAULT_RUNNING_PRIORITY);

	// Adapt device drive and current working directory
	// from the parent process.
	dnr =	GetCurrentDNR();
	db =	GetCurrentDB();
	SetThreadDNR(unipid, dnr);
	SetThreadDB(unipid, db);
	SetThreadPriority(unipid, THREAD_PRIORITY_NORMAL);

	// Identify process.
	sprintf(str, "process%x", unipid);
	IdentifyThread(unipid, str);

	// Define as a kernel process as default.
	SetThreadType(unipid, THREAD_TYPE_KERNEL);

	//-----------------------------------------------------
	// Restore interrupt state.
	set_eflags(flags);

	// Return UNIPID #
	return unipid;
}

// This function is used by LaunchApp.c...
DWORD _createAppThread(int CODESEG, int DATASEG, 
	int base, int pc, int stackoffs)
{
	DWORD key, flags;
	int dnr,db,i,unipid;
	char *ren;
	char str[256];

	//
	flags = get_eflags();
	disable();

	//
	unipid = newUniqueThread();
	key = RPID(unipid);

	// Set thread base.
	thread_base[key] = base;
	// Preferences.
	thread_special[key] = 0; // 0 = non-zombie
	thread_magic[key] = ThreadGiveMagic();

	// Note: thread base must be set before this function is called!
//	set_thread(key, stackoffs,
//			DATASEG,CODESEG,DATASEG,
//			pc);

	//
	SetThreadPriority(key, DEFAULT_RUNNING_PRIORITY);

	// Adapt device drive and current working directory
	// from the parent process.
	dnr =	GetCurrentDNR();
	db =	GetCurrentDB();
	SetThreadDNR(key, dnr);
	SetThreadDB(key, db);
	// Set CPU spending counter to zero
	thread_CPUspending[key] = 0;
	SetThreadPriority(key, THREAD_PRIORITY_NORMAL);

	//
	sprintf(str, "process%d", key);
	IdentifyThread(key, str);

	// Create TSS entry.
	NewTSS(&tasks->tss[key], pc, 0x48, key);
	// Set data segments.
	tasks->tss[key].ds = DATASEG;
	tasks->tss[key].es = DATASEG;
	tasks->tss[key].fs = DATASEG;
	tasks->tss[key].gs = DATASEG;
	// Set stack SS:ESP.
	tasks->tss[key].ss = DATASEG;
	tasks->tss[key].esp = stackoffs;
	// Set CS:EIP.
	tasks->tss[key].cs = CODESEG;
	tasks->tss[key].eip = pc;

	//
	set_eflags(flags);

	//
	return unipid;
}

// Returns PID
// note: doesn't actually make the thread working,
// use create_thread for actual creation.
// NEW: now avoids zombies.
DWORD new_thread(void)
{
	int i,key,flags,tm;

	//
	tm=0;
repeat:
	// Find free entry
	for(i=0; i<nr_threads; i++)
	{
		if( !thread_states[i]
			&&
		thread_special[i]!=ISZOMBIE
			&&
		thread_special[i]!=ISDEZOMBIE )
			// Return key
			return i;
	}

	// Already got too many threads?
	if(nr_threads>=(N_MAX_THREADS-4))
	{
		// Free all dezombies
		for(i=0; i<nr_threads; i++)
			if(thread_special[i]==ISDEZOMBIE)
				thread_special[i] = 0;

		// Repeat search
		tm++;
		if(tm>=10)
			// Out of threads!
			return 0;
		goto repeat;
	}

	// Allocate a new entry
	key=nr_threads; nr_threads++;

	// Return key
	return key;
}

// set_thread_memory_space
// -----------------------
//
// This function is needed to give applications
// own code/data segments.
// Though this function is not needed when
// a kernel thread is being created,
// because a kernel thread is created as default.
// So f.e. a systemcall which creates a thread,
// always creates an application thread,
// but a direct call to thread mgt. create_thread
// function creates a kernel mode thread.
//
void set_thread_memory_space(DWORD unipid,
			DWORD offset,
			WORD limit)
{
	int pid;

	// Convert UNIPID to PID
	pid = RPID(unipid);

	// RENDER CODE SEGMENT DESCRIPTOR
	// ------------------------------
	// Segment limit       (0-15 - 16 bits)
	thread_codedescs[0 + pid*8] = limit&255;
	thread_codedescs[1 + pid*8] = (limit>>8)&255;
	// Base                (0-15 - 16 bits)
	thread_codedescs[2 + pid*8] = (offset)&255;
	thread_codedescs[3 + pid*8] = (offset>>8)&255;
	// Base II             (16-23 - 8 bits)
	thread_codedescs[4 + pid*8] = (offset>>16)&255;
	// Flags
	thread_codedescs[5 + pid*8] = 0x9a;
	// Access
	thread_codedescs[6 + pid*8] = 0xcf;
	// Base III            (24-31 - 8 bits)
	thread_codedescs[7 + pid*8] = (offset>>24)&255;

	// RENDER DATA SEGMENT DESCRIPTOR
	// ------------------------------
	// Segment limit       (0-15 - 16 bits)
	thread_datadescs[0 + pid*8] = limit&255;
	thread_datadescs[1 + pid*8] = (limit>>8)&255;
	// Base                (0-15 - 16 bits)
	thread_datadescs[2 + pid*8] = offset&255;
	thread_datadescs[3 + pid*8] = (offset>>8)&255;
	// Base II             (16-23 - 8 bits)
	thread_datadescs[4 + pid*8] = (offset>>16)&255;
	// Flags
	thread_datadescs[5 + pid*8] = 0x92;
	// Access
	thread_datadescs[6 + pid*8] = 0xcf;
	// Base III            (24-31 - 8 bits)
	thread_datadescs[7 + pid*8] = (offset>>24)&255;
}

// This function sets a state fo a thread
// see thread.h for example priorities
// (e.g. THREAD_PRIORITY_NORMAL)
//
void set_prio(DWORD unipid, BYTE prio)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	thread_states[RPID(unipid)]=prio;
}

// This function gets a state of a thread
// See set_prio too.
//
BYTE get_prio(DWORD unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	return thread_states[RPID(unipid)];
}

// KillThread - terminates a process.
//
// This function kills a thread.
// A special method is used to kill a thread.
// In this method we do set the state of
// the specified thread to 255, which means
// in human language "waiting to be terminated".
// This method is only made to prevent a conflict
// being happending which would be something
// like following:
//
// - Thread is being killed.
// - Thread's entry is reused by some coincidence
//   at about the same time, causing an allocation
//   on the thread that is actually currently
//   still running.
//
// Special note:
// This method is still being unimplented since
// it only related to the cases where the thread
// terminates itself.
//
// 02.09.2003:
// - Added some rights management.
// - Immortal processes now cannot be terminated.
// 12.05.2002:
// - Dead process file descriptor deallocation.
// 25.04.2002(obsolete):
// - Dead process memory deallocation.
// (x.x.2003: now done at KZOMBIE)
// 24.04.2002:
// - Added thread GDT entry deallocation.
// - Added thread memory space deallocation.
//
void KillThread(DWORD pid)
{
	//
	KillThreadEx(pid, 0);
}

// The real kill thread function.
void KillThreadEx(DWORD unipid, DWORD options)
{
	void *threadBasement;
	int pid;
	int flags;

	//---------------------------------------------------
	// Verify that the process really exists.
	//
	UNIF(unipid) {} else return;

	// Get real PID
	pid = RPID(unipid);

	//---------------------------------------------------
	// Apply thread type rule.
	//
	if( (GetThreadType(unipid)&THREAD_TYPE_IMMORTAL) && GetCurrentThread()!=unipid )
	{
		// Refuse to kill an immortal process.
		printk("\nProcess %d attempted terminate an immortal process.\n",
			GetCurrentThread());
		ExitThread(0);
	}

	//
	if(pid==0)
	{
		// Refuse to kill process 0 (illegal request).
		panic("Attempted to kill process 0.");
	}

	// Already dead?
	if(thread_states[pid]==0)
	{
		return;
	}

	//----------------------------------------------
	// We have to do this safely
	flags = get_eflags();
	disable();

	//----------------------------------------------
	// Deallocate all PID devices.
	DevRemPID(pid);

	//----------------------------------------------
	// Set thread state to non-running.
	thread_states[pid]=0;

	//----------------------------------------------
	// We need to dellocate name?
	if(!illegal(thread_name[pid]) && thread_name[pid])
	{
	//// ** Not dellocated nowadays **:
	//	free(thread_name[pid]);
	//	thread_name[pid]=NULL;
	}

	// Free all terminals which this process owned
	FreePIDTerminals(pid);

	//
/*	//----------------------------------------------
	// We need to deallocate terminal?
	// Terminals 0-8 are static EGA terminals.
	if(thread_terminal[pid]>8)
	{
		// Deallocate the terminal
		FreeScreen(&screens[pid]);
	}*/

	//----------------------------------------------
	// We need to deallocate memory space?
	threadBasement = thread_base[pid];
	if(threadBasement)
	{
		// ___ THIS IS NOW DONE IN DEZOMBIE PROCESS ___
		// It is rather hard to deallocate itself
		// without getting a page fault.

		// Deallocate base
//		free(threadBasement);
	}

	// Set thread's message box PTR to NULL
	// (dezombie will deallocate the allocated data)
	if(thread_msgbox[pid])
	{
		thread_msgbox[pid] = NULL;
	}

	// We have a selector to free?
	if(thread_selector[pid]>=0x100)
	{
		FreeGDTEntry(thread_selector[pid]);
		thread_selector[pid]=0;
	}

	//
	deallocate_proc_filedes(pid);

	// Bugfix: First states[..] must be set to zero.
	// Before callinf freeprocmem.
	thread_states[pid]=0;
 
	// Bugfix: This must be done before endless wait loop.
	// Dellocate all kernel malloc -allocated memory.
	if(!XON_DEZOMBIE)	malloc_freeprocmem(pid);

	// ===========================================================
	// Finally clear all the preferences of the thread.
	//

	// Make scall not active for this thread.
	thread_scall_active[pid]=0;
	//
	thread_ds[pid]=0x10; // default => kernel data segment
	thread_es[pid]=0x10;
	thread_fs[pid]=0x10;
	thread_gs[pid]=0x10;
	thread_ss[pid]=0x10;
	thread_esps[pid]=0;
	if(thread_name[pid])
		strcpy(thread_name[pid], "zombie");
//	thread_cmdline[pid]=NULL;
	thread_tick[pid]=0;
	thread_base[pid]=0;
	thread_selector[pid]=0;
	thread_banks[pid]=0;
	thread_terminal[pid]=0;
	thread_type[pid]=0;
	// DNR & DB
	thread_dnr[pid]=0;
	thread_db[pid]=0;
	//
	if(!XON_DEZOMBIE)
		thread_special[pid]=0;
	else
		// For garbage collector thread:
		thread_special[pid]=ISZOMBIE;

	// !!!! FIXED HERE !!!!
	// We are the same thread?
	// YES => We conduct an immediate
	// switch to prevent conflicts.
	if(pid == GetCurrentThread() && !(options&KT_NO_THREAD_SWITCH) )
	{
		// Disable interrupts to secure schedule
		disable();
		while(1) { SwitchToThread(); }
	}

	// Restore flags
	set_eflags(flags);

	// Return back...
	return;
}

// Waits until specified thread exits
void wait_thread(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	//
	while( get_prio(unipid) ) { SwitchToThread(); }

	//
	return;
}

// Exit thread, exists current thread
//
void ExitThread(int rval)
{
	//
	KillThread(GetCurrentThread());
	while(1) { SwitchToThread(); }
}

#define CHECKSEG(x)	\
	if(x>0x1FF || x<0) \
	{ \
		panic("CHECKSEG: ILLEGAL SEGMENT REGISTER"); \
	}

//
void copysregstothread(void)
{
	thread_ds[nr_curthread] = tmpds; CHECKSEG(tmpds);
	thread_es[nr_curthread] = tmpes; CHECKSEG(tmpes);
	thread_fs[nr_curthread] = tmpfs; CHECKSEG(tmpfs);
	thread_gs[nr_curthread] = tmpgs; CHECKSEG(tmpgs);
	thread_ss[nr_curthread] = tmpss; CHECKSEG(tmpss);
}

//
void copysregsfromthread(void)
{
	tmpds = thread_ds[nr_curthread]; CHECKSEG(tmpds);
	tmpes = thread_es[nr_curthread]; CHECKSEG(tmpes);
	tmpfs = thread_fs[nr_curthread]; CHECKSEG(tmpfs);
	tmpgs = thread_gs[nr_curthread]; CHECKSEG(tmpgs);
	tmpss = thread_ss[nr_curthread]; CHECKSEG(tmpss);
}

//
void SetThreadStack(THREAD *t, BYTE *buf, int length)
{
	// set stack params
	t->stack =	buf;
	t->l_stack =	length;

	// wipe the stack (0xbd = C64 action replay filling code)
	memset(t->stack, 0xbd, t->l_stack);
}

// Returns current thread base offset
DWORD GetCurrentThreadBase(void)
{
	// Get base offset
	return GetThreadBase(GetCurrentThread());
}

// Returns thread limit
DWORD GetThreadLimit(int unipid)
{
	static DWORD i,i2,i3,i4,pid;
	int flags;

	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	// Get real PID
	pid = RPID(pid);

	//
	flags = get_eflags();
	disable();

	//
	i=0;
	//
	// Base                (0-15 - 16 bits)
	i |= thread_datadescs[0 + pid*8];
	i |= (thread_datadescs[1 + pid*8]<<8);

	//
	set_eflags(flags);

	//
	return i;
}

// Get a thread base
DWORD GetThreadBase(int unipid)
{
	static DWORD i,i2,i3,i4,flags;

	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	flags = get_eflags();
	disable();

	// New code added
	i = thread_base[RPID(unipid)];

	//
	set_eflags(flags);

	//
	return i;
}

/*
 * Creates a proper memory space for an application
 * to run at. It is needed when preparing to run
 * a program at a private memory space.
 * Naturally, the binary code(appbin) is copied within
 * the memory space.
 */
int MakeAppMemoryMap(APPLICATION *app,
                BYTE *appbin,int l_code,DWORD runsize)
{
	static int i,i2,i3,i4;

	//
	if(!appbin || !app ||
		!l_code || !runsize)
	{
		// If invalid parameters entered,
		// return an error value.
		return 1;
	}

	//
	// Thoughts about the allocation
	// of the application memory space:
	//
	// It's actually quite funny since the allocation of
	// another application's virtual memory space is
	// actually made by using the kernel's malloc.
	// It isn't very low level, but I like it.
	//

	// Allocate space for the program
	app->buf =     malloc( ((runsize/65536)+1)*65536 );
	app->l_buf =   ((runsize/65536)+1)*65536;

	// Specify each of the sections seperately:
	app->stackarea =       app->buf;
	app->comarea =         (app->buf + (1024*8 * 1));
	app->codearea =        (app->buf + (1024*8 * 2));

	//
	dprintk("Caught binary copying: stk %x, com %x, cod %x, %d\n",
		app->stackarea,
		app->comarea,
		app->codearea,
		l_code);
//	ESCLOCK();

	// Copy the binary inside the application's virtual
	// memory space
	for(i=0; i<l_code; i++) { app->codearea[i]=appbin[i]; }

	//
	return 0;
}

//
void MapThreadGDT(APPLICATION *app)
{
	//
	dprintk("MapThreadGDT function call:\n");
	dprintk("<This is debug note> suggestion as base = 0x%x, pid=%d, length=%d banks\n",
		app->buf,app->thread->pid,
		app->l_buf/65536);

	// Specify memory area
	set_thread_memory_space(app->thread->pid, app->buf, app->l_buf/65536);
}

// This function terminates an application,
// it does also do the neccesary memory
// deallocation(freeing).
//
int TerminateApplication(APPLICATION *app)
{
	int flags;

	//
	if(!app)return 1;

	// - - - - - - - - - - - - - - - - - - - - - - - -
	// Disable interrupts(push flags too)
	flags = get_eflags();
	disable();

	//
	app->stackarea =	NULL;
	app->comarea =		NULL;
	app->codearea =	NULL;

	//
	app->l_buf =		0;

	// If something allocated, deallocate it.
	if(app->buf)
		free(app->buf);
	app->buf =		NULL;

	//
	KillThread(app->thread->pid);

	//
	app->thread->pid=0;

	// - - - - - - - - - - - - - - - - - - - - - - - -
	// Restore flags
	set_eflags(flags);

	//
	return 0;
}

// Convert real PID to UNIPID
DWORD GetThreadUniquePID(int pid)
{
	//
	return (thread_uniqid[pid] & UNIQMASK2) | (pid & UNIQMASK);
}

//
DWORD GetRunningThreadsCount(void)
{
	int i,count;

	//
	for(i=0,count=0; i<nr_threads; i++)
	{
		if(GetThreadPriority( GetThreadUniquePID(i) )) count++;
	}

	//
	return count;
}

// The GetTickCount function retrieves the number of milliseconds that have elapsed since JTMOS was started. 
DWORD GetTickCount(void)
{
	return glvar.timer[10];
}

// Get tick count #2
DWORD GetTickCount2(void)
{
	return glvar.timer[11];
}

// Set thread type
void SetThreadType(int unipid, int which)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return;

	// Set thread type
	thread_type[RPID(unipid)] = which;
}

// Get thread type
int GetThreadType(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	// Get type
	return thread_type[RPID(unipid)];
}

// Thread DNR
int GetThreadDNR(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return thread_dnr[RPID(unipid)];
}

// Set thread DNR
int SetThreadDNR(int unipid, int what)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	thread_dnr[RPID(unipid)] = what;
	return 0;
}

//
int GetThreadDB(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return thread_db[RPID(unipid)];
}

//
int SetThreadDB(int unipid, int what)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	thread_db[RPID(unipid)] = what;
	return 0;
}

//
int GetCurrentDNR(void)
{
	return GetThreadDNR(GetCurrentThread());
}

//
int GetCurrentDB(void)
{
	return GetThreadDB(GetCurrentThread());
}

//
void SleepSecond(void)
{
	DATE d,od;

	// Get old date
	rtc_getdate(&od);

	// Skip past this second
	while(1)
	{
		// Get time
		rtc_getdate(&d);
		if(d.second!=od.second)
			break;

		// CPU idling
		if(Multitasking)
			SwitchToThread();
	}
}

// Sleeps specified duration of seconds
int sleep(int seconds)
{
	int i;

	//
	SleepSecond();

	// Sleep specified amount of time
	for(i=0; i<seconds; i++)
	{
		SleepSecond();
		if(Multitasking!=0)	SwitchToThread();
	}
	return 0;
}

// Get thread CPU spending
int GetThreadSpending(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return thread_CPUspending[RPID(unipid)];
}

// Get thread memory usage
// Returns amount of bytes thread uses.
// 0 for kernel mode.
int GetThreadMemoryUsage(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return thread_banks[RPID(unipid)] * 1024*64;
}

// Get thread name
int GetThreadName(int unipid, char *s)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	if(thread_name[RPID(unipid)]==NULL)
		strcpy(s, "[NULL]");
		else
		strcpy(s, thread_name[RPID(unipid)]);
	return 0;
}

// Returns the beginning of memory region that
// is visible to the specified thread
DWORD GetThreadMemoryStart(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return GetThreadBase(unipid);
}

// Returns the end of memory region that
// the specified thread can access.
DWORD GetThreadMemoryEnd(int unipid)
{
	// Check that the process really exists.
	UNIF(unipid) {} else return 0;

	//
	return GetThreadBase(unipid) + GetThreadMemoryUsage(unipid);
}

//


