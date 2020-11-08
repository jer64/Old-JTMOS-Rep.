//
#ifndef __INCLUDED_GPF_H__
#define __INCLUDED_GPF_H__

//
void ESCLOCK(void);
void STACKDUMP(DWORD);
void PRGDUMP(void);
void PROCESSINFO(void);
void FATALEXCEPTION(const char *s);
void tss_dump(TSSEG *s);

// corner.mac
extern char irqActive;
extern int EXCEPTION_HANDLER_ACTIVE;

//
#define DESCLOCK while(inportb(0x60) == 1); \
	while(inportb(0x60) != 1);

//
#endif

//
