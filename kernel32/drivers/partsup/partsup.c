/*** PARTITION TABLE SUPPORT ***
 * partsup.c - (C) 2008 Jari Tuominen (jari@vunet.org).
 */

//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
#include "hd.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "partsup.h"

//
static int hda1_device_call(DEVICE_CALL_PARS);
static int hda2_device_call(DEVICE_CALL_PARS);
static int hda3_device_call(DEVICE_CALL_PARS);
static int hda4_device_call(DEVICE_CALL_PARS);

//
static BYTE *master_boot_record;
static PARTITION *ptbl[4];
static char *partdevs[]={
"hda1",
"hda2",
"hda3",
"hda4"
};
static char *handlers[]={
hda1_device_call,
hda2_device_call,
hda3_device_call,
hda4_device_call
};

//
PARTITION *get_partition_table_by_name(char *devname)
{
	int i;

	//
	for(i=0; i<4; i++)
	{
		if( !strcmp(partdevs[i], devname) ) { return ptbl[i]; }
	}

	//
	return NULL;
}

// hda1
static int hda1_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	static char devname[256];
	static PARTITION *tbl;
	int sec,endsec;

	//
	tbl = ptbl[0];

	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_READBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				READ, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_WRITEBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				WRITE,
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size = tbl->sector_count;
		break;

		// 
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size = 512;
		break;

		//
		default:
		break;
	}

	//
	return 0;
}

// hda2
static int hda2_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	static char devname[256];
	static PARTITION *tbl;
	int sec,endsec;

	//
	tbl = ptbl[1];

	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_READBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				READ, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_WRITEBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				WRITE, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size = tbl->sector_count;
		break;

		// 
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size = 512;
		break;

		//
		default:
		break;
	}

	//
	return 0;
}

// hda3
static int hda3_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	static char devname[256];
	static PARTITION *tbl;
	int sec,endsec;

	//
	tbl = ptbl[2];

	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_READBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				READ, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_WRITEBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				WRITE, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size = tbl->sector_count;
		break;

		// 
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size = 512;
		break;

		//
		default:
		break;
	}

	//
	return 0;
}

// hda4
static int hda4_device_call(DEVICE_CALL_PARS)
{
	//
	DWORD *size;
	static char devname[256];
	static PARTITION *tbl;
	int sec,endsec;

	//
	tbl = ptbl[3];

	//
	switch(n_call)
	{
		//
		case DEVICE_CMD_READBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				READ, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_WRITEBLOCK:
		sec = tbl->begsec+p1;
		endsec = tbl->begsec+tbl->sector_count;
		if(sec<endsec)
		{
			hdSectorRW_LBA(chd,
				po1,512,
				sec,
				0, // 0=master, 1=slave
				WRITE, // 0=read, 1=write
				1); // sector count
		}
		break;

		//
		case DEVICE_CMD_GETSIZE:
		size=po1;
		*size = tbl->sector_count;
		break;

		// 
		case DEVICE_CMD_GETBLOCKSIZE:
		size=po1;
		*size = 512;
		break;

		//
		default:
		break;
	}

	//
	return 0;
}

//
static int LoadPartitionTable(void)
{
	DWORD *p;
	int i,i2,i3,i4;
	static char str[256];

	// ---- GETTING THE PARTITION TABLE ------------------------------
	//
	master_boot_record = malloc( getblocksize(ND("hda")) );
	readblock(ND("hda"), 0, master_boot_record);

	//
	if(master_boot_record[0x1FE] != 0x55 ||
		master_boot_record[0x1FF] != 0xAA)
	{
		printk("%s: invalid master boot record", __FILE__);
		return 1;
	}

	//
	ptbl[0] = master_boot_record+(446+(0x10*0));
	ptbl[1] = master_boot_record+(446+(0x10*1));
	ptbl[2] = master_boot_record+(446+(0x10*2));
	ptbl[3] = master_boot_record+(446+(0x10*3));

	//
	printk("Disk Partitions\n");

	// ---- REGISTERING APPROPRIATE DEVICES ------------------------------
	//
	for(i=0; i<4; i++)
	{
		p = ptbl[i];
		if(!p[0] && !p[1])
		{
			break;
		}

		// Register the device
		sprintf(str, "hda%d", i+1);
		printk("%s: beginning=%d, %d sectors (%dM), system_indicator=%x\n",
			str, ptbl[i]->begsec, ptbl[i]->sector_count, ((ptbl[i]->sector_count*512)/(1024*1024))+1, ptbl[i]->system_indicator);
		driver_register_new_device(str, handlers[i], DEVICE_TYPE_BLOCK);
	}

	//
	return 0;
}

// Registers hda1, hda2, etc. devices.
int partition_table_support_init(void)
{
	//
	if( ND("hda")<=0 )
	{
		printk("%s: partition table support requires IDE HD (not detected)\n", __FILE__);
		return 1;
	}

	//
	if( LoadPartitionTable() ) { return 1; }

	//
	return 0;
}

//


