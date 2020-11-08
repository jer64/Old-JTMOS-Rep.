/*** Low level IO functions ***/
#ifndef __INCLUDED_IO_H__
#define __INCLUDED_IO_H__

//
#include "basdef.h"

#ifdef __CPLUSPLUS__
#extern "C" {
#endif

#define outb(value,port) \
__asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))


#define inb(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); \
_v; \
})

#define outb_p(value,port) \
__asm__ ("outb %%al,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))

#define inb_p(port) ({ \
unsigned char _v; \
__asm__ volatile ("inb %%dx,%%al\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})

#define outw_p(value,port) \
__asm__ ("outw %%ax,%%dx\n" \
		"\tjmp 1f\n" \
		"1:\tjmp 1f\n" \
		"1:"::"a" (value),"d" (port))

#define inw_p(port) ({ \
unsigned short int _v; \
__asm__ volatile ("inw %%dx,%%ax\n" \
	"\tjmp 1f\n" \
	"1:\tjmp 1f\n" \
	"1:":"=a" (_v):"d" (port)); \
_v; \
})

/*
#define outd_p(value,port) \
__asm__ ("outd %%eax,%%dx\n" \
                "\tjmp 1f\n" \
                "1:\tjmp 1f\n" \
                "1:"::"a" (value),"d" (port))

#define ind_p(port) ({ \
DWORD _v; \
__asm__ volatile ("ind %%dx,%%eax\n" \
        "\tjmp 1f\n" \
        "1:\tjmp 1f\n" \
        "1:":"=a" (_v):"d" (port)); \
_v; \
})
*/

#ifdef __CPLUSPLUS__
}
#endif

extern void outportb(WORD port,BYTE value);
extern BYTE inportb(WORD port);
extern void outportw(WORD port,WORD value);
extern WORD inportw(WORD port);

// from asmio.asm
void outportd(DWORD _portad,DWORD _value);
DWORD inportd(DWORD _portad);

#endif
