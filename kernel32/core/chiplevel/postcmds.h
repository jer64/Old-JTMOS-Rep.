//
#ifndef __INCLUDED_POSTCMDS_H__
#define __INCLUDED_POSTCMDS_H__

// Video functions
void postman_pcmd_setmode(POSTMANCMD *p);
void postman_pcmd_setpalette(POSTMANCMD *p);
void postman_pcmd_setpalettemap(POSTMANCMD *p);

// Audio functions
void postman_pcmd_getaudioposition(POSTMANCMD *p);
void postman_pcmd_sendaudio(POSTMANCMD *p);
void postman_pcmd_initaudio(POSTMANCMD *p);

//
#endif

//
