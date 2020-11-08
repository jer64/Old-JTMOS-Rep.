///////////////////////////////////////////////////////////////////////////////////////////
// ReadChain.c
// Bug: Misses last <512 bytes of data.
// (upd: bug probably fixed by now)
///////////////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "dir.h"
#include "flexCorruption.h"
#include "ReadChain.h"

// RETURN VALUE:
// Returns number of blocks read.
//
// If amount equals to -1 then whole chain is read.
int jtmfs_ReadFileChain(BYTE *buf, int dnr, int _d,
			int preciseLength,
			int offset)
{
	static int i,i2,i3,i4,d,newd,c,
			ii,ii2,ii3,ii4,bsz,skipBytes;
	static DIRWALK dw;
	static char *tmp;
	static int Amount,skip,boffset,fixed;
	static BYTE *o;

	//
	DEBUGLINE
	FFCHECK

	//
	d = _d;
	tmp = jtmfsAccess.rwtmp;

	// Device must be valid
	if( !isOK(dnr) ) return -1;

	//
	if(jtmfs_LastBlock(d))return 0;

	//---------------------------------------------------------------------
	//
	// For debugging purposes... (to see if the buffer is loaded at full length)
//	memset(buf, 0xBD, preciseLength);

	//
	Amount = (preciseLength/getblocksize(dnr))+1;
	// Determine amount of blocks to skip before starting writing
	// (according offset)
	skip = offset/getblocksize(dnr);
	// Linear offset in block sized byte radius
	boffset = skip*getblocksize(dnr);
	// How many bytes in first block to skip to get correct offset
	skipBytes = offset-boffset;

	// READ DATA FROM FILE CHAIN
	for(i=0,c=0,o=buf; i<(Amount+1); )
	{
		//
		FFCHECK

		// Get next block or End of chain...
		newd = jtmfat_getblock(dnr,d);

		// Do possible skips if needed, so we get to the right offset
		if(skip)
		{
			skip--;
			goto past;
		}

		//----------------------------------------------------
		// * READ OPERATION *

		//
		bsz = getblocksize(dnr);

		// Do operation.
		readblock(dnr, d, tmp);

		// Copy to the actual buffer.
		for(ii2=0; ii2<bsz;
				ii2++)
		{
			if(skipBytes)
				skipBytes--;
			else
			{
				// Check byte boundary
				if(c<preciseLength)
				{
					// Store
					*o = tmp[ii2]; o++;
					// Increase amount of bytes read
					c++;
				}
			}
		}

		//
/*		printk("%s: buf's first two bytes = 0x%x, 0x%x -- read offset = %d\n",
			__FUNCTION__,
			o[0], o[1],
			offset);*/
	//	getch();

		// Increase the amount of blocks read
		i++;

		//----------------------------------------------------
		// * NEXT BLOCK *
past:
		// Unexpected chain end?
		if( jtmfs_LastBlock(newd) )
		{
			return i;
		}

		// Change to new block
		d = newd;
	}
enough:

	//
	if(c != preciseLength)
	{
//		panic("c != preciseLength");
	}

	//
	FFCHECK
	return c;
}


