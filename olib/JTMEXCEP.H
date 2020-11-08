/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS exception handler library
 *
 * (c)Copyright 1999 by Jari Tapio Tuominen, all rights reserved.
 *
 *
 */

#include <signal.h>

//
Exception_SIGSEGV()
{
 jtm_cyanscreen(" Exception fault: 'SIGSEGV' occured - Press any key ");
 jtmapp_shutdown();
 exit(0);
}

//
void ExceptionHandlerSystemOn(void)
{
 //
 signal(SIGSEGV,Exception_SIGSEGV);
}
