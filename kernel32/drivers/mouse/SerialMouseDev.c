// ===============================================
// SerialMouse.c
// Serial Mouse Driver
// (C) 2002 by Jari Tuominen
//
// From now on SLIP will use COM2,
// serial mouse will use COM1 as default.
// ===============================================
#include "kernel32.h"
#include "driver_sys.h"
#include "ps2mouse.h" // ps2mouse structure
#include "SerialMouse.h"
#include "SerialMouseDev.h"
#include "keropt.h" // SERIAL_MOUSE_COM

int SerialMouseInit(void)
{
	printk("Serial Mouse Driver 1.0\n");
	SerialMouse_register_device();
	//SerialMouse_chooseCom(SERIAL_MOUSE_COM);
	SerialMouse_setupirqs();
}

int SerialMouse_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("serialmouse") )
	{
		printk("?? serialmouse: device already registered");
		return 1;
	}

	// Register the device
	driver_register_new_device("serialmouse",
		SerialMouse_device_call, DEVICE_TYPE_CHAR);
	return 0;
}

// check driver_api.h.
//
int SerialMouse_device_call(DEVICE_CALL_PARS)
{
	//
	switch(n_call)
	{
		case DEVICE_CMD_INIT:
		break;
		default:
		break;
	}
}
