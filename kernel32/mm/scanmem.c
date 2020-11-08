///////////////////////////////////////////////////////////////////////////////////
// scanmem.c
// (C) 2004-2005 Jari Tuominen.
///////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "scanmem.h"

//////////////////////////////////////////////////////////////////////////////////////////
MEMORY_INFORMATION ms;

//////////////////////////////////////////////////////////////////////////////////////////
// Returns amount of total memory space in bytes.
//
int scanmem1(void)
{
	DWORD *mem;
	DWORD mem_count, a;
	DWORD memkb;
	BYTE irq1, irq2;
	DWORD cr0;
	DWORD *ptr;
	char str[256];
 
	// save IRQ's
	irq1=inportb(0x21);
	irq2=inportb(0xA1);
 
	// kill all irq's
	outportb(0x21, 0xFF);
	outportb(0xA1, 0xFF);
 
	//
	memkb=0;

	//
	for(mem=0x100000; ; memkb+=64,mem+=(1024*64>>2))
	{
		//
		a = *mem;
		*mem=0x55AA55AA;

		//
		asm("":::"memory");
		//
		if(*mem!=0x55AA55AA)
			break;
		else
		{
			*mem=0xAA55AA55;
			asm("":::"memory");
			if(*mem!=0xAA55AA55)
				break;
		}

		//
		asm("":::"memory");
		*mem=a;
	}while(memkb<4096);

	//
	outportb(0x21, irq1);
	outportb(0xA1, irq2);

	// Require atleast 16M of RAM.
	if(memkb<16384)
	{
		sprintf(str, "Insufficient amount of RAM detected (%dK).", memkb);
		disable();
		panic(str);
	}

	// Experimental wipe.
//	memset(0x600000, 0xAA, (memkb*1024)-0x600000);

	//
	return memkb*1024;
}

//////////////////////////////////////////////////////////////////////////////////////////
int scanmem(void)
{
	//
	int i;

	//
	i=scanmem1();
	ms.amount=i;
 
	//
	pd32(i/(1024*1024));
	print("M ram\n");
	return i;
}

//////////////////////////////////////////////////////////////////////////////////////////
int GetRamAmount(void)
{
	//
	return ms.amount;
}

//

