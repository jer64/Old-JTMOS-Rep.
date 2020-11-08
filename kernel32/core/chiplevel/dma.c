/*
 * dma.c - DIRECT MEMORY ACCESS - Function Library
 *
 *	floppy.c uses this, so its needed,
 *	other hardware support providing
 *	drivers will probably need this
 *	eventually everything will be counted
 *	on the workability of this library.
 *
 *	the floppy uses dma 2
 *
 * (C)2002 by Jari Tuominen(jari@vunet.org)
 *
 */
#include "basdef.h"
#include "io.h"
#include "dma.h"

// defines for accessing the upper and lower byte of an integer
#define LOW_BYTE(x)         (x & 0x00FF)
#define HI_BYTE(x)          ((x & 0xFF00) >> 8)

// quick-access registers and ports for each DMA channel
BYTE MaskReg[8]   = { 0x0A, 0x0A, 0x0A, 0x0A, 0xD4, 0xD4, 0xD4, 0xD4 };
BYTE ModeReg[8]   = { 0x0B, 0x0B, 0x0B, 0x0B, 0xD6, 0xD6, 0xD6, 0xD6 };
BYTE ClearReg[8]  = { 0x0C, 0x0C, 0x0C, 0x0C, 0xD8, 0xD8, 0xD8, 0xD8 };
//
BYTE PagePort[8]  = { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A };
BYTE AddrPort[8]  = { 0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC };
BYTE CountPort[8] = { 0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE };

//
void _dma_xfer(BYTE DMA_channel, BYTE page, unsigned int offset, unsigned int length, BYTE mode);

//
void dma_xfer(BYTE channel, DWORD address, unsigned int length, BYTE read)
{
	BYTE page=0, mode=0;
	unsigned int offset = 0;
	
	if(read)
		mode = 0x48 + channel;
	else
		mode = 0x44 + channel;
		
	page = address >> 16;
	offset = address & 0xFFFF;
	length--;
	
	_dma_xfer(channel, page, offset, length, mode);	
}	
		
//
void _dma_xfer(BYTE DMA_channel, BYTE page, unsigned int offset, unsigned int length, BYTE mode)
{
	// make sure this will be done smoothly
	asm("cli");

	// set up the DMA channel so we can use it --
	// this tells the DMA that we're
	// going to be using this channel (it's masked)
	outportb(MaskReg[DMA_channel], 0x04 | DMA_channel);

	// clear any data transfers that are currently executing
	outportb(ClearReg[DMA_channel], 0x00);

	// send the specified mode to the DMA
	outportb(ModeReg[DMA_channel], mode);

	// send the offset address - the first byte is the low base offset,
	// the second byte is the high offset
	outportb(AddrPort[DMA_channel], LOW_BYTE(offset));
	outportb(AddrPort[DMA_channel], HI_BYTE(offset));

	// send the physical page that the data lies on
	outportb(PagePort[DMA_channel], page);

	// send the length of the data.  Again, low byte first
	outportb(CountPort[DMA_channel], LOW_BYTE(length));
	outportb(CountPort[DMA_channel], HI_BYTE(length));

	// ok, we're done.  Enable the DMA channel (clear the mask)
	outportb(MaskReg[DMA_channel], DMA_channel);

	// Re-enable interrupts before we leave
	asm("sti");
}

//

