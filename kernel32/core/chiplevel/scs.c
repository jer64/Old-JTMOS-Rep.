// ======================================================================
// scs.c - JTMOS system call services.
// (C) 2002-2005,2014,2016 by Jari Tuominen
// (jari.t.tuominen@gmail.com).
//
// Note: all system calls implement it's callers stack,
// therefore all pointer references to the local variables,
// that are stored in the present stack are either invalid
// or must be specifically converted from USER memory space
// addressing to the KERNEL memory space addressing.
//
// This is a major bug that have been raging the system
// for a very long time.
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

//
void *scall_functions[N_MAX_SCALLS]={
	// 0
	// dummy entry, never ought to be called
	scall_dummy,
	// 1 SYS_IDLE
	idle_moment,
	// 2 SYS_remove
	scall_remove,
	// 3 SYS_mkfs
	scall_mkfs,
	// 4 SYS_rename
	scall_rename,
	// 5
	scall_dummy,
	// 6 SYS_PUTS
	scall_puts,
	// 7 SYS_PUTCH
	scall_putchar,
	// 8 SYS_EXIT
	scall_exit,
	// 9 SYS_CLRSCR
	scall_clrscr,
	// 10 SYS_GETCH
	scall_getch,
	// 11 postcmd
	scall_postcmd,
	// 12 SYS_GETCURDEV
	scall_getcurdev,
	// 13 SYS_GOTOXY
	scall_gotoxy,
	// 14 SYS_GETTICKCOUNT
	scall_gettickcount,
	// 15 SYS_GETAMEMSIZE
	scall_getamemsize,
	// 16 SYS_GETCURRENTTHREAD
	scall_getcurrentthread,
	// 17 SYS_GETARGC
	scall_getargc,
	// 18 SYS_GETARGV
	scall_getargv,
	// 19 SYS_open
	scall_open,
	// 20 SYS_close
	scall_close,
	// 21 SYS_read
	scall_read,
	// 22 SYS_write
	scall_write,
	// 23 SYS_lseek
	scall_lseek,
	// 24 SYS_cexec
	scall_cexec,
	// 25 SYS_getdevnrbyname
	scall_getdevnrbyname,
	// 26 SYS_readblock
	scall_readblock,
	// 27 SYS_writeblock
	scall_writeblock,
	// 28 SYS_waitprocterm
	scall_waitprocterm,
	// 29 SYS_findfirst
	scall_findfirst,
	// 30 SYS_findnext
	scall_findnext,
	// 31 SYS_textcolor
	scall_textcolor,
	// 32 SYS_textbackground
	scall_textbackground,
	// 33 SYS_inputmouse
	scall_inputmouse,
	// 34 SYS_setmouserect
	scall_setmouserect,
	// 35 SYS_vgadrawframe
	scall_vgadrawframe,
	// 36 SYS_GETCURDB
	scall_getcurdb,
	// 37 SYS_vgawaitretrace
	scall_vgawaitretrace,
	// 38 SYS_chdir
	scall_chdir,
	// 39 SYS_getdevicename
	scall_getdevicename,
	// 40 SYS_getdate
	scall_getdate,
	// 41 SYS_WHEREX
	scall_wherex,
	// 42 SYS_WHEREY
	scall_wherey,
	// 43 SYS_vgaselectplane
	scall_vgaselectplane,
	// 44 getdevicenrbyname
	scall_getdevicenrbyname,
	// 45 
	scall_getthreadpriority,
	// 46
	scall_getthreadspending,
	// 47
	scall_getthreadname,
	// 48
	scall_getthreadmemoryusage,
	// 49
	scall_getthreadcount,
	// 50
	scall_getctrl,
	// 51
	scall_getalt,
	// 52 Activate thread's MSGBOX
	scall_activatemsgbox,
	// 53 Send message to a PID
	scall_sendmessage,
	// 54 Receive message
	scall_receivemessage,
	// 55 getthreadpid
	scall_getthreadpid,
	// 56 identifythread
	scall_identifythread,
	// 57 getseconds
	scall_getseconds,
	// 58 globalcopy
	scall_globalcopy,
	// 59 getthreadterminal
	scall_getthreadterminal,
	// 60 setthreadterminal
	scall_setthreadterminal,
	// 61 NewTerminal
	scall_newterminal,
	// 62 outputterminalkeyboardbuffer
	scall_outputterminalkeyboardbuffer,
	// 63 inputterminalkeyboardbuffer
	scall_inputterminalkeyboardbuffer,
	// 64 copyterminal
	scall_copyterminal,
	// 65 killthread
	scall_killthread,
	// 66 getcursorlocation
	scall_getcursorlocation,
	// 67 setint
	scall_setint,
	// 68 enableirq
	scall_enableirq,
	// 69 diskchange
	scall_diskchange,
	// 70 getthreaduniquepid
	scall_getthreaduniquepid,
	// 71 registernewdevice
	scall_registernewdevice,
	// 72 switchtothreadex
	scall_switchtothreadex,
	// 73 createterminal
	scall_createterminal,
	// 74 setterminalowner
	scall_setterminalowner,
	// 75 getterminalowner
	scall_getterminalowner,
	// 76 serialget
	scall_serialget,
	// 77 serialput
	scall_serialput,
	// 78 setserialportspeed
	scall_setserialportspeed,
	// 79 receiverawpacket
	scall_receiverawpacket,
	// 80 transmitrawpacket
	scall_transmitrawpacket,
	// 81 devicecall
	scall_devicecall,
	// 82 milsleep
	scall_milsleep,
	// 83 opensocket
	scall_opensocket,
	// 84 closesocket
	scall_closesocket,
	// 85 readsocket
	scall_readsocket,
	// 86 writesocket
	scall_writesocket,
	// 87 switchtothread
	scall_switchtothread,
	// 88 getcwd
	scall_getcwd,
	// 89 stat
	scall_stat,
	// 90 fstat
	scall_fstat,
	// 91 SYS_wgetch
	scall_wgetch,
	// 92 SYS_mkdir
	scall_mkdir,
	// 93 kmalloc
	scall_kmalloc,
	// 94 kfree
	scall_kfree,
	// 95 postcmd
	scall_postcmd,

	// 96 fexist OBSOLETE
	NULL,
	// 97 getfiledatablock OBSOLETE
	NULL,
	// 98 scall_isdriveready OBSOLETE
	NULL,
	// 99
	scall_diskchange,
	// 100 getfilesize OBSOLETE
	NULL,
	// 101
	scall_read1,
	// 102
	scall_creat1,
	// 103
	scall_unlink1,
	// 104 OBSOLETE setsize
	NULL,
	// 105
	scall_nothing,
	// 106
	scall_nothing
};

// Number of system calls registered
DWORD nr_scall_func = 120;


//--------------------------------------------------------------------------
//
// A2K.
// Application to Kernel memory space address conversion function.
//
DWORD A2K(DWORD appoffs)
{
	return _A2K(appoffs, GetCurrentThread());
}

// PID specific.
DWORD _A2K(DWORD appoffs, int pid)
{
	DWORD tb;

	//
	if(	appoffs >= 0x80000000 && 
		appoffs <= 0x8FFFF000 )
	{
		//
		if( !(tb=GetThreadBase(pid)) )
		{
			dprintk("Unknown 0x8NNN NNNN address.\n");
			goto past;
		}

		// Seems to be an application address.
		return (appoffs-0x80000000)+tb;
	}

past:
	// Not an application address..
	return appoffs;
}

//--------------------------------------------------------------------------
//
static void LogThisSystemCall(SYSCALLPAR)
{
	char str[256];

	//
	if(scall_eax!=SYS_PUTCH && scall_eax!=SYS_IDLE)
	{
		sprintf(str, "SYSCALL%d: ebx=%x, ecx=%x, edx=%x, esi=%x, edi=%x, ebp=%x\n",
			scall_eax,
			scall_ebx, scall_ecx, scall_edx,
			scall_esi, scall_edi, scall_ebp);
		WriteSystemLog(str);
	}
}

// Called by scall_handler before jumping to
// the service function.
void scallActive(SYSCALLPAR)
{
	//
	if( (GetThreadType(GetCurrentThread())&THREAD_TYPE_SYSCALL_TRACE) )
	{
		//
		LogThisSystemCall(MYSYSCALLPAR);
	}

	//
	thread_scall_active[RPID(GetCurrentThread())]++;
}

//--------------------------------------------------------------------------
// This function is called by scall_handler
// after the system call has been served.
void scallDeactive(void)
{
	//
	thread_scall_active[RPID(GetCurrentThread())]--;
}

//--------------------------------------------------------------------------
//
// Nothing doer.
//
DWORD scall_nothing(SYSCALLPAR)
{
	//
	return 0;
}

//--------------------------------------------------------------------------
//
// scall_puts(scall #6).
//
// Note: base address is needed because
// we need to translate the application's
// own virtual memory address space to
// memory space that is visible to kernel.
// f.e.:
// - Kernel can see everything.
// (supervising)
//
// - Application can see only itself.
// (restricted)
//
//
DWORD scall_puts(SYSCALLPAR)
{
	DWORD ad;
	char *p;
	int i,l;

	//
	ad = A2K(scall_ebx);
	p = ad;
	l = strlen(p);
	for(i=0; i<l; i++)
		putch(p[i]);
	return 0;
}

//--------------------------------------------------------------------------
// Wait until key is hit and return the key code.
int scall_wgetch(SYSCALLPAR)
{
	//
	return getch();
}

//--------------------------------------------------------------------------
// Input key from current terminal's keyboard buffer
DWORD scall_getch(SYSCALLPAR)
{
	//
	return getch1();
}

//--------------------------------------------------------------------------
//
// scall_putchar(scall #7).
// BL = character to print on screen.
//
// New: Modified for visible_terminal.
//
DWORD scall_putchar(SYSCALLPAR)
{
	int i;
	char *scr;
	char str[10];

	// Print char
	sputch(&screens[thread_terminal[RPID(GetCurrentThread())]], scall_ebx&255);

	//
	return 0;
}

//--------------------------------------------------------------------------
// This system call is used to exit a thread.
// F.e. an application can be ended with this
// function very well.
//
DWORD scall_exit(SYSCALLPAR)
{
	// Terminate the current thread.
	KillThread( GetCurrentThread() );

	// Wait until being looped out.
	while(1) { SwitchToThread(); }

	//
	return 0;
}

//--------------------------------------------------------------------------
//
// Clears screen.
//
DWORD scall_clrscr(SYSCALLPAR)
{
	//
	_clrscr();
	return 0;
}

//--------------------------------------------------------------------------
// scall_getcurdev - get current device
//
// Returns currently selected default device.
// Will be changed later! TODO.
//
DWORD scall_getcurdev(SYSCALLPAR)
{
	int devnr;

	//
	devnr = GetCurrentDNR();
	return devnr;
}

//--------------------------------------------------------------------------
DWORD scall_getcurdb(SYSCALLPAR)
{
	int db;

	//
	db = GetCurrentDB();
	return db;
}

//--------------------------------------------------------------------------
//
void scall_relaxthread(SYSCALLPAR)
{
	//
	SwitchToThread();
}

//--------------------------------------------------------------------------
//
void scall_gotoxy(SYSCALLPAR)
{
	gotoxy(scall_ebx,scall_ecx);
}

//--------------------------------------------------------------------------
//
DWORD scall_gettickcount(SYSCALLPAR)
{
	return GetTickCount();
}

//--------------------------------------------------------------------------
// scall_getamemsize
//
// This function returns the size of the memory
// allocated for the process use.
// This is only for application processes,
// does not apply to kernel processes.
DWORD scall_getamemsize(SYSCALLPAR)
{
	return thread_memkb[RPID(GetCurrentThread())]*1024;
}

//--------------------------------------------------------------------------
//
DWORD scall_getcurrentthread(SYSCALLPAR)
{
	return GetCurrentThread();
}

//--------------------------------------------------------------------------
//
DWORD scall_getargc(SYSCALLPAR)
{
	int i;
	char *s;

	s=thread_cmdline[RPID(GetCurrentThread())];
	if( !s )return 0;
	if( !isValidString(s) )return 0;

	//
	return CountWords(s);
}

//--------------------------------------------------------------------------
// scall_ebx = which argument
// scall_ecx = pointer to string where to store the answer
void scall_getargv(SYSCALLPAR)
{
	char *s,which;

	//
	which=scall_ebx;
	s = A2K(scall_ecx);

	//
	if( which>scall_getargc(MYSYSCALLPAR) || !scall_getargc(MYSYSCALLPAR) )
	{
		strcpy(s,"");
		return;
	}

	//
	strcpy(s,"");
	strcpyarg(s, thread_cmdline[RPID(GetCurrentThread())], which);
}

//--------------------------------------------------------------------------
// Order init -process to create new task.
DWORD scall_cexec(SYSCALLPAR)
{
	char *s,*s2,*s3,*s4;
	DWORD pid;
	char fname[256];
	int i,*spid;

	//	scall_ebx	(char *)	file name + arguments 
	//	scall_ecx	int		wait until process exits [TRUE/FALSE]
	//	scall_edx	(char *)	work directory
	//	scall_esi	(int *)		pointer to address where to store PID
	//		(if equals to NULL, then ignored)
	//	scall_edi	(int)		terminal ID

	//
	pid = 0;
	s =	A2K(scall_ebx); // string: exe fname&args
	//	scall_ecx = [0 or 1] wait until process exits
	s2 =	A2K(scall_edx); // work dir
	spid =	A2K(scall_esi);
	*spid = 0;

	//
	if(isValidString(s) && strlen(s))
	{
		// Cut file name.
		strcpy(fname, s);
		for(i=0; i<strlen(fname); i++)
			if(fname[i]==' ')fname[i]=0;

		// #1 argument =	file name(ends at the first space)
		// #2 argument = 	whole command line
		// s2 =			running path(where the program will run at)
		pid = centralprocess_exec(fname, s,  GetCurrentDNR(), GetCurrentDB(),
					s2,
					scall_edi); // terID

		// Store PID
		*spid = pid;
	}
	else
		// Invalid parameter string(s) detected
		return -2;

	// Creation/load(not found) failed if pid equals zero
	// Return value -1 means error
	if(!pid)
		// No process created
		return -1;

	// We were ordered to wait here?
	if( !scall_ecx )
		// No wait, return:
		return 0;

	//
	WaitProcessTermination(pid);
	// Return value 0 means that
	// program terminated like it used to
	return 0;
}

//--------------------------------------------------------------------------
// scall_ebx = PID
DWORD scall_waitprocterm(SYSCALLPAR)
{
	//
	enable();
	WaitProcessTermination(scall_ebx);
}

//--------------------------------------------------------------------------
// scall_ebx = fname.
// scall_ecx = FFBLK typedef structure.
DWORD scall_findfirst(SYSCALLPAR)
{
	char *p,*p2,*pp;

	//
	return 0;
}

//--------------------------------------------------------------------------
// scall_ebx = fd.
// scall_ecx = FFBLK typedef structure.
DWORD scall_findnext(SYSCALLPAR)
{
	char *p,*p2,*pp;

	//
	return 0;
}

//--------------------------------------------------------------------------
void scall_textcolor(SYSCALLPAR)
{
	//
	textcolor(scall_ebx);
}

//--------------------------------------------------------------------------
void scall_textbackground(SYSCALLPAR)
{
	//
	textbackground(scall_ebx);
}

//--------------------------------------------------------------------------
DWORD scall_inputmouse(SYSCALLPAR)
{
	//
	switch(scall_ebx)
	{
		//
		case 0:
		return mouse.x;
		case 1:
		return mouse.y;
		case 2:
		return mouse.buttons;

		//
		default:
		return 0;
	}
}

//--------------------------------------------------------------------------
void scall_setmouserect(SYSCALLPAR)
{
	//
	mouse_setframe(scall_ebx, scall_ecx,
			scall_edx, scall_esi);
	//
	mouse_setposition(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
void scall_vgadrawframe(SYSCALLPAR)
{
        char *vf,*src;
        int i,i2,v1,v2;

	//
	v1 = scall_ecx;
	v2 = scall_edx;

	// Define VGA memory.
	vf=0xA0000;
	// Define source.
	src = A2K(scall_ebx);

	// Task Switching Friendly
	enable();

	//
	switch( dispadmin_getcurrentmode() )
	{
		// 320x200 256c video mode
		case 0x13:
		if( (v1+v2)<=64000 )
			memcpy(vf + v1, src, v2);
		break;
		// 640x480 2c video mode
		case 0x11:
		if( (v1+v2)<=38400 )
			memcpy(vf + v1, src, v2);
		break;
		// 640x480 16c video mode
		case 0x12:
		if( (v1+v2)<=0xffff )
			memcpy(vf + v1, src, v2);
		break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------
DWORD scall_vgawaitretrace(SYSCALLPAR)
{
	//
	vga_waitretrace();
	return 0;
}

//--------------------------------------------------------------------------
// chdir [path]
// EAX   EBX
DWORD scall_chdir(SYSCALLPAR)
{
        // ----------------------------------------------
        // chdir
        //
	return _chdir(scall_ebx);
}

//--------------------------------------------------------------------------
// SYS_mkfs [path]
DWORD scall_mkfs(SYSCALLPAR)
{
	return mkfs(scall_ebx);
}

//--------------------------------------------------------------------------
// UNDER TODO AT THE MOMENT. USE MV INSTEAD.
// scall_rename: dnr, old file name, new file name, db
DWORD scall_rename(SYSCALLPAR)
{
        int ack;

        //
/*	return rename(scall_ebx,scall_ecx);*/
	return 0;

}


//--------------------------------------------------------------------------
// scall_mkdir: directory name, type
DWORD scall_mkdir(SYSCALLPAR)
{
	//
	return mkdir(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
// SYS_remove
// remove [path]
// EAX    EBX
DWORD scall_remove(SYSCALLPAR)
{
        int r;
        char *p,*p2;
        int i,i2,ack;

	//
	p2 = A2K(scall_ebx);

        // Ignore errors.
        if( illegal(p2) )
		//
		return 1;

	//
	unlink(p2);

        // Post it.
	return 0;
}

//--------------------------------------------------------------------------
// dnr, strbuf*
int scall_getdevicenrbyname(SYSCALLPAR)
{
	char *p;
	p=A2K(scall_ebx);

	//
	if(p && isValidString(p))
	{
		return driver_getdevicenrbyname(p);
	}
	else
	{
		printk("%s: Malformed request to system call\n", __FUNCTION__);
		return 0;
	}
}

//--------------------------------------------------------------------------
// dnr, strbuf*
DWORD scall_getdevicename(SYSCALLPAR)
{
	char *p;

	//
	p=A2K(scall_ecx);

	//
	if(p && isValidString(p))
	{
		driver_getdevicename(scall_ebx, p);
	}
	else
	{
		printk("%s: Malformed request to system call\n", __FUNCTION__);
		return 1;
	}

	//
	return 0;
}

//--------------------------------------------------------------------------
//
DWORD scall_getdate(SYSCALLPAR)
{
	char *p;

	//
	p=A2K(scall_ebx);
	rtc_getdate(p);

	//
	return 0;
}

//--------------------------------------------------------------------------
DWORD scall_wherex(SYSCALLPAR)
{
	//
	return wherex();
}

//--------------------------------------------------------------------------
DWORD scall_wherey(SYSCALLPAR)
{
	//
	return wherey();
}

//--------------------------------------------------------------------------
// VGA select plane where to draw and write.
DWORD scall_vgaselectplane(SYSCALLPAR)
{
	// Select VGA plane.
	setwplane(scall_ebx & 3);
	setrplane(scall_ebx & 3);
	return 0;
}

//--------------------------------------------------------------------------
// Get thread priority: pid.
int scall_getthreadpriority(SYSCALLPAR)
{
	//
	return GetThreadPriority(scall_ebx);
}

//--------------------------------------------------------------------------
// Get thread spending: pid.
int scall_getthreadspending(SYSCALLPAR)
{
	//
	return GetThreadSpending(scall_ebx);
}

//--------------------------------------------------------------------------
// Get thread name: pid, name.
int scall_getthreadname(SYSCALLPAR)
{
	char *p;

	//
	p = A2K(scall_ecx);
	return GetThreadName(scall_ebx, p);
}

//--------------------------------------------------------------------------
// Get thread memory usage: pid
int scall_getthreadmemoryusage(SYSCALLPAR)
{
	//
	return GetThreadMemoryUsage(scall_ebx);
}

//--------------------------------------------------------------------------
// Get number of threads
int scall_getthreadcount(SYSCALLPAR)
{
	//
	return GetThreadCount();
}

//--------------------------------------------------------------------------
int scall_getctrl(SYSCALLPAR)
{
	// Only when available to us
	if( thread_terminal[RPID(GetCurrentThread())]==visible_terminal )
		return GetCTRL();
	else
		return 0;
}

//--------------------------------------------------------------------------
int scall_getalt(SYSCALLPAR)
{
	// Only when available to us
	if( thread_terminal[RPID(GetCurrentThread())]==visible_terminal )
		return GetALT();
	else
		return 0;
}

//--------------------------------------------------------------------------
// Enables the caller thread to
// receive messages from other processes.
// (automatically activated nowadays, but still can be manually used...)
int scall_activatemsgbox(SYSCALLPAR)
{
	//
	return ActivateThreadMessageBox(GetCurrentThread());
}

//--------------------------------------------------------------------------
// Send message to a process.
int scall_sendmessage(SYSCALLPAR)
{
	void *p;

	// Get PTR
	p=A2K(scall_ebx);

	// Send message
	return sendMessage(p, scall_ecx);
}

//--------------------------------------------------------------------------
// Receive message.
int scall_receivemessage(SYSCALLPAR)
{
	void *p;

	// Get PTR
	p=A2K(scall_ebx);

	// Receive message
	return receiveMessage(p);
}

//--------------------------------------------------------------------------
// Resolve thread name to a PID.
int scall_getthreadpid(SYSCALLPAR)
{
	void *p;

	// Get PTR
	p=A2K(scall_ebx);

	//
	return GetThreadPID(p);
}

//--------------------------------------------------------------------------
// Give thread a name
// (ONLY allows to identify itself,
//  future modifications will be made,
//  to allow system processes to do
//  identifying itself.)
int scall_identifythread(SYSCALLPAR)
{
	void *p;

	// Get PTR
	p=A2K(scall_ecx);

	// Only allow to modify itself
	if(scall_ebx==GetCurrentThread())
	{
		// Identify
		IdentifyThread(scall_ebx, p);
	}
	else
	{
		//
		printk("%s: Sorry PID%d, I cannot allow you to identify PID%d..\n",
			__FUNCTION__, GetCurrentThread(), scall_ebx);
		// Error
		return 1;
	}

	// Return OK
	return 0;
}

//--------------------------------------------------------------------------
// Return second count
int scall_getseconds(SYSCALLPAR)
{
	//
	return GetSeconds();
}

//--------------------------------------------------------------------------
// Copy a global memory region.
//
//	globalcopy(srcPID,srcPTR, dstPID,dstPTR, length);
//
//	scall_ebx	source PID
//	scall_ecx	source offset
//	scall_edx	destination PID
//	scall_esi	destination offset
//	scall_edi	copy length in bytes
//
int scall_globalcopy(SYSCALLPAR)
{
	BYTE *src,*dst;
	int srcPID,dstPID,len;
	char str[256];

	// Get PIDs
	srcPID = scall_ebx;
	dstPID = scall_edx;

	// Get copy length
	len = scall_edi;

	// Get PTRs
	src = _A2K(scall_ecx, srcPID);
	dst = _A2K(scall_esi, dstPID);

	//
	if( illegal(dst) || illegal(src) )
	{
		//
		sprintf(str, "Illegal globalcopy(%x,%x, %d).",
			dst,src,len);
		panic(str);
	}

	// Perform copy
	memcpy(dst, src, len);

	//
	return 0;
}

//--------------------------------------------------------------------------
//
int scall_getthreadterminal(SYSCALLPAR)
{
	//
	return GetThreadTerminal(scall_ebx);
}

//--------------------------------------------------------------------------
//
int scall_setthreadterminal(SYSCALLPAR)
{
	//
	SetThreadTerminal(scall_ebx, scall_ecx);
	return 0;
}

//--------------------------------------------------------------------------
// Create a new terminal.
int scall_newterminal(SYSCALLPAR)
{
	// Returns terminal ID
	return NewTerminal();
}

//--------------------------------------------------------------------------
// Send a byte to terminal's keyboard buffer.
int scall_outputterminalkeyboardbuffer(SYSCALLPAR)
{
	//
	return OutputTerminalKeyboardBuffer(
		&screens[scall_ebx], scall_ecx);
}

//--------------------------------------------------------------------------
// Send a byte to terminal's keyboard buffer
int scall_inputterminalkeyboardbuffer(SYSCALLPAR)
{
	//
	return InputTerminalKeyboardBuffer(
		&screens[scall_ebx]);
}

//--------------------------------------------------------------------------
// Copy specified terminal to a buffer.
int scall_copyterminal(SYSCALLPAR)
{
	BYTE *p;
	int ter;

	// Get parameters
	ter = scall_ebx;
	p = A2K(scall_ecx);

	// Copy
	memcpy(p, screens[ter].buf,
		screens[ter].width*screens[ter].height*2);

	//
	return 0;
}

//--------------------------------------------------------------------------
// Terminate a process
int scall_killthread(SYSCALLPAR)
{
	//
	KillThread(scall_ebx);
	return 0;
}

//--------------------------------------------------------------------------
// Get cursor location in specified terminal.
int scall_getcursorlocation(SYSCALLPAR)
{
	//
	return GetCursorLocation(scall_ebx);
}

//--------------------------------------------------------------------------
// Set interrupt handler.
int scall_setint(SYSCALLPAR)
{
	// Add a new handler to the interrupt
	addInterruptHandler(scall_ebx,
		scall_ecx,scall_edx,scall_esi);
	return 0;
}

//--------------------------------------------------------------------------
// Enable IRQ #.
int scall_enableirq(SYSCALLPAR)
{
	//
	enable_irq(scall_ebx);
	return 0;
}

//--------------------------------------------------------------------------
//
int scall_diskchange(SYSCALLPAR)
{
	//
	diskchangednr(scall_ebx);
	return 0;
}

//--------------------------------------------------------------------------
// Converts real PID ID (linear PID) to unique PID.
// Unique PIDs(UNIPIDs) are used to fix many issues.
// These UNIPIDs prevent same process IDs getting
// used too often, in fact quite rarely.
int scall_getthreaduniquepid(SYSCALLPAR)
{
	//
	return GetThreadUniquePID(scall_ebx);
}

//--------------------------------------------------------------------------
// Register a new device.
int scall_registernewdevice(SYSCALLPAR)
{
	//
	return driver_register_new_deviceEx(A2K(scall_ebx),
			scall_ecx, scall_edx,
			scall_esi,
			scall_edi,
			scall_ebp);
	// int driver_register_new_deviceEx(const char *device_name,
	//                                void *device_call, int CS,
	//                                int pid,
	//                                WORD type,
	//                                int Alias)
}

//--------------------------------------------------------------------------
// Switch to specified thread.
int scall_switchtothreadex(SYSCALLPAR)
{
	//
	SwitchToThreadEx(scall_ebx);

	//
	return 0;
}

//--------------------------------------------------------------------------
// Create a new terminal.
int scall_createterminal(SYSCALLPAR)
{
	int terID;

	//
	terID = NewTerminal();

	//
	SetTerminalOwner(terID, GetCurrentThread());

	//
	return terID;
}

//--------------------------------------------------------------------------
// Set terminal owner.
int scall_setterminalowner(SYSCALLPAR)
{
	//
	return SetTerminalOwner(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
// Createa a new terminal.
int scall_getterminalowner(SYSCALLPAR)
{
	//
	return GetTerminalOwner(scall_ebx);
}

//--------------------------------------------------------------------------
//
int scall_serialget(SYSCALLPAR)
{
	//
	return serial_get(scall_ebx);
}

//--------------------------------------------------------------------------
//
int scall_serialput(SYSCALLPAR)
{
	//
	return serial_put(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
//
int scall_setserialportspeed(SYSCALLPAR)
{
	//
	SetSerialPortSpeed(scall_ebx, scall_ecx);
	return 0;
}

//
/*int device_call(long device_nr,
                long n_function,
                long v1,long v2,long v3,long v4,
                void *p1,void *p2)
*/

//--------------------------------------------------------------------------
// Receive raw packet.
int scall_receiverawpacket(SYSCALLPAR)
{
	// scall_ebx	device number
	// scall_ecx	PTR to packet buffer

	//
	return device_call(scall_ebx, // device number
		DEVICE_CMD_RECEIVERAWPACKET, // receive raw packet
		0,0,0,0,
		A2K(scall_ecx),NULL); // PTR to packet buffer.
}

//--------------------------------------------------------------------------
// Transmit raw packet
int scall_transmitrawpacket(SYSCALLPAR)
{
	// scall_ebx	device number
	// scall_ecx	PTR to packet buffer
	// scall_edx	packet length in bytes

	//
	return device_call(scall_ebx, // device number
		DEVICE_CMD_TRANSMITRAWPACKET, // receive raw packet
		scall_edx,0,0,0, // packet length
		A2K(scall_ecx),NULL); // PTR to packet buffer
		
		
}

//--------------------------------------------------------------------------
// Device call.
// (obsolete ????)
int scall_devicecall(SYSCALLPAR)
{
	DWORD ad;
	DCPACKET *p;
	int dnr,rv;

	//-------------------------------------------------------------
	// Get parameters
	//

	// scall_ebx	dnr
	// scall_ecx	PTR to DCPACKET

	//
	dnr = scall_ebx;

	//
	ad = A2K(scall_ecx); p=ad;

	//-------------------------------------------------------------
	// Check
	//

	// Check device
	if( !isValidDevice(dnr) )
	{
		//
		printk("%s: INVALID DEVICE %d (p=0x%x)\n",
			__FUNCTION__, *p, p);
		return -1;
	}

	// Check function # validity
	if(p->n_function<=0)
	{
		//
		printk("%s: INVALID DEVCALL FUNCTION(%d)\n",
			__FUNCTION__, p->n_function);
		return -1;
	}

	//
	rv = device_call(dnr, // device number
		p->n_function, // function nr.
		p->par1,p->par2,p->par3,p->par4,
		p->p1,p->p2); // pointers

	//
	return rv;
}

//--------------------------------------------------------------------------
// Sleep specified amount of 1/100 seconds
int scall_milsleep(SYSCALLPAR)
{
	//
	MilSleep(scall_ebx);

	//
	return 0;
}

//--------------------------------------------------------------------------
//
int GetTCPIPStackDNR(void)
{
	int dnr;

	//
	dnr = driver_getdevicenrbyname("network");
	return dnr;
}

//--------------------------------------------------------------------------
// Open socket.
int scall_opensocket(SYSCALLPAR)
{
	int rv;

	// scall_ebx	32-bit IP address
	// scall_ecx	port address

	//
	rv = device_call(GetTCPIPStackDNR(), // device number
		DEVICE_CMD_OPENSOCKET,
		scall_ebx,scall_ecx,0,0,
		NULL,NULL);

	//
	return rv;
}

//--------------------------------------------------------------------------
// Close socket.
int scall_closesocket(SYSCALLPAR)
{
	int rv;

	// scall_ebx	socket handle (int)

	//
	rv = device_call(GetTCPIPStackDNR(), // device number
		DEVICE_CMD_CLOSESOCKET,
		scall_ebx,0,0,0,
		NULL,NULL);

	//
	return rv;
}

//--------------------------------------------------------------------------
// Read socket.
int scall_readsocket(SYSCALLPAR)
{
	int rv;

	// scall_ebx	socket handle
	// scall_ecx	PTR to buffer

	//
	rv = device_call(GetTCPIPStackDNR(), // device number
		DEVICE_CMD_READSOCKET,
		scall_ebx,A2K(scall_ecx),0,0,
		NULL,NULL);

	//
	return rv;
}

//--------------------------------------------------------------------------
// Write socket.
int scall_writesocket(SYSCALLPAR)
{
	int rv;

	// scall_ebx	socket handle
	// scall_ecx	PTR to buffer
	// scall_edx	number of bytes

	//
	rv = device_call(GetTCPIPStackDNR(), // device number
		DEVICE_CMD_WRITESOCKET,
		scall_ebx,scall_edx,0,0,
		A2K(scall_ecx),NULL);

	//
	return rv;
}

//--------------------------------------------------------------------------
// Schedule.
void scall_switchtothread(SYSCALLPAR)
{
	//
	SwitchToThread();
}

//--------------------------------------------------------------------------
// Get current work directory path.
// Arguments: [getcwd] [string]
int scall_getcwd(SYSCALLPAR)
{
	DWORD ad;
	void *p;

	//
	return getcwd(scall_ebx);
}

//--------------------------------------------------------------------------
void *scall_kmalloc(SYSCALLPAR)
{
	return malloc(scall_ebx);
}

//--------------------------------------------------------------------------
void scall_kfree(SYSCALLPAR)
{
	free(scall_ebx);
}


//--------------------------------------------------------------------------
// scall_postcmd
//
// New: Modified to transfer execution immediatly
// to postman process so no CPU time is spent
// elsewhere, which might lead into a markable
// inefficiency.
//
int scall_postcmd(SYSCALLPAR)
{
	return 0;
}

//--------------------------------------------------------------------------
// scall_open
//
int scall_open(SYSCALLPAR)
{
	// pathname, flags
	return open(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
// scall_close
//
int scall_close(SYSCALLPAR)
{
	// file descriptor
	return close(scall_ebx);
}

//
int scall_read(SYSCALLPAR)
{
	// pathname, flags
	return read(scall_ebx, scall_ecx, scall_edx);
}

//--------------------------------------------------------------------------
// scall_close
//
int scall_write(SYSCALLPAR)
{
	// file descriptor
	return write(scall_ebx, scall_ecx, scall_edx);
}

//--------------------------------------------------------------------------
// scall_fnctl
//
int scall_fnctl(SYSCALLPAR)
{
	// file descriptor
	//return fnctl(scall_ebx);
	return 0;
}

//--------------------------------------------------------------------------
// scall_fnctl
//
int scall_lseek(SYSCALLPAR)
{
	// fd, offset, whence
	return lseek(scall_ebx,scall_ecx,scall_edx);
}

//--------------------------------------------------------------------------
// scall_stat
//       int stat(const char *pathname, struct stat *buf);
//       int fstat(int fd, struct stat *buf);
//       int lstat(const char *pathname, struct stat *buf);
//
int scall_stat(SYSCALLPAR)
{
	// pathname, buf
	return stat(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
// scall_fstat
//       int stat(const char *pathname, struct stat *buf);
//       int fstat(int fd, struct stat *buf);
//       int lstat(const char *pathname, struct stat *buf);
//
int scall_fstat(SYSCALLPAR)
{
	// fd, buf
	return fstat(scall_ebx, scall_ecx);
}

//--------------------------------------------------------------------------
// scall_lstat
//       int stat(const char *pathname, struct stat *buf);
//       int fstat(int fd, struct stat *buf);
//       int lstat(const char *pathname, struct stat *buf);
//
int scall_lstat(SYSCALLPAR)
{
	// pathname, buf
	return lstat(scall_ebx, scall_ecx);
}
