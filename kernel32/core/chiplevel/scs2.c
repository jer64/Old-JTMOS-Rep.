// ======================================================================
#include "kernel32.h"
#include "systemcalls.h" // aka scs.h
#include "scconf.h" // contains only defines, works with nasm
#include "ega.h" // N_VIRTUAL_TERMINALS
#include "threads.h" // SwitchToThreadEx
#include "scs.h"
#include "scs2.h"
#include "scs_devices.h" // sys_readblock, etc.
#include "mouse.h"
#include "msgbox.h"
#include "driver_diskchange.h"
#include "serial.h"

// setsize
// Parameters: [setsize] [path] [size]
// OBSOLETE
int scall_setsize(SYSCALLPAR)
{
        static int key1,pr,i,i2,i3,i4,devnr;

        // TODO
//      VERIFY_PTR(p->pid, p->par1)

        // Check path
/*        if( scall_ebx && isValidString(scall_ebx) && strlen(scall_ebx) )
        {
                devnr = scall_eax; // dnr
                ReportMessage("jtmfs_SetSize(devnr=%d, '%s'(0x%x), 0, size=%d);\n",
                        devnr, scall_ebx, scall_ebx, p->v1);
        }   */
/*** int jtmfs_SetSize(int dnr,
                const char *name, int newSize,
                int db) ***/
/*	return setsize(scall_ebx, scall_ecx); */
	return 0;
}

// 1: File.
// 2: Directory.
// Other values: Unknown.
int scall_fexist1(SYSCALLPAR)
{
	int fd;

        //
        if( scall_eax && isValidString(scall_eax) && strlen(scall_eax) )
        {
		//
		fd = open(scall_ebx, 0);
		close(fd);
		if(fd<0) { return 0; }
			else
			{ return 1; }
        }
        else
        {
                printk("%s: Malformed request.\n", __FUNCTION__);
        }

        //
        return 0;
}

// int jtmfs_GetFileSize(int dnr, const char *fname, int db)
// Return value:
// 0 =          Success
// Non-zero =   Error
// -1 =         File not found.
//
// Returns size of the specified file in bytes.
//
int scall_getfilesize(SYSCALLPAR)
{
	int fd,sz;

	//
	fd = open(scall_ebx, 0);
	sz = lseek(fd, 0, SEEK_END);
	close(fd);
	return sz;
}

// int jtmfs_isDriveReady(int dnr)
// TRUE =       Drive is formatted and works properly.
// FALSE =      Drive is not formatted.
//
int scall_isdriveready(SYSCALLPAR)
{
	// NOT SUPPORTED
	return 0;
}

// jtmfs_CreateFile [dnr] [file name]
//
// Non-zero = error
// Zero = no error
//
//
int scall_creat1(SYSCALLPAR)
{
	//
	return creat(scall_ebx, O_CREAT);
}

//
int scall_unlink1(SYSCALLPAR)
{
	//
	return unlink(scall_ebx);
}

/*** #define jtmfs_ReadFile(dnr,name,db,buf,showMeter,prec,offs) \
        _jtmfs_ReadFile(__FUNCTION__, dnr,name,db,buf,showMeter,prec,offs)
***/
//				scall_ebx,	// name
//				scall_ecx,	// db
//				scall_edx,	// buf
//				0,		// 0: showMeter
//				scall_esi,	// prec
//				scall_edi);	// offs
//
int scall_read1(SYSCALLPAR)
{
	int fd;

	return read(scall_ebx, scall_ecx, scall_edx);
}

/*** #define jtmfs_WriteFile(dnr,name,db,buf,showMeter,prec,offs) \
        _jtmfs_WriteFile(__FUNCTION__, dnr,name,db,buf,showMeter,prec,offs)
***/
//
int scall_write1(SYSCALLPAR)
{
	//
	write(scall_ebx, scall_ecx, scall_edx);
}

// scall_getfiledatablock [eax] fname
int scall_getfiledatablock(SYSCALLPAR)
{
	// OBSOLETE
	return 0;
}

//
