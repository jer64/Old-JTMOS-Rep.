#ifndef __INCLUDED_HD_H__
#define __INCLUDED_HD_H__

//#define ENABLE_HD_DEBUG_MESSAGES

// Use cache?
#define USE_HD_CACHE

#define FAILED(x) \
                printf("%s/%s/%d: %s", \
                        __FILE__, __FUNCTION__, __LINE__, x); \
		abort();

// Quick portability from kernel:
#define printk printf
//#define dprintk printf
//
#define panic(x) \
	if(1) { \
	printf("Line=%d, function=%s\n", __LINE__, __FUNCTION__); \
	printf(x); abort(); \
	}

//
#ifndef DWORD
typedef unsigned long int DWORD;
typedef unsigned short int WORD;
typedef unsigned char BYTE;
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


// -------------------------------------------------------------
#include <stdio.h>
#include "hdparam.h"

// -------------------------------------------------------------
// 0x03F6
#define HD_ENABLE_IRQS		2
#define HD_RESET_BIT		4

// ===============================================
//
#define HDDEBUG        dprintk("[%s / %s  Line %d]\n", \
                        __FILE__, __FUNCTION__, __LINE__);


// ===============================================
//
extern void dhd_asmirqhandler14(void);
extern void dhd_asmirqhandler15(void);

//
#define DEFAULT_DRIVE 0

//
#define READ		0xFCE2
#define WRITE		0xE37B

//
typedef struct
{
	//
	int units;	// Units(512 bytes sectors) per a block,
			// determined upon drive size.
	int driveBroken; // set to 1 when drive is useless
	char slave; // 0=Master, 1=Slave
	int size;
	int cylinders,heads,sectors;
	int extra;
	int n_blocks,blocksize;
	char ModelNumber[50];
	char FirmwareRev[10];
	char SerialNumber[30];
	char GiveUpFast;

	// Cylinder buffer length for this drive(in bytes)
	// (each of the buffers in cache use this same size)
	int l_buf;

        // Number of faults happended durring run time of this drive
        // (write faults)
        int fault;

	//
	HDPARAM hp;
}HD;

// Used as an identifier whetever has irq happended
// for specified IDE #.
extern int ideirqtick[10];

//
void hdTestDrive(void);

// Friendly names: HD status bits
#define HD_STATUS_BUSY		0x80
#define HD_STATUS_READY		0x40
#define HD_STATUS_WRITE_ERROR	0x20
#define HD_STATUS_SEEK_COMPLETE	0x10
#define HD_STATUS_DRQ		0x08
#define HD_STATUS_ECC		0x04
#define HD_STATUS_INDEX		0x02
#define HD_STATUS_ERROR		0x01

// Standard names
#define IDE_ERR_STAT    0x01
#define IDE_INDEX_STAT  0x02
#define IDE_ECC_STAT    0x04    /* Corrected error */
#define IDE_DRQ_STAT    0x08
#define IDE_SEEK_STAT   0x10
#define IDE_WRERR_STAT  0x20
#define IDE_READY_STAT  0x40
#define IDE_BUSY_STAT   0x80

//
extern HD **hd;
extern HD *chd;
int hdInit(void);

//
#include "hdIRQ.h"
#include "hdReset.h"
#include "hdDev.h"
#include "hdMisc.h"
#include "chs.h"
#include "hdDetect.h"
#include "hdCache.h"
#include "chs.h"
#include "hdRW.h"
#include "hdparam.h"
#include "hdBlockRW.h"
#include "hdReset.h"
#include "low.h"

//
int dprintk(const char *fmt, ...);

#endif


