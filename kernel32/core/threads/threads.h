#ifndef	__INCLUDED_THREADS_H__
#define	__INCLUDED_THREADS_H__

//
#define pid_t int

// Options for KillThreadEx.
#define KT_NO_THREAD_SWITCH		1

// Looping PIDs is a way to prevent PID collisions
//#define LOOPING_PIDS

// Unique PIDs is another way to prevent PID collisions
// (not working yet!)
//#define USE_UNIQUE_PIDS

//
#include "basdef.h"
#include "env.h" // ENVDB
#include "tss.h"
#include "int.h"
#include "msgbox.h"
#include "unipid.h"

// We still need to modify this structure abit,
// but it's okay even like this, just so it
// takes enough space will fit the purpose.
typedef struct
{
	//
	char something[100];
}FPUREGS;

//
DWORD GetThreadUniquePID(int pid);
int unif(int x);

// FREEZE & UNFREEZE, freezes CPU so that
// no interrupts will occur and therefore
// also no task scheduling will interrupt
// the action.
// NOTE: freeze and unfreeze can be done
// only once within a single function area.
#define FREEZE()	int ___flags; \
			___flags = get_eflags(); \
			disable();

#define UNFREEZE()	set_eflags(___flags);


//
#include "threadscfg.h"

//
#include "basdef.h"
#include "syscallcodes.h"
#include "LaunchApp.h"
#include "threadpriorities.h"
#include "terminal.h"

// Default running priority for new threads
#define DEFAULT_RUNNING_PRIORITY		THREAD_PRIORITY_TIME_CRITICAL

//
typedef struct
{
	//
	int pid;     // process ID
	BYTE *stack;  // stack pointer
	int l_stack; // stack length
}THREAD;

//
typedef struct
{
	//
	THREAD *thread;
	// PID number for the application
	int pid;
	// Whole area
	int l_buf;
	BYTE *buf;
	// Seperate areas
	BYTE *stackarea,*comarea,*codearea;
}APPLICATION;

//
extern void copysregstothread(void);
extern void copysregsfromthread(void);

// Thread Management
//
DWORD new_thread(void);
void set_thread(DWORD _handle1,DWORD _stackoffs,
		DWORD _ss,DWORD _cs,DWORD _ds,
		DWORD _process_pc);

// From thread_db.c:
// Data that is allocated by the thread management code
 int nr_curthread,nr_threads;
// ASM code
 DWORD RenderThreadStack(DWORD _stack_offs,DWORD _pc,DWORD pid,
			DWORD _ss,DWORD _cs,DWORD _ds);
//
BYTE thread_scall_active[N_MAX_THREADS];
DWORD thread_magic[N_MAX_THREADS];
DWORD threadtmp[200];
DWORD thread_uniqid[N_MAX_THREADS];
BYTE thread_type[N_MAX_THREADS];
BYTE thread_codedescs[(N_MAX_THREADS*8)];
BYTE thread_datadescs[(N_MAX_THREADS*8)];
BYTE thread_states[N_MAX_THREADS];
DWORD thread_esps[N_MAX_THREADS];
char *thread_name[N_MAX_THREADS]; // NULL = no name(name/identification)
BYTE thread_tick[N_MAX_THREADS];
// stack segment selector register status
WORD thread_ss[N_MAX_THREADS];
//
FPUREGS *thread_fpuregs[N_MAX_THREADS];
ENVDB *thread_envdb[N_MAX_THREADS];
// data segments
WORD thread_ds[N_MAX_THREADS];
WORD thread_es[N_MAX_THREADS];
WORD thread_fs[N_MAX_THREADS];
WORD thread_gs[N_MAX_THREADS];
WORD thread_selector[N_MAX_THREADS];
WORD thread_ml[N_MAX_THREADS]; // memory load
DWORD thread_base[N_MAX_THREADS];
WORD thread_banks[N_MAX_THREADS]; // Amount of 64K banks allocated for the thread.
WORD thread_memkb[N_MAX_THREADS];
BYTE thread_terminal[N_MAX_THREADS];
BYTE thread_special[N_MAX_THREADS];
char *thread_cmdline[N_MAX_THREADS];
BYTE thread_idling[N_MAX_THREADS];
// Current Device Number
DWORD thread_dnr[N_MAX_THREADS];
// Current Dir Block
DWORD thread_db[N_MAX_THREADS];
// CPU spending
DWORD thread_CPUspending[N_MAX_THREADS];
// EIP
DWORD thread_eip[N_MAX_THREADS];
// Thread's Message Box
MSGBOX *thread_msgbox[N_MAX_THREADS];
// Pointer to shell structure.
DWORD thread_shell_pointer[N_MAX_THREADS];

//
int GetCurrentThread(void);
DWORD GetCurrentThreadBase(void);
DWORD GetThreadBase(int pid);
BYTE get_prio(DWORD pid);

//
DWORD create_thread(DWORD _pr_stack,DWORD _stacksize,DWORD _pr_pc);
int GetCurrentTerminal(void);

//
void TrackThread(int pid, char val);
void ThreadCreateStack(THREAD *t,int _stacksize);
void ThreadDestroyStack(THREAD *t);
void IdentifyThread(DWORD pid, const char *_ident);
void _IdentifyThread(DWORD pid, const char *_ident);
//
int GetThreadCount(void);
BYTE GetThreadPriority(DWORD pid);
void SetThreadPriority(DWORD pid,BYTE prio);
//
DWORD create_app_thread(APPLICATION *app,
	DWORD _pr_stack,DWORD _stacksize,DWORD _pr_pc,DWORD _real_esp);
void SetThreadStack(THREAD *t, BYTE *buf, int length);
DWORD GetRunningThreadsCount(void);
DWORD GetThreadLimit(int pid);
int GetThreadTerminal(int i);
void SetThreadTerminal(int i,int ter);

// from asmapp.asm
void asmtestapp(void);
// from asmlaunchapp.asm
void asmlaunchapp(void);
extern DWORD appoffset;
extern WORD appseg;
extern DWORD appstackoffs;
extern DWORD apprealoffs;
int SetThreadCmdLine(int pid, const char *CmdLine);
int SwitchToThread(void);
int SwitchToThreadEx(int unipid);

// Get amount of ticks since kernel was started
DWORD GetTickCount(void);
DWORD GetTickCount2(void);
void WaitProcessTermination(int pid);
void SetThreadType(int pid, int which);
int GetThreadType(int pid);

// DNR / DB
int GetThreadDNR(int pid);
int SetThreadDNR(int pid, int what);
int GetThreadDB(int pid);
int SetThreadDB(int pid, int what);
int GetCurrentDNR(void);
int GetCurrentDB(void);
int sleep(int seconds);

//
int GetThreadName(int pid, char *s);
int GetThreadMemoryUsage(int pid);
int GetThreadSpending(int pid);
// Get thread memory region
DWORD GetThreadMemoryStart(int pid);
DWORD GetThreadMemoryEnd(int pid);

//
int GetThreadML(int pid);
void SetThreadML(int pid, int load);

//
int GetSwitchCount(void);

//
int GetThreadPID(const char *name);

//
int getpid(void);

//
extern int force_switch_thread_nr;
extern int Multitasking;

//
void KillThread(DWORD pid);
void KillThreadEx(DWORD unipid, DWORD options);
int ThreadGiveMagic(void);

//
DWORD _createAppThread(int CODESEG, int DATASEG,
        int base, int pc, int stackoffs);

//
DWORD CreateThreadA(DWORD _pr_stack, DWORD _stacksize, DWORD _pr_pc);
DWORD create_threadEx(DWORD _pr_stack, DWORD _stacksize, DWORD _pr_pc,
        WORD codeSeg, WORD dataSeg);
void Immortal(int unipid);
void ExitThread(int rval);
TSSEG *GetThreadTSS(int unipid);

//
#include "IdleThread.h"
#include "threadSwitch.h"

//
#endif

//


