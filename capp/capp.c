//
#include <stdio.h>
#include <stdlib.h>

//
#define BYTE unsigned char
#define WORD unsigned short int
#define DWORD unsigned int

//
int taskSwitchNr;
void **tasks;
int XON_DEZOMBIE = 0;
DWORD tmpds;
DWORD tmpes;
DWORD tmpfs;
DWORD tmpgs;
DWORD tmpss;

//
int main(int argc, char **argv)
{
	//
	
}

void idle_moment(void)
{
}

void *malloc1(int _count)
{
}

int ChangeChunkOwner(void *_ptr, int unipid)
{
}

int SetMemoryName(void *_ptr, char *_name)
{
}

void free1(void *_ptr)
{
	free(_ptr);
}

void dmemset(void *_buf, char _ch, int _length, char *_fun)
{
	memset(_buf,_ch,_length);
}

void InitTSS(void)
{
}

void NewTSS(void *t, DWORD sysad, WORD link, int pid)
{
}

void LOADTR(WORD _x)
{
}

void FATALERROR(const char *s)
{
	printf("* %s *\n", s);
	while(1) { }
}

DWORD get_eflags(void)
{
	return 0;
}

void set_eflags(DWORD _content)
{
}

void allocate_shell_data(void)
{
}

int illegal(DWORD _ad)
{
	return 0;
}

void FreePIDTerminals(int _pid)
{
}

int deallocate_proc_filedes(int _pid)
{
	//
	return 0;
}

void malloc_freeprocmem(int _pid)
{
}

void fardcall(DWORD eip, DWORD cs,
	DWORD r1, DWORD r2,
	DWORD r3, DWORD r4,
	DWORD r5, DWORD r6,
	DWORD r7)
{
}

void xputs(const char *_s, char *_fun)
{
	printk("%s", _s);
}

int ReportMessage(const char *_fmt, ...)
{
	// TODO
}

void StretchPrint(const char *_s, int _len)
{
	//
	printk("%s", _s);
}

int isValidString(const char *_s)
{
	int i,ok;

	//
	for(i=0,ok=0; i<256; i++)
	{
		if(!_s[i]) { ok=1; break; }
	}

	//
	return ok;
}

void *procmalloc(DWORD _l, int _pid, const char _propert_name)
{
	return malloc(_l);
}

void dmemcpy(void *_o, void *_s, DWORD _l, char *_fun)
{
	memcpy(_o, _s, _l);
}

void p32(DWORD _val)
{
	printf("%u", _val);
}

void pd32(DWORD _val)
{
	printf("%d", _val);
}

/*
../kernel32/heart/threads/threads.o: In function `GetTickCount':
threads.c:(.text+0x1734): undefined reference to `glvar'
../kernel32/heart/threads/threads.o: In function `GetTickCount2':
threads.c:(.text+0x1744): undefined reference to `glvar'
../kernel32/devsys/devapi.o: In function `getsize':
devapi.c:(.text+0xd3): undefined reference to `device_call'
../kernel32/devsys/devapi.o: In function `getblocksize':
devapi.c:(.text+0x127): undefined reference to `device_call'
../kernel32/devsys/devapi.o: In function `devapi_unmountDNR':
devapi.c:(.text+0x19a): undefined reference to `device_call'
devapi.c:(.text+0x1b1): undefined reference to `device_call'
../kernel32/devsys/devapi.o: In function `devapi_mountDNR':
devapi.c:(.text+0x1d9): undefined reference to `device_call'
../kernel32/devsys/devsys.o:devsys.c:(.text+0xb0): more undefined references to `device_call' follow
../kernel32/devsys/devsys.o: In function `devsys_init':
devsys.c:(.text+0x20d): undefined reference to `pd32'
../kernel32/heart/threads/exec.o: In function `ExecKProc':
exec.c:(.text+0x58): undefined reference to `GetRunningNumber'
../kernel32/heart/threads/gdtmanipulation.o: In function `SetCallGate':
gdtmanipulation.c:(.text+0x178): undefined reference to `gdt_table'
../kernel32/heart/threads/gdtmanipulation.o: In function `SetCDEntryEx':
gdtmanipulation.c:(.text+0x238): undefined reference to `gdt_table'
../kernel32/heart/threads/LaunchApp.o: In function `AdaptAppPages':
LaunchApp.c:(.text+0x85): undefined reference to `GetPage'
../kernel32/heart/threads/LaunchApp.o: In function `LaunchAppEx':
LaunchApp.c:(.text+0xe7): undefined reference to `xputs'
LaunchApp.c:(.text+0x112): undefined reference to `caster'
LaunchApp.c:(.text+0x141): undefined reference to `xputs'
LaunchApp.c:(.text+0x17a): undefined reference to `xputs'
LaunchApp.c:(.text+0x1af): undefined reference to `xputs'
LaunchApp.c:(.text+0x212): undefined reference to `xputs'
LaunchApp.c:(.text+0x24a): undefined reference to `ReportMessage'
LaunchApp.c:(.text+0x25f): undefined reference to `xputs'
LaunchApp.c:(.text+0x2cc): undefined reference to `xputs'
LaunchApp.c:(.text+0x30d): undefined reference to `ReportMessage'
LaunchApp.c:(.text+0x322): undefined reference to `xputs'
LaunchApp.c:(.text+0x348): undefined reference to `xputs'
../kernel32/heart/threads/msgbox.o: In function `sendMessage':
msgbox.c:(.text+0x125): undefined reference to `illegal'
msgbox.c:(.text+0x1bf): undefined reference to `illegal'
msgbox.c:(.text+0x20b): undefined reference to `p32'
msgbox.c:(.text+0x2b6): undefined reference to `dmemcpy'
../kernel32/heart/threads/msgbox.o: In function `receiveMessage':
msgbox.c:(.text+0x3db): undefined reference to `dmemcpy'
../kernel32/heart/threads/scheduler.o: In function `_idle_moment':
scheduler.c:(.text+0x5): undefined reference to `scallsIsWorking'
../kernel32/heart/threads/scheduler.o: In function `MapApplicationMemory':
scheduler.c:(.text+0x28): undefined reference to `GetKernelPageDirPTR'
../kernel32/heart/threads/scheduler.o: In function `LLSchedule':
scheduler.c:(.text+0x72): undefined reference to `unbusyTSS'
scheduler.c:(.text+0x97): undefined reference to `tss_goto'
../kernel32/heart/threads/scheduler.o: In function `TaskPitHandler':
scheduler.c:(.text+0xa7): undefined reference to `pit_handler'
scheduler.c:(.text+0xb3): undefined reference to `outportb'
../kernel32/heart/threads/threadSwitch.o: In function `vmware_sleep':
threadSwitch.c:(.text+0x19): undefined reference to `vmsleep'
../kernel32/heart/dataproc/dfunc.o: In function `dsleep':
dfunc.c:(.text+0x22): undefined reference to `jtmos_debug'
../kernel32/heart/dataproc/dfunc.o: In function `dprintk':
dfunc.c:(.text+0x4f): undefined reference to `jtmos_debug'
dfunc.c:(.text+0x73): undefined reference to `svprintf'
dfunc.c:(.text+0x7f): undefined reference to `WriteSystemLog'
dfunc.c:(.text+0x88): undefined reference to `serial_debug_line'
dfunc.c:(.text+0x94): undefined reference to `ReportMessage'
../kernel32/jtmfs/createfat.o: In function `jtmfat_createfat':
createfat.c:(.text+0xa1): undefined reference to `devapi_TriggerCacheFlush'
createfat.c:(.text+0x144): undefined reference to `devapi_TriggerCacheFlush'
createfat.c:(.text+0x150): undefined reference to `DiskChange'
createfat.c:(.text+0x29e): undefined reference to `devapi_TriggerCacheFlush'
../kernel32/jtmfs/createfat.o: In function `BuildInfoBlock':
createfat.c:(.text+0x3de): undefined reference to `ReportMessage'
../kernel32/jtmfs/createfat.o: In function `WriteInformationBlock':
createfat.c:(.text+0x5b1): undefined reference to `readblock'
createfat.c:(.text+0x61a): undefined reference to `writeblock'
../kernel32/jtmfs/createfat.o: In function `ReadInformationBlock':
createfat.c:(.text+0x6a1): undefined reference to `readblock'
../kernel32/jtmfs/createfat.o: In function `SetKernelChecksum':
createfat.c:(.text+0x730): undefined reference to `readblock'
createfat.c:(.text+0x74c): undefined reference to `writeblock'
../kernel32/jtmfs/createfat.o: In function `InstallBootLoader':
createfat.c:(.text+0x7ab): undefined reference to `boot_bin_length'
createfat.c:(.text+0x7b1): undefined reference to `boot_bin'
createfat.c:(.text+0x7b7): undefined reference to `dmemcpy'
createfat.c:(.text+0x816): undefined reference to `boot_bin_length'
createfat.c:(.text+0x83f): undefined reference to `writeblock'
../kernel32/jtmfs/createfat.o: In function `WriteKernel':
createfat.c:(.text+0x8cd): undefined reference to `ReportMessage'
createfat.c:(.text+0x8e5): undefined reference to `writeblock'
createfat.c:(.text+0x90b): undefined reference to `ReportMessage'
createfat.c:(.text+0x919): undefined reference to `ReportMessage'
../kernel32/jtmfs/createfat.o: In function `CopyKernel':
createfat.c:(.text+0x9ee): undefined reference to `ReportMessage'
createfat.c:(.text+0xa1c): undefined reference to `readblock'
createfat.c:(.text+0xa30): undefined reference to `ReportMessage'
../kernel32/jtmfs/dirdbAccess.o: In function `dirdb_clearCacheForDB':
dirdbAccess.c:(.text+0x12c): undefined reference to `FlushAll'
../kernel32/jtmfs/dirdbAccess.o: In function `db_ReadDirectory':
dirdbAccess.c:(.text+0x67f): undefined reference to `procmalloc'
dirdbAccess.c:(.text+0x69a): undefined reference to `readblock'
../kernel32/jtmfs/dirdbAccess.o: In function `readDir':
dirdbAccess.c:(.text+0x78c): undefined reference to `procmalloc'
dirdbAccess.c:(.text+0x7ee): undefined reference to `ReportMessage'
../kernel32/jtmfs/dirdb.o: In function `pause':
dirdb.c:(.text+0x13): undefined reference to `ESCLOCK'
../kernel32/jtmfs/dirdb.o: In function `dirdb_init':
dirdb.c:(.text+0xb1): undefined reference to `procmalloc'
../kernel32/jtmfs/dir.o: In function `jtmfs_dir':
dir.c:(.text+0x197): undefined reference to `StretchPrint'
dir.c:(.text+0x1a2): undefined reference to `StretchPrint'
dir.c:(.text+0x1da): undefined reference to `getch1'
dir.c:(.text+0x208): undefined reference to `StretchPrint'
dir.c:(.text+0x228): undefined reference to `StretchPrint'
dir.c:(.text+0x248): undefined reference to `StretchPrint'
dir.c:(.text+0x267): undefined reference to `StretchPrint'
dir.c:(.text+0x287): undefined reference to `StretchPrint'
../kernel32/jtmfs/dir.o:dir.c:(.text+0x2a7): more undefined references to `StretchPrint' follow
../kernel32/jtmfs/flexFat.o: In function `flexFatInit':
flexFat.c:(.text+0xc0): undefined reference to `procmalloc'
../kernel32/jtmfs/flexFat.o: In function `flexAllocateFat':
flexFat.c:(.text+0x243): undefined reference to `procmalloc'
flexFat.c:(.text+0x301): undefined reference to `procmalloc'
../kernel32/jtmfs/flexFat.o: In function `flexRWFAT':
flexFat.c:(.text+0x420): undefined reference to `Attention'
flexFat.c:(.text+0x48d): undefined reference to `Attention'
flexFat.c:(.text+0x50b): undefined reference to `Attention'
flexFat.c:(.text+0x639): undefined reference to `readblock'
flexFat.c:(.text+0x68e): undefined reference to `writeblock'
flexFat.c:(.text+0x6da): undefined reference to `writeblock'
flexFat.c:(.text+0x702): undefined reference to `Attention'
../kernel32/jtmfs/format.o: In function `jtmfs_formatdrive':
format.c:(.text+0x63): undefined reference to `DiskChange'
format.c:(.text+0x12a): undefined reference to `FlushAll'
format.c:(.text+0x158): undefined reference to `FlushAll'
../kernel32/jtmfs/fsaddblocks.o: In function `jtmfs_add_blocks_for_entry':
fsaddblocks.c:(.text+0xd2): undefined reference to `glvar'
../kernel32/jtmfs/fsadd.o: In function `jtmfs_adddirentry':
fsadd.c:(.text+0xdf): undefined reference to `ReportMessage'
../kernel32/jtmfs/fsdos.o: In function `findfirst':
fsdos.c:(.text+0xaf): undefined reference to `new_filedes'
fsdos.c:(.text+0xcf): undefined reference to `get_filedesptr'
fsdos.c:(.text+0x1bc): undefined reference to `FreeFD'
../kernel32/jtmfs/fsdos.o: In function `findnext':
fsdos.c:(.text+0x1e4): undefined reference to `get_filedesptr'
../kernel32/jtmfs/fslast.o: In function `jtmfs_seektolastentry':
fslast.c:(.text+0x3f): undefined reference to `print'
fslast.c:(.text+0x4c): undefined reference to `print'
fslast.c:(.text+0x5c): undefined reference to `print'
fslast.c:(.text+0xbc): undefined reference to `ReportMessage'
../kernel32/jtmfs/isJTMFS.o: In function `onNonJTMFSDrive':
isJTMFS.c:(.text+0xe): undefined reference to `DiskChange'
../kernel32/jtmfs/jtmfat.o: In function `write_fat':
jtmfat.c:(.text+0xba): undefined reference to `writeblock'
../kernel32/jtmfs/jtmfat.o: In function `allocate_fat':
jtmfat.c:(.text+0x138): undefined reference to `procmalloc'
jtmfat.c:(.text+0x166): undefined reference to `procmalloc'
jtmfat.c:(.text+0x209): undefined reference to `writeblock'
../kernel32/jtmfs/jtmfat.o: In function `touch_fat':
jtmfat.c:(.text+0x24d): undefined reference to `writeblock'
../kernel32/jtmfs/jtmfat.o: In function `jtmfat_init':
jtmfat.c:(.text+0x2a3): undefined reference to `procmalloc'
jtmfat.c:(.text+0x2e8): undefined reference to `procmalloc'
jtmfat.c:(.text+0x364): undefined reference to `procmalloc'
jtmfat.c:(.text+0x3be): undefined reference to `procmalloc'
../kernel32/jtmfs/jtmfat.o: In function `jtmfat_LoadNewInfoBlock':
jtmfat.c:(.text+0x41e): undefined reference to `procmalloc'
jtmfat.c:(.text+0x44d): undefined reference to `ReportMessage'
jtmfat.c:(.text+0x47a): undefined reference to `readblock'
jtmfat.c:(.text+0x4b0): undefined reference to `dmemcpy'
../kernel32/jtmfs/jtmfat.o: In function `jtmfat_getrootdir1':
jtmfat.c:(.text+0x8ce): undefined reference to `GetSeconds'
jtmfat.c:(.text+0x8e7): undefined reference to `GetSeconds'
../kernel32/jtmfs/jtmfsAccessInit.o: In function `jtmfsAccessInit':
jtmfsAccessInit.c:(.text+0x29): undefined reference to `procmalloc'
jtmfsAccessInit.c:(.text+0x5a): undefined reference to `procmalloc'
jtmfsAccessInit.c:(.text+0x73): undefined reference to `procmalloc'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_GetFileDataBlock':
jtmfsAccess.c:(.text+0x2dc): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_DeleteFile':
jtmfsAccess.c:(.text+0x35c): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_CreateFile':
jtmfsAccess.c:(.text+0x3fd): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_GetFileSize':
jtmfsAccess.c:(.text+0x5dc): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `_jtmfs_WriteFile':
jtmfsAccess.c:(.text+0x6cd): undefined reference to `isValidString'
jtmfsAccess.c:(.text+0x795): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0x7da): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0x7fe): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0x819): undefined reference to `ReportMessage'
../kernel32/jtmfs/jtmfsAccess.o: In function `_jtmfs_ReadFile':
jtmfsAccess.c:(.text+0x903): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_AddBlocks':
jtmfsAccess.c:(.text+0xa4c): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_Fexist':
jtmfsAccess.c:(.text+0xaec): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_SetSize':
jtmfsAccess.c:(.text+0xbac): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_RenameFile':
jtmfsAccess.c:(.text+0xc50): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o:jtmfsAccess.c:(.text+0xc60): more undefined references to `isValidString' follow
../kernel32/jtmfs/jtmfsAccess.o: In function `GetCurrentDrive':
jtmfsAccess.c:(.text+0xd21): undefined reference to `scall'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_chdir':
jtmfsAccess.c:(.text+0xdb5): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfsAccess.o: In function `_jtmfs_chdir':
jtmfsAccess.c:(.text+0xf28): undefined reference to `isValidString'
jtmfsAccess.c:(.text+0xfa7): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0xfc0): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0x10a4): undefined reference to `ReportMessage'
jtmfsAccess.c:(.text+0x10b9): undefined reference to `ReportMessage'
../kernel32/jtmfs/jtmfsAccess.o: In function `jtmfs_CreateDirectory':
jtmfsAccess.c:(.text+0x129d): undefined reference to `isValidString'
../kernel32/jtmfs/jtmfs.o: In function `jtmfs_inc_try':
jtmfs.c:(.text+0x77): undefined reference to `print'
jtmfs.c:(.text+0x85): undefined reference to `pd32'
jtmfs.c:(.text+0x95): undefined reference to `print'
jtmfs.c:(.text+0xaf): undefined reference to `procmalloc'
jtmfs.c:(.text+0xce): undefined reference to `procmalloc'
../kernel32/jtmfs/jtmfs.o: In function `jtmfs_createdirectory':
jtmfs.c:(.text+0x325): undefined reference to `illegal'
jtmfs.c:(.text+0x490): undefined reference to `ReportMessage'
jtmfs.c:(.text+0x4ce): undefined reference to `ReportMessage'
jtmfs.c:(.text+0x539): undefined reference to `ReportMessage'
jtmfs.c:(.text+0x5e3): undefined reference to `writeblock'
jtmfs.c:(.text+0x61c): undefined reference to `ReportMessage'
jtmfs.c:(.text+0x64e): undefined reference to `ReportMessage'
../kernel32/jtmfs/jtmfs.o: In function `jtmfs_expandchain':
jtmfs.c:(.text+0x879): undefined reference to `ReportMessage'
../kernel32/jtmfs/jtmfs.o: In function `jtmfs_setsize':
jtmfs.c:(.text+0xd03): undefined reference to `ReportMessage'
../kernel32/jtmfs/mount.o: In function `mountDNR':
mount.c:(.text+0x18): undefined reference to `DiskChange'
../kernel32/jtmfs/mount.o: In function `unmountDNR':
mount.c:(.text+0x3f): undefined reference to `DiskChange'
../kernel32/jtmfs/ReadChain.o: In function `jtmfs_ReadFileChain':
ReadChain.c:(.text+0x195): undefined reference to `readblock'
../kernel32/jtmfs/WriteChain.o: In function `jtmfs_WriteFileChainEx':
WriteChain.c:(.text+0x148): undefined reference to `ReportMessage'
WriteChain.c:(.text+0x17c): undefined reference to `ReportMessage'
WriteChain.c:(.text+0x268): undefined reference to `readblock'
WriteChain.c:(.text+0x328): undefined reference to `writeblock'
WriteChain.c:(.text+0x40a): undefined reference to `writeblock'
WriteChain.c:(.text+0x45a): undefined reference to `ReportMessage'
../kernel32/jtmfs/writeDir.o: In function `writeDir':
writeDir.c:(.text+0x140): undefined reference to `procmalloc'
collect2: error: ld returned 1 exit status
make: *** [buildcapp] Error 1
*/
