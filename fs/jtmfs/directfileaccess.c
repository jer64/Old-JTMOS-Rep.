//==============================================================================
// Direct File Access Functions
// (C) 2020 by Jari Tuominen.
// Added locking mechanism to prevent thread violations.
//==============================================================================
#include "basdef.h"
#include "kernel32.h"
#include "scs.h"
#include "devsys.h"
#include "jtmfs.h"
#include "jtmfat.h"
#include "devapi.h"
#include "sysgfx.h" // SetVideoMode, setvmode
#include "filedes.h"
#include "lock.h"
#include "directfileaccess.h"
//#include "dispadmin.h" // dispadmin_getcurrentmode
//#include "verify_ptr.h"

//
#define ddprintk dprintk

// Read/Write temporary block buffer
static BYTE *rwtmp=NULL;
static int l_rwtmp;

//
int renamefile(const char *oldpath, const char *newpath)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4;
	LOCK

	//
	if( oldpath && isValidString(oldpath) && strlen(oldpath)
		&&
		newpath && isValidString(newpath) && strlen(newpath) )
	{
jtmfs_RenameFile(GetCurrentDNR(), oldpath,newpath, GetCurrentDB());
	}

	//
	UNLOCK
	return 0;
}

// Create file
int createfile(const char *path)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,rv,devnr;
	
	LOCK

	//
	if( path && isValidString(path) && strlen(path) )
	{
		//
		ReportMessage("%s: Calling jtmfs_CreateFile, dnr=%d, db=%d, fname='%s'.\n",
			__FUNCTION__, GetCurrentDNR(), GetCurrentDB(), path);
rv = jtmfs_CreateFile(GetCurrentDNR(), GetCurrentDB(), path, 0);
	}
	else
	{
		//
		ddprintk("%s: Invalid call.\n");
		rv = 1984;
	}

	//
	UNLOCK
	return rv;
}

// Remove a file
int deletefile(const char *path)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,devnr;
	
	LOCK

	// Check path
	if( path && isValidString(path) && strlen(path) )
	{
jtmfs_DeleteFile(GetCurrentDNR(), path, GetCurrentDB());
	}

	//
	UNLOCK
	return 0;
}

// Read data from specified part of a file.
// Usage: readfile path, buf, offset, count.
int readfile(char *path, char *buf, int offset, int count)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,devnr,d1,l,bs,s,ii;
	DWORD ad,offset1,count1;
	char *tp;
	BYTE *tmp;
	void *ldbuf,*fname;
	
	LOCK

	//
	dprintk("%s: Read %d bytes from '%s', beginning from offset 0x%x\n",
		__FUNCTION__, count, path, offset);

	// Check amount of bytes to read
	if(count >= (1024*1024*1) || count <= 0)
	{
		dprintk("%s: illegal size %d\n",
			__FUNCTION__, count);
		return 0;
	}

	//
	// par1 = fname
	// par2 = load buffer
	// v1 = amount to load in bytes
	// v2 = read offset
	if(!isValidString(path))
	{
		printk("%s: INVALID STRING\n", __FUNCTION__);
		return 0;
	}

	//
	fname = path;
	ldbuf = buf;
	offset1 = offset;

	//
	FFCHECK

	//
	ddprintk("%s: '%s' 0x%x-0x%x\n",
		__FUNCTION__, path,offset1,offset1+count);

	//---------------------------------------------------------------------
	// Is it a device?
	if( (d1=driver_getdevicenrbyname(path))!=-1 )
	{
		// ************** TODO ****************** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// Read device.
		bs = getblocksize(d1);
		s = offset1/bs;		// start block offset
		l = (count/bs)+1 + s;	// end block offset
		count1 = count/bs+1;

		//
		if(rwtmp==NULL || l_rwtmp<(bs*count))
		{
			if(rwtmp)	free(rwtmp);
			l_rwtmp = bs*(count1+2);
			rwtmp = malloc(l_rwtmp);
		}
		memset(rwtmp, 0xBD, l_rwtmp);

		//
		tmp = rwtmp;

		// Read whole area at once.
		for(i=0,tp=tmp; i<(count+1); i++,tp+=bs)
		{
			//
			ReportMessage("r");
			readblock(d1, i+s, tp);
		}

		// Copy area from the tmp to client buffer.
		ad = tmp; ad += (offset1 % bs);
		memcpy(ldbuf,ad, count);

		//
		ReportMessage("\n");
		UNLOCK
		return 0;
	}

	//---------------------------------------------------------------------
	// par1 = fname
	// par2 = load buffer
	// v1 = amount to load in bytes
	// v2 = read offset
	if( path && buf && isValidString(path) && strlen(path) )
	{
		ReportMessage("%s: '%s', dnr=%d, db=%d, len=%d\n",
			__FUNCTION__, path,
			GetCurrentDNR(), GetCurrentDB(), count);
/*
// Reads specified amount of blocks from the specified file
int _jtmfs_ReadFile(const char *caller,
                        int dnr, const char *name,
                        int db,
                        BYTE *buf, char showMeter,
                        int preciseLength,
                        int offset)*/
		jtmfs_ReadFile(GetCurrentDNR(),path,GetCurrentDB(),
			buf, 0, count, offset);
		UNLOCK
		return 0;
	}

	//
	UNLOCK
	return 0;
}

// Write specified part to a file
// par1 = fname
// par2 = data buffer
// v1 = amount to write in bytes
// v2 = write offset
int writefile(const char *path,char *buf,int offset,int count)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,devnr,bs,l,d1,s,ii,count1;
	char *tp;
	BYTE *tmp;
	
	LOCK

	// Check amount of bytes to read
	if(count >= (1024*1024*1) || count <= 0)
	{
		ReportMessage("%s: illegal size %d\n",
			__FUNCTION__, count);
		UNLOCK
		return 0;
	}

	//
	if(!isValidString(path))
	{
		UNLOCK
		return 0;
	}

	//---------------------------------------------------------------------
	// Is it a device?
	if( (d1=driver_getdevicenrbyname(path))!=-1 )
	{
		// Read device
		bs = getblocksize(d1);
		s = offset/bs;		// start block offset
		l = (count/bs)+1 + s;	// end block offset
		count1 = count/bs;
		if(rwtmp==NULL || l_rwtmp<(bs*count))
		{
			if(rwtmp)	free(rwtmp);
			l_rwtmp = bs*(count1+2);
			rwtmp = malloc(l_rwtmp);
		}
		tmp = rwtmp;

		// Copy client area to the write buffer
		memcpy(tmp, buf, count1);

		// Write whole area at once
		for(i=0,tp=tmp; i<count1; i++,tp+=bs)
		{
			//
			ReportMessage("w");
			writeblock(d1, i+s, tp);
		}

		//
		ReportMessage("\n");
		
		//
		UNLOCK
		return 0;
	}

	//
	if( path && buf && isValidString(path) && strlen(path) )
	{
		//
		dprintk("%s: '%s', dnr=%d, db=%d, len=%d\n",
			__FUNCTION__,
			path, GetCurrentDNR(), GetCurrentDB(), count);

		//
		devnr = GetCurrentDNR();
		jtmfs_WriteFile(GetCurrentDNR(),
			path, GetCurrentDB(), buf,
			0, count, offset); // v1=amount, v2=offset
		UNLOCK
		return 0;
	}

	//
	UNLOCK
	return 0;
}

//
int getfilesize(const char *path)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,devnr,d1,sz;
	
	LOCK


	// Validate string
	if(!isValidString(path))
	{
		//
		UNLOCK
		// Invalid string
		return 0;
	}

	//--------------------------------------------------------
	// Is it a device?
	if( (d1=driver_getdevicenrbyname(path))!=-1 )
	{
		// Get device size in bytes and set it as return value
		sz = getblocksize(d1)*getsize(d1);
		UNLOCK
		return sz;
	}

	//--------------------------------------------------------
	// Check path
	if( path && isValidString(path) && strlen(path) )
	{
		// Get file size and set it as return value
		sz = 
			jtmfs_GetFileSize(
			GetCurrentDNR(), path,
				GetCurrentDB());
		UNLOCK
		return sz;
	}
	else
	{
		//
		dprintk("%s: Defective file name\n", __FUNCTION__);
		UNLOCK
		return 0;
	}

	//
	UNLOCK
	return 1;
}

//
int addfileblocks(const char *path, int count)
{
	//
	int key1,pr,i,i2,i3,i4;

	LOCK

	// Check path
	if( path && isValidString(path) && strlen(path) )
	{
		dprintk("jtmfs_AddBlocks(devnr=%d, '%s'(0x%x), 0, count=%d);\n",
			GetCurrentDNR(), path, path, count);
		jtmfs_AddBlocks(
		GetCurrentDNR(), path, GetCurrentDB(), count);
		UNLOCK
		return 1;
	}
	else
	{
		dprintk("%s: error par1==NULL\n", __FUNCTION__);
		UNLOCK
		return 0;
	}

	//
}

//
int setsize(const char *path, int newsize)
{
	int key1,pr,i,i2,i3,i4,dnr,db;

	LOCK

	// Check path
	if( path && isValidString(path) && strlen(path) )
	{
		dnr = GetCurrentDNR();
		db = GetCurrentDB();
		ReportMessage("jtmfs_SetSize(devnr=%d, '%s'(0x%x), 0, size=%d);\n",
			dnr, path,path, newsize);
		jtmfs_SetSize(dnr, path, newsize, db);
		UNLOCK
		return 0;
	}
	else
	{
		dprintk("%s: error par1==NULL\n", __FUNCTION__);
		UNLOCK
		return 1;
	}

	//
}

// Check file existance and return it's type.
// -1/0: Not found.
// 1: File.
// 2: Directory.
// Other values: Unknown.
int exist(const char *path)
{
	LOCK

	//
	if( path && isValidString(path) && strlen(path) )
	{
		// v4 will contain the return value.
		UNLOCK
		return
			jtmfs_Fexist(GetCurrentDNR(), path, GetCurrentDB());
	//	printk("%s: Returns %d on '%s'(len=%d)-\n",
	//	__FUNCTION__, p->v4, p->par1, strlen(p->par1));
	}
	else
	{
		printk("%s: Malformed request.\n", __FUNCTION__);
		UNLOCK
		return 0;
	}

	//
}

// Start a directory search
int findfirst(const char *path, void *ffblk)
{
	int fd;
	
	LOCK

	// const char *path,	FFBLK *
	// (par1)		(par2)
	dprintk("%s: path(par1)='%s', par2=%x\n",
			__FUNCTION__, path, ffblk);
	// Do findfirst
	fd = jtmfs_findfirst(GetCurrentDNR(), GetCurrentDB(), path, ffblk);
	// Put fd under sender's ownership
	changeFDOwner(fd, GetCurrentThread());
	
	UNLOCK
	//
	return fd;
}

// Continue directory search
int findnext(int fd, void *ffblk)
{
	int i;

	LOCK

	// int fd,	FFBLK *
	// (v1)		(par1)
	i =
		jtmfs_findnext(GetCurrentDNR(), GetCurrentDB(), fd, ffblk);
	UNLOCK
	return i;
}

// Get file data block number
void getfiledatablock(const char *path)
{
	int i;

	LOCK

	//
	i = 
		jtmfs_GetFileDataBlock(GetCurrentDNR(), GetCurrentDB(), path);
	UNLOCK
	return i;
}

// Create directory
int createdirectory(const char *path)
{
	LOCK

	//
	jtmfs_CreateDirectory(GetCurrentDNR(), GetCurrentDB(), path);
	
	//
	UNLOCK

	//
	return 1;
}

// Format a drive(multitasking safe)
void formatdrive(int dnr)
{
	int r;

	//
	LOCK
	r = jtmfs_formatdrive(dnr);
	UNLOCK
	return r;
}

// Change directory
void changedirectory(const char *path)
{
	// ----------------------------------------------------
	int key1,pr,i,i2,i3,i4,devnr,r;
	
	LOCK

	// Check path
	if( path!=NULL && isValidString(path) && strlen(path) )
	{
		// Change to specified directory
		r = jtmfs_chdir(path);
	}
	else
	{
		// Arguments were bad
		r=1;
	}
	
	//
	UNLOCK

	// Give out a return value.
	return r;
}

// Checks whether if specified drive is ready for writing/reading,
// in other words that the drive is healthy and properly formatted.
void isdriveready(int dnr)
{
	LOCK

	// Set return value
	jtmfs_isDriveReady(dnr);
	
	//
	UNLOCK

	//
	return 0;
}

// Get current work directory.
int getcwd(const char *path, int max_len)
{
	int i;
	
	LOCK

	// Set return value
	i = jtmfs_GetCWD(path, max_len);
	
	//
	UNLOCK
	return i;
}

// THE OPEN FUNCTION 
//
int open(const char *pathname, int flags)
{
	FILEDES *f;
	
	LOCK

	// GET NEWLY ALLOCATED FD PTR
	f = NewFD();
	// SET NAME
	strcpy(f->name, pathname);
	//
	f->flags = flags;
	
	UNLOCK

	//
	return f->fd;
}

//
int creat(const char *pathname, int flags)
{
	FILEDES *f;
	
	LOCK

	// GET NEWLY ALLOCATED FD PTR
	f = NewFD();
	// SET NAME
	strcpy(f->name, pathname);
	//
	f->flags = flags;
	//
	createfile(pathname);
	
	//
	UNLOCK

	//
	return f->fd;	
}

//
int close(int fd)
{
	//
	LOCK
	
	//
	FreeFD(fd);
	
	//
	UNLOCK
	
	//
	return 0;
}

//
int read(int fd, void *buf, int count)
{
	FILEDES *f;
	
	//
	LOCK
	
	//
	f = get_filedesptr(fd);
	//
	//int readfile(char *path, char *buf, int offset, int count)
	readfile(f->name, buf, f->offset, count);
	f->offset += count;
	
	//
	UNLOCK

	//
	return 0;
}

//
int write(int fd, void *buf, int count)
{
	FILEDES *f;
	
	//
	LOCK
	
	//
	f = get_filedesptr(fd);
	//
	//int readfile(char *path, char *buf, int offset, int count)
	writefile(f->name, buf, f->offset, count);
	f->offset += count;
	
	//
	UNLOCK
	
	//
	return 0;
}

//
int lseek(int fd, int offset, int whence)
{
	FILEDES *f;
	
	//
	LOCK
	
	//
	f = get_filedesptr(fd);
	//
	f->offset = offset+whence;
	
	//
	UNLOCK
	
	//
	return 0;
}

//
int readdir(unsigned int fd, void *dirp,
          unsigned int count)
{
	LOCK
	// TODO!!
	UNLOCK
	return 0;
}

//
int mkdir(const char *pathname, int mode)
{
	LOCK
	//
	createdirectory(pathname);
	//
	UNLOCK
	//
	return 0;
}

//
int rmdir(const char *pathname)
{
	//
	LOCK
	//
	UNLOCK
	//
	return 0;
}

//
int unlink(const char *pathname)
{
	int i;
	
	LOCK

	//
	i = deletefile(pathname);

	//
	UNLOCK
}

//
int chdir(const char *pathname)
{
	int i;
	
	LOCK

	//
	i = jtmfs_chdir(pathname);

	//
	UNLOCK
}

//
int mkfs(const char *path)
{
	LOCK
	
	//
	formatdrive( driver_getdevicenrbyname(path) );

	//
	UNLOCK
	//
	return 0;
}


// Fixed: Actually calls the JTMFS file system functions to flush the cache.
// Cache has three designed levels: FAT, Directories, and Device Driver Cache.
int sync(void)
{
	LOCK
	
	int dnr;

	//
	dnr = GetCurrentDNR();
	flexFlushFAT(dnr);
	dirdbFlush();
	//driver_FlushDriver(dnr);
	_FlushCache();
	
	
	//
	UNLOCK
	return 0;
}


//
int syncfs(int fd)
{
	LOCK

	// TODO !!
	
	//
	UNLOCK
	return 0;
}

