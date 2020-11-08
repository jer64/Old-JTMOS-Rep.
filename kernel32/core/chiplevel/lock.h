#ifndef __INCLUDED_LOCK_H__
#define __INCLUDED_LOCK_H__

//
#define LOCK static int granted_pid = 0; if(Multitasking) { disable(); while(granted_pid!=0) { SwitchToThread(); } granted_pid = GetCurrentThread(); enable(); }

//
#define UNLOCK if(Multitasking) { granted_pid = 0; }

#endif

