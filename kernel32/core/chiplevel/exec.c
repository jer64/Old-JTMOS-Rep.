// ===========================================================
// exec - execute a program
//
// ExecProc - Creates & executes a new application mode process
// ExecKProc - Creates & executes a new kernel mode process
//
// (C) 2002 by Jari Tuominen(jari@vunet.org)
// ===========================================================
#include "int.h"
#include "cpu.h"
#include "sysglvar.h"
#include "kernel32.h" // FATALEXCEPTION
#include "delay.h"
#include "threads.h"

//
void ExecProc(void *appcode)
{
 //
 ExecKProc(appcode);

 //
}

// ExecKProc(testapp,32768);
//
// Create a kernel mode process.
// Runs inside kernel memory.
//
void ExecKProc(void *appcode)
{
 // ------------------------------------------------------------------
 char *appstack;
 long key1,pr,key;
 char str[255];

 // ------------------------------------------------------------------
 // Create app's stack(Later will be changed to thread's ownership).
 // This makes it possible that the thread stack will be
 // automatically deallocated upon termination of it(done at
 // create_thread function).
 appstack=malloc(1024*32);

 //
 key1=0;

 // Create app's thread
 key1 = create_thread(appstack, 1024*16, appcode);
 //
 sprintf(str, "proc%d", GetRunningNumber());
 IdentifyThread(key1, str);

 // Select a safe priority level.
 SetThreadPriority(key1, THREAD_PRIORITY_NORMAL);

 //
 return;
}

//

