/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*

    ######## ######################      #######       #####          ########  
      ####   ##   ####   ##  ######      #####       ###   ###      ###   ####  
      ####   ##   ####   ##   ######     #####      ###     ###     ##      ##  
      ####   #    ####    #   # ####    # ####     ####     ####   ###      ##  
      ####        ####        # ####    # ####     ###       ###   ####      #  
      ####        ####        # #####   # ####    ####       ####  ######       
      ####        ####        #  ####  #  ####    ####       ####  #########    
      ####        ####        #  ####  #  ####    ####       ####   ##########  
      ####        ####        #  ######   ####    ####       ####    ########## 
 ##   ####        ####        #   #####   ####    ####       ####       ####### 
####  ####        ####        #   #####   ####     ###       ###   #       #### 
####  ####        ####        #    ###    ####     ####     ####   ##       ### 
###   ###         ####        #    ###    ####      ###     ###    ###      ##  
 #   ####         ####       ###   ###    ####       ###   ###     ####    ###  
  #####         ########   #######  #   ########       #####       # #######

                                 Since 1997


            Jari Tapio Tuominen's Multimedia Operating System

 */

#define JTMSYSTEM

#define JTMSND_STATE_ON // *Virtually*

#ifdef GRAOSFIXED
#else
#define nograos
#endif

/*** UNIX COMPATIBLE JTMOS INCLUDE PROCESS ***/
#ifdef JTMPORT2UNIX
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <jtmglvar.h>
#include <jtmconv.h>
#include <jtmfile.h>
#else
#include <malloc.h>
/*** NORMAL JTMOS INCLUDE PROCESS ***/
#ifndef jtm_aname
 #define jtm_aname "(undefined)"
#endif

#ifndef jtm_aversion
 #define jtm_aversion 1.00
#endif

#ifndef jtm_ausage
 #define jtm_ausage "(undefined)"
#endif

#ifndef jtm_acopyright
 #define jtm_acopyright "(undefined)"
#endif

#ifndef jtm_aname
 #define jtm_aname "(undefined)"
#endif

#ifndef jtm_adescription
 #define jtm_adescription "(undefined)"
#endif

#ifndef jtm_areqarg
 #define jtm_areqarg 0
#endif





#define __jtmdj_h_included

#define jtmscr_maxvbuffersize 1600*1200
#ifndef nograos
 #define graos
#endif

#define PLEASE_DONT_RESET_DSP
//#define NO_SWINS
#define NO_JTMVM
//#define NO_JTMMOUSE

/*
 

 */

/*#ifdef JTMOS_SAFEMODE
#define NO_JTMMOUSE
#define NO_JTMKEYB
#define NO_JTMTASK
//#define NO_JTMVM
#endif*/

// DJGPP stanard libs that are quite useful and quite used.

#include <stdio.h>

#ifndef JTMOS_Linux
 #include <conio.h>
 #include <io.h>
 #include <dpmi.h>
 #include <crt0.h>
 #include <sys/farptr.h>
 #include <sys/movedata.h>
 #include <go32.h>
 #include <pc.h>
 #include <time.h>
 #include <dir.h>
 #include <bios.h>
 #include <process.h>
#endif

/*** BEGINNING OF JTMOS LIBRARIES INCLUDE PROCESS ***/

/*EXTENDED -- JTMOS LIBRARIES(C)1997-98 by Jari Tuominen, all rights reserved.*/
#ifndef _jtmos_noglobaldef              // Global variables.




#include <jtmglvar.h>
#include <jtmc64.h>
JDLLREG jdllreg;
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifndef _jtmos_noctrl                   // JTMOS keyboard/mouse controls+etc
#include <jtmctrl.h>
#endif

#ifndef _jtmos_noansi                   // JTMOS ansi windowing routines
 #ifndef JTMOS_Linux
  #include <jtmansi.h>
 #endif
#endif

#include <jtmjf.h>

#include <jtmtask.h>

#ifndef _jtmos_nogfxlib                 // GraOS (Graphics User Interace)
 #ifndef JTMOS_Linux
  #include <jtmdjgfx.h>
 #endif
#endif

#ifndef _jtmos_nosndlib                 // SndOS (Audio Generator)
 #ifndef JTMOS_Linux
  #include <jtmsnd.h>
 #endif
#endif

// Allows: Device mounting
#include <jtmvdrv.h>


#ifndef _jtmos_nojsclib                 // Script (JTMOS Script Parser);
 #include <jtmjsc.h>
#endif

#ifndef _jtmos_nojfmlib                 // JTMFM, JTMOS File Manager.
 #include <jtmfm.h>
#endif

#ifndef _jtmos_nojtmguic                //
 #include <jtmguic.h>
#endif

#ifndef _jtmos_nojtmwww
 #include <jtmwww.h>
#endif

#include <jtmacom.h> // For the Operating System !
#include <jtm.h>

#include <jtmexcept.h>

#include <jtmapp.h>                     // Application Specified Definations
                                        // Contains f.e. function main().

#ifdef __cplusplus
}
#endif
/*** IF UNIX COMPATIBLE ... ***/
#endif
/*** THE END OF JTMOS LIBRARIES INCLUDE PROCESS ***/

