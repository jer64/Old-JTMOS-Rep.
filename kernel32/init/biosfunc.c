//
#include "kernel32.h"
#include "biosfunc.h"

// Default to A:.
static int boot_drive=0;

//
int GetBiosBootDrive(void)
{
	return boot_drive;
}

//
void SetBiosBootDrive(int drv)
{
	boot_drive = drv;
}

//







