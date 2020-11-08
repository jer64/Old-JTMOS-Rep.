//
#ifndef __INCLUDED_PROCESSPRIORITIES_H__
#define __INCLUDED_PROCESSPRIORITIES_H__

//
#define THREAD_PRIORITY_TIME_CRITICAL           1
#define THREAD_PRIORITY_HIGHEST                 1
#define THREAD_PRIORITY_ABOVE_NORMAL            1
#define THREAD_PRIORITY_NORMAL                  1
#define THREAD_PRIORITY_BELOW_NORMAL            2
#define THREAD_PRIORITY_IDLE                    4
#define THREAD_PRIORITY_LOWEST                  8

// Termination of the process is not possible.
#define THREAD_TYPE_IMMORTAL			1
// The process is an application.
#define THREAD_TYPE_APPLICATION			2
// The process is a service.
#define THREAD_TYPE_SERVICE			4
// The process runs in kernel mode.
#define THREAD_TYPE_KERNEL			8
// The process system calls are being traced.
#define THREAD_TYPE_SYSCALL_TRACE		16

#endif

