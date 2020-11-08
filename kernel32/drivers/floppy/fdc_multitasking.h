#ifndef __INCLUDED_FDC_MULTITASKING_H__
#define __INCLUDED_FDC_MULTITASKING_H__

// See dhd_multitasking.h for relevant explanation
// of the multitasking related implementations.

//#define FDC_DRIVER_SAFEMODE

//
#ifdef FDC_DRIVER_SAFEMODE
#define SCHEDULE() nop()
#else
#define SCHEDULE() idle_moment()
#endif

#endif

