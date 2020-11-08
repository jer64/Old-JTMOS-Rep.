// sqdsk.h
// 07/2003: added checksum support
#ifndef __INCLUDED_SQDSK_H__
#define __INCLUDED_SQDSK_H__

//
#ifndef DWORD
typedef unsigned long int DWORD;
typedef unsigned char BYTE;
typedef unsigned short int WORD;
#endif

// file description entry
typedef struct
{
	//
        char fname[31];
        int offset,length,chksum;
}FE;

//
typedef struct
{
	//
        WORD nr_files;
        FE **fe;
        char identifier[20];
}SQDSK;
extern SQDSK sqdsk;

// Area selection offset (A or B)
extern int seloffs;
//
extern int extract_archive(char *fname, char *spec);

//
extern int quiet;
extern char specific[256];
extern char dstdir[256];
extern int seloffs;

#endif


