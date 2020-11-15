 //-----------------------------------------------------------------------
// WriteChain.c
// (C) 2003-2005 by Jari Tuominen.
//-----------------------------------------------------------------------
#include "kernel32.h"
#include "jtmfsAccess.h"
#include "jtmfat.h"

//
int jtmfs_WriteFileChain(BYTE *buf, int dnr, int _d,
		int preciseLength,
		int offset)
{
	DEBUGLINE

	//
	return jtmfs_WriteFileChainEx(buf, dnr, _d, preciseLength, offset, 1);
}

// jtmfs_WriteFileChainEx
//
// More blocks is allocated if the chain does not contain
// enough chains for the operation.
// New: All newly allocated blocks will be initially erased before use.
// RETURN VALUE:
// -1 means error.
// -2 means disk full.
//  0 means no error.
int jtmfs_WriteFileChainEx(BYTE *buf, int dnr, int _d,
		int preciseLength, int offset, int useCache)
{
	static DWORD i,i2,i3,i4,d,newd,c,
		ii,ii2,ii3,ii4,offs,new_block,bsz,skipBytes,end;
	static int nb;
	static DIRWALK dw;
	static BYTE *tmp,*tmp2,*s,*o;
	static int amount,skip,boffset,fixed,first=TRUE;

	//
	DEBUGLINE

	//
	bsz = getblocksize(dnr);

	// Determine amount in blocks
	amount = (preciseLength/getblocksize(dnr))+1;

	// Determine amount of blocks to skip before starting writing
	// (according offset)
	skip = offset/getblocksize(dnr);
	// Linear offset in bytes to beginning of read place
	boffset = skip*getblocksize(dnr);
	// Determine amount of bytes to skip
	skipBytes = offset-boffset;

	// Get tmp PTRs
	d = _d;
	tmp = jtmfsAccess.rwtmp;
	tmp2 = jtmfsAccess.rwtmp2;

	// Device must be valid
	if( !isOK(dnr) ) return -1;

	// Check db validity
	if(d<0 || d>=getsize(dnr))
	{
		ReportMessage("%s: Requested block(%d) is exceeding drive limits\n",
			__FUNCTION__, d);
		return 1200;
	}

	// Check last block
	if(jtmfs_LastBlock(d))
	{
		ReportMessage("%s: Error! The last block was proposed as dblock param(%d)\n",
			__FUNCTION__, d);
		return 1201;
	}

	// READ DATA FROM FILE CHAIN
	for(i=0,s=buf,c=0,first=TRUE; i<(amount+1); )
	{
		// Get next block or End of chain...
		newd = jtmfat_getblock(dnr,d);

		// Do possible skips if needed, so we get to the right offset
		if(skip)
		{
			skip--;
			goto past;
		}

		//--------------------------------------------------
		// * WRITING PROCESS *

		//
		if(first==TRUE || (i+1)==amount)
		{
			first = FALSE;
			readblock(dnr, d, tmp);
		}
		else
			memset(tmp, 0, bsz);

		// Copy to the actual buffer
		offs = i*bsz;
		for(ii2=0; ii2<bsz;
				ii2++)
		{
			// Write at fixed boundary
			if(skipBytes)
				skipBytes--;
			else
			{
				// Check byte boundary
				if(c<preciseLength)
				{
					// Store a single byte
					tmp[ii2] = *s; s++;
					// Increase amount of bytes written
					c++;
				}
				else
					tmp[ii2] = 0xBD;
			}
		}

		// Do operation: To DEVAPI cache or directly to the driver.
		writeblock(dnr, d, tmp);
		printk(".");

		// Increase the amount of blocks we've written
		i++;

		//---------------------------------------------------
		// * GO TO NEXT BLOCK *
past:
		// Unexpected chain end & demand for more blocks?
		if( jtmfs_LastBlock(newd) )
		{
			// ALLOCATE MORE BLOCKS
			new_block = jtmfat_getfreeblock(dnr);
			nb = new_block;
			if(nb==-1)
			{
				printk("%s: Disk full!\n",
					__FUNCTION__);
				return -2;
			}
			if(new_block==0 ||
				new_block>=0xFFFFFFF8)
			{
				// Error: Wrong kind of BLOCK #
				// ReportMessage (...) ...
				printk("%s: ERROR: getfreeblock returns obscure block (%x) - FAT system failure or disk full?\n",
					__FUNCTION__, new_block);
				AnalyzeFATs();
				return 1;
			}
			jtmfat_setblock(dnr, d, new_block);
			jtmfat_setblock(dnr, new_block,
						0xFFFFFFF8);
			dprintk("%s: Chain expansion %d -> %d\n",
				__FUNCTION__,
				d, new_block);

			// Swap to it
			newd = new_block;

			// Erase it initially(yes this is needed)
			memset(tmp2, 0, getblocksize(dnr));
			writeblock(dnr, newd, tmp2);
		}

		// Change to new block
		d = newd;
	}
enough:
	ReportMessage("%s: I wrote %d block(s) (requested = %d blocks, %d bytes precise=%d bytes)\n",
		__FUNCTION__, i, amount, amount*getblocksize(dnr), preciseLength);
	return 0;
}


