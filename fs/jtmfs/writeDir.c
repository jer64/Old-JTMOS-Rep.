//================================================================
// writeDir.c
// part of DirDB
//================================================================
#include "kernel32.h"
#include "dirdb.h"
#include "dirdbAccess.h"
#include "dirdbWalk.h"

// Write directory on drive
// (Used by flush function - For cache system only.)
// Return value:
// 0 on success
// non-zero on error
int writeDir(DIR *d)
{
	int blocks,x,er;
	static char str[256];
	static BYTE *tmp;
	static int l_tmp;

	//
	DIRDEBUG

	//
	if(d==NULL)
		return 1;

	// Determine amount of blocks to write
	if(d->type==ID_DIRECTORY)
	{
		d->n_entries = countEntries(d);
		blocks = sizeof(JTMFS_FILE_ENTRY)*d->n_entries/getblocksize(d->dnr);
		blocks++;
	}
	else
	// ...
	if(d->type==ID_FILE)
	{
		//
		blocks = d->blocks;

		//
		printk("%s: Writing file dnr=%d, db=%d, buf=0x%x.\n",
			__FUNCTION__,
			d->dnr, d->db,
			d->buf);
	}
	else
	{
		// Unknown type.....
		printf("%s: Unknown DIR->type -type (%d).\n",
			__FUNCTION__, d->type);
		return 300;
	}

	// Determine amount of entries directory has
	if(d->type==JTMFS_ID_DIRECTORY)
	{
		//
		printk("%s: Writing directory dnr=%d, db=%d, buf=0x%x.\n",
			__FUNCTION__,
			d->dnr, d->db,
			d->buf);

		//
		d->n_entries = countEntries(d);
		// Inform
		driver_getdevicename(d->dnr, str);
		printk("%s: Writing directory on drive %s:\n(%d entries detected for this directory)\n",
			__FUNCTION__,
			str,
			d->n_entries);
	}

	// Allocate tmp buffer
	l_tmp = (blocks+1)*getblocksize(d->dnr);
	tmp = malloc(l_tmp);
	if(tmp==NULL)
	{
		printf("%s: Out of memory --- blocks=%d, bs=%d\n",
			__FUNCTION__,
			blocks, getblocksize(d->dnr));
		return 303;
	}

repeat:
	//
	printk("%s: writing %dK bytes (%d entries)\n",
		__FUNCTION__,
		l_tmp/1024,
		d->n_entries);
	//
	// Write directory blocks on chain
	printk("0x%x, %d,%d, %d,  %d\n",
		d->buf, d->dnr,d->db, l_tmp,0);
	er=jtmfs_WriteFileChain(
		d->buf, d->dnr, d->db,
		l_tmp, 0);

	//
	if(er==-2)
	{
		printk("Disk full... Can't write directory.\n");
		return 10;
	}
	else
	if(er)
	{
		printk("writeDir: error %d\n", er);
		panic("writeDir error");
	}

	// Verify
	jtmfs_ReadFileChain(
		tmp, d->dnr, d->db,
		l_tmp, 0);

	// Compare
	for(x=0; x<l_tmp; x++)
		if(d->buf[x]!=tmp[x])
		{
			printf("Difference at offset 0x%x\n", x);
		/**	printk("dirdb:\n");
			DisplayMemory(d->buf+x-64, 128, 0);
			printk("disk:\n");
			DisplayMemory(tmp+x-64, 128, 0);	**/
			//getch();
			//goto repeat; // !!
			break;
		}

	//
	free(tmp);
	l_tmp=0;
	return 0;
}


