//==================================================================================
// Device driver programming interface.
// **** OBSOLETE !!!!!!! Don't use... ****
//==================================================================================
#include <stdio.h>
#include "dpi.h"
#include "dcpacket.h"

// Devica call
int DeviceCall(int dnr,
	int n_function,
	int v1,int v2,int v3,int v4,
	void *p1,void *p2)
{
	DCPACKET p;
	int rv;

	//
	if(n_function<=0)
	{
		//
		printf("%s: n_function <= 0 * INVALID FUNCTION\n",
			__FUNCTION__);
		abort();
	}

	//
	p.n_function = n_function;
	p.par1 = v1;
	p.par2 = v2;
	p.par3 = v3;
	p.par4 = v4;
	p.p1 = p1;
	p.p2 = p2;

	//
	rv = scall(SYS_devicecall, dnr,&p,0, 0,0,0);

	//
	return rv;
}

// Register a new device
int registerNewDevice(const char *name,
	void *call,int cs,
	int pid,int type,
	int alias)
{
	//
	return scall(SYS_registernewdevice,
		name,call,cs, pid,type,alias);
}

//
int getdevicename(int dnr, char *name)
{
	//
        return scall(SYS_getdevicename, dnr,name,0, 0,0,0);
}

//
int getdevicenrbyname(const char *name)
{
	//
        return scall(SYS_getdevicenrbyname, name,0,0, 0,0,0);
}

//
int getcurdev(void)
{
	//
        return scall(SYS_GETCURDEV, 0,0,0, 0,0,0);
}


//


