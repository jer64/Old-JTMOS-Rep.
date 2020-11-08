//=====================================================
// LIBC Process Management.
// (C) 2002-2005 by Jari Tuominen.
//=====================================================
#include <jtmos/basdef.h>
#include <jtmos/process.h>
#include <jtmos/malloc.h>
#include <jtmos/env.h>
#include <jtmos/jmp_buf.h>
#include <jtmos/date.h>
#include <jtmos/msgbox.h>

//
extern DWORD appregionstart,appregionend,end;
// Variables for usefulness
int errno=0;
int __djgpp_set_sigint_key=0;
// For emulation purpose:
DWORD ___djgpp_exception_state_ptr;
// Current path search order
char *SysSearchPath=JTMOS_PATH;
// Current work directory
char SysCWD[100]="/";

// Set terminal owner
int SetTerminalOwner(int terID, int pid)
{
	//
	return scall(SYS_setterminalowner, terID,pid,0, 0,0,0);
}

// Get terminal owner PID
int GetTerminalOwner(int terID)
{
	//
	return scall(SYS_getterminalowner, terID,0,0, 0,0,0);
}

// Create a new terminal
int CreateTerminal(void)
{
	//
	return scall(SYS_createterminal, 0,0,0, 0,0,0);
}

// Switch CPU to execute specified process
int SwitchToThreadEx(int pid)
{
	//
	return scall(SYS_switchtothreadex, pid,0,0, 0,0,0);
}

// Converts linear PID(real PID) to unique PID.
// Unique PIDs are the PIDs that are being used with the system.
int GetThreadUniquePID(int p)
{
	//
	return scall(SYS_getthreaduniquepid, p,0,0, 0,0,0);
}

// Commit a disk change
// All device buffers and caches are deallocated,
// and prepared for a re-read.
int diskChange(int dnr)
{
	//
        return scall(SYS_diskchange, dnr,0,0, 0,0,0);
}

// Set interrupt handler
int setint(int vecnr,
	void *handlerPTR,
	int selector,
	int type)
{
	//
	return scall(SYS_setint,
		vecnr,handlerPTR,selector, type,0,0);
}

// Enable interrupt #
int enable_irq(int irqnr)
{
	//
	return scall(SYS_enableirq,
		irqnr,0,0, 0,0,0);
}

// Get specified terminal's cursor location
int GetCursorLocation(int ter)
{
	//
	return scall(SYS_getcursorlocation,
		ter,0,0, 0,0,0);
}

// Copy terminal's contents to a buffer
int CopyTerminal(int terID, BYTE *buf)
{
	//
	return scall(SYS_copyterminal,
		terID,buf,0, 0,0,0);
}

//
int OutputTerminalKeyboardBuffer(int terid, int ch)
{
	//
	return scall(SYS_outputterminalkeyboardbuffer,
		terid,ch,0, 0,0,0);
}

//
int InputTerminalKeyboardBuffer(int terid)
{
	//
	return scall(SYS_inputterminalkeyboardbuffer,
		terid,0,0, 0,0,0);
}

// Creates a new terminal
int NewTerminal(void)
{
	//
	return scall(SYS_newterminal, 0,0,0, 0,0,0);
}

// Returns current used terminal ID
int GetCurrentTerminal(void)
{
	//
	return GetThreadTerminal(GetCurrentThread());
}

// Get thread terminal
int GetThreadTerminal(int pid)
{
	//
	return scall(SYS_getthreadterminal, pid,0,0, 0,0,0);
}

// Creates a new terminal
int SetThreadTerminal(int pid, int what)
{
	//
	return scall(SYS_newterminal, pid,what,0, 0,0,0);
}


// Copy memory chunk from process to process
int globalcopy(int srcPID, DWORD srcOffs,
	int dstPID, DWORD dstOffs,
	int amount)
{
	//
	return scall(SYS_globalcopy, srcPID, srcOffs,
		dstPID, dstOffs,
		amount,0);
}

//
int GetSeconds(void)
{
	//
	return scall(SYS_getseconds, 0,0,0, 0,0,0);
}

//
int IdentifyThread(int pid, const char *name)
{
	//
	return scall(SYS_identifythread, pid,name,0, 0,0,0);
}

// Get thread's PID by name
// Return value -1 means "not found".
int GetThreadPID(const char *name)
{
	//
	return scall(SYS_getthreadpid, name,0,0, 0,0,0);
}

// Receive message
int receiveMessage(MSG *m)
{
	//
	return scall(SYS_receivemessage, m,0,0, 0,0,0);
}

// Send message to a PID
int sendMessage(MSG *m, int pid)
{
	//
	return scall(SYS_sendmessage, m,pid,0, 0,0,0);
}

// Enable PID's ability to receive messages
void startMessageBox(void)
{
	//
	scall(SYS_activatemsgbox, 0,0,0, 0,0,0);
}

// Get current working directory.
char *getcwd(char *buf, int size)
{
	//
	return scall(SYS_getcwd, buf,size,0, 0,0,0);
}

// Change directory.
int chdir(const char *path)
{
	//
	return scall(SYS_chdir, path,0,0, 0,0,0);
}

//
int getc(FILE *f)
{
	return fgetc(f);
}

//
int getchar(void)
{
	return getch();
}

// stackavail - TODO!!!!!!!!!!!!
//
// This function returns the number
// of bytes that are available on the stack. 
int stackavail(void)
{
	return 100;
}

//        abort - cause abnormal program termination
// 
// SYNOPSIS
//        #include <stdlib.h>
// 
//        void abort(void);
void abort(void)
{
	printf("\nAbnormal program termination(PID=%d).\n",
		GetCurrentThread());
	exit(1);
}

char *strerror(int errnum)
{
	return NULL;
}

// clock - Determine processor time
int clock(void)
{
	return 0;
}

//
int WaitProcessTermination(int pid)
{
	return scall(SYS_waitprocterm, pid,0,0, 0,0,0);
}

// Execute program and search it from various paths.
// Note: Waits until command is done.
int system(const char *_cmd)
{
	char *p,*s,*e,*es;
	int i,i2,i3,i4,ii,l,ll;
	char str[100],cmd[100];
	int r;

	//
	if(_cmd==NULL)return 1;

	//
	return cexec(_cmd, "");
}

/*//
int _system(const char *cmd, const char *path)
{
	char tmp[100];
	int pid,rv;

	// Set return value to zero
	rv=0;

	//
	pid = cexec(cmd, tmp);
	return rv;
}*/

// Run a process in background
// Return value: zero means error, other means PID
int bgexec(const char *fname, const char *workDir)
{
	//
	return bgexecEx(fname, workDir, GetCurrentTerminal());
}

//
int bgexecEx(const char *fname, const char *workDir, int terID)
{
	int pid;

	//
	if(fname==NULL)return 0;
	if(!isValidString(fname))return 0;

	//
	pid = 0;

	// fname, [wait until termination], workdir, pid, terID, 0,0
	if( scall(SYS_cexec, fname,0,workDir, &pid,terID,0)<0 )
		return 0;
	else
		return pid;
}

// Request system caster process to create a new process.
//
// Example:
// cexec("ls.bin /home/tapio", "/");
//
// Return value:
// Zero =	Error
// Non-zero =	PID value, which indicates "no error"
// workDir =	Specifies work directory at work at for the prg
int cexec(const char *fname, const char *workDir)
{
	//
	return cexecEx(fname, workDir, GetCurrentTerminal());
}

//
int cexecEx(const char *fname, const char *workDir, int terID)
{
	int pid;

	//
	if(fname==NULL)return 0;
	if(!isValidString(fname))return 0;

	//
	pid = 0;

	// fname, [wait until termination], workdir, pid, terID, 0,0
	if( scall(SYS_cexec, fname,1,workDir, &pid,terID,0)<0 )
		return 0;
	else
		return pid;
}

//
DWORD GetAmountFree(void)
{
	//
	return 12345; // nut number
	//return malloc_getamountfree();
}

//
DWORD GetProgramStart(void)
{
	DWORD ad;

	//
	ad=&appregionstart;
	return ad;
}

//
DWORD GetProgramEnd(void)
{
	DWORD ad;

	//
	ad=&appregionend;
	return ad;
}

// Returns size of the application memory in bytes.
DWORD GetMemorySize(void)
{
	//
	return scall(SYS_GETAMEMSIZE, 0,0,0, 0,0,0);
}

//
int kill(int pid, int sig)
{
	//
	return 0;
}

// Terminates the process
int KillThread(int pid)
{
	//
	return scall(SYS_killthread, pid,0,0, 0,0,0);
}

// exit
// A normal way to terminate program.
// Calls system call scall_exit to
// terminate the current process.
//
void exit(int _code)
{
	// 8 => scall_exit
	scall(SYS_EXIT, 0,0,0, 0,0,0);

	// This never should happend:
	puts("LIBC internal error #100 - SYS_EXIT malfunction\n");
	while(1) { SwitchToThread(); }

	//
}

// Idle a moment, saves CPU time.
int SwitchToThread(void)
{
	//
	return scall(SYS_IDLE, 0,0,0, 0,0,0);
}

//
DWORD GetTickCount(void)
{
	//
	return scall(SYS_GETTICKCOUNT, 0,0,0, 0,0,0);
}

// Sleep specified amount of milliseconds.
// Units = 1/100 seconds.
// 25  = 1/4 second
// 50  = 1/2 second
// 100 = 1 second
void MilSleep(int dur)
{
	//
	scall(SYS_milsleep, dur,0,0, 0,0,0);
}

// Fix later, TODO.
void Sleep(DWORD cMilliseconds)
{
	//
	long i,i2,i3,i4;

	//
	i4 = GetTickCount();

	//
	for(; GetTickCount()<(i4+cMilliseconds); )
	{
		//
		SwitchToThread();
	}

	//
}

// ExitThread, works ok.
void ExitThread(DWORD dwExitCode)
{
	//
	exit(dwExitCode);
}

// See GetCurrentThread...
int getpid(void)
{
	return GetCurrentThread();
}

// Return current process ID.
int GetCurrentThread(void)
{
	//
	return scall(SYS_GETCURRENTTHREAD, 0,0,0, 0,0,0);
}

// TODO
char SetThreadPriority(int hThread,int nPriority)
{
	//

	//
	return 0;
}

// Get thread priority
int GetThreadPriority(int pid)
{
	return scall(SYS_getthreadpriority, pid,0,0, 0,0,0);
}

// Get thread spending
int GetThreadSpending(int pid)
{
	return scall(SYS_getthreadspending, pid,0,0, 0,0,0);
}

// Get thread name
int GetThreadName(int pid, char *s)
{
	return scall(SYS_getthreadname, pid,s,0, 0,0,0);
}

// Get thread memory usage
int GetThreadMemoryUsage(int pid)
{
	return scall(SYS_getthreadmemoryusage, pid,0,0, 0,0,0);
}

//
int GetThreadCount(void)
{
	return scall(SYS_getthreadcount, 0,0,0, 0,0,0);
}

//
void SleepSecond(void)
{
        DATE d,od;

	//
        getdate(&od);

        // Skip past this second
        while(1)
        {
		//
                getdate(&d);
                if(d.second!=od.second)break;
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
		//
		SleepSecond();
		SwitchToThread();
	}
	return 0;
}

// Get page size
size_t getpagesize(void)
{
	//
	return 4096;
}

//


