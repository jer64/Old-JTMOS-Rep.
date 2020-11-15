//--------------------------------------------------------------------------------------------
// dir.c - Directory displayer kernel mode application.
//--------------------------------------------------------------------------------------------
#include "kernel32.h"
#include "jtmfs.h"
#include "dir.h"

// Dir function (ls)
// -----------------
//
// This function outputs directory from specified block(linear) to stdout.
int jtmfs_dir(int dnr, int which)
{
	BYTE *buf,*p;
	WORD *wp;
	DWORD *dwp,i,i2,i3,i4,ii,ii2,ii3,ii4,l;
	JTMFS_FILE_ENTRY *fe;
	int k;
	DWORD ad;
	int current_block;
	char *str,*str2,*res1,*res2;
	int totalBytes,totalFiles,totalDirs;
	DIRWALK *dw;
	int lastone;

	//
	dw = malloc(sizeof(DIRWALK));

	// (TODO: add a dnr validity check here)
	//
	// JTMFS required
	if(!jtmfat_isJTMFS(dnr))
	{
		//
		printk("Looks like the drive is not JTMFS formatted.\n");
		return 1;
	}


	// Note: this must be freed afterwards:
	buf=malloc(16384);
	//
	str = malloc(256); str2 = malloc(256);
	res1 = malloc(256); res2 = malloc(256);
 
	// Begin walking
	if( jtmfs_ffirst(dnr, which, dw) )return 1;

	//
	for(i=0,totalFiles=0,totalBytes=0,totalDirs=0,lastone=0;
			; i++)
	{
		// Get current dir entry
		fe = dw->fe;

		// Give user a possibility to stop the directory listing
		if( (k = getch1())==27 )break;
 
		// Is it a ghost?->continue without notifying it (deleted file)
		// (ghost :  length == -1 )
		if( fe->length==-1 ) goto emptyEntry;
  
		// Empty filename? -> End of dir.
		if( !strlen(fe->name) )
		{
			break;
		}
   
		// --------- Start output string creation here: ----------
		strcpy(res1,""); strcpy(res2,"");

		//
		switch(fe->type)
		{
			// DIRECTORY
			case JTMFS_ID_DIRECTORY:
			strcat(res1, "[");
			break;
			// FILE
			default:
			break;
		}

		// PRINT FILENAME
		//
		// Note: I show filename so that all spaces show correctly,
		//       this requires putting the filename inside '"'s.
		sprintf(str, "%s", fe->name);
		strcat(res1, str);

		//
		switch(fe->type)
		{
			// DIRECTORY
			case JTMFS_ID_DIRECTORY:
			totalDirs++;
			strcat(res1, "]");
			break;
			// FILE
			default:
			totalFiles++;
			totalBytes += fe->length;
			sprintf(res2, "%d bytes", fe->length);
			break;
		}

		// Print strings in a style
		StretchPrint(res1, 30);
		StretchPrint(res2, 15);
		//
		printk("\n");

emptyEntry:
		//
		if( jtmfs_fnext(dnr, dw) )
			break;
	}

	//
	StretchPrint("", 10);
	sprintf(res1, "%d File(s)",	totalFiles);
	StretchPrint(res1, 15);
	sprintf(res1, "%d bytes",	totalBytes);
	StretchPrint(res1, 15);
	printk("\n");
	StretchPrint("", 10);
	sprintf(res1, "%d Dir(s)",	totalDirs);
	StretchPrint(res1, 15);
	sprintf(res1, "%d bytes",	totalBytes);
	StretchPrint(res1, 15);
	printk("\n");
	//
	//printk("End of directory.\n");

	//
	free(buf); free(str); free(str2);
	free(res1); free(res2);

	//
	free(dw);
 
	//
	return 0;
}

