/*
 * fdc.c - Floppy Disk Drive Low Level Functions
 * 
 * Copyright	(C) 1998 Fabian Nunez.
 *		(C) 2002-2004,2014 Jari Tuominen.
 * Ported to JTMOS & performance modifications by Jari Tuominen.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * The author can be reached by email at: fabian@cs.uct.ac.za
 * 
 * or by airmail at: Fabian Nunez
 *                   10 Eastbrooke
 *                   Highstead Road
 *                   Rondebosch 7700
 *                   South Africa
 */
#include "basdef.h"
#include "kernel32.h"
#include "threads.h"
#include "fdc.h"
#include "fdcCache.h"
#include "driver_api.h"
#include "driver_sys.h"
#include "syssh.h"
#include "dma.h"

//
#define ddprintk dprintk
#define WHERE dprintk("%s/%d\n", __FUNCTION__, __LINE__);

//
FDC fdc;

// Currently selected drive.
// 0=A:, 1=B:, ... (0-3)
int fdc_drive=0;

// FDC controller version number (init needs this).
// Defaults to "version yet unknown".
int fdc_version = -1;

/* globals */
static volatile BOOL done = FALSE;
static BOOL dchange = FALSE;
static BOOL motor = FALSE;
static int mtick = 0;
static int tmout = 0;
static BYTE status[7] = { 0 };
static BYTE statsz = 0;
static BYTE sr0 = 0;
static BYTE fdc_track = 0xff;
static DrvGeom geometry = { DG144_HEADS,DG144_TRACKS,DG144_SPT };
extern void fdc_asmirqhandler6(void);

/* physical address of track buffer located below 1M */
// Define a location above the kernel.
// NOTE: also used by sound blaster.
//static long tbaddr = 0x90000;
static long tbaddr = 0x4000;

// Setup default delay values
void defaultDelays(FDCDELAY *d)
{
	// delay 500ms for motor to spin up
	d->motorOn=		500;
	// now let head settle for 15ms
	d->seek=		15;
	// The actual WAITFDC most used TIME-OUT VALUE
	d->fdcWait=		1000;
	//
	d->sendByte=		1000; // 128
	d->getByte=		1000; // 128

	// These values below are always same:
	d->maxTries=		4;
	d->safeMode=		1;	// Triggers safe mode on 2nd try

	// Delay 2000 milliseconds(2 seconds) then
	// turn floppy motor off(idle enough => motor off).
	d->floppyMotorOff=	1000;
}

// Setup safe delay values
void safeDelays(FDCDELAY *d)
{
	// delay 500ms for motor to spin up
	d->motorOn=		1000;
	// now let head settle for 15ms
	d->seek=		30;
	// The actual WAITFDC most used TIME-OUT VALUE
	d->fdcWait=		1000;
	//
	d->sendByte=		1000; // 128
	d->getByte=		1000; // 128

	// These values below are always same:
	d->maxTries=		4;
	d->safeMode=		1;	// Triggers safe mode on 2nd try

	// Delay 2000 milliseconds(2 seconds) then
	// turn floppy motor off(idle enough => motor off).
	d->floppyMotorOff=	1000;
}

// This function is called upon disk change
void FDC_DISKCHANGE(void)
{
	WHERE
#ifdef DISKCHANGE_AFFECTS_FDC_BUFFERS
	// Undo FDC buffers because disk changed
	fdc.ent[0].isfree=1;
	diskchangednr( driver_getdevicenrbyname("floppy") );
#endif
}

//
void fd_selectdrive(int which)
{
	WHERE
	if(which<=3)
	{
		fdc_drive=which&3;
	}
}

//
void fd_sleep(int waitAmount_ms)
{
	//
	int i,i2,i3,i4;

	WHERE
	//
	i4=(291*waitAmount_ms)/1000;

	//
	enable();
	//
	for(i=GetTickCount(); GetTickCount()<(i+i4); )
	{
		SCHEDULE();
	}
}

//
void __fd_sleep(int waitAmount_ms)
{
	int i,i2,i3,i4;
	enable();
	WHERE
	for(i=GetTickCount(); (GetTickCount()-i)<waitAmount_ms; )
	{
		printk("%d < %d   \r",
			(GetTickCount()-i), waitAmount_ms );
		SCHEDULE();
	}
}

//
void _______fd_sleep(int ms)
{
	int i,i2,t;

	//
	ms=ms/1000;
	if(!ms)ms++;
	for(t=GetSeconds(); (GetSeconds()-t)<ms; )
		idle_moment();
}

// ==========================================================
// FDC driver main init function 
// ==========================================================
void fdc_init(void)
{
	WHERE
	//
	printk("3.5\" 1.44M disk device driver 1.0\n");

	// Structure init
	memset(&fdc, 0, sizeof(FDC));

	// Setup delays
	defaultDelays(&fdelay);

	// Allocate: Two tracks per entry.
	// Only first entry in use at the moment.
	fdc.ent[0].buf =	malloc(512*18*2);
	ChangeChunkOwner(fdc.ent[0].buf, 0);
	fdc.ent[0].isfree =	1;
	fdc.ent[0].writeback =	0;

	// HW init
	fd_init();

	//
//	floppy_register_device();
}

/* helper functions */
/* init driver */
void fd_init(void)
{
	int i,i2;

	WHERE
	//
	ddprintk("\rfdc.c: Floppy driver initialising ...                    \r");
   
	/* install IRQ6 handler */
	setint(M_VEC+6,
		fdc_asmirqhandler6,
		GIMMIE_CS(),
		D_PRESENT + D_INT + D_DPL3);
	enable_irq(6);

	//
	ddprintk("\rfdc.c: Determining HW version                    \r");
	/* get floppy controller version */
	for(i2=0,i=-1; i2<1000 && i!=-1; i2++)
	{
		fd_sendbyte(CMD_VERSION);
		i = fd_getbyte();
	}
	fdc_version = i;

	//
	if(i==-1)
	{
		printk("fdc.c: Could not receive controller version.\n");
	}
	else
	if(i == VER_OLDNECFDC)
	{
		// Old!
		printk("fdc.c: NEC765 controller found. Not fully supported.     \n");
	}
	else
	{
		// Modern.
		printk("fdc.c: Enhanced controller found(0x%x).                  \n", i);
	}

	//
	fd_reset();
}

/* deinit driver */
void fd_deinit(void)
{
	WHERE
	/* stop motor forcefully */
	outportb(FDC_DOR,0x0c|fdc_drive);
}

/* fd_sendbyte() routine from intel manual */
void fd_sendbyte(int byte)
{
	volatile int msr;
	int tmo;

	WHERE
	for (tmo=0; tmo<fdelay.getByte; tmo++)
	{
		msr = inportb(FDC_MSR);
		if( (msr & 0xc0)==0x80 )
		{
			outportb(FDC_DATA,byte);
			return;
		}

		inportb(0x80);   /* delay */
	}
}

/* fd_getbyte() routine from intel manual */
int fd_getbyte()
{
	volatile int msr;
	int tmo;

	WHERE
	for (tmo=0; tmo<fdelay.getByte; tmo++)
	{
		msr = inportb(FDC_MSR);
		if ((msr & 0xd0) == 0xd0)
		{
			return inportb(FDC_DATA);
		}
		inportb(0x80);   /* delay */
	}

	return -1;   /* read timeout */
}

/* this waits for FDC command to complete */
BOOL fd_waitfdc(BOOL sensei)
{
	int t;

	tmout = fdelay.fdcWait;   /* set timeout to 1 second */

	WHERE
	/* wait for IRQ6 handler to signal command finished */
	for(t=rtc_getseconds(); !done && (rtc_getseconds()-t)<2; )
	{
		SCHEDULE();
//		ddprintk("tmout=%d, done=%d     \r", tmout,done);
	}

	/* read in command result bytes */
	WHERE
	statsz = 0;
	while ((statsz < 7) && (inportb(FDC_MSR) & (1<<4)))
	{
		status[statsz++] = fd_getbyte();
	}

	if (sensei)
	{
		/* send a "sense interrupt status" command */
		fd_sendbyte(CMD_SENSEI);
		sr0 = fd_getbyte();
		fdc_track = fd_getbyte();
	}
   
	//
	done = FALSE;

	//
	if (!tmout)
	{
		/* timed out! */
		if(inportb(FDC_DIR) & 0x80)  /* check for diskchange */
			dchange = TRUE;

		//
		FDC_DISKCHANGE();
		return FALSE;
	}
	else
	return TRUE;
}

/* This is the IRQ6 handler */
void fdc_irqhandler6(void)
{
	/* Check for disk change - Added by Jari. */
	if(inportb(FDC_DIR) & 0x80)
	{
		dchange = TRUE;
		FDC_DISKCHANGE();
		WHERE
	}

	/* signal operation finished */
	done = TRUE;

	/* EOI the PIC */
	outportb(0x20,0x20);
}

/* This is the timer (int 0x1C) handler */
void fd_int1c(void)
{
	//
	if (tmout) --tmout;     /* bump timeout */
   
	// Turn off engine after 3 seconds of idling.
	if( (mtick>0 && (GetSeconds()-mtick)>=3) && motor )
	{
		//
		WHERE
		outportb(FDC_DOR,0x0c|fdc_drive);  /* turn off floppy motor */
		motor = FALSE;
	}
}

/*
 * converts linear block address to head/track/sector
 * 
 * blocks are numbered 0..heads*tracks*spt-1
 * blocks 0..spt-1 are serviced by head #0
 * blocks spt..spt*2-1 are serviced by head 1
 * 
 * WARNING: garbage in == garbage out
 */
void fd_block2hts(int block,int *head,int *track,int *sector)
{
	*head = (block % (geometry.spt * geometry.heads)) / (geometry.spt);
	*track = block / (geometry.spt * geometry.heads);
	*sector = block % geometry.spt + 1;
}

/**** disk operations ****/

// This gets the FDC to a known state.
// BUG: Only works with newer FDC controllers.
void fd_reset(void)
{
	WHERE
	// Precaution.
	if(fdc_version==-1)
		// Yet unknown FDC version.
		return;

	WHERE

	/* stop the motor and disable IRQ/DMA */
	outportb(FDC_DOR,0|fdc_drive);
   
	//
	mtick = GetSeconds();
	motor = FALSE;

	/* program data rate (500K/s) */
	outportb(FDC_DRS,0);

	//
	if(fdc_version != VER_OLDNECFDC)
	{
		/* NEW FDC: re-enable interrupts */
		outportb(FDC_DOR,0x1A|fdc_drive);
	}
	else
	{
		/* OLD FDC: re-enable interrupts */
		outportb(FDC_DOR,0x0C|fdc_drive);
	}

	/* resetting triggered an interrupt - handle it */
	done = TRUE;
	fd_waitfdc(TRUE);

	/* specify drive timings (got these off the BIOS) */
	fd_sendbyte(CMD_SPECIFY);
	fd_sendbyte(0xDF);  /* SRT = 3ms, HUT = 240ms */

	//
	if(fdc_version != VER_OLDNECFDC)
	{
		// NEW FDC.
		fd_sendbyte(0x06);  /* Head load time =6ms, DMA */
	}
	else
	{
		// OLD FDC.
		fd_sendbyte(0x02);  /* HLT = 16ms, ND = 0 */
	}
 
	/* clear "disk change" status */
	fd_seek(1);
	fd_recalibrate();

	//
	dchange = FALSE;
}

/* this returns whether there was a disk change */
BOOL fd_diskchange(void)
{
	return dchange;
}

/* this turns the motor on */
void fd_motoron(void)
{
	WHERE

	//
	if(!motor)
	{
		//
		mtick = -1;     /* stop motor kill countdown */
		outportb(FDC_DOR,0x1c|fdc_drive);
		fd_sleep(fdelay.motorOn); /* delay 500ms for motor to spin up */
		motor = TRUE;
	}
}

/* this turns the motor off */
void fd_motoroff(void)
{
	WHERE

	if (motor)
		/* start motor kill countdown: 36 ticks ~ 2s */
		mtick = GetSeconds();
}

/* recalibrate the drive */
void fd_recalibrate(void)
{
	WHERE

	/* turn the motor on */
	fd_motoron();
   
	/* send actual command bytes */
	fd_sendbyte(CMD_RECAL);
	fd_sendbyte(0);

	/* wait until seek finished */
	fd_waitfdc(TRUE);
   
	/* turn the motor off */
	fd_motoroff();
}

/* seek to track */
BOOL fd_seek(int track)
{
	WHERE

	if (fdc_track == track)  /* already there? */
		return TRUE;
   
	fd_motoron();
   
	/* send actual command bytes */
	fd_sendbyte(CMD_SEEK);
	fd_sendbyte(0);
	fd_sendbyte(track);

	/* wait until seek finished */
	if (!fd_waitfdc(TRUE))
		return FALSE;     /* timeout! */

	/* now let head settle for 15ms */
	fd_sleep(fdelay.seek);
   
	fd_motoroff();
   
	/* check that seek worked */
	if ((sr0 != 0x20) || (fdc_track != track))
		return FALSE;
	else
		return TRUE;
}

/* checks drive geometry - call this after any disk change */
BOOL log_disk(DrvGeom *g)
{
	WHERE

	/* get drive in a known status before we do anything */
	fd_reset();

	/* assume disk is 1.68M and try and read block #21 on first track */
	geometry.heads = DG168_HEADS;
	geometry.tracks = DG168_TRACKS;
	geometry.spt = DG168_SPT;

	if(fd_read_block(20,NULL))
	{
		/* disk is a 1.68M disk */
		if(g)
		{
			g->heads = geometry.heads;
			g->tracks = geometry.tracks;
			g->spt = geometry.spt;
		}
		return TRUE;             
	}
   
	/* OK, not 1.68M - try again for 1.44M reading block #18 on first track */
	geometry.heads = DG144_HEADS;
	geometry.tracks = DG144_TRACKS;
	geometry.spt = DG144_SPT;

	if(fd_read_block(17,NULL))
	{
		/* disk is a 1.44M disk */
		if(g)
		{
			g->heads = geometry.heads;
			g->tracks = geometry.tracks;
			g->spt = geometry.spt;
		}
		return TRUE;
	}
   
	/* it's not 1.44M or 1.68M - we don't support it */
	return FALSE;
}

/* write blocks */
BOOL fd_write_blocks(int block, BYTE *blockbuff, int amount)
{
	int rv;
	int tries;

	WHERE

	//
	defaultDelays(&fdelay);
	//
	for(tries=fdelay.maxTries; tries; tries--)
	{
		rv = fdc_rw(block,blockbuff,FALSE, amount);

		if(rv==FALSE)
		{
			// --: Write failed:
			ddprintk("%s: Write blocks failed on block %d\n",
				__FUNCTION__, block);
		}
		else
		{
			// --: Write OK
			break;
		}
	}

	//
	return rv;
}

/* read blocks */
BOOL fd_read_blocks(int block, BYTE *blockbuff, int amount)
{
	int rv,tries;
	tries=0;

	WHERE

	defaultDelays(&fdelay);
	for(tries=0; tries<fdelay.maxTries; tries++)
	{
		if(tries)
			ddprintk("%s: Retrying read block %d, try=%d\n",
				__FUNCTION__,
				block, tries);
	
		rv = fdc_rw(block,blockbuff,TRUE, amount);
		if(rv==TRUE)
		{
			// Read OK
			break;
		}
		else
		{
			// Read failed:
			if(tries==fdelay.safeMode)
			{
				fd_reset();
				safeDelays(&fdelay);
				ddprintk("%s: Trying safe parameters\n",
					__FUNCTION__);
			}
	
		}
	}

	// If eventually failed, do tell about it:
	if(rv==FALSE)
	{
		ddprintk("%s: fd_read_block READ FAILURE after %d tries\n",
			__FUNCTION__, tries);
	}
	return rv;
}

/*
 * since reads and writes differ only by a few lines, this handles both.  This
 * function is called by fd_read_block() and write_block()
 */
// TODO - Make it load whole track at once.
BOOL fdc_rw(int block,BYTE *blockbuff,BOOL read, int amount)
{
	int head,track,sector,tries,i,ii;

	WHERE

	/* convert logical address into physical address */
	fd_block2hts(block,&head,&track,&sector);
	ddprintk("block %d = %d:%d:%d\n",block,head,track,sector);
   
	/* spin up the disk */
	fd_motoron();

	if (!read && blockbuff)
	{
		/* copy data from data buffer into track buffer */
		memcpy(tbaddr,	blockbuff,	512*amount);
	}
   
	for(tries=0; tries<fdelay.maxTries; tries++)
	{
		/* check for diskchange */
		if(inportb(FDC_DIR) & 0x80)
		{
			FDC_DISKCHANGE();
			dchange = TRUE;
			fd_seek(1);  /* clear "disk change" status */
			fd_recalibrate();
			fd_motoroff();
			return FALSE;
		}
         
		/* move head to right track */
		if(!fd_seek(track))
		{
			ddprintk("%s: Seek time-out\n",
				__FUNCTION__);
			fd_motoroff();
			return FALSE;
		}
      
		/* program data rate (500K/s) */
		outportb(FDC_CCR,0);
      
		/* send command */
		if(read)
		{
			dma_xfer(FDC_DMA,tbaddr,512*amount,FALSE);
			fd_sendbyte(CMD_READ);
		}
		else
		{
			dma_xfer(FDC_DMA,tbaddr,512*amount,TRUE);
			fd_sendbyte(CMD_WRITE);
		}
      
		fd_sendbyte(head << 2);
		fd_sendbyte(track);
		fd_sendbyte(head);
		fd_sendbyte(sector);
		fd_sendbyte(2);               /* 512 bytes/sector */
		fd_sendbyte(geometry.spt);
		if (geometry.spt == DG144_SPT)
			fd_sendbyte(DG144_GAP3RW);  /* gap 3 size for 1.44M read/write */
		else
			fd_sendbyte(DG168_GAP3RW);  /* gap 3 size for 1.68M read/write */
		fd_sendbyte(0xff);            /* DTL = unused */
      
		/* wait for command completion */
		/* read/write don't need "sense interrupt status" */
		if(!fd_waitfdc(FALSE))
		{
			ddprintk("%s: Operation(R/W) time-out error\n",
				__FUNCTION__);
			return FALSE;   /* timed out! */
		}
      
		if((status[0] & 0xc0) == 0) break;   /* worked! outta here! */
   
		fd_recalibrate();  /* oops, try again... */
	}
   
	/* stop the motor */
	// FCE2E37B
	fd_motoroff();

	if (read && blockbuff)
	{
		/* copy data from track buffer into data buffer */
		memcpy(blockbuff,	tbaddr,		512*amount);
	}

/*	ddprintk("status bytes: ");
	for (i = 0;i < statsz;i++)
		ddprintk("%02x ",status[i]);
	ddprintk("\n");*/

	return (tries != fdelay.maxTries);
}

/* this formats a track, given a certain geometry */
BOOL format_track(BYTE track,DrvGeom *g)
{
	int i,h,r,r_id,split;
	static BYTE tmpbuff[256];

	WHERE

	/* check geometry */
	if (g->spt != DG144_SPT && g->spt != DG168_SPT)
		return FALSE;
   
	/* spin up the disk */
	fd_motoron();

	/* program data rate (500K/s) */
	outportb(FDC_CCR,0);

	fd_seek(track);  /* seek to track */

	/* precalc some constants for interleave calculation */
	split = g->spt / 2;
	if (g->spt & 1) split++;
   
	for (h = 0;h < g->heads;h++)
	{
		/* for each head... */
      
		/* check for diskchange */
		if (inportb(FDC_DIR) & 0x80)
		{
			FDC_DISKCHANGE();
			dchange = TRUE;
			fd_seek(1);  /* clear "disk change" status */
			fd_recalibrate();
			fd_motoroff();
			return FALSE;
		}

		i = 0;   /* reset buffer index */
		for (r = 0;r < g->spt;r++)
		{
			/* for each sector... */

			/* calculate 1:2 interleave (seems optimal in my system) */
			r_id = r / 2 + 1;
			if (r & 1) r_id += split;
	 
			/* add some head skew (2 sectors should be enough) */
			if (h & 1)	
			{
				r_id -= 2;
				if (r_id < 1) r_id += g->spt;
			}
      
			/* add some track skew (1/2 a revolution) */
			if (track & 1)
			{
				r_id -= g->spt / 2;
				if (r_id < 1) r_id += g->spt;
			}
	 
			/**** interleave now calculated - sector ID is stored in r_id ****/

			/* fill in sector ID's */
			tmpbuff[i++] = track;
			tmpbuff[i++] = h;
			tmpbuff[i++] = r_id;
			tmpbuff[i++] = 2;
		}

		/* copy sector ID's to track buffer */
		memcpy(tmpbuff, tbaddr, i);
//		movedata(_my_ds(),(long)tmpbuff,_dos_ds,tbaddr,i);
      
		/* start dma xfer */
		dma_xfer(FDC_DMA,tbaddr,i,TRUE);
      
		/* prepare "format track" command */
		fd_sendbyte(CMD_FORMAT);
		fd_sendbyte(h << 2);
		fd_sendbyte(2);
		fd_sendbyte(g->spt);
		if (g->spt == DG144_SPT)      
			fd_sendbyte(DG144_GAP3FMT);    /* gap3 size for 1.44M format */
		else
			fd_sendbyte(DG168_GAP3FMT);    /* gap3 size for 1.68M format */
		fd_sendbyte(0);     /* filler byte */
	 
		/* wait for command to finish */
		if(!fd_waitfdc(FALSE))
			return FALSE;
      
		if(status[0] & 0xc0)
		{
			fd_motoroff();
			return FALSE;
		}
	}
   
	fd_motoroff();
	return TRUE;
}

