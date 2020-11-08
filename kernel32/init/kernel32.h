//
#ifndef __INCLUDED_KERNEL32_H__
#define __INCLUDED_KERNEL32_H__

// Uncomment line below to enable boot to integrated GUI.
#define GRAOS_BOOT_UP
#define BORLAND

// SEE ALSO keropt.h -file.
//
#include "basdef.h"
#include "env.h"
#include "cpu.h"
#include "ega.h"
#include "printf.h"
#include "int.h"
#include "system_call.h"
#include "gpf.h"
#include "threads.h"
#include "buildnr.h"
#include "string.h"
#include "panic.h"
#include "keropt.h"
#include "malloc.h"
#include "system.h"
#include "rtc.h"
#include "pit.h"
#include "hwdet.h"
#include "gdtmanipulation.h"
#include "debug.h"
#include "biosfunc.h" // GetBiosBootDrive
#include "coprocessor_not_found.h"
#include "reboot.h"
#include "centralprocess.h"
#include "vmware.h"
#include "scanasc.h"
#include "initsystem.h"
#include "vesa.h"
#include "term.h"
//#include "term.h"
//#include "partsup.h"
#include "jtmkeys.h"
#include "filedef.h"
#include "filebuf.h"
#include "filedes.h"
#include "file.h"
#include "random.h"
#include "fcntlbits.h"

//
#define TRACE printf

// Structure prototype for
// boot loader at memory address 0x7C00.
typedef struct 
{
	//
	BYTE nothing[509];
	BYTE bootdrv;
	WORD signature;		 // Must be 0xAA55 or 0x55AA.
	BYTE partition_table[512];
	BYTE more_code[0];
}LDR;

//
extern char *DEFAULT_ROOT_DEVICE;

//
#define panic(x) FATALERROR(x)
#define printk printf

// This macro can be used to run a function at the specified void pointer
//
// Call with calling pointer
#define sys(x,p) (*(void(*)(void *)) x)(p);

//
extern DWORD edata, etext, kernelend;

//
void WaitSystemStart(void);
extern void boot_up(void);

//
extern BYTE testdrv[];
extern BYTE boot_bin[];
extern int boot_bin_length;

//
extern int bootdrv;
extern int systemStarted;
// Safe mode in the beginning of system start up
extern int EGASAFE;
extern int KINIT;
DWORD KernelInitPassed;

//
typedef struct
{
	//
	WORD com1_addr;
	WORD com2_addr;
	WORD com3_addr;
	WORD com4_addr;
	//
	WORD lpt1_addr;
	WORD lpt2_addr;
	WORD lpt3_addr;
	WORD lpt4_addr;
}BIOS_DATA;
extern BIOS_DATA bios_data;
__attribute__ ((__noreturn__)) void _boot_up(void);

//
#include "systemcalls.h"

// Useful for all POSIX FS API calls, like open, close, etc.
#include "fcntlbits.h"

//
int mkdir(const char *pathname, int type);
int chdir(const char *path);
int unlink(const char *fname);
int initfs(int dnr);
int open(const char *name, int flags);
int close(int fd);
int read(int fd, void *buf, int count);
int write(int fd, void *buf, int count);
int lseek(int fd, int offset, int whence);
int mkfs(const char *path);
int sync(void);
int syncfs(int fd);

// FOR /MINIMAL SYSTEM SHELL/
// Active device.
extern char str_device[255];
// f.e. "ls" "/" "-la"
// created using sptstr
extern char *cmdpar[100];
// this string will contain the entire command line
extern char *wholestring;

#endif
