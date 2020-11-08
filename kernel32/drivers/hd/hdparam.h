#ifndef __INCLUDED_HDPARAM_H__
#define __INCLUDED_HDPARAM_H__

//
#include <stdio.h>
#include "hd.h"

//
typedef struct
{
	// 6C
	DWORD chs1;
	WORD chs2;

	// 72
	BYTE sec_cap;
	BYTE something1;
	BYTE something2;
	BYTE something3;

	// 76
	WORD multipleSectorSupport;
	// 78
	DWORD n_lba;	// Number of LBA sectors available for user (zero if LBA is not supported)
	// 7C
	WORD singleWordDMATransfers;
	WORD multiWordDMATransfers;
	//
}HDP_MODERN;

//
typedef struct
{
	// 00
	WORD geninfo;
	WORD cylinders;
	WORD reserved;
	WORD heads;
	// 08
	WORD misc1; // vendor specific (obsolete)
	// 0A
	WORD misc2; // -''-
	// 0C
	WORD sectors;
	// 0E
	WORD vendor1; // some more vendor specific words
	// 10
	WORD vendor2;
	// 12
	WORD vendor3;
	// 14
	BYTE SerialNumber[20]; // 10 words, 20 bytes
	// 28
	WORD vendor4;
	// 2A
	WORD controllerBufferSize; // in 512 bytes units
	// 2C
	WORD vendor5;
	// 2E
	BYTE FirmwareRev[8]; // 4 words, 8 bytes
	// 36
	BYTE ModelNumber[40]; // 20 words, 40 bytes

	// 5E	WORD	read/write multiple support
	//		bits 7-0: maximum number of sectors per block supported
	//		00h if read/write multiple not supported
	//		bits 15-8: vendor-specified
	WORD RW_MultipleSupport;
	// 60
	WORD canDoDDTransfers;
	// 62
	WORD capabilities;
	// 64
	WORD securityMode;
	// 66: Timing related
	WORD PIOdata1;		// pio data cycle timing
	// 68
	WORD PIOdata2;	// single-word DMA data transfer cycle timing
	// 6A
	WORD fieldValidity;

	// 6C
	HDP_MODERN mod;

	//
	WORD RESERVED[160];
}HDPARAM;

#endif

