#ifndef __INCLUDED_HDPARAM_H__
#define __INCLUDED_HDPARAM_H__

typedef struct
{
	// 00
	WORD geninfo;
	WORD cylinders;
	WORD reserved;
	WORD heads;
	// 04
	WORD misc1;
	WORD misc2;
	WORD sectors;
	WORD InterSectorGap;
	// 08
	WORD PhaseLock;
	WORD N_VendorStatusWords;
	// 0A
	BYTE SerialNumber[20];
	// 14
	WORD ControllerType;
	WORD BufferSizeX512;
	WORD ECCbytes;
	// 17
	BYTE FirmwareRev[8];
	BYTE ModelNumber[40];
	WORD MultiImple;
	WORD DDIOTYPE;
	WORD RES1;
	WORD RES2;
	WORD PIOdata1;
	WORD PIOdata2;
	WORD RESERVED[160];
}HDPARAM;

#endif

