//
#include "kernel32.h"
#include "jtmfat.h"
#include "isJTMFS.h"

// This function is called when a non-JTMFS drive is detected.
void onNonJTMFSDrive(int dnr)
{
	// Clear all cache buffers for this drive
	// This is a precaution.
	DiskChange(dnr);
}

// 0 = Non-JTMFS FS
// 1 = JTMFS FS
int isJTMFSBootBlock(int dnr, JTMFAT_INFOBLOCK *b)
{
	// FORMAT DETECTION
	if(
		b->detstr[0]=='J' &&
		b->detstr[1]=='T' &&
		b->detstr[2]=='M' &&
		b->detstr[3]=='F' &&
		b->detstr[4]=='S')
	{
		//
		return TRUE;
	}
	else
	{
		//
		onNonJTMFSDrive(dnr);
		return FALSE;
	}
}


