//
#ifndef __INCLUDED_SYSTEMCALLS_H__
#define __INCLUDED_SYSTEMCALLS_H__
//
#include "basdef.h"
#include "localscall.h"

// Max. amount of time that a single postman command can take.
// Longer commands should be splitted automatically
// into several shorter commands.
// (TODO)
// 5 minutes.
#define TM_POSTCMD	(60*5)

//
extern DWORD A2K(DWORD appoffs);
extern DWORD _A2K(DWORD appoffs, int pid);

//
#define SYSCALLPAR	DWORD scall_eax,\
			DWORD scall_ebx,\
			DWORD scall_ecx,\
			DWORD scall_edx,\
			DWORD scall_esi,\
			DWORD scall_edi,\
			DWORD scall_ebp

#define MYSYSCALLPAR	scall_eax,\
			scall_ebx,\
			scall_ecx,\
			scall_edx,\
			scall_esi,\
			scall_edi,\
			scall_ebp

// Max. amount of system calls specified at once
#define N_MAX_SCALLS		1000

// external functions
extern void relax_thread(void);
extern void scall_dummy(SYSCALLPAR);

// basic variables
extern void *scall_functions[N_MAX_SCALLS];
extern DWORD nr_scall_func;

//
DWORD scall_nothing(SYSCALLPAR);

//
void scallDeactive(void);
void scallActive(SYSCALLPAR);

// MISCELLANEOUS FUNCTIONS
DWORD scall_cexec(SYSCALLPAR);
DWORD scall_puts(SYSCALLPAR);
DWORD scall_putchar(SYSCALLPAR);
DWORD scall_exit(SYSCALLPAR);
DWORD scall_clrscr(SYSCALLPAR);
DWORD scall_getch(SYSCALLPAR);
int scall_postcmd(SYSCALLPAR);
int scall_getthreaduniquepid(SYSCALLPAR);

//
DWORD scall_getcurdev(SYSCALLPAR);
DWORD scall_getcurdb(SYSCALLPAR);
DWORD scall_getdevicename(SYSCALLPAR);
DWORD scall_chdir(SYSCALLPAR);

//
int scall_setint(SYSCALLPAR);
int scall_enableirq(SYSCALLPAR);

//
int scall_getthreadmemoryusage(SYSCALLPAR);
int scall_getthreadname(SYSCALLPAR);
int scall_getthreadspending(SYSCALLPAR);
int scall_getthreadpriority(SYSCALLPAR);
int scall_getthreadcount(SYSCALLPAR);
void scall_relaxthread(SYSCALLPAR);
void scall_gotoxy(SYSCALLPAR);
DWORD scall_gettickcount(SYSCALLPAR);
DWORD scall_getamemsize(SYSCALLPAR);
DWORD scall_getcurrentthread(SYSCALLPAR);
DWORD scall_getargc(SYSCALLPAR);
void scall_getargv(SYSCALLPAR);
DWORD scall_waitprocterm(SYSCALLPAR);
void scall_textcolor(SYSCALLPAR);
void scall_textbackground(SYSCALLPAR);
int scall_getcursorlocation(SYSCALLPAR);
int scall_switchtothreadex(SYSCALLPAR);
void scall_switchtothread(SYSCALLPAR);

//
DWORD scall_findfirst(SYSCALLPAR);
DWORD scall_findnext(SYSCALLPAR);
DWORD scall_inputmouse(SYSCALLPAR);
void scall_setmouserect(SYSCALLPAR);
int scall_diskchange(SYSCALLPAR);

// VGA functions
void scall_vgadrawframe(SYSCALLPAR);
DWORD scall_vgaselectplane(SYSCALLPAR);
DWORD scall_vgawaitretrace(SYSCALLPAR);
int scall_killthread(SYSCALLPAR);

//
DWORD scall_wherex(SYSCALLPAR);
DWORD scall_wherey(SYSCALLPAR);

// Time / Date calls
DWORD scall_getdate(SYSCALLPAR);

//
int scall_getalt(SYSCALLPAR);
int scall_getctrl(SYSCALLPAR);

// Sockets
int scall_opensocket(SYSCALLPAR);
int scall_closesocket(SYSCALLPAR);
int scall_readsocket(SYSCALLPAR);
int scall_writesocket(SYSCALLPAR);

//
int scall_activatemsgbox(SYSCALLPAR);
int scall_receivemessage(SYSCALLPAR);
int scall_sendmessage(SYSCALLPAR);
int scall_getdevicenrbyname(SYSCALLPAR);
int scall_getthreadpid(SYSCALLPAR);
int scall_identifythread(SYSCALLPAR);
int scall_globalcopy(SYSCALLPAR);

//
int scall_getseconds(SYSCALLPAR);

//
int scall_getthreadterminal(SYSCALLPAR);
int scall_setthreadterminal(SYSCALLPAR);
int scall_newterminal(SYSCALLPAR);
int scall_outputterminalkeyboardbuffer(SYSCALLPAR);
int scall_inputterminalkeyboardbuffer(SYSCALLPAR);
int scall_copyterminal(SYSCALLPAR);
int scall_registernewdevice(SYSCALLPAR);

//
int scall_createterminal(SYSCALLPAR);
int scall_setterminalowner(SYSCALLPAR);
int scall_getterminalowner(SYSCALLPAR);
int scall_serialget(SYSCALLPAR);
int scall_serialput(SYSCALLPAR);
int scall_setserialportspeed(SYSCALLPAR);
int scall_transmitrawpacket(SYSCALLPAR);
int scall_receiverawpacket(SYSCALLPAR);
int scall_devicecall(SYSCALLPAR);
int scall_devicecall(SYSCALLPAR);
int scall_milsleep(SYSCALLPAR);
int scall_wgetch(SYSCALLPAR);
DWORD scall_rename(SYSCALLPAR);
DWORD scall_remove(SYSCALLPAR);
DWORD scall_mkfs(SYSCALLPAR);
DWORD scall_mkdir(SYSCALLPAR);

//
int scall_getcwd(SYSCALLPAR);
//
void *scall_kmalloc(SYSCALLPAR);
void scall_kfree(SYSCALLPAR);

//
#include "postman.h"

//
#include "syscallcodes.h"

//
typedef struct {
  char lfn_magic[6];        /* LFN: the magic "LFN32" signature */
  short lfn_handle;         /* LFN: the handle used by findfirst/findnext */
  unsigned short lfn_ctime; /* LFN: file creation time */
  unsigned short lfn_cdate; /* LFN: file creation date */
  unsigned short lfn_atime; /* LFN: file last access time (usually 0) */
  unsigned short lfn_adate; /* LFN: file last access date */
  char ff_reserved[5];      /* used to hold the state of the search */
  unsigned char ff_attrib;  /* actual attributes of the file found */
  unsigned short ff_ftime;  /* hours:5, minutes:6, (seconds/2):5 */
  unsigned short ff_fdate;  /* (year-1980):7, month:4, day:5 */
  unsigned long ff_fsize;   /* size of file */
  char ff_name[260];        /* name of file as ASCIIZ string */
}VFAT_FFBLK;

//
int scall_read1(SYSCALLPAR);
int scall_write1(SYSCALLPAR);
int scall_creat1(SYSCALLPAR);
int scall_unlink1(SYSCALLPAR);

//
int scall_read(SYSCALLPAR);
int scall_write(SYSCALLPAR);
int scall_creat(SYSCALLPAR);
int scall_unlink(SYSCALLPAR);
int scall_open(SYSCALLPAR);
int scall_close(SYSCALLPAR);
int scall_lseek(SYSCALLPAR);
int scall_stat(SYSCALLPAR);
int scall_lstat(SYSCALLPAR);
int scall_fstat(SYSCALLPAR);

//
#endif

//

