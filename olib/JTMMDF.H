/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*** MISC.DEF. ***/
void write(char *txt);

/******** CONIO STUFF **********************/
#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

enum text_modes { LASTMODE=-1, BW40=0, C40, BW80, C80, MONO=7, C4350=64 };

#define BLINK   0x80    /*  blink bit   */

#define SIGABRT	288
#define SIGFPE	289
#define SIGILL	290
#define SIGSEGV	291
#define SIGTERM	292
#define SIGINT  295

