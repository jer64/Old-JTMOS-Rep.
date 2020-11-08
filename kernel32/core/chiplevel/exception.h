#ifndef __INCLUDED_EXCEPTION_H__
#define __INCLUDED_EXCEPTION_H__

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

//
typedef struct
{
 // Exception's identifier code
 DWORD code;
 // Stack pointer/segment at the exception time
 // (32-Bit Linear Address)
 DWORD esp,ss;
 //
 DWORD other,other2,other3,other4;
 //
}EXCEPTION;

//
extern EXCEPTION exception;

// ??
//extern DWORD lowlevel_esp,lowlevel_ss;

//
void exception_0(void);
void exception_1(void);
void exception_2(void);
void exception_3(void);
void exception_4(void);
void exception_5(void);
void exception_6(void);
void exception_7(void);
void exception_8(void);
void exception_9(void);
void exception_10(void);
void exception_11(void);
void exception_12(void);
void exception_13(void);
void exception_14(void);
void exception_15(void);
void exception_16(void);

// from excep.asm
extern void ASMexception_0(void);
extern void ASMexception_1(void);
extern void ASMexception_2(void);
extern void ASMexception_3(void);
extern void ASMexception_4(void);
extern void ASMexception_5(void);
extern void ASMexception_6(void);
extern void ASMexception_7(void);
extern void ASMexception_8(void);
extern void ASMexception_9(void);
extern void ASMexception_10(void);
extern void ASMexception_11(void);
extern void ASMexception_12(void);
extern void ASMexception_13(void);
extern void ASMexception_14(void);
extern void ASMexception_15(void);
extern void ASMexception_16(void);
void HaltNow(void);

//
void setup_exception_handlers(void);


#endif

