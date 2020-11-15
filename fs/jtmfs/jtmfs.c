// ==================================================
// JTMOS FILE SYSTEM
// -----------------
//
// (C) 2001-2004 by Jari Tuominen
//		(jari.tuominen@kanetti.fi).
//
// JTMFS is dependent on JTMFAT.
// JTMFAT is dependent on a block device,
// which varies.
// The block device is accessible via
// the BLOCK/CHAR DEVICE API implementation,
// which uses a caching system.
//
// jtmfsAccess.c        FS plugins access layer(high)
// jtmfat.c             JTMOS file allocation system(low)
// jtmfs.c              JTMOS file system(low)
// fswalk.c		Dir walking functions(low)
// fsfind.c		File finding functions(low)
// fsadd.c		Directory entry adding functions(low)
// fsaddblocks.c	Allocate more blocks on for(low)
//			file or directory
// fsdev.c		Functions that figure out
//			which device is currently
//			assigned to each of the processes(low)
// ==================================================
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
#include "jtmfs.h"
#include "fswalk.h"
#include "fsadd.h"
#include "fsdos.h"
#include "fname_translation.h"
#include "jtmfsAccess.h"
#include "jtmfsAccessInit.h"

//----------------------------------------------------------------------
//
DEBUGSTR debugstr;

//
struct
{
	int try;
}jtmfs_structure;

//----------------------------------------------------------------------
// Called by DiskChange system function.
int jtmfs_DiskChange(int dnr)
{
	// Indicate boot block content change.
	fatsys->isJTMFS[dnr]==FALSE;
}

//
void InitDW(DIRWALK *dw)
{
	memset(dw, 0, sizeof(DIRWALK));
	dw->buf_length = 8192;
}

//
void EndDirWalk(DIRWALK *dw)
{
	//
	dw->isWalking=0;
}

//
void jtmfs_inc_try(void)
{
	BYTE *dummy_buf,*dummy_buf2;

	//
	jtmfs_structure.try++;
 
	// print out the try number
	print("{"); pd32(jtmfs_structure.try); print("}");
 
	// make allocation system test(see if it works at all :))
	dummy_buf= malloc(1024*4+4368+jtmfs_structure.try);
	dummy_buf2=malloc(1024*4+2352+jtmfs_structure.try);
	free(dummy_buf);
	free(dummy_buf2);
}

// -----------------------------------------------------------
//
void jtmfs_init(void)
{
	//
	jtmfs_init_try();
	jtmfsAccessInit();
}

// -----------------------------------------------------------
//
void jtmfs_init_try(void)
{
	jtmfs_structure.try=0;
}

// -----------------------------------------------------------
// jtmfs_ffupdatefe
//
// Updates dw->fe to point at a correct location
// of dw->buf, this is done by using dw->current_offset.
// Updating the pointer structure makes the findfirst/next
// functions generally much easier to use and more efficient too.
//
void jtmfs_ffupdatefe(DIRWALK *dw)
{
	// Update offset with modern walk
	dw->fe = dw->d->buf+dw->current_offset;

	// For debugging :
#ifdef JTMFSDEBUG
	dprintk("%s: dw->fe=0x%x, dw->d->buf=0x%x, dw->buf=0x%x\n",
		__FUNCTION__,
		dw->fe,
		dw->d->buf,
		dw->buf);
#endif
}



/* typedef struct
 * {
 *  // sizeof, bytes :
 *  // 0x00
 *  char			name[31];
 *  unsigned short int 	type;
 *  unsigned int	length; (if -1 then entry is a ghost, deleted)
 *  unsigned int      protection;
 *  unsigned char		owner;
 *  unsigned int	creationdate;
 *  unsigned int	lastaccessdate;
 *  unsigned int	lastchangedate;
 *  unsigned int	track;
 *  unsigned int	block;
 *  // 0x40 .... bingo
 *  //
 * }JTMFS_FILE_ENTRY;
 */

// Use this function to create a single file/dir entry!
int jtmfs_fentry(	JTMFS_FILE_ENTRY *e,
			const char *name,
			unsigned short int 	type,
			unsigned int	length,
			unsigned char		protection,
			unsigned char		owner,
			unsigned int	creationdate,
			unsigned int	lastaccessdate,
			unsigned int	lastchangedate,
			unsigned int	FirstDataBlock,
			unsigned int	LastDataBlock)
{
	// If entry is NULL:
	if(e==NULL || name==NULL)
		return 1;

	// If filename error:
	if(!strlen(name) || strlen(name)>31)
		return 2;

	// First wipe
	memset(e, 0, sizeof(JTMFS_FILE_ENTRY));
	// Then copy
	strcpy(e->name,	name);
	e->type=		type;   // 0x80 = directory
	e->length=		length; // -1 = deleted / aka ghost
	e->FirstDataBlock=	FirstDataBlock;
	e->LastDataBlock=	LastDataBlock;
	e->magicNumber=		0xFCE2E37B;

	// Return success.
	return 0;
}			

//--------------------------------------------------------------------------------
//
// Creates a directory entry.
//
int jtmfs_cedir(JTMFS_FILE_ENTRY *e,const char *name,
	int FirstDataBlock,int LastDataBlock)
{
	// null reference checker
	if(!e || !name)
	{
		panic("[Oops!] jtmfs_cedir called with e==NULL || name==NULL\n");
		return;
	}

	// clear structure
	memset(e,0,sizeof(JTMFS_FILE_ENTRY));

	// copy name
	strcpy(e->name, name);
	e->FirstDataBlock =	FirstDataBlock;
	e->LastDataBlock =	LastDataBlock;
	e->magicNumber =	0xFCE2E37B;

	// type = directory(0x80)
	e->type = JTMFS_ID_DIRECTORY;

	//
	return 0;
}

// Creates end of directory
void jtmfs_cedirend(JTMFS_FILE_ENTRY *e)
{
	// NULL?
	if(e==NULL)
	{
		panic("[Oops!] jtmfs_cedirend called with e==NULL\n");
		return;
	}
 
	// Clear entry
	memset(e, 0, sizeof(JTMFS_FILE_ENTRY));
}

// jtmfs_createdirectory
//
// dblock = block where the directory are about to be stored at
//	This fuction writes an independent directory on the disk.
//	It is not a subdirectory, unless otherwise specified later on.
//
//  	Note!: the block where the directory will be made at,
//	MUST be free, if not so, no directory will be created.
//
int jtmfs_createdirectory(int device_nr, int dblock, int backdb,
		const char *name)
{
	static JTMFS_FILE_ENTRY *fe[100];
	static unsigned char *p;
	static DWORD ad;
	static int i,i2,i3,i4,alal;
	static BYTE *buf; // It gotta be atleast as big as largest possible block
	static int flags;
	static char temp[65536];
	char dirname[256];

	//-----------------------------------------------------------------------------
	//
	if(!isValidDevice(device_nr)) return 10;
	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))return 1;
	//
	if( illegal(name) )
		panic("ILLEGAL POINTER FOR NAME STRING, CREATE DIRECTROY");
	if( strlen(name)<=0 )
	{
		printk("%s: name string is empty!\n",
			__FUNCTION__);
		return 1;
	}
	printk("%s: (1)\n", __FUNCTION__);

	//-----------------------------------------------------------------------------
	// Get only the directory name(e.g. resolve from path string).
	for(i=strlen(name)-1,i2=strlen(name); i>=0 && name[i]!='/' && i2; i2--,i--);
	i++;
	strcpy(dirname, name+i);

	//
	alal = 0;
	printk("%s: (2)\n", __FUNCTION__);

	// --------------------------------------------------------------------
	dirdb_clearCacheForDB(device_nr, dblock);

	// TODO: here we should check for the validity of the device_nr
	//
	if(jtmfat_getblock(device_nr,dblock))
	{
		//
		alal = 1;
		//panic("jtmfs_createdirectory: Oops! Attempting to create directory on already allocated block");
	}
	printk("%s: (3)\n", __FUNCTION__);

	// Warning, this must be freed afterwards:
	flags=get_eflags(); disable();
	buf = temp;
	set_eflags(flags);

	// Wipe the block buffer.
	// Note: I didn't do this right away, the result was that 
	// the directory was very messy... And it messed the workability of
	// the entire directory mechanism... :]
	ReportMessage("%s: memset\n", __FUNCTION__);
	memset(buf, 0, 16384);

	printk("%s: (4)\n", __FUNCTION__);

	// init the 100 entries array
	ReportMessage("%s: Initializing 100 entries array.\n", __FUNCTION__);
	for(i=0; i<100; i++)
	{
		ad=((DWORD)buf)+i*sizeof(JTMFS_FILE_ENTRY);
		fe[i]=((DWORD)ad);
	}

	printk("%s: (5)\n", __FUNCTION__);

	// Create few basic directory references
	//				first	last
	//				block	block
	ReportMessage("%s: A few cedirs...\n", __FUNCTION__);
	jtmfs_cedir(	fe[0], ".",  	dblock,dblock );
	jtmfs_cedir(	fe[1], "..", 	backdb,backdb );
	// Add directory name into directory.
	jtmfs_cedir(	fe[2], dirname,	0,0 );
	fe[2]->type = JTMFS_ID_DIRNAME;
	fe[2]->length = -1;
	// Set end of directory.
	jtmfs_cedirend(fe[3]);

	printk("%s: (6)\n", __FUNCTION__);

	// Write on the drive
	printk("%s: writeblock(%d,%d, %x)\n", __FUNCTION__, device_nr,dblock,buf);
	writeblock(device_nr, dblock, buf);
	printk("%s: Done.\n", __FUNCTION__);
 
	printk("%s: (7)\n", __FUNCTION__);

	//
	ReportMessage("%s: setblock\n", __FUNCTION__);
	// Allocate it to the FAT(End of chain, so which means
	//				that there is no next block)
	if(!alal)
		jtmfat_setblock(device_nr,dblock, 0xFFFFFFF8);
	ReportMessage("%s: Done.\n", __FUNCTION__);


	printk("%s: (8)\n", __FUNCTION__);

 
	//
	return 0;
}

//
int jtmfs_getrootdir(int device_nr)
{
	if(!isValidDevice(device_nr)) return 0;

	// We will just pass this request for the JTMFAT
	// (JTMFAT nowadays does this stuff).
	return jtmfat_getrootdir(device_nr);
}

// Create root directory(used by formatdrive)
int jtmfs_createrootdir(int dnr)
{
	int rdnr;
	char *str_root="/";

	// Device validity check
	if(!isValidDevice(dnr))
	{
		printk("%s: DNR %d is not a valid device!",
			__FUNCTION__, dnr);
		return 2;
	}
	// JTMFS required
	if(!jtmfat_isJTMFS(dnr))
	{
		printk("%s: DNR %d is not formatted! Cannot create directory.",
			__FUNCTION__, dnr);
		return 1;
	}

	// Get root directory block #
	rdnr = jtmfs_getrootdir(dnr);
	// Root directory block validity check
	if(rdnr<=0)
	{
		printk("%s: Illegal root dir block = %d!\n",
			__FUNCTION__, rdnr);
	}
	else
	{
		// Create root directory
		printk("%s: Root dir block = %d\n",
			__FUNCTION__, rdnr);
		printk("%s: Creating directory on it:\n", __FUNCTION__);
		jtmfs_createdirectory(dnr, rdnr, 0, str_root);
		printk("%s: Directory creation completed.\n", __FUNCTION__);
	}
	return 0;
}

// Lists root directory's contents
int jtmfs_dirroot(int device_nr)
{
	if(!isValidDevice(device_nr)) return 1;

	// JTMFS required
	//
	jtmfs_dir( device_nr, jtmfs_getrootdir(device_nr) );

	//
	return 0;
}

// Checks if it is the last entry?
// Return value: non-zero if true, else zero
int jtmfs_ffIsLast(int device_nr,DIRWALK *dw)
{
	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))return 1;

	//
	if( !strlen(dw->fe->name) ) return 1;
 
	//
	return 0;
}

int jtmfs_clearwbuf(int device_nr,DIRWALK *dw)
{
	//
	return 0;
}

// EXPAND FILE OR DIRECTORY CHAIN
// ------------------------------
//
// This function expands a file chain, so that it will contain
// one more block.
// Return value: new block's number
// Negative value means error.
//
int jtmfs_expandchain(int device_nr,int block)
{
	int new_block;

	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))return -1;

	// TODO: make a device_nr validity check here
	if(1)
	{
		//
		ReportMessage(">");

		// Is 'block' the last block in the chain?
		// not?->error
		if( jtmfat_getblock(device_nr, block)!=0xfffffff8 )
		{
			//
			DEBUGINFO
			dprintk("jtmfs_expandchain: error, the offered block is not last one in the chain\n");
			return -2;
		}
		else
		{
			// Get a new free block
			new_block = jtmfat_getfreeblock(device_nr);
	
			//
			if(!new_block)
			{
				DEBUGINFO
				dprintk("jtmfs_expandchain: Out of disk space.\n");
				return -3;
			}
	
			//
			if(new_block == block)
			{
				DEBUGINFO
				dprintk("jtmfs_expandchain error: new_block(%d) == block(%d).\n",new_block,block);
				dprintk("Perhaps this is a logic error in jtmfat_getfreeblock().\n");
				panic("oops");
			}
	
			// Set 'block' to continue at 'new_block'
			jtmfat_setblock(device_nr, block,new_block);
			if( jtmfat_getblock(device_nr,block)!=new_block )
			{
				DEBUGINFO
				dprintk("jtmfs_expandchain: fatset failed.\n");
				return -6;
			}	
	
			// Then set 'end of chain marking' of chain at the 'new_block'
			jtmfat_setblock(device_nr,new_block, 0xFFFFFFF8);

			//
			if( jtmfat_getblock(device_nr,block)!=new_block )
			{
				DEBUGINFO
				panic("jtmfs_expandchain: fatset failed.\n");
				return -4;
			}
		
			//
			return new_block;
		}
	}
 	else
 	{
		DEBUGINFO
		print("jtmfs_expandchain: fatal error\n");
		return -5;
	}
	 
	//
	return 0;
}

// Reads current block location at the buffer
// I tried to optimize it to load only when neccesary,
// but it didn't really work right :\
// TODO: WHY it is needed to load the block everytime?
int jtmfs_ffreadlocation(int device_nr,DIRWALK *dw)
{
	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))return 1;

	//
	if(dw)
	{
		if(dw->isWalking)
			dirdb_flushRequirement(dw);
	}
 
	//
	return 0;
}

// Writes current buffer to the disk
int jtmfs_ffwritelocation(int device_nr, DIRWALK *dw)
{
	// JTMFS required
	if(!jtmfat_isJTMFS(device_nr))
	{
		dprintk("%s: DNR%d = non-JTMFS file system\n",
			__FUNCTION__, device_nr);
		return 1;
	}

	//
	if(dw!=NULL)
	{
		dirdb_flushRequirement(dw);
	}
	else
	{
		dprintk("%s: ERROR Called with dw==NULL\n",
				__FUNCTION__);
	}
 
	//
	return 0;
}

/* typedef struct
 * {
 *  // sizeof, bytes :
 *  // 0x00
 * 00 char			name[31];
 * 20 unsigned short int 	type;
 * 22 unsigned int	length;
 * 26 unsigned int      protection;
 * 2a unsigned char		owner;
 * 2b unsigned int	creationdate;
 * 2f unsigned int	lastaccessdate;
 * 33 unsigned int	lastchangedate;
 * 37 unsigned int	track;
 * 3b unsigned int	block;
 * 3f unsigned char	unused;
 *  // 0x40 .... bingo
 *  //
 * }JTMFS_FILE_ENTRY;
 */

//
int jtmfs_entryIsDeleted(JTMFS_FILE_ENTRY *fe)
{
	if(fe->length==-1)
			return 1;
			else
			return 0;
}

// jtmfs_deletefile dnr,fname,dirblock
// -----------------------------------
//
// Sets the direntry of the specified file as "free",
// and frees all blocks that are allocated by the file,
// if any.
//
// New: if dirblock equals to zero, it'll be treated as
// the rootdirectory(default).
//
int jtmfs_deletefile(int device_nr,const char *fname,int dirblock)
{
	static DIRWALK dw;
	static int delete_blocknr;
	static int dirblock1;

	// Sanity check
	if(dirblock>=getsize(device_nr))return 1;

	// Get fixed dirblock
	// If dirblock is zero, get the root directory(default).
	if(!dirblock) 
		dirblock1 = jtmfs_getrootdir(device_nr);
	else
		dirblock1 = dirblock;

	//
	InitDW(&dw);

	// Find the file
	if( !jtmfs_ffindfile(device_nr,fname,dirblock1,&dw) )
	{
		// Not found?
		// Note: Heh, dir walk won't be ended when it has not even started!
		//// EndDirWalk(&dw);
		return 1;
	}

	// After 'ffindfile' dirblock* parameter will be not used.
	// Instead the structure 'fe' will be used. If any data is
	// allocated.
	if(dw.fe->FirstDataBlock)
	{
		//
		delete_blocknr = dw.fe->FirstDataBlock;

		// --- Currently the deletchain seems not to work properly ---
		// Try to delete the chain as we are now deleting the file
		if( jtmfs_deletechain(device_nr,delete_blocknr) )
		{
			//
			EndDirWalk(&dw);
			// Return non-zero if not succeeded
			return 2;
		}
	}

	// At this point the file should not have any reserved blocks,
	// so we can set the directory entry as free as well.
	//
	// Remove the entry from the directory
	// This is done by setting the file's length to minus one(-1).
	//
	dw.fe->length = -1;

	// Then we write the block back.
	jtmfs_ffwritelocation(device_nr,&dw);

	//
	EndDirWalk(&dw);

	// We're clear!
	return 0;
}

// ------------------------------------------------------------------------------
// jtmfs_renamefile(dev, "floppy:lala", "lolo", rootdir);
//
// Renames a file or files.
// REN [drive:][path]filename1 filename2.
// Note that you cannot specify a new drive or path for your destination file.
//
int jtmfs_renamefile(int device_nr,
		const char *OldName, const char *NewName,
			int dirblock)
{
	static DIRWALK dw;
	static int delete_blocknr;
	static int dirblock1;

	// Sanity check
	if(dirblock>=getsize(device_nr))return 1;

	// Get fixed dirblock
	// If dirblock is zero, get the root directory(default).
	if(!dirblock) 
		dirblock1 = jtmfs_getrootdir(device_nr);
	else
		dirblock1 = dirblock;

	//
	InitDW(&dw);

	// Find the file
	if( !jtmfs_ffindfile(device_nr,OldName,dirblock1,&dw) )
	{
		// Not found?
		// Note: Heh, dir walk won't be ended when it has not even started!
		//// EndDirWalk(&dw);
		return 1;
	}

	// MODIFY FILE NAME
	//
	strcpy(dw.fe->name, NewName);

	// Then we write the block back.
	jtmfs_ffwritelocation(device_nr,&dw);

	//
	EndDirWalk(&dw);

	// We're clear!
	return 0;
}

// ------------------------------------------------------------------------------
// jtmfs_setsize(dev, "floppy:lala", new_size, rootdir);
//
// Set file fixed size (in bytes).
//
// This function only sets the visible size, but does
// not affect the real allocated blocks amount.
//
int jtmfs_setsize(int device_nr,
		const char *OldName, int newSize,
			int dirblock)
{
	DIRWALK dw;
	int delete_blocknr;
	int dirblock1;

	// Sanity check
	if(dirblock>=getsize(device_nr))return 1;

	// Get fixed dirblock
	// If dirblock is zero, get the root directory(default).
	if(!dirblock) 
		dirblock1 = jtmfs_getrootdir(device_nr);
	else
		dirblock1 = dirblock;

	//
	InitDW(&dw);

	// Find the file
	if( !jtmfs_ffindfile(device_nr, OldName, dirblock1, &dw) )
	{
		// Not found?
		// Note: Heh, dir walk won't be ended when it has not even started!
		//// EndDirWalk(&dw);
		printf("%s: file not found '%s'\n",
			__FUNCTION__, OldName);
		return 1;
	}

	// MODIFY FILE NAME
	//
	dw.fe->length = newSize;

	//
	ReportMessage("%s: '%s': File size set to %d bytes.\n",
		__FUNCTION__, OldName, dw.fe->length);

	// Then we write the block back.
	jtmfs_ffwritelocation(device_nr, &dw);

	//
	EndDirWalk(&dw);

	// We're clear!
	return 0;
}

// DELETECHAIN
// -----------
//
// Deletes a file/dir chain
// (frees all the blocks of the specified chain)
//
// Returns zero on success
// and non-zero on error
//
// NEW: now ignores chain that is allocated on block zero.
//
// ** TODO **
// Something wrong here, it does not delete the chain.
// Instead the data is left there, maybe
// dump of the fat table will tell what has
// been happended.
// But it definetly does not deallocate the chain.
//
int jtmfs_deletechain(int device_nr, int blocknr)
{
	DWORD i,i2,i3,i4,b,block,first;

	// Sanity check
	if(blocknr>=getsize(device_nr))return 1;

	//
	for(block=blocknr,first=1; ; )
	{
		// Check if the block is the last block
		if( (b=jtmfat_getblock(device_nr,block))==0xfffffff8 )
		{
			jtmfat_setblock(device_nr,block,0);
			return 0;
		}

		// Nothing allocated?(First block on chain and zero block reference)
		if(first && !b)
		{
			dprintk("jtmfs_deletechain: nothing allocated, block=%d, blocknr=%d, b=%d, first=%d\n",
					block,blocknr,b,first);
			return 1;
		}

		//
		first=0;

		// Unexpected code?
		if(b>=0xfffffff0)
		{
			dprintk("deletechain: FAT integrity error, unexpected F-code");
			dsleep(1);
			//waitkey();
		}

		// Jump to itself?
		if(b==block)
		{
			dprintk("deletechain: FAT integrity error infinite jump %d => %d\n",b,block);
			dsleep(1);
			return 1;
		}

		// Free first block
		jtmfat_setblock(device_nr,block,0);

		// Jump to next block on chian
		block=b;
	}

	//
	return 0;
}

// The real getfilesize function.
// Returns size of the file in bytes.
//
int jtmfs_getfilesize(int dnr, const char *fname, int db)
{
	static DIRWALK dw;

	// Find the file
	if( !jtmfs_ffindfile(dnr,fname,db,&dw) )
	{
		return -1;
	}

	return dw.fe->length;
}

// Returns size of the specified file.
//
// PERFORMANCE NOTIFICATION:
// This is a determistic diagnostics function and it is SLOW.
// It is easier to just read the file entry and use it's
// file size identifier variable to determine size of the file.
//
int jtmfs_getfileblocksize(int device_nr, const char *fname, int dblock)
{
	static DWORD i,i2,i3,i4,db,block,new_block,amount_to_allocate,amount;
	static DIRWALK dw;

	// Find the file
	if( !jtmfs_ffindfile(device_nr,fname,dblock,&dw) )
	{
		// Not found? HEH! RETURN ZERO IF NOT FOUND.
		// We simply can't return other.
		return 0;
	}

	// Get location of the data
	block = dw.fe->FirstDataBlock;

	// No data block? => return size 0
	if(!block)return 0;

	// BLOCK WALK CODE
	// Walk until we are at the end of the chain
	for(amount=1; ; amount++)
	{
		//
		db=jtmfat_getblock(device_nr,block);

		// End of chain? => break
		if(db==0xFFFFFFF8)break;
		// Reserved area? => break(Error situation really!)
		if(db==0xFFFFFFF7)break;
		//
		if(db>=0xFFFFFFF0)
		{
			dprintk("FAT error in chain(media failure?)\n");
			dprintk("jtmfs_getfilesize: error, unknown FAT value(0x%x) on block %d\n",db,block);
			dsleep(1);
//			waitkey();
			EndDirWalk(&dw);
			return 2;
		}
		if(db==block)
		{
			dprintk("FAT error in chain(media failure?)\n");
			dprintk("jtmfs_getfilesize: infinite jump to itself(FAT)\n");
			dsleep(1);
//			waitkey();
			EndDirWalk(&dw);
			return 3;
		}

		//
		block=db;
	}

	//
	EndDirWalk(&dw);

	//
	return amount;
}

// Return value:
// 0 = Entry not found.
// 1 = Entry found(FILE).
// 2 = Entry found(DIRECTORY).
int jtmfs_fexist(int dnr, char *fname, int dblock)
{
	static DIRWALK dw;
	int rv;

	//
	InitDW(&dw);

	// Is it a device name?
	if( driver_getdevicenrbyname(fname)!=-1 )
	{
		// Describe it as a directory:
		return 2;
	}

	//
	if( jtmfs_ffindfile(dnr, fname, dblock, &dw) )
	{
		// Determine return value upon entry type
		switch(dw.fe->type)
		{
			// DIRECTORY
			case JTMFS_ID_DIRECTORY:
			rv = 2;
			break;

			// DIRNAME
			case JTMFS_ID_DIRNAME:
			rv = 3;
			break;

			// FILE
			default:
			rv = 1;
			break;
		}

		//
		EndDirWalk(&dw);
		return rv;
	}
	else return 0;
}

//

