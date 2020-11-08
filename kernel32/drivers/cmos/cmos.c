/****************************************************************
 *								*
 * cmos.c - CMOS driver						*
 * (C)2002 by Jari Tuominen(jari@vunet.org           *
 *								*
 ****************************************************************/
//
#include "cmos.h"
#include "cmos_dev.h"
#include "basdef.h"
#include "rtc.h"

// Reads a CMOS register
int cmos_read(int addr)
{
	//
	outportb(0x70|(addr&63),0x70);
	return inportb(0x71);
}

//
BYTE cmos_batterycheck(void)
{
	//
	if( cmos_read(0x0d)&0x80 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//
void cmos_init(void)
{
	//
	print("cmos battery state: ");
	if( cmos_batterycheck()==TRUE )
	{
		//
		print("CMOS RAM has power, good\n");
	}
	else
	{
		//
		print("CMOS RAM has lost power\n");
	}

	//
	cmos_register_device();
}

//

