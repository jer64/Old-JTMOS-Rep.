// ------------------------------------------------------
// dhd_detect.c
// Drive detection code.
// (C) 2003-2004 by Jari Tuominen.
// ------------------------------------------------------
#include <stdio.h>
#include "hd.h"
#include "hdDetect.h"

// Detect drive(master or slave) on IDE0
int hdDetect(HD *h)
{
	int i,registered;

	// First trigger fast reset for all current IDEx devices
	hdReset1();

	// Register up only if read on #0 block succeeds:
	// Try master and slave.
	for(i=0; i<2; i++)
	{
		//
		if( (registered=hdTryDevice(h, i)) )
		{
			//
			break;
		}
	}

	// If registered, process parameters
	if(registered)		hdPrimaryDetect(h);

	
	//
	return registered;
}

void RecalculateTotalBlocks(HD *h)
{
	//
#ifdef FORCE_BLOCKS
	h->n_blocks = FORCE_BLOCKS;
#else
        h->n_blocks = h->cylinders*h->heads*h->sectors;
#endif
}

// Receive drive parameters
int GetHDParam(HD *h, int drive, WORD *hdp)
{
	static WORD i,i2,w,w2,w3,w4;
	int t;
	HDPARAM *hh;

	//
	if(h==NULL)	panic("GetHDParam called with h==NULL");

	// Order to get drive information
	outportb(0x1F6, drive ? 0xB0:0xA0);
	outportb(0x1F7, 0xEC);

	// Wait until there's no data available
	for(t=GetSeconds(); (inportb(0x1F7) != 0x58); )
	{
		//
		if( (GetSeconds()-t)>3 )
		{
			//
			return 1;
		}

		//
		SwitchToThread();
	}

	// Read the data.
	for(i2=0; i2<256; i2++)
	{
		if(hdp!=NULL)	hdp[i2] = inportw(0x1F0);
				else
					inportw(0x1F0);
	}

	// Fix names, those are written in weird way always (big endian)
	if(hdp!=NULL)
	{
		hh=hdp;
		StrFix(hh->ModelNumber, 40);
		StrFix(hh->FirmwareRev, 8);
		StrFix(hh->SerialNumber, 20);
	}

	// Completed OK!
	return 0;
}

// Probe HD: Master or Slave
int hdTryDevice(HD *h, int selection)
{
	int tries;
	int registered;

	//
	h->slave = selection;

	//
	registered=0;

	//
	for(tries=0; tries<2; tries++)
	{
		// Try to get params
		if( !GetHDParam(h, h->slave, NULL) )
		{
			// OK drive detected
			registered=1;
			break;
		}
	}
	return registered;
}


// Fix string
int StrFix(BYTE *s, int l)
{
	int i;
	DWORD v1,v2;

	//
	for(i=0; i<(l-1); i+=2)
	{
		v1=s[i+0]; v2=s[i+1];
		s[i+0]=v2; s[i+1]=v1;
	}
}

// Detect and configure device.
int hdPrimaryDetect(HD *h)
{
	int i,i2,i3,i4;
	void *p;
	DWORD *dp;
	DWORD ad,ad2;

	////////////////////////////////////////////////////////////////////////////////
        // Specify block size(default 512 bytes)
        h->blocksize = 512;

	// Get data
	memset(&h->hp, 0, sizeof(HDPARAM));
	GetHDParam(h, h->slave, &h->hp);

	////////////////////////////////////////////////////////////////////////////////
	//
	if( h->hp.mod.n_lba )
		h->lbaMode = TRUE;
	else
		h->lbaMode = FALSE;

	// Set hardware level preference.
	HD_SetLBAMode(h->lbaMode);

	//
	h->n_lba = h->hp.mod.n_lba;

	////////////////////////////////////////////////////////////////////////////////
	//
	if(h->lbaMode)
	{
		// Zeroes mean LBA mode.
		h->cylinders =	1;
		h->heads = 	1;
		h->sectors = 	1;

		//
		h->n_blocks = h->n_lba;		// number of blocks
		h->units =	1;		// x 512 bytes
		
		// Anyway, all of that CHS stuff should be not of concern when in LBA mode.
	}
	else
	{
		// Collect data, precalculate
		h->cylinders =	h->hp.cylinders;
		h->heads =	h->hp.heads;
		h->sectors =	h->hp.sectors;
		RecalculateTotalBlocks(h);
		hdDetermineUnits(h);
	}

	////////////////////////////////////////////////////////////////////////////////
	//
	if(h->hp.cylinders)
	{
		//
		for(i=0; i<40; i++) {
			h->ModelNumber[i]=h->hp.ModelNumber[i];
		}
		h->ModelNumber[i]=0;
		//
		for(i=0; i<8; i++) {
			h->FirmwareRev[i]=h->hp.FirmwareRev[i];
		}
		h->FirmwareRev[i]=0;
		//
		for(i=0; i<20; i++) {
			h->SerialNumber[i]=h->hp.SerialNumber[i];
		}
		h->SerialNumber[i]=0;

		//
		printk("HD parameters: ");
		if(h->lbaMode)
		{
			printk("[LBA MODE] %d LBA sectors (%dM)\n",
				h->n_lba, (h->n_lba*512)/(1024*1024));
		}
		else
		{
			printk("[CHS MODE] Cylinders=%d, Heads=%d, Sectors=%d\n",
				h->cylinders, h->heads, h->sectors);
		}
		printk("HD MODEL   : %s\n", h->ModelNumber);
		printk("HD SERIAL  : %s  ", h->SerialNumber);
		printk("HD FIRMREV : %s\n", h->FirmwareRev);
	}
	else
	{
		printk("Warning: No HDPARAM could be received.\n");
		return 1;
	}
	return 0;
}

// Determine size of block for the device(good for FAT)
void hdDetermineUnits(HD *h)
{
	// ---------------------------------------------------------------
	// Determine block size from the drive size.
	// We simulate bigger blocks for FAT system.

	// For drives bigger than 256 MB:
	if( h->n_blocks > (2* 1024*768) )
	{
		h->units = 16;
	}
	else
	if( h->n_blocks > (2* 1024*256) )
	{
		h->units = 8;
	}
	else
	// For drives bigger than 80 MB:
	if( h->n_blocks > (2* 1024*80) )
	{
		h->units = 4;
	}
	else
	// For drives bigger than 20 MB:
	if( h->n_blocks > (2* 1024*20) )
	{
		h->units = 2;
	}
	else
		h->units = 1;

	// Report
	printk("%s: 512 bytes units per block = %d\n",
		__FUNCTION__, h->units);
}


