// mount.c
#include "kernel32.h"
#include "jtmfs.h"
#include "root.h"
#include "mount.h"

//
int mountDNR(int dnr)
{
	//
        driver_mountDNR(dnr);
        DiskChange(dnr);
        return 0;
}

//
int unmountDNR(int dnr)
{
	//
	DiskChange(dnr);
        driver_unmountDNR(dnr);
        return 0;
}
