#ifndef __INCLUDED_SCS_DEVICES_H__
#define __INCLUDED_SCS_DEVICES_H__

//
#include "scs.h"

//
DWORD scall_getdevnrbyname(SYSCALLPAR);
DWORD scall_readblock(SYSCALLPAR);
DWORD scall_writeblock(SYSCALLPAR);

#endif
