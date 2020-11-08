//
#ifndef __INCLUDED_POSTMAN_H__
#define __INCLUDED_POSTMAN_H__

//
#include "msgbox.h"

// Allow thread switching
//#define SPEED_UP_WITH_SWITCHING

// Time-out in seconds
#define SERVICE_RESTART_SECONDS		60*5

// Allow restarting of services
extern int ALLOW_SERVICE_RESTARTING;

// ----- KFILESYS ---------------------------------------------
// Define range
#define KFILESYS_1		0
#define KFILESYS_2		999

// FILE FUNCTIONS
#define PCMD_CREATE_FILE	1
#define PCMD_DELETE_FILE	2
#define PCMD_WRITE_FILE		3
#define PCMD_READ_FILE		4
#define PCMD_GET_FILE_SIZE	5
#define PCMD_ADD_BLOCKS		6
#define PCMD_EXIST		7
#define PCMD_READBLOCK		8
#define PCMD_WRITEBLOCK		9
#define PCMD_FINDFIRST		10
#define PCMD_FINDNEXT		11
#define PCMD_RENAME_FILE	12
#define PCMD_SETSIZE		13
#define PCMD_GETFILEDATABLOCK	14
#define PCMD_CREATEDIRECTORY	15
#define PCMD_FORMATDRIVE	16
#define PCMD_CLOSE		17
#define PCMD_CHDIR		18
#define PCMD_IS_DRIVE_READY	19
#define PCMD_GETCWD		20

// ----- KMULTIMEDIA -------------------------------------------
// Define range
#define KMULTIMEDIA_1		1000
#define KMULTIMEDIA_2		2999

// VIDEO FUNCTIONS
#define PCMD_SETMODE		1000
#define PCMD_DRAWFRAME		1001
#define PCMD_SETPALETTE		1002
#define PCMD_SETPALETTEMAP	1003
// AUDIO FUNCTIONS
#define PCMD_INITAUDIO		2000
#define PCMD_SENDAUDIO		2001
#define PCMD_GETAUDIOPOSITION	2002

// ------------------------------------------------
#define N_MAX_POSTMAN_POSTS 50

//
#define POSTMAN_MAGIC		0xFCE2E37B

//
typedef struct
{
	// 0xFCE2E37B
	DWORD magic;
	// Indicator of work status(TRUE=done, FALSE=still undone)
	int *acknowledged;
	// Return value
	int *rv;
	// Other variables
	int id;
	char *par1,*par2,*par3,*par4,*par5,*par6,*par7,*par8;
	int v1,v2,v3,v4;
	int dnr,db,pid;
}POSTMANCMD;

//
typedef struct
{
	//
	POSTMANCMD post[N_MAX_POSTMAN_POSTS];
}POSTMAN;

// Postman service structure
typedef struct
{
        // Packet PTR
        POSTMANCMD *p;
	// Temporary buffer for incoming packet
	MSG tmp;
        // ID
        int id;
        // Last second when we ran
        int t;
}PSERVICE;
extern PSERVICE kfilesys, kmultimedia;


//
extern POSTMAN postman;

//
void postman_init(void);
int postman_postrequest(POSTMANCMD *p);
void postmanThread(void);

//
extern int postmanPID;

//
#ifdef JTMOSVERSION
extern THREAD PostmanThread;
extern THREAD kfilesysThread;
extern THREAD kmultimediaThread;
#endif

//
#endif
