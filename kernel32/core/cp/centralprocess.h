#ifndef __INCLUDED_CENTRALPROCESS_H__
#define __INCLUDED_CENTRALPROCESS_H__

// CENTRALPROCESS ERRORS
#define CENTRALPROCESS_ERROR_FILENOTFOUND	1
#define CENTRALPROCESS_ERROR_LAUNCHAPPERROR	2
#define CENTRALPROCESS_ERROR_CORRUPTEDEXE	3

// Time-out values
#define CENTRALPROCESS_EXEC_WAIT_TIMEOUT	6000

// 
typedef struct
{
	// EXE binary file is stored here
	int l_bin;
	BYTE *bin;
	// Executable precise size in bytes
	int exesz;
	// Set when centralprocess cannot get any new parameters
	int             isBusy;
	// File name of the binary to load the process binary
	// from, that will be created.
	char            fname[256];
	char            path[256];
	// 
	int             appPID;
	int             terminalID;
	//
	int memrq;
	//
	int error, dnr,db;
	//
	char		runPath[256];
}VERBATIM;

// Internal functions:
void            centralprocess_run(void);
void            centralprocess_init(void);
int             centralprocess_runexe(const char *fname);

// Functions used by system calls:
int             centralprocess_exec(const char *fname, const char *path,
		int dnr, int db,
		const char *runPath,
		int terID);
int		centralprocessExecEx(const char *fname, const char *path,
		int terminalID,
		int dnr, int db,
		const char *runPath);
void trap1(void);

// 
VERBATIM   centralprocess;

//
int isExeBinary(BYTE *buf, int l);
void centralprocess_runexe1(void);

//
#include "exeidentificationmodule.h"
#include "centralprocessMain.h"

#endif
