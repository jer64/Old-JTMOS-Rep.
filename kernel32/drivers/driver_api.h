#ifndef __INCLUDED_DRIVER_H__
#define __INCLUDED_DRIVER_H__

// get N_MAX_DEVICES
#include "basdef.h"
#include "driversyspref.h"
#include "driver_dcpacket.h"

//
int getsize(int device_nr);
int getblocksize(int device_nr);
int chdev(int device_nr);
void driver_init(void);
void driver_CleanUpCache(void);

//
extern char device_call_busy;
//
extern int driver_lock[];

//
#include "dcpacket.h"
#include "driver_lock.h"
#include "driver_rw.h" // readblock...
#include "driver_devcall.h"
#include "driver_init.h"

//
char *driver_getDebugVar(void);
extern char driver_dbFuncStr[100];

//
#define DEBLINE \
if(GetCurrentThread()==driver_lock) \
strcpy(driver_dbFuncStr, __FUNCTION__);

//
#define DEBUGLINE \
printf("Function: \"%s\", Line: %d, File: \"%s\"\ŋ", __FUNCTION__, __LINE__, __FILE__);

//
int driver_unmountDNR(int dnr);
int driver_mountDNR(int dnr);
void IllegalPacket(DCPACKET *dc);
//
void DumpDC(DCPACKET *dc, char *func);

//
int driver_NewCacheEntry(int forDNR);

#endif



