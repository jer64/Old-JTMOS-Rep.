// UNIQUE PROCESS ID SYSTEM
#ifndef __INCLUDED_UNIPID_H__
#define __INCLUDED_UNIPID_H__

//
#define UNIF(x) if(unif(x))

// Mask for getting the PID ID
#define UNIQMASK        0x0000FFFF
// Mask for getting the UNIQUE ID
#define UNIQMASK2	0xFFFF0000
// UNIPID bit shifting count
#define UNIQSHIFT       16

// Get real PID
#define RPID(x) \
	(x&UNIQMASK)

#endif


