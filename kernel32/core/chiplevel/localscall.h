//
#include "basdef.h"

//
#define LOCAL_SCALL_REGS	DWORD lscall_eax=scall_eax; \
				DWORD lscall_ebx=scall_ebx; \
				DWORD lscall_ecx=scall_ecx; \
				DWORD lscall_edx=scall_edx; \
				DWORD lscall_esi=scall_esi; \
				DWORD lscall_edi=scall_edi; \
				DWORD lscall_ebp=scall_ebp;

