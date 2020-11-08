#ifndef _I386_JMP_BUF_H
#define _I386_JMP_BUF_H

/* Define the machine-dependent type `jmp_buf'.  Intel 386 version.  */
#include "basdef.h"

#define __ptr_t DWORD

typedef struct jmp_buf {
  unsigned long __eax, __ebx, __ecx, __edx, __esi;
  unsigned long __edi, __ebp, __esp, __eip, __eflags;
  unsigned short __cs, __ds, __es, __fs, __gs, __ss;
  unsigned long __sigmask; /* for POSIX signals only */
  unsigned long __signum; /* for expansion */
  unsigned long __exception_ptr; /* pointer to previous exception */
  unsigned char __fpu_state[108]; /* for future use */
} jmp_buf[1];

#endif /* _I386_JMP_BUF_H */

