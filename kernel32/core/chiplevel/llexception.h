#ifndef __INCLUDED_LLEXCEPTION_H__
#define __INCLUDED_LLEXCEPTION_H__

//
#include "basdef.h"

#ifndef EXCMAC
#define EXCMAC                  asm("cli"); \
                                asm("movw $0x10,%ax"); \
                                asm("movw %ds,%ax"); \
                                asm("movw %es,%ax"); \
                                asm("movw %fs,%ax"); \
                                asm("movw %gs,%ax"); \
                                asm("movw %ss,%ax"); \
                                asm("movl $0xffff,%esp");
#endif

// ??
//extern DWORD lowlevel_esp,lowlevel_ss;

//
void llexception_0(void);
void llexception_1(void);
void llexception_2(void);
void llexception_3(void);
void llexception_4(void);
void llexception_5(void);
void llexception_6(void);
void llexception_7(void);
void llexception_8(void);
void llexception_9(void);
void llexception_10(void);
void llexception_11(void);
void llexception_12(void);
void llexception_13(void);
void llexception_14(void);
void llexception_15(void);
void llexception_16(void);

// from excep.asm
extern void ASMllexception_0(void);
extern void ASMllexception_1(void);
extern void ASMllexception_2(void);
extern void ASMllexception_3(void);
extern void ASMllexception_4(void);
extern void ASMllexception_5(void);
extern void ASMllexception_6(void);
extern void ASMllexception_7(void);
extern void ASMllexception_8(void);
extern void ASMllexception_9(void);
extern void ASMllexception_10(void);
extern void ASMllexception_11(void);
extern void ASMllexception_12(void);
extern void ASMllexception_13(void);
extern void ASMllexception_14(void);
extern void ASMllexception_15(void);
extern void ASMllexception_16(void);
extern void HaltNow(void);

//
void setup_llexception_handlers(void);


#endif

