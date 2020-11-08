// threads_db.c - threads database.
// (C) 2003-2005 by Jari Tuominen.
#include "kernel32.h"
#include "threads.h"

// If scheduling will be wanted to be
// controlled to emphasize some certain
// process, then this variable is set to the
// specified process ID.
// It kinda wakes up the process even if it
// is set to sleep.
int force_switch_thread_nr=0;

// Used for temporary storing:
DWORD threadtmp[200];

//
// NOTE ON THESE ARRAYS BELOW:
//
// Low level assembly language based functions will be
// accessing some of these arrays, in fact all of these,
// except for the thread_name which is mostly made
// just because of easy identification.
// 
// Each of the descriptors are size of 8 bytes.
// These are used to make a local memory space
// for each of the running applications.
// For a kernel mode thread this descriptor
// entry will have no effect,
// because kernel mode threads use the kernel's
// predefined memory space descriptor(CS/DS).
//
// Task scheduler mostly copies one of
// the descriptors from the table to
// the system GDT.
// In the GDT, there is a place for
// application's code/data segment descriptors.
// In addition to this there is also descriptors
// for the kernel level code/data segments.
// I am not yet planned it so that the application
// code/data segment descriptors would run at
// a different privilege level than the kernel's one,
// but that'll be one of the possibilities.
//

// Variables for thread management
int nr_threads=0,nr_curthread=0;
FPUREGS *thread_fpuregs[N_MAX_THREADS]={NULL};
DWORD thread_uniqid[N_MAX_THREADS]={0};
char *thread_name[N_MAX_THREADS]={NULL};
ENVDB *thread_envdb[N_MAX_THREADS]={NULL};
BYTE thread_states[N_MAX_THREADS]={0};
DWORD thread_CPUspending[N_MAX_THREADS];
DWORD thread_esps[N_MAX_THREADS]={0};
DWORD thread_eip[N_MAX_THREADS]={0};
// Indicates that a system call is active for the thread.
BYTE thread_scall_active[N_MAX_THREADS]={0};
WORD thread_ss[N_MAX_THREADS]={0};
WORD thread_ds[N_MAX_THREADS]={0};
WORD thread_es[N_MAX_THREADS]={0};
WORD thread_fs[N_MAX_THREADS]={0};
WORD thread_gs[N_MAX_THREADS]={0};
WORD thread_ml[N_MAX_THREADS]={0};
BYTE thread_tick[N_MAX_THREADS]={0};
BYTE thread_idling[N_MAX_THREADS]={0};
WORD thread_selector[N_MAX_THREADS]={0};
// specifically allocated selector in GDT table(0 if none)
BYTE thread_codedescs[(N_MAX_THREADS*8)]={NULL};
BYTE thread_datadescs[(N_MAX_THREADS*8)]={NULL};
// NULL = no name(name/identification)
DWORD thread_base[N_MAX_THREADS]={0};
// Amount of 64K banks allocated for the thread.
// (obsolete)
WORD thread_banks[N_MAX_THREADS]={0};
// Size of memory in kbs.
WORD thread_memkb[N_MAX_THREADS]={0};
BYTE thread_terminal[N_MAX_THREADS]={NULL};
// Specifies the number of terminal the thread is using for stdout/stderr.
// Value 255 indicates that there is no output terminal at all.
// So forth if 255 then all stdout/stderr output goes to /dev/null.
BYTE thread_special[N_MAX_THREADS]={0};
BYTE thread_type[N_MAX_THREADS]={0};
// Current Device Number
DWORD thread_dnr[N_MAX_THREADS]={0};
// Current Dir Block
DWORD thread_db[N_MAX_THREADS]={0};
// Thread magic - Used for identifying a thread
DWORD thread_magic[N_MAX_THREADS]={0};
// Thread's Message Box
MSGBOX *thread_msgbox[N_MAX_THREADS]={NULL};
// Pointer to thread's shell structure.
DWORD thread_shell_pointer[N_MAX_THREADS];

/*
 * Thread Type Identifier
 *
 *	Bit #	|Description
 *	--------+---------------------------
 *	0	|Special disk access rights(cache manager process)
 *	1	|Is application
 * 	2	|Is driver
 *	3	|Is service
 *	4	|Undefined
 *	5	|Undefined
 *	6	|Undefined
 *	7	|Undefined
 *
 */

char *thread_cmdline[N_MAX_THREADS]={NULL};
//

// Cannot use dynamic allocation because the assembly
// code also needs to be able to access it.
//BYTE *thread_codedescs;
//BYTE *thread_datadescs;

