#ifndef __INCLUDED_DHD_H__
#define __INCLUDED_DHD_H__

#include "kernel32.h"

// ===============================================
// * This affects performance alot:
#define HD_BUFFER_AMOUNT		2
#define MAX_BUFTRACKS			HD_BUFFER_AMOUNT+10
// * Safe boundary to prevent some odd buffer overflows
#define DHD_BUFFER_SAFEBOUNDARY		1024*256

// ===============================================
// 0x03F6
#define HD_ENABLE_IRQS		2
#define HD_RESET_BIT		4

// ===============================================
//
#define HDDEBUG        dprintk("[%s / %s  Line %d]\n", \
                        __FILE__, __FUNCTION__, __LINE__);
// ===============================================
// OPTIONS SECTION
#define ALLOW_RECALIBRATION			YES
//#define ALWAYS_DISABLE_WRITE_CACHE
#define DISABLE_INTERRUPTS_DURRING_OPERATIONS

// ===============================================
//#define LIMIT_HD_SIZE
#define HD_MAX_SIZE_ALLOWED			1024*1024 * 375
					//                  ^^ MEGS

// ===============================================
// When HD is wanted to be trimmed in some size(trick)
//#define LIMIT_CYLINDERS		10

// ===============================================
// Debugging on or off
//#define BUFDEB          printk
#define BUFDEB                        //

// ===============================================
//
// Some HDs might want different values,
// but since autodetection is being
// done by reading sector, we cannot
// setup that high values...
#define TM_OUT_RW2_SECS		5
#define HD_RW_TRIES		2
#define HD_WAIT_TIMEOUT_SECS	3	// Secs
#define HD_WAIT_TIMEOUT		3000	// MSEC
#define HD_SECTORRW_TIMEOUT	2000	// MSEC
// How many times shall we try to write a sector at max.?
#define MAX_WRITE_TRIES 4
// ===============================================

//
#include "basdef.h"
#include "hdparam.h"

//
#define DHD_JARIS_HD
//#define DHD_EMU2_HD

// How many sectors shall be used in testing?(read/write HD tests)
#define TESTNRSEC 2000

//
#ifdef DHD_JARIS_HD
#define DHD_DEFAULT_SIZE	126
#define DHD_DEFAULT_CYLINDERS	895
#define DHD_DEFAULT_HEADS	5
#define DHD_DEFAULT_SECTORS	55
#endif

//
#ifdef DHD_EMU_HD
#define DHD_DEFAULT_SIZE        135
#define DHD_DEFAULT_CYLINDERS   262
#define DHD_DEFAULT_HEADS       16
#define DHD_DEFAULT_SECTORS     63
#endif

//
#ifdef DHD_EMU2_HD
#define DHD_DEFAULT_SIZE        108
#define DHD_DEFAULT_CYLINDERS   209
#define DHD_DEFAULT_HEADS       16
#define DHD_DEFAULT_SECTORS     63
#endif

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
	char used; // zero=free, non-zero=allocated
	char writeBack; // non-zero=write back required, zero=not needed
	int c,h,s;
	int l,accessCount;
	BYTE *buf;
}BUFTRACK;

//
typedef struct
{
	int n_tracks;
	BUFTRACK track[MAX_BUFTRACKS];
}TRACKDB;

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

	// Track buffer Data Base
	TRACKDB db;
	//
	HDPARAM hp;
}HDSPEC;

// 
extern int dhd_blockrw(HDSPEC *h, BYTE *buf,
		unsigned long int blocknr,
		long drivenr,
		long rw);

//
extern void dhd_readtest(HDSPEC *h);
extern void dhd_writetest(HDSPEC *h);
extern void dhd_writetest2(HDSPEC *h);
extern int dhd_seek(HDSPEC *h, char *buf,long headnr,long sectornr,
                long cylinder,
                long drivenr, // 0=master,1=slave(I think)
                long rw, // 0=read, 1=write (UNUSED)
                long sectorcount); // (UNUSED);

//
extern HDSPEC hdspec[10];

// Number of hdspecs registered
extern int n_hdspecs;
// Used as an identifier whetever has irq happended
// for specified IDE #.
extern int ideirqtick[10];
//
void dhd_command(int which);
int dhd_errorcheck(HDSPEC *h, long rw,long drivenr,long headnr,long sectornr,long cylinder);
int dhd_TellStatus(HDSPEC *h);

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

extern int HD_BUFFERING_ENABLED;

//
typedef struct
{
        BYTE *tmp;
}GENHD;
extern GENHD genhd;

//
#ifdef DISABLE_INTERRUPTS_DURRING_OPERATIONS
#define DISABLE()	nop()
#define ENABLE()	nop()
#endif

//
int dhd_SetFeature(HDSPEC *h, int which, int what);
void dhd_recalibrate(HDSPEC *h);
int dhd_detectparams(HDSPEC *h);

//
extern HDSPEC *CurrentHD;

//
void RecalculateTotalBlocks(HDSPEC *h);
//
int dhd_rw(HDSPEC *h, BYTE *buf, int rw);

//
#include "dhd_lowLevelRWAccess.h"
#include "dhd_multitasking.h"

#endif
