// LIBC emulation
#define _LIBC
// Required includes for kernel module and application development.
#include "kernel32.h"	// Preferences for kernel32(init)
#include "keropt.h"	// Not included in kernel32.h !
#include "macrodef.h"
#include "ega.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "vmware.h"
#include "dir.h"
#include "sb.h"
#include "gus.h"
#include "syssh.h"
#include "driver_sys.h"
#include "zero.h"
#include "null.h"
#include "pci.h"
#include "vga.h"
#include "statusline.h"
#include "lpt.h"
#include "postman.h"
#include "centralprocess.h" // tcp, the central process
#include "fdc_dev.h"
#include "fdc2_dev.h"
#include "SerialMouse.h" // SerialMouseInit
#include "paging.h"
#include "slip.h"
#include "tcp.h"
#ifdef GRAOS
#include "graos.h"
#endif
#include "hd.h" // init
#include "partsup.h" // partition table support init
#include "logging.h" // initSystemLogging
#include "_virtualfilesystem_devcall.h"
#include "initsystem.h" // InitSystem()...
// Include most of kernel libraries 
#include "system.h"
#include "sscanf.h"
#include "dirtype.h"
#include "fsdos.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "filefind.h"
#include "errno.h"
#include "ctypefamily.h"
#include "gets.h"
#include "string.h"


#define LIBCERROR panic
