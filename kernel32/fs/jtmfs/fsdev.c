// ==============================================================================================
// fsdev.c
//
// File API - Device / path determination for each process.
// Yes, each process has own unique path/device identifiers,
// and each process does not rely on other procsses current
// directory path.
//
// JTMOS file I/O application implementation for applications and medium level system functions
// (C) 2002 Jari Tuominen(jari.tuominen@kanetti.net)
// ==============================================================================================
#include "kernel32.h"
#include "jtmfat.h" // getfreeblock
#include "jtmfs.h" // comes with fileapi as well
#include "ramdisk.h" // default_ramdisk
#include "fsdos.h"

/*
// Returns current device that is assigned to the process to use.
int GetCurrentDevice(void)
{
	// THIS IS A HACK, TODO LATER!
	return fileapi.dnr;
}

//
int SetCurrentDevice(int dnr)
{
	//
	fileapi.dnr = dnr;
	return 0;
}
*/
