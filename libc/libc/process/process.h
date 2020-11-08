//
#ifndef __INCLUDED_PROCESS_H__
#define __INCLUDED_PROCESS_H__

//
#define JTMOS_EXE_FMT	".bin"
#define JTMOS_PATH	"/bin;/ram/bin;/hda/bin;/usr/bin;ram;"

// Application memory configuration options
#define APPRAM_64K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=64"
#define APPRAM_128K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=128"
#define APPRAM_256K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=256"
#define APPRAM_512K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=512"
#define APPRAM_1024K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=1024"
#define APPRAM_2048K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=2048"
#define APPRAM_4096K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=4096"
#define APPRAM_8192K	"SQUIRREL VERIFY APPLICATION MEMORY CONFIGURATION=8192"

// Memory request macro
#define SYSMEMREQ(x) static char *memcfgdef=x;

//
#include <jtmos/basdef.h> // DWORD
#include <jtmos/postman.h>
#include <jtmos/processpriorities.h>
#include <jtmos/syscallcodes.h>
#include <jtmos/signal.h>
#include <jtmos/jmp_buf.h>
#include <jtmos/setjmp.h>
#include <jtmos/longjmp.h>
#include <jtmos/file.h>
#include <jtmos/msgbox.h>
#include <jtmos/scall.h>

//
extern void exit(int _code);
extern void idle_thread(void);
extern void idle_moment(void);
int GetCursorLocation(int ter);

//
extern int errno;

//
int stackavail(void);

//
int getpid(void);
int GetThreadUniquePID(int p);
int cexec(const char *fname, const char *workDir);
int cexecEx(const char *fname, const char *workDir, int terID);
int bgexec(const char *fname, const char *workDir);
int bgexecEx(const char *fname, const char *workDir, int terID);
int SwitchToThread(void);
int SwitchToThreadEx(int pid);

//
void Sleep(DWORD cMilliseconds);
void ExitThread(DWORD dwExitCode);
int GetCurrentThread(void);
char SetThreadPriority(int hThread,int nPriority);
int WaitProcessTermination(int pid);

// Adapted from Win32 API
DWORD GetTickCount(void);
//
DWORD GetMemorySize(void);
DWORD GetProgramStart(void);
DWORD GetProgramEnd(void);
int system(const char *string);
int clock(void);
int CopyThreadTerminal(int terID, BYTE *buf);

//
extern int errno;

//
char *strerror(int errnum);
void abort(void);
int diskChange(int dnr);

//
int getc(FILE *f);
int getchar(void);

//
int kill(int pid, int sig);
int KillThread(int pid);
int GetThreadPriority(int pid);
int GetThreadSpending(int pid);
int GetThreadName(int pid, char *s);
int GetThreadMemoryUsage(int pid);
int IdentifyThread(int pid, const char *name);
int NewTerminal(void);
int GetThreadTerminal(int pid);
int SetThreadTerminal(int pid, int what);
int OutputTerminalKeyboardBuffer(int terid, int ch);
int InputTerminalKeyboardBuffer(int terid);
int GetCurrentTerminal(void);

//
int sleep(int seconds);
void SleepSecond(void);
int GetSeconds(void);

//
void startMessageBox(void);
int receiveMessage(MSG *m);
int sendMessage(MSG *m, int pid);
int GetThreadPID(const char *name);
int globalcopy(int srcPID, DWORD srcOffs,
        int dstPID, DWORD dstOffs,
        int amount);
int SetTerminalOwner(int terID, int pid);
int GetTerminalOwner(int terID);
int CreateTerminal(void);
void MilSleep(int dur);

//
#include <jtmos/errno.h>

//
#endif

//



