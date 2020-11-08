// hdBuffer.c - buffer read/write
// Remembers single track at once.
#include "kernel32.h"
#include "hd.h"
#include "hdBuffer.h"

//
typedef struct
{
	// Is it free entry?
	int isFree;
	// Write back required?
	int writeback;
	// Cylinder, Head
	int c,h;
	// Buffer for data
	BYTE *buf;
}TRACK;
static TRACK t[10];

//
void hdBufferInit(HD *h)
{
	int i;

	//
	memset(&t, 0, sizeof(t));

	//
	for(i=0; i<10; i++)
	{
		// Mark as unused(free)
		t[i].isFree =	TRUE;
		// Allocate track buffer
		t[i].buf =	malloc(h->sectors*h->blocksize);
	}
}

//
static void loadTrack(HD *h, TRACK *t, int cylinder,int head)
{
	//
	if(t->writeback)
	
}

//
hdBufferedSectorRW(HD *h,
	BYTE *buf,int l_buf,
	int head,int sector,int cylinder,
	int driveNr, // 0=master, 1=slave
	int rw, // 0=read, 1=write
	int sectorCount)
{
	TRACK *t;

	// Get track PTR
	t = &t[0];

	//
	if(rw==READ)
	{
		if(t->isFree)
		{
			//
			t->isFree = FALSE;
			loadTrack(h, t, cylinder,head);
		}
		else
		{
			//
			if(t->cylinder!=cylinder || t->head!=head)
			{
				//
				loadTrack(h, t, cylinder,head);
			}
		}
	}
	else
	if(rw==WRITE)
	{
	}
}




