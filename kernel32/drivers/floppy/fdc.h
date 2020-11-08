/*
 * fdc.h
 * 
 * header for floppy controller handler
 * 
 * Copyright (C) 1998  Fabian Nunez
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

#ifndef FDC_H_
#define FDC_H_

// Define if buffers must be cleared upon disk change
#define DISKCHANGE_AFFECTS_FDC_BUFFERS

// Preferedences
// -------------
#define FDC_DMA		2

//
#define VER_OLDNECFDC	0x80

//
#include "mytypes.h"

// Some low level preferences
// --------------------------
extern int V_DELAY_MOTORON;
extern int V_DELAY_SEEK;
extern int X_TIMER;
extern int V_WAITFDC_DELAY;
extern int FD_GETBYTE_TMOUT;
extern int FD_SENDBYTE_TMOUT;
extern int FD_MAX_TRIES;


// Cache entry for FDC
// (Each of these entries contain 18*2 sectors,
//  meaning 2 in tracks and
//  18K in size)
typedef struct
{
	// Buffer PTR to 18*2 sectors of data
	BYTE *buf;
	// Entry is free? 1=True, 0=False
	int isfree;
	// Range
	int s,e;
	// Writeback request indicator
	int writeback;
}FDCENT;

//
typedef struct
{
	//
	FDCENT ent[2];
}FDC;
extern FDC fdc;


/* datatypes */

/* drive geometry */
typedef struct DrvGeom {
   BYTE heads;
   BYTE tracks;
   BYTE spt;     /* sectors per track */
} DrvGeom;

/* drive geometries */
#define DG144_HEADS       2     /* heads per drive (1.44M) */
#define DG144_TRACKS     80     /* number of tracks (1.44M) */
#define DG144_SPT        18     /* sectors per track (1.44M) */
#define DG144_GAP3FMT  0x54     /* gap3 while formatting (1.44M) */
#define DG144_GAP3RW   0x1b     /* gap3 while reading/writing (1.44M) */

#define DG168_HEADS       2     /* heads per drive (1.68M) */
#define DG168_TRACKS     80     /* number of tracks (1.68M) */
#define DG168_SPT        21     /* sectors per track (1.68M) */
#define DG168_GAP3FMT  0x0c     /* gap3 while formatting (1.68M) */
#define DG168_GAP3RW   0x1c     /* gap3 while reading/writing (1.68M) */

/* IO ports */
#define FDC_DOR  (0x3f2)   /* Digital Output Register */
#define FDC_MSR  (0x3f4)   /* Main Status Register (input) */
#define FDC_DRS  (0x3f4)   /* Data Rate Select Register (output) */
#define FDC_DATA (0x3f5)   /* Data Register */
#define FDC_DIR  (0x3f7)   /* Digital Input Register (input) */
#define FDC_CCR  (0x3f7)   /* Configuration Control Register (output) */

/* command bytes (these are 765 commands + options such as MFM, etc) */
#define CMD_SPECIFY (0x03)  /* specify drive timings */
#define CMD_WRITE   (0xc5)  /* write data (+ MT,MFM) */
#define CMD_READ    (0xe6)  /* read data (+ MT,MFM,SK) */
#define CMD_RECAL   (0x07)  /* recalibrate */
#define CMD_SENSEI  (0x08)  /* sense interrupt status */
#define CMD_FORMAT  (0x4d)  /* format track (+ MFM) */
#define CMD_SEEK    (0x0f)  /* seek track */
#define CMD_VERSION (0x10)  /* FDC version */


/* Function prototypes		1*/
void fdc_init(void);
void fd_init();
void fd_deinit(void);

/* More prototypes		2*/
void fd_sendbyte(int byte);
int fd_getbyte();
void fd_irq6(void);
void fd_int1c(void);
BOOL fd_waitfdc(BOOL sensei);
BOOL fdc_rw(int block,BYTE *blockbuff,BOOL read, int amount);

/* Even more prototypes		3*/
void fd_reset(void);
BOOL fd_diskchange(void);
void fd_motoron(void);
void fd_motoroff(void);
void fd_recalibrate(void);
BOOL fd_seek(int track);
BOOL fd_log_disk(DrvGeom *g);
BOOL fd_read_blocks(int block,BYTE *blockbuff, int count);
BOOL fd_write_blocks(int block,BYTE *blockbuff, int count);
BOOL fd_format_track(BYTE track,DrvGeom *g);
void fdc_irqhandler6(void);
void fd_int1c(void);
void fd_selectdrive(int which);

//
extern void fdc_asmirqhandler6(void);

//
//extern int tmout;

//
#include "fdc_dev.h"

//
typedef struct
{
	int motorOn;	// 500
	int seek;	// 15ms head settle period
	int factor;	// 16
	int fdcWait;	// 4000
	int getByte;	// 4000
	int sendByte;	// 4000
	int maxTries;	// 3 - How many tries?
	int safeMode;	// 2 - On how manyth shall we try safe mode?
	int floppyMotorOff; // 2000 = 2 seconds
}FDCDELAY;
FDCDELAY fdelay;

#include "fdc_multitasking.h"

#endif /* FDC_H_ */
