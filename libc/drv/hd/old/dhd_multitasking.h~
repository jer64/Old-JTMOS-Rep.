#ifndef __INCLUDED_DHD_MULTITASKING_H__
#define __INCLUDED_DHD_MULTITASKING_H__

// Enable this to make the IDE HD driver work at optimal
// safety level. When the safe mode is enabled,
// no other interrupts will break the driver's operating,
// therefore possibilities of concurrency problems are
// very unlikely.
//#define HD_DRIVER_SAFEMODE

//
#ifdef HD_DRIVER_SAFEMODE
#define SCHEDULE() nop()
#else
#define SCHEDULE() idle_moment()
#endif

#endif


