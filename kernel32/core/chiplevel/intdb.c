//------------------------------------------------------
// Interrupt database
//
// This database implementation makes possible for
// interrupts to have multiple handlers.
//------------------------------------------------------
#include "kernel32.h"
#include "int.h"
#include "intdb.h"

// Interrupt database
INTDB intdb;

// Init interrupt database system
int initInterruptDB(void)
{
	//
	memset(&intdb, 0, sizeof(INTDB));
}

// Create new interrupt handler entry
int newHandlerEntryID(INTHAN *e)
{
	int i,i2,i3,i4,found;
	IHANDLER *ih;

	// Search for entry that could be reused
	for(i=0,found=-1; i<e->nr_handlers; i++)
	{
		// Get PTR
		ih = &e->handler[i];

		// Found free entry?
		if(ih->isFree)
			found = i;
	}

	// Need to allocate a new entry?
	if(found==-1)
	{
		// Reserve new one
		found = e->nr_handlers;
		e->nr_handlers++;
	}

	//
	return found;
}


// Add a new handler to specified interrupt
int addInterruptHandler(int nr,
		void *ptr, int selector, int type)
{
	int id,i,i2;
	IHANDLER *e;

	// Get new entry for the interrupt handler
	id = newHandlerEntryID(&intdb.i[nr]);

	// Get handler structure
	e = &intdb.i[nr].handler[id];

	//
	e->eip = ptr;
	e->cs = selector;
	e->type = type;

	//
	return 0;
}
