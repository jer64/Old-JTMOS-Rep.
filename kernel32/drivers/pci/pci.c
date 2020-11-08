//----------------------------------------------------------
//
// 		   JTMOS PCI BUS SUPPORT
//
// - PCI device registration as "pci"
// - PCI bus type detection
// - PCI device detection
//
// Requires inportd && outportd (must be implemented in io).
//
// (C)2001-2003 by Jari Tuominen(jari@vunet.org)
//
//----------------------------------------------------------
//
#include "kernel32.h"
#include "sysmem.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
//
#include "pci.h"

//
PCIBRIDGE pci;



//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int pci_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;

	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;
		//
		case DEVICE_CMD_READBLOCK:
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		break;
		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size=1;
		break;
		// return 0, it's not a block device
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size=0;
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		size=po1;
		*size=0;
		break;
		//
		case DEVICE_CMD_PUTCH:
		size=po1;
		*size=0;
		break;
		//
		case DEVICE_CMD_TELL:
		size=po1;
		*size=0;
		break;
		//
		default:
		break;
	}

	//
	return 0;
}

// Registers device
int pci_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("pci") )
	{
		print("cannot register device pci: device already registered");
		return 1;
	}

	// Register the device
	driver_register_new_device("pci",pci_device_call,DEVICE_TYPE_CHAR);

	//
	return 0;
}

//
// PCI device init
//
//
void pci_init(void)
{
	// As default no PCI bridge is detected
	pci.type=0;

	// Brobe for PCI bus type and devices
	pci_probe();

	// If PCI bus detected
	if(pci.type)
	{
		// Register "pci" device
		pci_register_device();
	}
	else
	{
		//
		printk("No PCI bus found.\n");
	}

	//
}

//
// pci_probe function
//
// - Probes for PCI devices
// - detects PCI type
//
// 2 PCI bridge types supported currently
//
void pci_probe(void)
{
	DWORD tmp,a1,a2;
	int i,i2,i3,i4;

	//
	printk("Probing for PCI bus devices: ");

	// 1. PROBE FOR PCI BUS TYPE
	// Determines in which way we shall
	// communicate with it and does it
	// actually exist.
	//
	outportb(0xcf8,0);
	outportb(0xcfa,0);

	//
	if(!inportb(0xcf8) && !inportb(0xcfa))
	{
		pci.type=2;
	}
	else
	{
		//
		tmp=inportd(0xcf8);
		outportd(0xcf8,0x80000000);
		if(inportb(0xcf8==0x80000000))
			pci.type=1;
		outportb(0xcf8,tmp);
	}

	//
	if(pci.type)printk("PCI bus type %d\n",pci.type);

	// 2. PROBE FOR CARDS
	// If PCI bridge found,
	// probe for PCI cards.
	// There is two codes,
	// and two PCI bridge types.
	//
	switch(pci.type)
	{
		// FOR PCI BRIDGE TYPE #1
		case 1:
		for(i=0; i<512; i++)
		{
			outportd(0xcf8,0x80000000+(i*2048));
			tmp=inportd(0xcfc);
			//
			a1=tmp&0xffff; a2=(tmp>>16)&0xffff;
			// Data is not 0xffffffff or 0 => Device entry.
			if( a1!=0xffff &&
				a2!=0xffff )
			{
				// if both values are not zeroes
				if( !(!a1 && !a2) )
				printk("PCI Device: Vendor: %x  Device: %x\n", a1,a2);
			}
		}
		break;

		// FOR PCI BRIDGE TYPE #2
		case 2:
		// Bus select?
		outportb(0xcf8,0x80);
		outportb(0xcfa,0x00);
		//
		for(i=0; i<16; i++)
		{
			tmp=inportd(i*256 + 0xc000);
			//
			a1=tmp&0xffff; a2=(tmp>>16)&0xffff;
			//
			if( a1=0xffff && a2!=0xffff )
			{
				// if both vlaues are not zeroes
				if( !(!a1 && !a2) )
				{
					printk("PCI Device: Vendor: %x  Device: %x\n",
						a1,a2);
				}
			}
		}
		//
		outportb(0xcf8,0);
		break;

		// For no PCI bridge
		default:
		break;
	}

	//
}


