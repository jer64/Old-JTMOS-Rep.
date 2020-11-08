#ifndef __INCLUDED_INTDB_H__
#define __INCLUDED_INTDB_H__

// Single interrupt handler structure
typedef struct
{
	// This entry is free? [TRUE or FALSE]
	char isFree;
	// code segment selector
	int cs;
	// pointer to handler
	int eip;
	// type (unused)
	int type;
}IHANDLER;

// Interrupt handler structure
typedef struct
{
	// Amount of handlers registered
	int nr_handlers;
	// Handlers for this interrupt
	IHANDLER handler[10];
}INTHAN;

// Interrupt database
typedef struct
{
	//
	int nr_interrupts;
	INTHAN i[256];
}INTDB;

//
int addInterruptHandler(int nr,
		void *ptr, int selector, int type);
int newHandlerEntryID(INTHAN *e);
int initInterruptDB(void);

//
extern INTDB intdb;

#endif


