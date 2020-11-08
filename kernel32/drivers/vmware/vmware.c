//
#include "kernel32.h"
#include "vmware.h"

// Running under VMware or not?
int vmware=FALSE;
//
static char *id_string = "VMware";

//
int InitVmwareDriver(void)
{
	char *p;
	int i,i2,l;

	//
	vmware = FALSE;

	//
	p = 0xF0000;

	//
	l = strlen(id_string);

	//
	for(i=0,i2=0; i<256; i++)
	{
		if( p[i]==id_string[i2] )
		{
			i2++;
			if(i2==l)
			{
				vmware = TRUE;
				break;
			}
		}
		else
		{
			i2=0;
		}
	}

	//
	return vmware;
}

