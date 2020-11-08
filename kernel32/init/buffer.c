//============================================================
// FIFO buffer functions.
// (C) 2003-2005 by Jari Tuominen.
//============================================================
#include "kernel32.h"
#include "buffer.h"

// Initialize buffer
void InitBuffer(FBUF *b)
{
	// Define head & tail.
	b->head=	0; // used for WRITING
	b->tail=	0; // used for READING	
	// Define buffered count.
	b->count=	0;
	// Total count.
	b->total=	0;
	// Set magic.
	b->magic=	FIFOBUF_MAGIC;
}

// Create a new buffer
void CreateBuffer(FBUF *b, int len)
{
	static BYTE tmp[8192];

	//
	if(mallocOK)
	{
		// Define allocation amount and allocate
		b->l_buf =	len;
		b->buf =	malloc(b->l_buf);
		// Initialize buffer
		InitBuffer(b);
	}
	else
	{
		//
		memset(b, 0, sizeof(FBUF));
	}
}

// For buffer reading.
int TailForward(FBUF *b)
{
	//
	if(b->count)
	{
		//
		b->count--;
		b->tail++;

		//
		if(b->tail >= b->l_buf)
			// Loop at beginning.
			b->tail=0;
		return 0;
	}
		// Nothing there to get
	else	return 1;
}

// For buffer writing.
int HeadForward(FBUF *b)
{
	//
	if(b->count<b->l_buf)
	{
		//
		b->count++;
		b->head++;
		if(b->head >= b->l_buf)
			// Loop at beginning.
			b->head=0;
		return 0;
	}
		// Buffer already full
	else	return 1;
}

//------------------------------------------------------------
int PushBuffer(FBUF *b, BYTE byte1)
{
	//
	if(b==NULL || b->buf==NULL || b->magic != FIFOBUF_MAGIC)
		return 2;

	//
	if(b->magic!=0xFCE2E37B)
		// Magic mistach(error!)
		return 1;
	if( !HeadForward(b) )
		{
			//
			b->buf[b->head % b->l_buf] = byte1;
			// Increase total
			b->total++;
			return 0; // push ok
		}
		else	return 1; // buffer full
}

//------------------------------------------------------------
int PopBuffer(FBUF *b)
{
	static char str[8192];

	//
	if(b==NULL || b->buf==NULL || b->magic != FIFOBUF_MAGIC)
		return -2;

	//
	if( !TailForward(b) )
	{
		//
	/*	sprintf(str, "[POPBUFFER-%s-%c]\n",
			thread_name[nr_curthread],
			b->buf[b->tail % b->l_buf]);
		writetext(&screens[0], str);*/
		// Increase total.
		b->total++;
		// pop ok
		return b->buf[b->tail % b->l_buf];
	}
	else
		// buffer empty
		return -1;
}


//------------------------------------------------------------
//
// Quick access...
//

//
int GetBuffer(FBUF *b)
{
	int flags,input_byte;

	//
	flags = get_eflags();
	disable();
	input_byte = PopBuffer(b);
	set_eflags(flags);
	return input_byte;
}

//
int PutBuffer(FBUF *b, int output_byte)
{
	int v,flags;

	//
	flags = get_eflags();
	disable();
	v = PushBuffer(b, output_byte);
	set_eflags(flags);
	return v;
}

// Push a WORD into buffer
int PushBufferW(FBUF *b, WORD w)
{
	int rv;

	//
	if( (rv=PushBuffer(b, w & 255)) )
		return rv;
	return PushBuffer(b, w>>8 & 255);
}

// Pop a WORD from buffer
int PopBufferW(FBUF *b)
{
	int w,bit;

	//
	w = PopBuffer(b);
	if(w==-1)
		return -1;
	bit = PopBuffer(b);
	if(bit==-1)
		return -1;
	w |= bit<<8;
	return w;
}

// Returns amount of bytes wrote.
int ReadBuffer(FBUF *b, BYTE *buf, int len)
{
	int i,d;

	// Loop through
	for(i=0; i<len; i++)
	{
		if( (d=PopBuffer(b))==-1 )
			// Out of space
			break;
		buf[i]=d;
	}

	// Return amount of bytes read.
	return i;
}

// Returns amount of bytes read
int WriteBuffer(FBUF *b, BYTE *buf, int len)
{
	int i,d;

	// Loop through
	for(i=0; i<len; i++)
	{
		if( PushBuffer(b, buf[i]) )
			// Out of data
			break;
	}

	// Return amount of bytes written.
	return i;
}

//
