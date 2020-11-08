/*** Low level IO functions ***/
#include "io.h"
#include "delay.h"
#include "basdef.h" // BYTE, WORD, DWORD..

//
void outportb(WORD port,BYTE value)
{
 outb_p(value,port);
}

//
BYTE inportb(WORD port)
{
 return inb_p(port);
}

//
void outportw(WORD port,WORD value)
{
 outw_p(value,port);
}

//
WORD inportw(WORD port)
{
 return inw_p(port);
}

/*
//
void outportd(WORD port,DWORD value)
{
 outd_p(value,port);
}

//
DWORD inportd(WORD port)
{
 return ind_p(port);
}
*/

