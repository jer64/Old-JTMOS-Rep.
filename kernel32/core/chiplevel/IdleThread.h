#ifndef __INCLUDED_IDLETHREAD_H__
#define __INCLUDED_IDLETHREAD_H__

//
#include "basdef.h"

//
extern DWORD IdleThreadCounter;

//
void IdleThreadProgram(void);
void init_IdleThread(void);
extern void jump1234(void);
extern THREAD IdleThread;

#endif
