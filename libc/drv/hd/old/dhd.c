// ===================================================================
// IDE Hard Disk Driver
// (C) 2001,2002 by Jari Tuominen
// This is the IDE Hard Disk driver for The JTMOS Operating System.
// - Includes buffering mechanism(buffers entire cylinders at once)
// - Reads entire cylinders at once, this is a standard method
// - Verify on write and read -operations
// - Errors are being handled
// - IDE HD parameter autodetection
// ===================================================================
#include "int.h"
#include "cpu.h" // nop()
#include "sysglvar.h"
#include "kernel32.h"
#include "delay.h"
#include "io.h"
#include "dma.h"
#include "dhd.h"
#include "devsys.h"
#include "dhd_detect.h"
#include "dhd_dev.h"
#include "dhd_func.h"
#include "hdparam.h"
#include "dhd_sectorRW.h"

// Enable IRQs for IDE
//#define USE_IDE_INTERRUPTS

// Enable sanity check at dhd_init
//#define HD_SANITY_CHECK

// 1=ENABLED, 0=DISABLED
#define ENABLE_BUFFERING	0
// State variable
int HD_BUFFERING_ENABLED;

//
static int dhd_action_counter=0;
// Current IDE HD in use
HDSPEC *CurrentHD;
// HD specifications for sereval hds
HDSPEC hdspec[10];
// Number of hdspecs registered
int n_hdspecs;
// Used as an identifier whether has an IRQ
// occured for specified IDE device #.
int ideirqtick[10];

//
GENHD genhd;

int dhd_TellStatus(HDSPEC *h)
{
	char numbers[]={ 1,2,4,8,16,32,64,128 };
	char *list[]={	"Error", "INDEX",
			"ECC",	"DRQ",
			"Seek Complete", "Write Error",
			"Ready",
			"Busy"};
	int i,i2,i3,i4,stat;

	HDDEBUG

	stat = dhd_GetStatus();

	printk("Status = 0x%x  ", stat);
	for(i=0; i<8; i++)
	{
		if( stat&numbers[i] )
			printk("%s; ", list[i]);
	}
	printk("\n");

	// If error
	if(stat==0xD0)
	{
		sleep(1);
		stat = dhd_GetStatus();
		if(stat==0xD0)
			dhd_resetHD();
	}

	// If noticed that no drive exists do tell so:
	if(stat==0) return 1;
	return 0;
}

void dhd_command(int which)
{
	outportb(0x1f7, which);
}

/*
  3F6h  -  bits 7-4:  Reservados                    3F7h  -  bit 7:  Línea de cambio de disquetes.
           bit 3:     0 - Reduce Write Current)              bit 6:  Write gate
                      1 - Head 3 select enable)              bit 5:  Head select 3 / Reduced Write Current
           bit 2:     1 - Disk reset enable                  bit 4:  Head select 2
                      0 - Disk reset disable                 bit 3:  Head select 1
           bit 1:     0 - Disk initialization enable         bit 2:  Head select 0
                      1 - Disk initialization disable        bit 1:  Drive select 1
           bit 0:     Reservado                              bit 0:  Drive select 0
*/

//
void dhd_resetHD1(void)
{
	int i,irq;

#ifdef USE_IDE_INTERRUPTS
	irq=HD_ENABLE_IRQS;
#else
	irq=0;
#endif

	HDDEBUG
	// Trigger and release reset -line
	outportb(0x3F6, HD_RESET_BIT|irq); for(i=0; i<10000; i++) { } // |2 for IRQs
	outportb(0x3F6, 0|irq); for(i=0; i<10000; i++) { }
}

// Does following things:
// 1) Reset & init line
// 2) Init line
// 3) Wait completion
void dhd_resetHD(HDSPEC *h)
{
	HDDEBUG
	dhd_resetHD1();
	dhd_waituntilready(h);
}

void dhd_recalibrate(HDSPEC *h)
{
	HDDEBUG
#ifdef ALLOW_RECALIBRATION=YES
	dhd_chooseDrive(h->slave);
	dhd_resetHD(h);
	dhd_waituntilready(h);
	dhd_command(0x10);
	dhd_waituntilready(h);
#endif
}

// Choose drive
int dhd_chooseDrive(int which)
{
	outportb(0x1F6, (inportb(0x1F6)&(0xFF-0x10)) | which<<4);
}

int dhd_SetFeature(HDSPEC *h, int which, int what)
{
	HDDEBUG

	dhd_waituntilready(h);

	dhd_chooseDrive(h->slave);
	outportb(0x1F1, which);
	outportb(0x1F3, what);
	dhd_command(0xEF);
	dhd_waituntilready(h);
}

void dhd_irqhandler14(void)
{   
	// Yes, IRQ been there:
	ideirqtick[0]=1;

	//
	// printk("dhd_irqhandler14: IRQ #14, IDE interrupt event\n");

	// Mark that we have been here
	outportb(0x20,0x20);
	outportb(0xA0,0x20); 
}

void dhd_irqhandler15(void)
{
	// IRQ been there:
	ideirqtick[1]=1;

	//
	// printk("dhd_irqhandler15: IRQ #15, IDE interrupt event\n");

	//
	outportb(0x20,0x20);
	outportb(0xA0,0x20);
}

void dhd_setuphdirqs(void)
{
	// irq14 (ide0)
	setint(S_VEC+(14-8),dhd_asmirqhandler14,SEG_CODE32,0x8e00);
	// irq15 (ide1)
	setint(S_VEC+(15-8),dhd_asmirqhandler15,SEG_CODE32,0x8e00);

	// Enable IRQs 14 and 15 in PIC #2
	outb_p(inb_p(0x21)&0xfb,0x21);
	outb(inb_p(0xA1)&0xbf,0xA1);
}

// Make a small acceptable size drive simulation
void TrimHD(HDSPEC *h)
{
	HDDEBUG

	//
//	h->cylinders=30;
//	return;

	//
	printk("Trimming HD, if this takes more than 4 seconds then reboot computer ...\n");
	while(     (h->sectors*h->cylinders*h->heads)
				>
			(HD_MAX_SIZE_ALLOWED/512)     )
	{
		h->cylinders--;
	}
	//
	RecalculateTotalBlocks(h);
}

void RecalculateTotalBlocks(HDSPEC *h)
{
	h->n_blocks = h->cylinders*h->heads*h->sectors;
}

int dhd_detectparams(HDSPEC *h)
{
	int i,i2,i3,i4;

	// Get data
	memset(&h->hp, 0, sizeof(HDPARAM));
	GetHDParam(h, h->slave, &h->hp);
	// Collect data, precalculate
	h->cylinders =	h->hp.cylinders;
	h->heads =	h->hp.heads;
	h->sectors =	h->hp.sectors;

#ifdef	LIMIT_CYLINDERS
	// Size limitation (Optional!)
	h->cylinders = LIMIT_CYLINDERS;
#endif
	RecalculateTotalBlocks(h);

	//
	if(h->hp.cylinders)
	{
		for(i=0; i<40; i++) {
			h->ModelNumber[i]=h->hp.ModelNumber[i];
		}
		h->ModelNumber[i]=0;

		for(i=0; i<8; i++) {
			h->FirmwareRev[i]=h->hp.FirmwareRev[i];
		}
		h->FirmwareRev[i]=0;

		for(i=0; i<20; i++) {
			h->SerialNumber[i]=h->hp.SerialNumber[i];
		}
		h->SerialNumber[i]=0;

		printk("HD parameters: ");
		printk("Cylinders=%d, Heads=%d, Sectors=%d\n",
			h->cylinders, h->heads, h->sectors);
		printk("HD MODEL   : %s\n", h->ModelNumber);
		printk("HD SERIAL  : %s  ", h->SerialNumber);
		printk("HD FIRMREV : %s\n", h->FirmwareRev);
	}
	else
	{
		printk("Warning: No HDPARAM could be received.\n");
		return 1;
	}

#ifdef LIMIT_HD_SIZE
	// Safeguard:
	if(	(h->sectors*h->cylinders*h->heads)
			>
		(HD_MAX_SIZE_ALLOWED/512)	)
	{
		printk("--- Warning -----------------------------------------\n");
		printk("System has detected too big hard disk.\n");
		printk("Small IDE hard disks are supported at the moment.\n");
		printk("This will change in future kernels.\n");
		printk("Simulating a small IDE hard disk ...\n");
		TrimHD(h);
		return 0;
	}
#endif
	return 0;
}

// Probe HD: Master or Slave
int dhd_tryDevice(HDSPEC *h)
{
	int tries;
	int registered;
	char tmp[512];

	h->GiveUpFast=1;
	for(tries=0,registered=0; tries<2; tries++)
	{
		// Try to get params
		if( !GetHDParam(h, h->slave, NULL) )
		{
			// OK drive detected
			dhd_register_device();
			registered=1;
			break;
		}

	/*	// Head = 0
		outportb(0x1F6, 0);
		// Choose drive MASTER or SLAVE
		dhd_chooseDrive(h->slave);
		// Count = 1
		outportb(0x1F2, 1);
		// Sector number = 1
		outportb(0x1F3, 1);
		// Cylinder = 0
		outportb(0x1F4, 0);
		outportb(0x1F5, 0);
		// Read
		dhd_command(0x20);
		// Get status
		if(dhd_GetStatus()!=0)
		{
			registered=1;
			break;
		}*/
/*
		if(!dhd_sectorRW2(h, tmp, 0,1,0, h->slave, READ,1))
		{
			registered=1;
			dhd_register_device();
			break;
		}*/
	}
	h->GiveUpFast=0;
	return registered;
}

// Checks drive functionallity
void dhd_sanityCheck(HDSPEC *h)
{
	//
	int i;

	printk("IDE HD Sanity Check:\n");
	for(i=0; i<h->cylinders; i++)
	{
		printk(".");
		dhd_sectorRW2(h, NULL, 		// HDSPEC, buffer ptr
				0,1,i,		// H/S/C
				h->slave,	// Drive #
				READ,		// READ or WRITE
				h->sectors);	// Amount of sectors
	}
}

// ======================================================
// Master hard disk driver init
// ======================================================
int dhd_init(void)
{
	//
	HDSPEC *h;
	int i,tries,registered;

	// Init structures
	memset(&hdspec[0], 0, sizeof(HDSPEC));
	memset(&hdspec[1], 0, sizeof(HDSPEC));

	// Set state variable
	HD_BUFFERING_ENABLED = ENABLE_BUFFERING;

	// Hard disk temporary buffer
	genhd.tmp = malloc(1024*1024);

	//
	printk("hda: ");

	// Get hang of HD #0
	h = &hdspec[DEFAULT_DRIVE];
	// Assign IDE drive #0 as default
	CurrentHD = h;

	// clear IDE #1 & #2 irq tick variables
	for(i=0; i<2; i++)
		ideirqtick[i] = 0;

	// Here we specify the default hd (we don't detect pars yet)
	h->size=	DHD_DEFAULT_SIZE;
	h->cylinders=	DHD_DEFAULT_CYLINDERS;
	h->heads=	DHD_DEFAULT_HEADS;
	h->sectors=	DHD_DEFAULT_SECTORS;
	h->units=	1; // 1x 512 bytes
	
	// Calculate total amount of blocks
	h->n_blocks = h->cylinders*h->heads*h->sectors;

	// Specify block size(default 512 bytes)
	h->blocksize = 512;

	// Setup interrupt handlers for IRQ14 and IRQ15 (PRIM/MAST IDE)
#ifdef USE_IDE_INTERRUPTS
	dhd_setuphdirqs();
#endif

	// reset hd
	// dhd_command(14); xsleep(1000);
	// dhd_command(4); xsleep(1000);
	// xsleep(2000);

	// First trigger fast reset for all current IDEx devices
	dhd_resetHD1();

	// Register up only if read on #0 block succeeds:
	// Try master and slave.
	for(h->slave=0; h->slave<2; h->slave++)
	{
		if( (registered=dhd_tryDevice(h)) )
		{
			break;
		}
	}

	// If a drive has been detected
	if(registered)
	{
		//
		if(!h->slave)
			printk("Found Hard Disk on IDE0/Master\n");
			else
			printk("Found Hard Disk on IDE0/Slave\n");
		//
		printk("Getting HD params\n");

		// Get drive params
		if( dhd_detectparams(h) )
		{
			// If prompted to give up, do so:
			registered=0;
			return 1;
		}

		//
		printk("buffer init\n");
		// This must be done before reading any sectors.
		if(HD_BUFFERING_ENABLED)
			dhd_bufferInit(h);
#ifdef ALWAYS_DISABLE_WRITE_CACHE
		// Turn on "disable write cache"
		dhd_SetFeature(h, 0x82, 1);
#endif
		// Use only in __special__ cases:
#ifdef HD_SANITY_CHECK
		dhd_sanityCheck(h);
#endif

		// ---------------------------------------------------------------
		// Determine block size from the drive size.
		// We simulate bigger blocks for FAT system.
	
		// For drives bugger than 256 MB:
		if( h->n_blocks > (2* 1024*256) )
		{
			h->units = 32;
		}
		else
		// For drives bugger than 100 MB:
		if( h->n_blocks > (2* 1024*100) )
		{
			h->units = 16;
		}
		else
		// For drives bugger than 20 MB:
		if( h->n_blocks > (2* 1024*20) )
		{
			// 4K block size
			h->units = 8;
		}
		else
			// 2K block size
			h->units = 4;
	
		// Report
		printk("%s: 512 bytes units per block = %d\n",
			__FUNCTION__, h->units);

		//
		// return dhd_initmechanically();
		//
	}
	else
	{
		printk("No Hard Disk detected.\n");
	}

	return 0;
}

//
void dhd_readtest(HDSPEC *h)
{
	int i,i2,bytes,key;
	char *teststr="J0T0M0O0S000102030405060708090A0B0C0D0E0F0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0";
	char *buf;

	//
	buf = malloc(1024*64);
 
	// Init drive, error => don't do the test
	//  if(dhd_init())return; 

	// Test loop, user break out possible
	while(1)
	{
		//
		printk("dhd.c, dhd_test.\n");

		// 2 h->n_blocks
		printk("Read test ...\n");
		for(i=0,bytes=0; i<10000+TESTNRSEC; i++)
		{  
			//
			pd32(i); printk("/"); pd32(h->n_blocks); printk("\r");
			//
			if( dhd_blockrw(h, 0xb8000,i,h->slave,READ) )
				break;

			//
			key=getch1(); if(key==27)break;
			SCHEDULE();
		}
		printk("\nDone.\n");

		//
		if(key==27)break;
		printk("\n");
	}
	printk("\n");

	//
	free(buf);
}

//
void dhd_writetest(HDSPEC *h)
{
	//
	long i,i2,bytes,key;
	char teststr[255];
	char *buf;
	long device_nr;

	//
	if( (device_nr = devsys_getdevicenrbyname("hda"))==-1)return;

	//
	strcpy(teststr,"0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX");

	//
	buf = malloc(1024*64);

	// Init drive, error => don't do the test
	// if(dhd_init())return; 

	// Test loop, user break out possible
	while(1)
	{
		//
		printk("dhd.c, dhd_test.\n");

		// 1
		printk("Write test ...\n");
		for(i=0,bytes=0; i<10000+TESTNRSEC; i++)
		{
			// 
			for(i2=0; i2<512; i2++)buf[i2]=teststr[i2&63]; 

			//
			pd32(i); printk("/"); pd32(h->n_blocks); printk("\r");
			//
			writeblock(device_nr,i,buf);
			//   if( dhd_blockrw(h, buf,i,0,WRITE) )break;

			//
			SCHEDULE();

			//
			key=getch1(); if(key==27)break;
		}

		//
		if(key==27)break;
		printk("\n");
	}
	printk("\n");

	//
	free(buf);
}

//
void dhd_writetest2(HDSPEC *h)
{
	//
	long i,i2,bytes,key;
	char teststr[255];
	char *buf;
	long device_nr,whole_length;

	//
	if( (device_nr = devsys_getdevicenrbyname("hda"))==-1)return;

	//
	whole_length = getsize( devsys_getdevicenrbyname("hda") );

	//
	strcpy(teststr,"0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX0123456789ABCDEFXXXX");

	//
	buf = malloc(1024*64);
 
	// Init drive, error => don't do the test
	// if(dhd_init())return; 

	// Test loop, user break out possible
	while(1)
	{
		//
		printk("dhd.c, dhd_test.\n");

		// 1
		printk("Write test ...\n");
		for(i=0,bytes=0; i<whole_length; i+=((whole_length/1000)+1))
		{
			// 
			for(i2=0; i2<512; i2++)buf[i2]=teststr[i2&63]; 

			//
			pd32(i); printk("/"); pd32(h->n_blocks); printk("\r");
			//
			writeblock(device_nr,i,buf);

			//
			key=getch1(); if(key==27)break;
		}

		//
		printk("Try again(y/n)?");
		if(key==27)break;
		printk("\n");
	}
	printk("\n");

	//
	free(buf);
}

//
void dhd_general_actions(void)
{
	//
	dhd_action_counter++;
	if(dhd_action_counter>=5000)
	{
	/*	dhd_action_counter=0;
		dhd_recalibrate(); */
	}
}

// dhd_blockrw function
//
// returns zero on success
int dhd_blockrw(HDSPEC *h, BYTE *buf,
		unsigned long int blocknr,
		long drivenr,
		long rw)
{
	int sectornr,headnr,cylindernr,retval,i,i2,try,try1,try2;

	//
	if(rw==READ || rw==WRITE)
	{
		// it's okay
	}
	else panic("dhd_blockrw: rw must be either READ or WRITE");

	// ---------------------------------------------------------------------
	// CONVERT LINEAR TO CHS
	sectornr = (blocknr % h->sectors)+1;
	headnr = (blocknr / h->sectors) % h->heads;
	cylindernr = (blocknr / h->sectors / h->heads);

	// ---------------------------------------------------------------------
	if(rw==READ)
	{
		retval=dhd_sectorRW(h,
			buf,headnr,sectornr,cylindernr,drivenr,rw);
	}
	else
	if(rw==WRITE)
	{
		retval=dhd_sectorRW(h,
			buf,headnr,sectornr,cylindernr,drivenr,WRITE);
	}

	return retval;
}

int dhd_errorcheck(HDSPEC *h,
	long rw,long drivenr,long headnr,long sectornr,long cylinder)
{
        //
        if( rw==READ && dhd_isError() )
        {
		printk("dhd.c/dhd_sectorrw: chs %d/%d/%d, READ FAILURE",
	                        cylinder,
	                        headnr,
	                        sectornr);
		sleep(2);
		printk("\n");
		return 2;
        }

        // Write fault when wrote?
        if( rw==WRITE && ((dhd_GetStatus()&32)==32 || dhd_isError()) )
        {
		printk("\n%s: chs %d/%d/%d, WRITE FAILURE!\n",
				__FUNCTION__,
				cylinder,
				headnr,
			sectornr);
		sleep(2);
		printk("\n");
		return 1;
        }
	return 0;
}

// buf, headnr,sectornr,cylindernr,  drivenr, rw, sectorcount
// buf, 0,1,0, 0, 0, 1
//
// Basic rules:
//
// Sectors begin always from 1, unlike cylinder and head.
// Cylinder and head number both begin from 0.
//
// NEW: now reports errors properly
//
int dhd_seek(HDSPEC *h, char *buf,long headnr,long sectornr,
		long cylinder,
		long drivenr, // 0=master,1=slave(I think)
		long rw, // 0=read, 1=write (UNUSED)
		long sectorcount) // (UNUSED)
{
        //
	WORD w;
        long i,lo,hi,stat;
   
	//
	if(!buf || !sectornr)panic("dhd_sectorrw !buf or !sectornr");

	//
	dhd_waituntilready(h);

        // Drive and head port, Drive 0, Head 0
        outportb( 0x01f6,0xa0+(headnr&7)+((drivenr&1)<<4) ); dhd_delay1();
        // Sector count port, read one sector
        outportb(0x01f2,sectorcount); dhd_delay1();
        // Sector number port, read sector one
        outportb(0x01f3,sectornr); dhd_delay1();
        // Cylinder low port, cylinder 0
        outportb(0x01f4,cylinder&0xff); dhd_delay1();
        // Cylinder high port, cylinder 0
        outportb(0x01f5,(cylinder>>8)&0xff); dhd_delay1();
        // Command port, read with retry
	// 0x70 = seek
	outportb( 0x01f7,0x70 );
	dhd_delay1();

	//
	while(1)
	{
	 //
	 dhd_delay1();
	 //
	 if( (dhd_GetStatus()&1) )return 1;
	 // wait until seek is complete
  	 if( (dhd_GetStatus()&16)==16 ) 
	 {
	  break;
	 }

	 //
	}

	//
        printk("error register=0x"); p8(dhd_readerror()); printk(" ");

	//
	return dhd_errorcheck(h,
		rw,drivenr,headnr,sectornr,cylinder);
}
/*
|  3  |  DRQ     | Data Request bit. When set it indicates that the drive   |
|     |          | is ready to transfer a word or byte of data between the  |
|     |          | host and the data port.                                  |
*/

// Single sector R/W operation
int dhd_rw(HDSPEC *h, BYTE *buf, int rw)
{
	//
	WORD w;
        int i,lo,hi,stat,t;

	//
	dhd_waituntilready(h);

	//
/*	printk("%s: h=0x%x, buf=0x%x, rw=%d\n",
		__FUNCTION__,
		h, buf, rw);
	getch();*/

        // Read data from data port,
        // data comes in and out of here
	//
	// Reads/Writes two bytes at once.
        for(i=0; i<h->blocksize; i+=2)
        {
		//
		if(rw==READ)
		{
			w = inportw(0x01F0);
			lo=w&255;
			hi=(w>>8)&255;
			//
			if(buf!=NULL) {
	               		buf[i+0] = lo;
				buf[i+1] = hi;
			}
		}
		else
		if(rw==WRITE)
		{
			if(buf!=NULL)
			{
				outportw( 0x01F0,
					(buf[i+0]&255) + ((buf[i+1]&255)<<8) );
			}
			else	outportw(0x01F0, 0);
		}
		else panic("dhd_rw NOT READ OR WRITE DEFINED!!!");
        }

	//
	dhd_waituntilready(h);

	//
	return 0;
}

/*****
// Single sector R/W operation
void dhd_rw(HDSPEC *h, BYTE *buf, char rw)
{
	//
	WORD w;
        long i,lo,hi,stat;

        dhd_waitcompletion(h);

        // Read data from data port,
        // data comes in and out of here
	//
	// Reads/Writes two bytes at once.
        for(i=0; i < h->blocksize; )
        {
		while( !(dhd_GetStatus()&8) )
		{
			SCHEDULE();
		}

		//
		if(rw==READ)
		{
			w = inportw(0x01F0);
			lo=w&255;
			hi=(w>>8)&255;
			//
               		buf[i] = lo; i++;
			buf[i] = hi; i++;
		}
		else
		if(rw==WRITE)
		{
			outportw( 0x01F0,
				(buf[i]&255) + ((buf[i+1]&255)<<8) );
			i+=2;
		}
        }

        dhd_waitcompletion(h);
}
*********/

/************************************************************************
;       1f0       r/w       data register, the bytes are written/read here
;	1f1	  ?	    IDE_FEATURE_REG
;       1f1       r         error register  (look these values up yourself)
;       1f2       r/w       sector count, how many sectors to read/write
;       1f3       r/w       sector number, the actual sector wanted
;       1f4       r/w       cylinder low, cylinders is 0-1024
;       1f5       r/w       cylinder high, this makes up the rest of the 1024
;       1f6       r/w       drive/head
;                              bit 7 = 1
;                              bit 6 = 0
;                              bit 5 = 1
;                              bit 4 = 0  drive 0 select
;                                    = 1  drive 1 select
;                              bit 3-0    head select bits
;       1f7       r         status register
;                              bit 7 = 1  controller is executing a command
;                              bit 6 = 1  drive is ready
;                              bit 5 = 1  write fault
;                              bit 4 = 1  seek complete
;                              bit 3 = 1  sector buffer requires servicing
;                              bit 2 = 1  disk data read corrected
;                              bit 1 = 1  index - set to 1 each revolution
;                              bit 0 = 1  previous command ended in an error
;       1f7       w         command register
;                            commands:
;                              50h format track
;                              20h read sectors with retry
;                              21h read sectors without retry
;                              22h read long with retry
;                              23h read long without retry
;                              30h write sectors with retry
;                              31h write sectors without retry
;                              32h write long with retry
;                              33h write long without retry
;
;  Most of these should work on even non-IDE hard disks.
 ************************************************************************/

