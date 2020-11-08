/*** Gravis Ultrasound support for JTMOS operating system ***
 * (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
 *
 * Gravis had one of the most nicest documentations,
 * big thanks to Gravis.
 *
 * This driver does not work at the moment, avoid using it.
 */

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
// ---- #include "fileapi.h"
#include "ramdisk.h"
#include "audio.h" // aud
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "gus.h"

// GUS properties
DWORD GUSBASE=0x220,GUSMEM;
int nr_voices=24;
int gusIRQ=11;
static loopLength=1024*32;
static safeBoundary=1024*32;
// Thread structure
THREAD gusThread;

// Borrowed from Linux 2.4.x.
// * needed by frequency setter function
static int      freq_div_table[] =
{
	44100,			/* 14 */
	41160,			/* 15 */
	38587,			/* 16 */
	36317,			/* 17 */
	34300,			/* 18 */
	32494,			/* 19 */
	30870,			/* 20 */
	29400,			/* 21 */
	28063,			/* 22 */
	26843,			/* 23 */
	25725,			/* 24 */
	24696,			/* 25 */
	23746,			/* 26 */
	22866,			/* 27 */
	22050,			/* 28 */
	21289,			/* 29 */
	20580,			/* 30 */
	19916,			/* 31 */
	19293			/* 32 */
};

// Get more audio data from FIFO
void gusGetAudio(void)
{
	int i,d1;

	// Copy the data
	for(i=0; i<loopLength; i++)
	{
		if( (d1 = GetBuffer(&aud))==-1 )
		{
			// Output default wafeform if no data available in buffer
			d1 = 0x80;
		}
		gus_poke(i, d1);
	}
}

// Play the current buffered audio chunk
void gusPlayAudio(void)
{
	//
	gus_setvolume(1, 0xFFFF);      // 0-0xFFFF
//	gus_setbalance(1, 7);  // 0-15(7 = MIDDLE)
	gus_setfreq(1, audio.frequency); // 11025,22050,44100
	gus_playvoice(1, 0 | 8, 0, 0, loopLength+safeBoundary);   // |8 = loop
}

// Initialize all memory we might use
void gusMemoryInit(void)
{
	int i,l;

	//
	l = 1024*256;
	for(i=0; i<l; i++)
		gus_poke(i, 0x80);
}

// Get more audio playing
void gusSendMoreAudio(void)
{
	// Get data
	gusGetAudio();
	// Play it
	gusPlayAudio();
}

//
void gusStartLoop(void)
{
	loopLength = 1024*32;
	audio.length = loopLength;
	audio.frequency = 11025;
	gusSendMoreAudio();
}

//
void gusProcess(void)
{
	int pos;

	//
	while(1)
	{
		// Get voice position
		pos = gus_voicepos(1) & 0xFFFFF;

		// Report position
		printk("GUS position=0x%x        \r",
			pos);

		// Send more audio when needed
		if(pos>=audio.length)
		{
			//
			printk("%s: >> Sending more audio data\n", __FUNCTION__);
			// Send a new chunk
			gusSendMoreAudio();
		}

		// Schedule
		SwitchToThread();
	}
}

// Create GUS thread
void init_gusThread(void)
{
	//
	ThreadCreateStack(&gusThread, 1024*16);
	gusThread.pid =
		CreateThreadA(gusThread.stack,
			gusThread.l_stack, gusProcess);

	//
	SetThreadTerminal(gusThread.pid, 6);

	// max. 31 characters
	IdentifyThread(gusThread.pid, "KGUS");
}


// Gravis Ultrasound interrupt handler
void gus_irqhandler(void)
{
	BYTE *p;
	int i;

	//
	p=0xB8000;
	for(i=0; i<(80*25*2); i++)
		p[i]++;

	//
	outportb(0x20, 0x20); // master PIC
	outportb(0xA0, 0xA0); // slave PIC
}

//
// Note: to see what does 'DEVICE_CALL_PARS' mean,
// check driver_api.h.
//
int gus_device_call(DEVICE_CALL_PARS)
{
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
		size[0]=1;
		break;
		// return 0, it's not a block device
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_PUTCH:
		size=po1;
		size[0]=0;
		break;
		//
		case DEVICE_CMD_TELL:
		size=po1;
		size[0]=0;
		break;
		//
		default:
		break;
	}

	//
	return 0;
}

// Registers device
int gus_register_device(void)
{
	// Check that the device isn't already registered
	if( driver_getdevicebyname("gus") )
	{
		printk("cannot register device gus: device already registered");
		return 1;
	}

	// Register the device
	driver_register_new_device("gus", gus_device_call,DEVICE_TYPE_CHAR);

	//
	return 0;
}

//
void gus_delay(void)
{
	int i;

	//
	for(i=0; i<100; i++)
		inportb(0x300);
}

//
int gus_peek(int loc)
{
	//
	outportb(GUSBASE+0x103, 0x43);
	outportw(GUSBASE+0x104, loc & 0xffff);
	outportb(GUSBASE+0x103, 0x44);
	outportb(GUSBASE+0x105, (loc>>16) & 0xff);

	//
	return inportb(GUSBASE+0x107);
}

//
void gus_poke(int loc,BYTE b)
{
	//
	outportb(GUSBASE+0x103, 0x43);
	outportw(GUSBASE+0x104, loc & 0xffff);
	outportb(GUSBASE+0x103, 0x44);
	outportb(GUSBASE+0x105, (loc>>16) & 0xff);

	//
	outportb(GUSBASE+0x107, b);
}

// From Linux 2.4.x III
void gus_write16(int reg, unsigned int data)
{
	outb((reg), u_Command);

	outb(((unsigned char) (data & 0xff)), u_DataLo);
	outb(((unsigned char) ((data >> 8) & 0xff)), u_DataHi);
}

// From Linux 2.4.x I
void gus_write8(int reg, unsigned int data)
{				/* Writes to an indirect register (8 bit) */
	//
	outb((reg), u_Command);
	outb(((unsigned char) (data & 0xff)), u_DataHi);
}

// From Linux 2.4.x II
void gus_setfreq(BYTE voice, WORD freq)
{
	unsigned int   divisor = freq_div_table[nr_voices - 14];
	unsigned short  fc;

	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);

// Always in non-iw mode.
	/* Interwave plays at 44100 Hz with any number of voices */
/*	if (iw_mode)
	fc = (unsigned short) (((freq << 9) + (44100 >> 1)) / 44100);
	else*/
	fc = (unsigned short) (((freq << 9) + (divisor >> 1)) / divisor);
	fc = fc << 1;
	//
	gus_write16(0x01, fc);
}

//
void gus_setvolume(BYTE voice,WORD volume)
{
	//
	gus_write8(0x0d, 0x03);	/* Stop ramp before setting volume */

	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 9);
	outportw(GUSBASE+0x104, volume);
}

//
void gus_setbalance(BYTE voice,BYTE balance)
{
	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 0x0c);
	outportb(GUSBASE+0x105, balance);
}

/*
//
void gus_setfreq(BYTE voice,WORD frequency)
{
	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 0x01);
	outportw(GUSBASE+0x104, frequency);
}
*/

//
DWORD gus_voicepos(BYTE voice)
{
	WORD temp0,temp1;

	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 0x8a);
	temp0=inportw(GUSBASE+0x104);
	outportb(GUSBASE+0x103, 0x8b);
	temp1=inportw(GUSBASE+0x104);
	return (temp0 << 7) | (temp1 >> 8);
}

//
void gus_stopvoice(BYTE voice)
{
	BYTE temp;

	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 0x80);
	temp=inportb(GUSBASE+0x105);
	outportb(GUSBASE+0x103,0);
	outportb(GUSBASE+0x105,(temp&0xdf) | 3);
	gus_delay();
	outportb(GUSBASE+0x103,0);
	outportb(GUSBASE+0x105,(temp&0xdf) | 3);
}

//
void gus_playvoice(BYTE voice, BYTE mode, int vbegin, int vstart, int vend)
{
	//
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x102, voice);
	outportb(GUSBASE+0x103, 0x0a);
	outportw(GUSBASE+0x104, vbegin >> 7);
	outportb(GUSBASE+0x103, 0x0b);
	outportw(GUSBASE+0x104, vbegin << 9);
	outportb(GUSBASE+0x103, 0x02);
	outportw(GUSBASE+0x104, vstart >> 7);
	outportb(GUSBASE+0x103, 0x03);
	outportw(GUSBASE+0x104, vstart << 9);
	outportb(GUSBASE+0x103, 0x04);
	outportw(GUSBASE+0x104, vend >> 7);
	outportb(GUSBASE+0x103, 0x05);
	outportw(GUSBASE+0x104, vend << 9);
	outportb(GUSBASE+0x103, 0x00);
	outportw(GUSBASE+0x105, mode & 0xfc);

	//
	outportb(GUSBASE,		0x01);
	outportb(GUSBASE+0x103,	0x4c);
	outportb(GUSBASE+0x105,	0x03);
}

// gus_detect
//
// Return value explanation:
// Non-zero =	GUS DETECTED
// Zero =	NO GUS DETECTED
//
int gus_detect(void)
{
	BYTE b;

	// Turn off memory writes
	outportb(GUSBASE+0x103, 0x4c);
	outportb(GUSBASE+0x105, 0x00);
	gus_delay();
	gus_delay();
	gus_delay();
	// Turn on memory writes
	outportb(GUSBASE+0x103, 0x4c);
	outportb(GUSBASE+0x105, 0x01);
	// Write something
	gus_poke(0,0xaa);
	gus_poke(0x100,0x55);

	// Check if it the write worked?
	b=gus_peek(0);
	if(b==0xaa)
	{
		// Detect memory amount
		GUSMEM = gus_findmem();

		// GUS detected!
		return 1;
	}

	// No GUS detected
	return 0;
}

//
void gus_reset(void)
{
	int i,mix_image;

	// Turn off memory writes(Reset GF1)
	outportb(GUSBASE+0x103, 0x4c);
	outportb(GUSBASE+0x105, 0);
	for(i=0; i<100; i++)gus_delay();
	// Turn on memory writes(Release reset)
	outportb(GUSBASE+0x103, 0x4c);
	outportb(GUSBASE+0x105, 1);
	gus_delay();

	// DMA control
	outportb(GUSBASE+0x103, 0x41);
	outportb(GUSBASE+0x105, 0x00);
	// Timer control
	outportb(GUSBASE+0x103, 0x45);
	outportb(GUSBASE+0x105, 0x00);
	// Sample control
	outportb(GUSBASE+0x103, 0x49);
	outportb(GUSBASE+0x105, 0x00);

	// (C)
	// Set max. amount of channels
	outportb(GUSBASE+0x103, 0x0e);
	outportb(GUSBASE+0x105, (nr_voices | 0xc0)); // how many channels(14), Linux11 had 24.

	// (u_Status) Touch the status register(Linux 2.4.x)
	inportb(GUSBASE+6);

	// (D)
	// DMA control
	outportb(GUSBASE+0x103, 0x41);
	inportb(GUSBASE+0x105);
	// Timer control
	outportb(GUSBASE+0x103, 0x49);
	inportb(GUSBASE+0x105);
	// Sample control
	outportb(GUSBASE+0x103, 0x8f);
	inportb(GUSBASE+0x105);

	//
	gus_delay(); gus_delay(); gus_delay(); gus_delay();

	// (E)
	// DMA control
	outportb(GUSBASE+0x103, 0x41);
	inportb(GUSBASE+0x105);
	// Timer control
	outportb(GUSBASE+0x103, 0x49);
	inportb(GUSBASE+0x105);
	// Sample control
	outportb(GUSBASE+0x103, 0x8f);
	inportb(GUSBASE+0x105);

	// 0x01 to 0x4c: reset
	// 0x07 to 0x4c: Master reset | DAC enable | IRQ enable
	outportb(GUSBASE+0x103, 0x4c);
	outportb(GUSBASE+0x105, 0x07);

	// -------------------------------------
	// Set up for Digital ASIC
	//
	outportb(GUSBASE+0x0f, 0x05);

	//
	mix_image = 0xFF;
	mix_image &= ~0x02;	/* Enable line out */
	mix_image |= 0x08;	/* Enable IRQ */
	//
	mix_image |= 0x02;	/* Disable line out (for a moment) */
	outportb(GUSBASE+0x0, mix_image);

	// IRQ DMA control
	outportb(GUSBASE+0xb, 0x00);
	// 
	outportb(GUSBASE+0xf, 0x00);
	// End of Digital ASIC setup
	// ----------------------------------------

	// --------------------------------------------
	// Interwave only(e.g. GUS PnP)
	// outportb(GUSBASE+0x103, 0x19);
	// outportb(GUSBASE+0x105, inportb(GUSBASE+0x105) | 0x01);
	// --------------------------------------------
	//
}

// Tests a memory location.
// Returns zero if memory location is not existant,
// non-zero if memory location exists.
int gus_testloc(DWORD ad)
{
	//
	gus_poke(ad,0x55);
	if(gus_peek(ad)!=0x55)return 0;
	gus_poke(ad,0xaa);
	if(gus_peek(ad)!=0xaa)return 0;
	gus_poke(ad,0xff);
	if(gus_peek(ad)!=0xff)return 0;
	gus_poke(ad,0x00);
	if(gus_peek(ad)!=0x00)return 0;
	return 1;
}

// gus_findmem
//
// Finds out how much memory there is on the GUS card.
// Returns amount of ram available in bytes.
DWORD gus_findmem(void)
{
	int ad,l;

	// Search atmost 16M of ram.
	l=1024*1024*16;

	// Write each of memory location,
	// with 1024 bytes step.
	for(ad=0; ad<l; ad+=1024)
	{
		if( !gus_testloc(ad) )break;
	}

	//
	return ad;
}

// Setup IRQ handler
void gus_setupirq(void)
{
	// Setup interrupt handler #11
	setint(S_VEC+3, // 8+3 = IRQ #11
		gus_ISR,
		GIMMIE_CS(),
		D_PRESENT + D_INT + D_DPL3);
	enable_irq(11);

	// Setup interrupt handler #11
	setint(M_VEC+7, // 7 = IRQ #7
		gus_ISR,
		GIMMIE_CS(),
		D_PRESENT + D_INT + D_DPL3);
	enable_irq(7);
}

// Detect & initialize Gravis Ultrasound
int gus_init(void)
{
	int i;

	//
	printk("Gravis Ultrasound driver 1.0 by Jari Tuominen\n");
	// Search through all valid addresses
	for(i=1; i<9; i++)
	{
		// Define GUS base
		GUSBASE = (i*0x10+0x200);

		// Detect GUS
		if(gus_detect())
		{
			// Report
			printk("gus.c: Found GUS at 0x%x, %dK RAM detected.\n", GUSBASE, GUSMEM/1024);
			// Reset GUS
			gus_reset();
			// Init IRQ handlers
		//	gus_setupirq();
			// Initialize memory
			gusMemoryInit();
			// Start audio loop
			gusStartLoop();
			// Lastly start the process
			// (no GUS thread)
		//	init_gusThread();
			// Register device
			gus_register_device();
			return 1;
		}
	}

	//
	return 0;
}

//

