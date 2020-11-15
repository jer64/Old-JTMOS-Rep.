// ========================================
// ENHANCED DIRECTORY CACHE DATABASE SYSTEM
// AKA EDCDS SYSTEM
// (C) 2002-2004 by Jari Tuominen.
//
// This file contains only the init routine
// and some few functions.
// Other functions are located in seperate
// dirdb* files.
// ========================================
#include "kernel32.h"
#include "dirdb.h"              // DIRDB main header file
#include "dirdbAccess.h"        // DIRDB access functions
#include "dirdbInformation.h"

// Show DIRDB information
int dirdbInformation(void)
{
	int i,i2;
	DIR *d;
	char str[100];

	//
	printk("----------------------------------------------------------\n");
	printk("EDCDS CACHE SYSTEM REPORT\n");
	printk("%d directories cached\n",
		dirdb.n_entries);

	//
	for(i=0; i<dirdb.n_entries; i++)
	{
		//
		d = &dirdb.dir[i];

		//
		driver_getdevicename(d->dnr, str);

		//
		printk("%d: Drive %s: (%d/%d)  contains %d dir entries  guard=%s\n",
			i,
			str,
			d->dnr,
			d->db,
			countEntries(d),
			d->guard==DIRDB_GUARD ? "valid" : "INVALID");
	}

	//
	printk("End of list.\n");

	//
	return 0;
}



