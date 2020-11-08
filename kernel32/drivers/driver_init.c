// =====================================================
// DEVAPI system - driver_init.c
// Cache part is removed because was 100% obsolete.
// (C) 2002-2004 by Jari Tuominen
// =====================================================
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "driver_api.h"
#include "driver_sys.h"
#include "driver_lock.h"
#include "driver_rw.h" // readblock...
#include "driver_devcall.h"
#include "driver_init.h"
//
#define PROGRESS	printk(".");
#define COMPLETED	printk("\n");

//
void driver_init(void)
{
	int i;
	char *p;

	//
	return;
}

