// KERNEL OPTIONS
// ======================================

// Process selection
extern int XON_FS;
extern int XON_HWDET;
extern int XON_IDLE;
extern int XON_DEZOMBIE;
extern int XON_CACHEFLUSH;
extern int XON_FATFLUSH;
extern int XON_KINIT;
extern int XON_POSTMAN;

//
extern int DISABLE_BLOCK_CACHE;
//
// >> DEVICE DRIVER SELECTION
// Which devices are probed upon start up?
// -- Recommended -----------
extern int KEYBOARD_ENABLED;
extern int FLOPPY_ENABLED;
extern int FLOPPY2_ENABLED;
extern int EXTHD_ENABLED;
extern int INTHD_ENABLED;
extern int RAMDISK_ENABLED;
// -- Misc. -----------------
extern int CPU_SPEED_TEST_ENABLED;
extern int CMOS_ENABLED;
// -- Multimedia ------------
extern int AUDIO_ENABLED;
extern int GUS_ENABLED;
extern int SB_ENABLED;
// -- Communication ---------
extern int SERMOUSE_ENABLED;
extern int sermouse_acceleration;
extern int ENABLE_MOUSE_COM1;
extern int ENABLE_MOUSE_COM2;
extern int SERIAL_MOUSE_COM;
extern int PS2MOUSE_ENABLED;
extern int PCI_ENABLED;
extern int LPT_ENABLED;
extern int RTC_ENABLED;
// -- Internet --------------
extern int SERIAL_ENABLED;
extern int SLIP_ENABLED;
extern int SYSDEV_ENABLED;
extern int ALTDEV_ENABLED;
extern int SYSNET_ENABLED;
//
extern int RD_DEFAULT_SIZE;
extern int STATUSLINE_ENABLED;

//
extern int XON_AUTOINSTALL;
//
extern int ENABLE_STARTUP;
//
extern int CacheFilesInMemoryUsingDIRDB;
// TIMERS
extern int SAFE_TIMER_FREQUENCY;
extern int FAIR_TIMER_FREQUENCY;
extern int GOOD_TIMER_FREQUENCY;
extern int STATIC_APP_MEM_SIZE;

//
extern char *DEFAULT_SYSTEM_NAME;
extern char *DEFAULT_USER_NAME;

//
extern int DRIVER_DISABLE_CACHE;
extern int driver_cache_size;

//
extern int START_SLIP;
extern int SLIP_COM_PORT_N;
extern int DefaultSlipBPS;

//
extern int directFlushFS;

//
extern int jtmos_debug;
extern int debug2;
extern int serial_debug_line;

//
extern int DISABLE_CACHE_AUTO_FLUSH;

//
extern int devser_debug;

//
extern char *DEFAULT_ROOT_DEVICE;
extern int RD_DEFAULT_BLOCK_SIZE;
#include "kercolor.h"




