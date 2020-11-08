
#ifndef __INCLUDED_JTMFAT_H__
#define __INCLUDED_JTMFAT_H__

// On which block does the info block reside?
// (Block)	(Description)
// 0:		Boot code/data
// 1:		Partition table
// 2:		JTMFS information block
#define FAT_INFO_BLOCK_OFFS	512
#define FAT_BOOT_SECTION_LEN_K	16
#define KERNEL_LEN_K		512

// Enable new FLEXFAT system.
#define FLEXFAT

//
#define jtmfat_getrootdir(x) _jtmfat_getrootdir(__FUNCTION__, x)

// Remove remarks from the defination below
// if you want to enable strict perfection -option.
// This option will cause panics on some concurrent
// problems, rather than trying to fix these.
//// #define STRICT_PERFECTION

// When formatting a drive, this is a max. value of how many
// times the progress indicator status must be updated.
// 100 means 100 times, this means update on every % increase.
// (obsolete)
#define INDICATOR_RADIUS        100

// Safe guard line that is REQUIRED!
// Recommanded: 1024*8 (8K)
#define FAT_SAFEGUARD		1024*512
// Contains roadmap for the FAT table.
#define L_ROADMAP		getsize(dnr)
// Contains cache 32-bit FAT table(4 bytes per entry):
#define L_FATCACHE		getsize(dnr)*4
// Contains CFAT structures for max. X different devices(X=value below).
#define N_CFAT_STRUCTURES	100

//
//#define JTMFAT_ALLOWTEXTS
// Uncomment this out if you want warnings/etc messages to be shown too.
//#define __DJTMFAT__

//
#include "basdef.h"
#include "devapi.h"

//
typedef struct
{
	// 11 bytes: (512-11 = 501)

	// Kernel 16-bit checksum.
	WORD correct_kernel_checksum;
	// Boot section length(offset to kernel on drive).
	WORD boot_len_k;
	// CHS settings.
	WORD cylinders;
	BYTE heads;
	BYTE sectors;
	// BIOS boot drive.
	BYTE bootdrv;
	// Boot record signature.
	// (0x55AA)
	WORD signature;
}BOOTRECORD;

// 48 bytes bootblock,
// res1,res2 = will contain sufficient space for the ASM jump instruction
// which will jump past the description information, to the code
// beginning after res7.
//
// New rule:
// Offsets now mean block numbers,
// aka block offset.
// length = length in blocks
//
typedef struct
{
	// First eight bytes are reserved(res1,res2 DWORDs).
	DWORD res1,res2;
	// Boot drive (0=floppy A:, 1=floppy B:, 0x80=drive C:, 0x81=drive D:).
	BYTE bootdrv;
	// Detection string: "JTMFS",0
	char detstr[6];
	// Offset to FAT and data area.
	DWORD fat_offs;
	DWORD data_offs;
	DWORD kernel_offs;	// 0 if no kernel featured.
	// System area length in 512 bytes blocks.
	DWORD sysarea_length; // (blocks)
	// System area before the data area,
	// system area is:
	// A) boot block B) info block C) FAT (in bytes)
	DWORD fat_length; // (blocks)
	// sysarea_length.
	DWORD bootblock_size; // (blocks)
	// DEVICE SPECIFIC.
	DWORD n_blocks,block_size;
	// FAT table checksum.
	DWORD fatChecksum32;
	// Max. value allowed on FAT table.
	int max_val;
	// Reserved.
	DWORD res3,res4,res5,res6,res7;
}JTMFAT_INFOBLOCK;

//
typedef struct
{
	char isActive;

	//
	DWORD *buf;
	int l_buf;

	//
	BYTE *actual;
	int l_actual;

	//
	char *roadMap;	// Info about which block has changed
	int l_roadMap;

	//
	int requiresFlush;
}CFAT;

//
typedef struct
{
	// Is drive removable?
	char isRemovable;
	// Last time when boot block was reloaded.
	int t;
}BOOTINF;

//
typedef struct
{
	// This is a new device? We have to load information of it,
	// where fat is located, how big it is, how big the device is, etc..
	// (This variable is set to non-zero usually by jtmfat_chdev)
	int virgin;
	// device number
	int dnr;
	// the bootblock, which contains all information
	// jtmfat_chdev will reload new bootblock to this structure.
	JTMFAT_INFOBLOCK *bb[100]; // << 100 is max. devices now !!!! (2)
	// This device have been formatted to JTMFS? 0=Nope, 1=Yes
	int isJTMFS[100]; // << 100 is max. devices now !!!! (1)
	// Boot block information.
	BOOTINF bootinf[100];
	// Temporary buffer, used for various purposes,
	// f.e. for storing a piece of fat table.
	BYTE *tmpbuf,*tbuf;

	// FAT structures.
	CFAT **fat;

	//
	BYTE *all_cfat_structures;
 //
}JTMFATSYS;

//
int _jtmfat_chdev(const char *caller, long dnr);
//
#define jtmfat_chdev(x) _jtmfat_chdev(__FUNCTION__, x)

//
int jtmfat_getrootdir1(int dnr);
int _jtmfat_getrootdir(const char *caller, int dnr);
int _jtmfat_loadinfoblock(const char *caller, int dnr);
//
#define jtmfat_loadinfoblock(x) \
	_jtmfat_loadinfoblock(__FUNCTION__, x)

//
#define jtmfat_getblock(dnr, db) \
	_flexFatGetBlock(__FUNCTION__, dnr, db)
#define jtmfat_setblock(dnr, which, what) \
	_flexFatSetBlock(__FUNCTION__, dnr,which,what)

//
long jtmfat_isJTMFS(int dnr);
void jtmfat_flushfatcache(void);
long jtmfat_getblocksallocated(long dnr);
long jtmfat_getblocksfree(long dnr);
long jtmfat_getfreeblock(long dnr);
int write_fat(int dnr);
int read_fat(int dnr);
int jtmfat_deactivateFatDNR(int dnr);

#define USE_DEBUGTRAP   char __str[255];
/*#define DEBUGTRAP       strcpy(__str, __FUNCTION__); \
DebugTrap(dnr, __str);*/

#define DEBUGTRAP	DebugTrap(dnr, __FUNCTION__);

//
JTMFAT_INFOBLOCK *jtmfat_getinfoblock(int dnr);
void jtmfat_DiskChange(int dnr);

//
extern int jtmfat_ready;
extern JTMFATSYS *fatsys;

//
#endif

