
/**** DEVICE MANAGEMENT SYSTEM - (C) 2001-2005,2017-2020 by Jari Tuominen (jari.t.tuominen@gmail.com). ***
 *
 * All devices get registered into the device sysCCCCCCCCCtem.
 *
 * DEVICE_CALL [DEVICE_N],...
 *
 * DEVICE_N is used when calling the device manager for
 * device information, like the device call void pointer.
 *
 */

// Include 'critical macros'.
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "driver_sys.h"

// Main structure for the device system
DEVDB *devdb;

//
int driver_getmaxdevices(void) { return N_MAX_DEVICES; }

/////////////////////////////////////////////////////////////////////////////////////////////
//
void driver_TestDevice(int dnr)
{
	char str[256],rv;

	//
	driver_getdevicename(dnr, str);

	//
//	printf("Pinging device %s: ...\n", str);
	if( (rv=driver_ping(dnr)) )
	{
		printk("%s: Device %s: is not responding to ping.\n",
			__FUNCTION__, str);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
int driver_ping(int dnr)
{
	DEVICE_ENTRY *de;

	//
	de = driver_getdevice(dnr);
	if(de==NULL)
		return -2;

	//
	if( (de->type&DEVICE_TYPE_NONKERNELDRV)==0 )
	{
		printk("%s: Device does not support remote device calls.\n",
			__FUNCTION__);
		return 0;
	}

	//
	if( device_call(dnr,
		DEVICE_CMD_PING,
		0,0,0,0, NULL,NULL)==-1 )
	{
		return -1;
	}

	//
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown all devices
int driver_ShutdownAllDevices(void)
{
	int amount,i;

	// Get amount of devices
	amount = driver_getdeviceamount();

	// Go through all devices
	for(i=0; i<amount; i++)
	{
		// Skip free entries.
		if( devdb->device[i].isfree )
			continue;

		// Send shutdown packet
		device_call(i, DEVICE_CMD_SHUTDOWN,
			0,0,0,0, NULL, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Remove all devices owned by specified PID.
int DevRemPID(int pid)
{
	int i,amount;
	DEVICE_ENTRY *e;

	//
	return 1;
	if(!pid)
		return 1;

	// Get amount of devices
	amount = driver_getdeviceamount();

	// Go through all devices
	for(i=0; i<amount; i++)
	{
		// Skip free entries.
		if( devdb->device[i].isfree )
			continue;

		// Is it owned by the specified PID?
		if( devdb->device[i].owner==pid )
			// Remove this device.
			driver_removedevice(i);
	}

	//
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Removes & disables a device.
int driver_removedevice(int dnr)
{
	DEVICE_ENTRY *e;

	//
	if( driver_getdevicename(dnr, NULL)==NULL )
		return 1;

	// Get entry PTR.
	e = &devdb->device[dnr];

	//
	e->isfree = TRUE;

	//
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Returns number of devices currently registered
inline int driver_nr(void)
{
	return devdb->n_devices;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Get device alias #
int driver_getAlias(int id)
{
	//
	return devdb->device[id].alias;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Is device number valid?
// 1=IS
// 0=IS NOT
int isValidDevice(int devnr)
{
	//
	if( devnr>=driver_getdeviceamount() || devnr<=0 )
		return FALSE;
	else
	{
		// Is it a freed device?
		if(devdb->device[devnr].isfree)
			return FALSE;
		//
		return TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
int driver_dummy(DCP)
{
	//
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// This function inits the device driver system.
void driver_sys_init(void)
{
	int i;

	// Print message
	printk("%s/%s: Device Driver Management System, ",
		__FUNCTION__, __FILE__);
	printk("maxdev="); printk("%d", driver_getmaxdevices());
	printk(", starting up ... ");

	// Init structure
	devdb = malloc(sizeof(DEVDB));
	memset(devdb,0,sizeof(DEVDB));
	// Leave device 0 unused
	devdb->n_devices=0;

	// Register dummy device.
	driver_register_new_device("dummy",
		driver_dummy,
		0);

	// OK!
	printk("OK\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Register new device
int driver_register_new_device(const char *device_name,
				void *device_call,
				WORD type)
{
	// Register it
	driver_register_new_deviceEx(device_name,
				device_call,0, // 0=NO CS(NON-FARCALL)
				0, // pid, zero means "no service process".
				type, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// This function allows registeration of a new device
// return value: 	key of the new device,
//			-1 on failure
// note: but anyway, any return value below 0 means always error
//
int driver_register_new_deviceEx(const char *device_name,
				void *device_call, int CS,
				int pid,
				WORD type,
				int Alias)
{
	int dnr;
	DEVICE_ENTRY *e;

	// Out of space on device registry?
	if(N_MAX_DEVICES<=devdb->n_devices) return -1;
 
	//
	if(device_name==NULL)
	{
		//
		printk("regdev error: device_name is NULL (II)\n");
		return -3;
	}

	// Determine DNR
	dnr = devdb->n_devices;

	// Get entry PTR
	e = &devdb->device[dnr];

	// Store the data
	strcpy(e->device_name, device_name);
	// Device type
	e->type = type;
	// CS:EIP
	e->cs = 0; // CS; TODO 2014
	e->device_call = device_call;
	// Alias
	e->alias = Alias;
	// Service PID
	e->pid = pid;
	// Set as reserved entry.
	e->isfree = FALSE;

	// Owner process.
	e->owner = pid;

	// Increase the amount of devices registered in
	devdb->n_devices++;

	//
	//// printk("[regdev<"); printf("%d", devdb->n_devices); printk(">]");

	//
	driver_enableDAPIReadCache( dnr );

	// Return the newly registered device's key
	return dnr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// This function returns the amount of devices currently registered
int driver_getdeviceamount(void)
{
	//
	return devdb->n_devices;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// This function returns a pointer to a device registry entry
// Returns NULL if not found
DEVICE_ENTRY *driver_getdevice(int which)
{
	// Are we within limits?
	if(which<0 || which>=N_MAX_DEVICES 
	    || which>devdb->n_devices )return NULL;
 
	// OK, request was legal, return pointer
	return &devdb->device[which];
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Convert string to device number.
int device(const char *name)
{
	//
	return driver_getdevicenrbyname(name);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
int getdevicenrbyname(const char *name)
{
	return driver_getdevicenrbyname(name);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Returns -1 if not found
// NEW: Skips path.
int driver_getdevicenrbyname(const char *path)
{
	int i,len;
	char *cptr_name;
	
	// Skip path strings.
	// E.g. "/home/jtmos/file.txt" will be "file.txt".
	len = strlen(path);
	cptr_name = path+len;
	for(i=len; i>=0; i--,cptr_name--)
	{
		if(*cptr_name=='/') { cptr_name++; break; }
	}

	//
	for(i=0; i<devdb->n_devices; i++)
	{
		// Skip free entries.
		if(devdb->device[i].isfree)
			continue;

		//
		if( !strcmp(devdb->device[i].device_name,cptr_name) )
		{
			// Is alias?
			if(devdb->device[i].alias)
				return devdb->device[i].alias;
			else
				return i;
		}
	}

	//
	return -1;
}

// Added drive aliasing support.
DEVICE_ENTRY *driver_getdevicebyname(const char *name)
{
	int i,id;
 
	//
	for(i=0; i<devdb->n_devices; i++)
	{
		// Skip free entries.
		if(devdb->device[i].isfree)
			continue;

		//
		if( !strcmp(devdb->device[i].device_name,name) )
		{
			// Is it alias?
			if(!devdb->device[i].alias)
		 		return &devdb->device[i];
				else
				{
					// Return alias reference
					id = devdb->device[i].alias;
					return &devdb->device[id];
				}
		}
	}
 
	//
	return NULL;
}

// Returns NULL if device_nr is invalid,
// otherwise returns a string pointer to the name matching
// the device_nr given as argument for the function.
// Note: the actual string is being copied to the parameter #2
char *driver_getdevicename(int device_nr, char *str)
{
	int i;

	//
	if(device_nr<0 || device_nr>=devdb->n_devices || devdb->device[device_nr].isfree)
	{
		if(str!=NULL)
			strcpy(str,"");
		return NULL;
	}

	//
	if(str!=NULL)
		strcpy(str,devdb->device[device_nr].device_name);

	//
	return devdb->device[device_nr].device_name;
}

void driver_disableDAPIReadCache(int dnr)
{
	//
	if(!isValidDevice(dnr))
			return;

	//
	devdb->device[dnr].DAPIReadCache=0;
}

//
void driver_enableDAPIReadCache(int dnr)
{
	//
	if(!isValidDevice(dnr))
			return;

	//
	devdb->device[dnr].DAPIReadCache=1;
}


