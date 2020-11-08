/*
 *
 *	JTMOS DEVICE SYSTEM
 *	(C) 2001-2008 by Jari Tuominen.
 *
 */

#ifndef __INCLUDED_DRIVER_SYS_H__
#define __INCLUDED_DRIVER_SYS_H__

// Name to Device number.
#ifdef ND
#error ND ALREADY DEFINED
#endif
#define ND driver_getdevicenrbyname

//
#include "driversyspref.h"
#include "basdef.h"
#include "driver_api.h"
#include "dcpacket.h"

//
typedef struct
{
	// Is the device busy currently?
	char busy;
	// Statistics, read/write count
	int reads,writes,errors,others;
	// Type of the device
	DWORD type;
	// Short name for the device(max. 30 characters)
	char device_name[32];
	// EIP: PTR to DEVICE_CALL handler function
	void *device_call;
	// CS: code segment
	int cs; 
	// PID of device driver's service process
	// (only for non-kernel mode drivers)
	int pid;
	// Owner PID of this device.
	int owner;
	//
	char isfree;

	// 0 if not file system.
	int fs_type;

	// Alias DNR
	int alias;
	// 1=Read cache enabled(Default), 0=Read cache disabled(e.g. floppy)
	int DAPIReadCache;
}DEVICE_ENTRY;

// DEVICE LIST STRUCTURE
typedef struct
{
	//
	WORD n_devices;
 	//
	DEVICE_ENTRY device[N_MAX_DEVICES];
}DEVDB;

//
extern DEVDB *devdb;

//
int driver_getmaxdevices(void);
void driver_sys_init(void);
int driver_register_new_device(const char *,
				void *,
				WORD);
int driver_getdeviceamount(void);
DEVICE_ENTRY *driver_getdevice(int);
DEVICE_ENTRY *driver_getdevicebyname(const char *name);
int driver_getdevicenrbyname(const char *name);
char *driver_getdevicename(int device_nr,char *str);
int isValidDevice(int devnr);
int driver_register_new_deviceEx(const char *device_name,
                                void *device_call, int CS,
				int pid,
                                WORD device_type,
                                int Alias);
int driver_getAlias(int id);

//
void driver_disableDAPIReadCache(int dnr);
void driver_enableDAPIReadCache(int dnr);
int driver_nr(void);
int driver_ShutdownAllDevices(void);
int device(const char *name);
int DevRemPID(int pid);
int driver_removedevice(int dnr);
int getdevicenrbyname(const char *name);

//

#endif




