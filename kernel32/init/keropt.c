// KERNEL OPTIONS
// =====================================================================================
//
// (C) 2002-2020 by Jari Tuominen (jari.t.tuominen@gmail.com).
//
// THE VERY DEFAULTS
// As default the machine will have an option
// to have SLIP protocol based connection using
// COM2 port, and serial mouse using COM1 port.
// PS/2 mouse driver will be enabled for input.
// Keyboard driver will be enabled too.
//
// >> DEVICE DRIVER SELECTION
// Which devices are probed upon start up?
//
#include "kernel32.h"

// -- Preferences -----------
// Verbose debug messages
// (Switchable to ON or OFF by 'debug' command
//  in smsh -system low level shell.)
// Enabling debug messages will considerably
// slow down the kernel, it is intended to be
// used for for debugging/testing purposes only.
int jtmos_debug=		0; // enables dprintk messages		(tons of information, incl. malloc calls)
int debug2=			0; // enables ReportMessage messages	(some information)
// 0 = use COM1 as serial debug line
// 1 = use COM2 as serial debug line
// -1 = disable serial debug line option
int serial_debug_line=		0;

// Disable block cache? (readblock/writeblock cache)
int DISABLE_BLOCK_CACHE=	TRUE;

// -- Heart processes -------
int XON_FS=			1;
int XON_HWDET=			1;
int XON_DEZOMBIE=		1;
int XON_IDLE=			0;
int XON_CACHEFLUSH=		0;
int XON_FATFLUSH=		1;
int XON_KINIT=			1;
int XON_POSTMAN=		0; // (1) Central CommuniCating Process CCCP
int STATUSLINE_ENABLED=		0; // (0) Displays some memory stuff.

// -- Recommended -----------
int KEYBOARD_ENABLED=		1; // (1) >Standard PC keyboard<
int FLOPPY_ENABLED=		1; // (1) >floppy disk drive<
int FLOPPY2_ENABLED=		0; // (0) floppy2 >floppy disk drive< is not supported yet!
int EXTHD_ENABLED=		0; // (0) external driver based >hard disk driver<
int INTHD_ENABLED=		1; // (1) internal IDE HD driver
int RAMDISK_ENABLED=		1; // (1) internal RAM disk driver
int devser_debug=		0;
// PS/2 mouse enabled or not?
// (It is safe to enable PS/2 mouse
// even if not really using it.).
int PS2MOUSE_ENABLED=		1;


// -- CACHE SYSTEMS CONFIGURATION -----------
// Disable automatic flushing of cache.
// (enabling may cause some concurrency
// problems sometimes?)
int DISABLE_CACHE_AUTO_FLUSH=1; // Now handled by [cache high] itself.
// This is nice to enable, loading up files speeds up nicely:
// (Warning: At the current development
// status it might not work properly yet.)
int CacheFilesInMemoryUsingDIRDB=	0;
// Flush FS in:
// 0 = cache high
// 1 = cache low
int directFlushFS=		0;


// -- Misc. -----------------
int CPU_SPEED_TEST_ENABLED=	0;
int CMOS_ENABLED=		0;
// Automatically install system at the start up
// (boot disk will act as an install disk).
// Plus start OS installation program,
// if programmed so.
int XON_AUTOINSTALL=		FALSE; // default: TRUE
// Start up
// (Obsolete! Use autoinstall.)
// 0=Start up in SMSH as default
// 1=Try to find system from various devices,
// and prompt for root disk.
int ENABLE_STARTUP=		0;

// -- Multimedia ------------

// Generic audio driver(GAD).
// (GAD must be enabled in order to
//  use any of the audio drivers.)
int AUDIO_ENABLED=		0;
// Gravis Ultrasound(old GUS) support
// (incomplete, does not work).
int GUS_ENABLED=		0;
// Sound Blaster 8-bit pro support.
int SB_ENABLED=			0;

// -- Communication ---------

// ENABLE SERIAL MOUSE DRIVER.
int SERMOUSE_ENABLED=		0;
// Serial mouse movement acceleration factor.
int sermouse_acceleration=	2;
// Which ports to use for Serial Mouse(s)?
// (use this one)
int ENABLE_MOUSE_COM1=		1;
int ENABLE_MOUSE_COM2=		0;

// -- Internet --------------
int SERIAL_ENABLED=		1; // NOTE: already enabled as CORE drivers in the kernel32.c
// Sysnet is obsolete.
int SYSNET_ENABLED=		0; //

// (OBSOLETE !)
// WHICH COM PORT SHOULD MOUSE USE AS DEFAULT?
// 0 = COM1, 1 = COM2(DEFAULT = COM1)
// (OBSOLETE !)
int SERIAL_MOUSE_COM=		0;

// PCI BUS support.
int PCI_ENABLED=		1;
// LPT port support.
int LPT_ENABLED=		1;

// -- MALFUNCTIONING THINGS ------
int ALTDEV_ENABLED=		0;
// Sysdev is disabled because of malfunctioning.
int SYSDEV_ENABLED=		0;

// -- SLIP ------------------
// Start slip at the boot up?(inet cmd).
int START_SLIP=			0;
// WHICH COM PORT TO USE FOR SLIP CONNECTION?
// 0=COM1, 1=COM2, etc.(DEFAULT = COM2)
int SLIP_COM_PORT_N=		0;
int DefaultSlipBPS=		115200; // 38400;


// -- MISC ----------------------------------
// Note: In half kilos, e.g. 1024 means 512 kilos.
// By multiplying the value by 2, you can use kilo values.
// NOTE: Now implementing especially small 512K ramdisk
// in order to keep memory requirements at minimum.
// (JTMOS should be able to be run on a 32M machine)
int RD_DEFAULT_BLOCK_SIZE=	512;
int RD_DEFAULT_SIZE=		2880;

// >> PIT timer frequency
//
// 0x10,0x100,0x800,0x1000 are recommended
// This is what kernel implements.
int SAFE_TIMER_FREQUENCY=0x1;
// Some other values
int FAIR_TIMER_FREQUENCY=0x2000;
int GOOD_TIMER_FREQUENCY=0x800;
// TIMER TICKS ARE NOW BEING CONVERTED IN PIT.C!
//
// 0x0010 for realtime activity
// 0x0100 for very high scheduling & timer
// 0x0400 for average scheduling
// 0x0800 for normal scheduling
// 0x1000 for slightly slow(good for Bochs)
// 0x0000(65536) for really lazy scheduling

// >> Default memory size for applications.
//
// For caster.c
// Amount of kiloebytes(K) to allocate as default for applications.
// Applications which spend alot memory, should use memory requirement
// macro to get more work memory.
int STATIC_APP_MEM_SIZE=256;

// >> Default machine/user info.
//
// Dummy defaults.
char *DEFAULT_SYSTEM_NAME="jtmos.info";
char *DEFAULT_USER_NAME="vai";

// >> Default root device
//
// Which device to mount as root default
char *DEFAULT_ROOT_DEVICE="ram";

// For kernel color settings
#include "kercolor.h"

