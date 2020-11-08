/************************************************************
 *                                                          *
 * cmos.c - CMOS driver                                     *
 * (C)2002 by Jari Tuominen(jari@vunet.org       *
 *                                                          *
 ************************************************************/
//
#include "kernel32.h"
#include "cmos.h"
#include "cmos_dev.h"
#include "basdef.h"
#include "rtc.h"

//
int cmos_device_call(DEVICE_CALL_PARS)
{
	return 0;
}

// Registers device
int cmos_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("cmos") )
	{
		printk("cmos: Device reg. failed");
		return 1;
	}

	// Register the device
	driver_register_new_device("cmos",
		cmos_device_call, DEVICE_TYPE_CHAR);

	//
	return 0;
}

