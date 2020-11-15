// ------------------------------------------------------------
// Requires jtmfs.h
//
#ifndef __INCLUDED_JTMFSACCESS_H__
#define __INCLUDED_JTMFSACCESS_H__

// #define DEBUG_WRITEBLOCK
#define isOK(X) (isValidDevice(X) && jtmfat_getrootdir(X)!=0)

//
#include "basdef.h"
#include "kernel32.h"
#include "devapi.h"
#include "jtmfsAccessInit.h"
#include "root.h"

// FILE API HEART STRUCTURE
typedef struct
{
	// Current path
	char *path;
	// Current directory block
	int db;
	// Current root device
	int dnr;
	//
	char *rwtmp,*rwtmp2;
}JTMFSACCESS;

// FILE DESCRIPTOR STRUCTURE
typedef struct
{
	// Current path
	char *path;
	// Current directory block
	int db;
	// Current root device
	int dnr;
}FILEDESC;

//
typedef struct
{
	//
	struct
	{
		// Current block where we are
		int bnr;
		// Offset in the file
		int offs;
		// Block buffer
		BYTE *buf;
		//
		char cmdget,cmdput;
	}dat;
	//
	DWORD dnr;
	DIRWALK dw;
	//
	char fname[33];
}HFILE;

//
extern JTMFSACCESS jtmfsAccess;

//
int jtmfs_CreateFile(int devnr1,int db,
                const char *fname1,int blocks);
int jtmfs_DeleteFile(int device_nr,const char *fname,int dirblock);

int jtmfs_GetFileSize(int devnr, const char *fname, int dirblock);
int jtmfs_AddBlocks(int devnr, const char *fname, int dirblock, int amount);
int jtmfs_Fexist(int dnr, const char *fname, int dirblock);
int jtmfs_RenameFile(int devnr1,
                const char *OldName, const char *NewName,
                int dirblock);
int jtmfs_SetSize(int devnr1,
                const char *OldName, int newSize,
                int dirblock);
DWORD GetCurrentDrive(void);
// Low level chdir
int jtmfs_chdir(const char *path);
int _jtmfs_chdir(const char *path);
// Wrapper to jtmfs_chdir
int IntelligentCHDIR(const char *path);
//int _IntelligentCHDIR(const char *path);
int jtmfs_CreateDirectory(int dnr, int db, const char *dirname);
int jtmfs_GetDirName(int dnr,int db, char *name);
int jtmfs_GetCWD(char *path, int max_len);

//
#define jtmfs_ReadFile(dnr,name,db,buf,showMeter,prec,offs) \
	_jtmfs_ReadFile(__FUNCTION__, dnr,name,db,buf,showMeter,prec,offs)

//
int _jtmfs_ReadFile(const char *caller,
                        int dnr, const char *name,
                        int db,
                        BYTE *buf, char showMeter,
                        int preciseLength,
                        int offset);
int jtmfs_WriteFile(int devnr, const char *fname,
                        int dirblock,
                        BYTE *buf, char showMeter,
                        int preciseLength,
			int offset);
int jtmfs_ReadFileChain(BYTE *buf, int dnr, int d,
		int preciseLength,
		int offset);
int jtmfs_WriteFileChain(BYTE *buf, int dnr, int d,
		int preciseLength,
		int offset);
int jtmfs_GetFileDataBlock(int dnr, int db, const char *name);
int jtmfs_isDriveReady(int dnr);

//
#include "ReadChain.h"
#include "WriteChain.h"

#endif

//
