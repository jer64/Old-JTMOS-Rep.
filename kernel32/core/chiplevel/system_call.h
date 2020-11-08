#ifndef __INCLUDED_SYSTEM_CALL_H__
#define __INCLUDED_SYSTEM_CALL_H__

//
#include "basdef.h"
#include "scconf.h" // contains only #define :s

//
extern int scallsIsWorking;

//
extern DWORD scall_eax;
extern DWORD scall_ebx;
extern DWORD scall_ecx;
extern DWORD scall_edx;
extern DWORD scall_esi;
extern DWORD scall_edi;
extern DWORD scall_ebp;

//
void scall_init(void);
extern void scall_handler(void);

// Example for calling:
// scall(1, 0,0,0,0 0,0,0);
// (calls relax_thread)

//
extern DWORD scall(DWORD eax,DWORD ebx,DWORD ecx,DWORD edx,
                DWORD esi,DWORD edi,DWORD ebp);

#endif

//

