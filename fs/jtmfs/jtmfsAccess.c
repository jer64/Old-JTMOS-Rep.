// ======================================================================================
// jtmfsAccess.c - JTMOS File System functions for Postman.
//
// (C) 2002-2020 Jari Tuominen(jari.t.tuominen@gmail.com).
//
// jtmfs_ functions now do file name translation,
// which includes device detection.
//
// int jtmfs_CreateFile(const char *devname,const char *fname,int blocks);
// jtmfs_DeleteFile dnr, name, db
// jtmfs_ReadFile dnr, name, db
// jtmfs_WriteFile dnr, name, db
// jtmfs_RenameFile dnr,
//		OldName, NewName,
//		db
// jtmfs_AddBlocks dnr, fname, db, amount
// ======================================================================================
#include "kernel32.h"
#include "jtmfat.h" // getfreeblock
#include "jtmfs.h" // comes with jtmfsAccess as well
#include "ramdisk.h" // default_ramdisk
#include "fsdos.h"
#include "fname_translation.h" // tell_fname
#include "root.h"
#include "jtmfsAccess.h"
#include "jtmfsPath.h"
#include "ReadChain.h"
#include "WriteChain.h"

//
JTMFSACCESS jtmfsAccess;

// Get current work directory.
// Converts current DNR/DB to a path string.
int jtmfs_GetCWD(char *path, int max_len)
{
	int dnr,db,sdnr,sdb,odnr,odb,limit;
	static char name[256],tmp[256],dn[256],work[256];
	DIR *dir;
	JTMFS_FILE_ENTRY *file_entry;

	// Define current directory location.
	dnr = GetCurrentDNR();
	db = GetCurrentDB();
	// Specify starting directory location.
	sdnr=dnr; sdb=db;

	// Get device name string.
	driver_getdevicename(dnr, dn);

	// Store initial path value.
	// E.g. /hda.
	// But if the drive is a root device,
	// we'll just use "/".
	if(dnr==getRootDNR())
	{
		// It is a root drive.
		strcpy(path, "/");
	}
	else
	{
		// It is not root drive,
		// specify device seperately.
		sprintf(path, "/%s/",
			dn);
	}

	//
	strcpy(work, "");

	// Follow down the path.
	for(limit=200; limit; limit--)
	{
		//
		printk("%s: DNR/DB=%d/%d\n",
			__FUNCTION__, GetCurrentDNR(), GetCurrentDB());
			

		// Convert DNR/DB to name.
		if( jtmfs_GetDirName(GetCurrentDNR(), GetCurrentDB(), name) )
		{
			//
			printk("%s: Could not convert DNR/DB to name string.\n",
				__FUNCTION__);

			// Failure.
			break;
		}

		// Is this the root directory?
		if(!strcmp(name, "/"))
		{
			// Yes, it is the root directory.
			// We're done here.
			printk("Answer string equals %s, we're at root directory now.\n",
				name);
			printk("Breaking off now.\n");
			break;
		}

		// Empty name string means error.
		if(!strcmp(name, ""))
		{
			printk("Got empty answer string.\n");
			printk("Breaking off now.\n");
			break;
		}

		// Affect work path.
		sprintf(tmp, "%s/", name);
		strcat(tmp, work);
		strcpy(work, tmp);

		// Store old directory location.
		odnr = GetCurrentDNR();
		odb = GetCurrentDB();

		// Go to directory below.
		jtmfs_chdir("..");

		// We are still at same place? Assume end.
		if(GetCurrentDNR()==odnr && GetCurrentDB()==odb)
		{
			//
			printk("%s: chdir('..') failed - returned same DNR/DB position.\n",
				__FUNCTION__);
			// We could also change return value here to indicate an error.
			// (TODO)
			break;
		}
	}

	// Build up path.
	strcat(path, work);

	// If string is of >1 length . .
	if(strlen(path)>1)
	{
		// Remove trivial '/' at the end of string.
		if(path[strlen(path)-1]=='/')
		{
			path[strlen(path)-1] = 0;
		}
	}

	// We're done here.
	return 0;
}


// TRUE =	Drive is formatted and works properly.
// FALSE =	Drive is not formatted.
int jtmfs_isDriveReady(int dnr)
{
	//
	if(!isValidDevice(dnr))
		return FALSE;
	return jtmfat_isJTMFS(dnr);
}

// jtmfs_GetDirName dnr db name
// Converts DNR/DB to directory name.
// Stores directory name in string pointed by name.
// Zero = success.
// Non-zero = error.
int jtmfs_GetDirName(int dnr,int db, char *name)
{
	static DIRWALK dw;

	// int dirdb_GetDirName(int dnr,int db, char *dest_name)
	if( dirdb_GetDirName(dnr,db, name) )
	{
		return 0;
	}
	else
	{
		// Dir name could not been resolved.
		strcpy(name, "");
		return 1;
	}
}

//
int jtmfs_GetFileDataBlock(int dnr, int db, const char *name)
{
	DIRWALK dw;
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 0;

	//
	resolveFileNamePath(name, &dnr, &db, &r);
	jtmfs_ffindfile(dnr, r.name, db, &dw);
	return dw.fe->FirstDataBlock;
}

//
int jtmfs_DeleteFile(int dnr, const char *name, int db)
{
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 1;

	//
	resolveFileNamePath(name, &dnr, &db, &r);
	// Not valid device?
	if( !isOK(dnr)  ) return 1;

	//
	return jtmfs_deletefile(dnr, r.name, db);
}

// jtmfs_CreateFile [dnr] [file name] [number of blocks to allocate]
//
// Non-zero = error
// Zero = no error
//
// Possible return values
// 1: Creation of file structure failed or file name not legal
// ....
//
// New: If file already exists by the name, it'll be deleted.
//
// "devname" can be acquired by f.e. driver_getdevicenrbyname(syssh.str_device).
// When "blocks" -parameter equals zero, no blocks will be added.
// Function is not multitasking capable.
//
int jtmfs_CreateFile(int dnr,int db,
		const char *name,int blocks)
{
	JTMFS_FILE_ENTRY fe;
	int i,i2,i3,i4,rv;
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 10;

	//
	if(name==NULL)return 11;

	//
	resolveFileNamePath(name, &dnr, &db, &r);

	//
	dprintk("%s: Device validity check.\n", __FUNCTION__);
	// Not valid device?
	if( !isOK(dnr) )
		return 1986;

	//
	dprintk("%s: Entry creation.\n", __FUNCTION__);
	//
	if( jtmfs_fentry(&fe,
	       r.name,0,0,
	       0,0,
	       0,0,0,
	       0,0) )
	{
		return 12;
	}

	//
	for(i=0; i<2; i++)
	{
		//
		dprintk("%s: Adding entry                \r", __FUNCTION__);
		//
		if( (rv=jtmfs_adddirentry(dnr, db, &fe))==10 )
		{
			//
			dprintk("%s: File '%s' already exists, replacing.\n",
				__FUNCTION__, fe.name);
			// Aha, file already exists? => Delete.
			if( jtmfs_DeleteFile(dnr, r.name, db) )return 1;
			continue;
		}
		if(rv)
			// Creation failed.
			return 100;
		break;
	}

	// Assign some blocks for it(grow up it's chain=file size).
	// But don't add any blocks if blocks equals zero.
	if(blocks)
	{
		dprintk("%s: jtmfs_add_blocks_for_entry       \r", __FUNCTION__);
		jtmfs_add_blocks_for_entry(dnr, r.name,
			db, blocks);
	}

	//
	dprintk("\n%s: Work done here.\n", __FUNCTION__);

	//
	return 0;
}

//
int jtmfs_LastBlock(DWORD block)
{
	//
	if(block>=0xFFFFFFF0)return 1;
	return 0;
}

// Return value:
// 0 =		Success
// Non-zero =	Error
// -1 =		File not found.
//
// Returns size of the specified file in bytes.
int jtmfs_GetFileSize(int dnr, const char *fname, int db)
{
	RESOLVE r;
	int d1;

	//
	DEBUGLINE

	//
	if( !isValidString(fname) )	return 1;

	//--------------------------------------------------------
	// Is it a device?
	if( (d1=driver_getdevicenrbyname(fname))!=-1 )
	{
		// Get device size in bytes and set it as return value
		return	getblocksize(d1)*getsize(d1);
	}

	//
	resolveFileNamePath(fname, &dnr, &db, &r);

	// Not valid device?
	if( !isOK(dnr) )
	{
		dprintk("%s: Invalid device %d\n",
			__FUNCTION__, dnr);
		return 0;
	}

	return jtmfs_getfilesize(dnr,r.name,db);
}

// Write data to file (length, offset)
int jtmfs_WriteFile(int dnr,
			const char *name,int db,
			BYTE *buf, char showMeter,
			int preciseLength,
			int offset)
{
	DWORD d,newd;
	int i,i2,i3,i4,ii,ii2,ii3,ii4,offs,expire;
	DIRWALK dw;
	char *tmp;
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 0;

	//
	tmp = jtmfsAccess.rwtmp;

	//
	resolveFileNamePath(name, &dnr, &db, &r);

	// Not valid device?
	if( !isOK(dnr) ) return 10;

	expire=0;
retry:
	//
	dprintk("%s: jtmfs_ffindfile '%s'\n",  __FUNCTION__, r.name);
	if( !jtmfs_ffindfile(dnr, r.name, db, &dw) )
	{
		// Not found => We don't create one here, return 1.
		return 1;
	}

	// Get data block
	d = dw.fe->FirstDataBlock;

	// Check that it is a data block, if not then handle the situation
	if(d==0)
	{
		// jtmfs_add_blocks_for_entry(int device_nr,char *fname,
		//	int dblock,amount)
		// If no data blocks => Allocate required amount
		ReportMessage("%s: jtmfs_add_blocks_for_entry '%s' length=%d\n",
			__FUNCTION__, r.name, preciseLength);
		if( jtmfs_add_blocks_for_entry(dnr,
				r.name, db,
				((offset+preciseLength)/getblocksize(dnr))+1 )==10 )
		{
			ReportMessage("%s: Disk full? (%s, %d bytes requested)\n",
				__FUNCTION__, r.name, preciseLength);
			return 2;
		}
		expire++;
		if(expire>=2)
		{
			ReportMessage("%s: Writing of file %s failed.\n",
				__FUNCTION__, r.name);
			return 3;
		}
		goto retry;
	}

	//
	ReportMessage(" -- jtmfs_WriteFileChain:\n");
	return jtmfs_WriteFileChain(buf, dnr, d,
		preciseLength,
		offset);
}

// Reads specified amount of blocks from the specified file
int _jtmfs_ReadFile(const char *caller,
			int dnr, const char *name,
			int db,
                        BYTE *buf, char showMeter,
			int preciseLength,
			int offset)
{
	int i,i2,i3,i4,d,newd,ii,ii2,ii3,ii4,offs,amount;
	DIRWALK dw;
	char *tmp;
	RESOLVE r;

	//
	dprintk("%s was called by %s\n",
		__FUNCTION__, caller);

	//
	FFCHECK

	// Estimate amount of blocks
	amount = (preciseLength/getblocksize(dnr))+1;

	//
	FFCHECK

	//
	tmp = jtmfsAccess.rwtmp;

	//
	FFCHECK

	//
	DEBUGLINE

	//
	FFCHECK

	//
	if( !isValidString(name) )	return 0;

	//
	FFCHECK

	//
	resolveFileNamePath(name, &dnr, &db, &r);

	//
	FFCHECK

	// Not valid device?
	if( !isOK(dnr) ) return 10;

	//
	FFCHECK

	//
	if( !jtmfs_ffindfile(dnr, r.name, db, &dw) )
	{
		// Not found => We don't create one here, return 1.
		return 1;
	}

	//
	FFCHECK

	// Get data block
	d = dw.fe->FirstDataBlock;

	//
	return jtmfs_ReadFileChain(buf, dnr, d,
		preciseLength,
		offset);
}

// Return value:
// 0 = OK
// 1 = Error
int jtmfs_AddBlocks(int dnr, const char *name, int db, int amount)
{
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 1;

	//
	resolveFileNamePath(name, &dnr, &db, &r);
	if( !isOK(dnr) )	return 1;

	//
	jtmfs_add_blocks_for_entry(dnr, r.name, db, amount);
	return 0;
}

// File existance checker.
int jtmfs_Fexist(int dnr, const char *name, int db)
{
	RESOLVE r;

	//
	DEBUGLINE

	//
/*	printk("%s: loc=%d/%d, name=%s.\n",
		__FUNCTION__, dnr,db, name);*/

	//
	if( !isValidString(name) )
	{
		printk("%s: Name is invaldi string.\n",
			__FUNCTION__);
		return 0;
	}

	//
	resolveFileNamePath(name, &dnr, &db, &r);
	if( !isOK(dnr) )
	{
		printk("%s: File system is invalid for loc %d/%d.\n",
			__FUNCTION__, dnr,db);
		return 0;
	}

	//
/*	printk("%s: Calling jtmfs_fexist with jtmfs_exist(%d, %s, %d);\n",
		__FUNCTION__, dnr, r.name, db);*/

	//
	return jtmfs_fexist(dnr, r.name, db);
}

// Set file fixed size (in bytes).
int jtmfs_SetSize(int dnr,
		const char *name, int newSize,
		int db)
{
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(name) )	return 1;

	//
	resolveFileNamePath(name, &dnr, &db, &r);
	if( !isOK(dnr) )	return 1;

	//
	return jtmfs_setsize(dnr, r.name, newSize, db);
}

//
int jtmfs_RenameFile(int dnr,
		const char *OldName, const char *NewName,
		int db)
{
	RESOLVE rOLD;
	RESOLVE rNEW;
	int tmp;

	//
	DEBUGLINE

	//
	if( !isValidString(OldName) || !isValidString(NewName) )
		return 1;

	//
	resolveFileNamePath(OldName, &dnr, &db, &rOLD);
	resolveFileNamePath(NewName, &tmp, &tmp, &rNEW);
	if( !isOK(dnr) )	return 1;

	//
	return jtmfs_renamefile(dnr, rOLD.name, rNEW.name, db);
}

//
DWORD GetCurrentDrive(void)
{
	DEBUGLINE
	return scall(SYS_GETCURDEV, 0,0,0, 0,0,0);
}

// chdir wrapper
int IntelligentCHDIR(const char *path)
{
	DEBUGLINE
	return jtmfs_chdir(path);
}

// Used by the system call "chdir"
// Return value:
// Zero = success.
// Non-zero = error.
int jtmfs_chdir(const char *pathi)
{
	char tmp[256];
	int i,i2,i3,i4,old_dnr,old_db;

	//
	if(pathi==NULL)return 3;
	if(!strlen(pathi))return 4;
	if( !isValidString(pathi) )	return 5;
	DEBUGLINE

	// Check out for root first
	if(pathi[0]=='/')
	{
		// Go to root, because so was adviced to be done
		if( _jtmfs_chdir("/") )return 2;
	}

	//
	old_dnr = GetCurrentDNR();
	old_db = GetCurrentDB();

	//
	for(i=0; ; )
	{
		// Skip past possible '/'
		for(; i<100; i++)
		{
			if(!pathi[i])goto endi;
			if(pathi[i]!='/'){ goto ok2; }
		}
		// Error in string(2):
		break;
ok2:

		// Copy the directory name
		for(i2=0; i<100; i++,i2++)
		{
			if(!pathi[i] || pathi[i]=='/')goto ok;
			tmp[i2]=pathi[i];
		}
		// Error in string(1):
		break;
ok:
		//
		tmp[i2]=0;

		// Commence chdir -operation
		if( _jtmfs_chdir(tmp) )
		{
			// Error! Return back to where we came from.
			SetThreadDNR(GetCurrentThread(), old_dnr);
			SetThreadDB(GetCurrentThread(), old_db);
			return 1;
		}
		if(i>=strlen(pathi))break;
		//
		dprintk(".");
	}
endi:
	return 0;
}

// Change to one place at once(part of jtmfs_chdir)
int _jtmfs_chdir(const char *pathi)
{
	int dnr,old_dnr,old_db,acdb,rootdnr;
	char str[100];

	DEBUGLINE

	//
	if( !isValidString(pathi) )	return 5;

	//
	old_dnr = GetCurrentDNR();
	old_db = GetCurrentDB();

	//
	strcpy(str, pathi);
	if( str[strlen(str)-1]==':' )
	{
		str[strlen(str)-1] = 0;
	}

	// Detect root directory
	if( !strcmp(str,"/") )
	{
		//
		ReportMessage("%s: OK I am going to root directory now\n",
			__FUNCTION__);
		ReportMessage("DNR/DB = %d/%d\n",
			GetCurrentDNR(),
			GetCurrentDB());
		//dsleep(1);
		// Move to root device, root
		if( jtmfs_isDriveReady(getRootDNR()) && isOK(getRootDNR()) )
		{
			dprintk("%s: Root directory is OK, that's good.\n",
				__FUNCTION__);
			dprintk("%s: SetThreadDNR( GetCurrentThread(), getRootDNR() );\n",
				__FUNCTION__);
			SetThreadDNR(GetCurrentThread(),
				getRootDNR());
			dprintk("%s: SetThreadDB( GetCurrentThread(), jtmfat_getrootdir(GetCurrentDNR()) );\n",
				__FUNCTION__);
			SetThreadDB(GetCurrentThread(),
				jtmfat_getrootdir(GetCurrentDNR()));
		}
		else
		{
			ReportMessage("%s: WARNING: Root drive(DNR %d) is not formatted!\n",
				__FUNCTION__, getRootDNR());
		}
		ReportMessage("%s: OK I am finished here.\n",
			__FUNCTION__);
		return 0;
	}

	// Try device change
	dnr = device(str);
	// Device is known?
	if(dnr>0)
	{
		// Only change if device is OK
		if( jtmfs_isDriveReady(getRootDNR()) && isOK(dnr) )
		{
			dprintk("Changing to block device %d.\n", dnr);
			// Define as a new current device
			SetThreadDNR(GetCurrentThread(), dnr);
			// Change root directory as default
			SetThreadDB( GetCurrentThread(),
				jtmfat_getrootdir(GetCurrentDNR()) );
			dprintk("Change complete: DNR/DB=%d/%d.\n",
					GetCurrentDNR(),
					GetCurrentDB());
			return 0;
		}
		else
		{
			// Error! Return back to where we came from.
			SetThreadDNR(GetCurrentThread(), old_dnr);
			SetThreadDB(GetCurrentThread(), old_db);
			return 1;
		}
	}
	else	dprintk("%s: is not a block device.\n", str);

	// Try directory
	if(jtmfs_Fexist(GetCurrentDNR(), str, GetCurrentDB())==2)
	{
		//
		acdb = jtmfs_GetFileDataBlock(
			GetCurrentDNR(),
			GetCurrentDB(),
			str);
		if(acdb==0)
		{
			// Zero block means let's go to root drive, root directory.

			// Get root drive.
			rootdnr = getRootDNR();
			// Switch to root drive, root directory.
			SetThreadDNR(GetCurrentThread(),
					rootdnr);
			SetThreadDB(GetCurrentThread(),
					jtmfat_getrootdir(rootdnr) );
		}
		else
		{
			// Directory exists, let's change to it
			SetThreadDB(GetCurrentThread(), acdb);
		}
		return 0;
	}

	// Unable to handle request, report with return value 1.
	dprintk("%s: Unable to handle '%s'\n",
			__FUNCTION__,
			str);
		
	return 1;
}

// [parent_db]
//     |
//     \____[target_db]
//
int jtmfs_CreateDirectory(int dnr,int db, const char *dirname)
{
	JTMFS_FILE_ENTRY e;
	int newBlock;
	RESOLVE r;

	//
	DEBUGLINE

	//
	if( !isValidString(dirname) )	return 5;

	//
	resolveFileNamePath(dirname, &dnr,&db, &r);

        //
        printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printk("r.name = \"%s\" -- dirname = \"%s\"\n",
		r.name, dirname);
        printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

	// Device must be valid ...
	if( !isOK(dnr) )	return 1;

	// Reserve a new block for the new directory.
	newBlock = jtmfat_getfreeblock(dnr);
	jtmfat_setblock(dnr, newBlock, 0xFFFFFFF8);

	// Create entry.
	jtmfs_cedir(&e, r.name, newBlock, 0);
	// Add it.
	jtmfs_adddirentry(dnr, db, &e);
	//
	jtmfs_createdirectory(dnr, newBlock, db, dirname);

	// Return assumed "OK".
	return 0;
}

// Reads FAT tables at the start up
void jtmfs_PreloadFATS(void)
{
	int dnr;

	//
	DEBUGLINE

	//
	dnr = driver_getdevicenrbyname("hda");
	//if(dnr!=-1)	driver_TriggerCacheActivation(dnr);
	dnr = driver_getdevicenrbyname("ram");
	//if(dnr!=-1)	driver_TriggerCacheActivation(dnr);
}

//

