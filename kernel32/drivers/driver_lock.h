#ifndef __INCLUDED_DRIVER_LOCK_H__
#define __INCLUDED_DRIVER_LOCK_H__

void driver_lockOff(int lockNr);
void driver_lockOn(int lockNr);
int driver_getLock(int lockNr);
void driver_forceSetLock(int lockNr, int what);
void driver_directAccessLockOn(void);
void driver_directAccessLockOff(void);

#endif
