#ifndef __INCLUDED_THREADSCFG_H__
#define __INCLUDED_THREADSCFG_H__

// The upper limit for the amount of threads that can
// be ran and/or be registered at once.
// Anything bigger than 5000 might lead into unworking kernel.
// Due to current allocation method of all the variables.
#define N_MAX_THREADS           250

// A process that is killed, and
// it's resources require deallocating.
#define ISZOMBIE                1
// A process entry that is waiting to be
// re-used.
#define ISDEZOMBIE		2

//
#define THREADS_STACKROLLBACK   1024

// Remove remark if you want debugging notes
#define THREADS_DEBUG

#endif


