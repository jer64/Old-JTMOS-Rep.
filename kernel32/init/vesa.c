//////////////////////////////////////////////////////////////////////////////////
// vesa.c
// VESA Linear Frame Buffer Support.
// (C) 2008 by Jari Tuominen.
//////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "paging.h"
#include "initsystem.h"
#ifdef GRAOS
#include "graos.h"
#include "directdraw.h"
#endif
#include "vesa.h"

// EGA palette.
BYTE egapal[]={
     0,0,0,
     128,0,0,
     0,128,0,
     128,128,0,
     0,0,128,
     128,0,128,
     0,128,128,
     192,192,192,
     128,128,128,
     255,0,0,
     0,255,0,
     255,255,0,
     0,0,255,
     255,0,255,
     0,255,255,
     255,255,255,
};

//
BYTE *vesa_frame_buf = NULL;
int vesa_enabled = FALSE;
BYTE *vb,*vesainfo;

//---------------------------------------------------------------------------------------
//
void BlitCurrentTerminalToVesa(void)
{
	SCREEN *s;
	BYTE *buf;
	static char str[256];
	int i,i2,l,x,y;

	//
	buf = screens[ID_VIRTUAL_TERMINAL_1].buf;

	//
	l = 80*60*2;
	for(y=0,i=0; y<60; y++)
	{
		flashy(y*640*8);
		for(x=0; x<80; x++,i+=2)
		{
			str[x] = buf[i];
		}
		str[x] = 0;
#ifdef GRAOS
		DirectDrawText(vesa_frame_buf,640,480,
			str,
			0,y<<3);
#endif
	}

	//
}

//
static void flash(void)
{
	//
	sleep(1);
	xmemset(vesa_frame_buf, RED, 640*480);
	sleep(1);
	xmemset(vesa_frame_buf, GREEN, 640*480);
	sleep(1);
	xmemset(vesa_frame_buf, BLUE, 640*480);
	sleep(1);
}

//
void flashy(DWORD ad)
{
	//
	sleep(1);
	xmemset(vesa_frame_buf+ad, RED, 640*8);
	sleep(1);
	xmemset(vesa_frame_buf+ad, GREEN, 640*8);
	sleep(1);
	xmemset(vesa_frame_buf+ad, BLUE, 640*8);
	sleep(1);
}

//---------------------------------------------------------------------------------------
//
// Checks whether if bootloader made a VESA video mode setup for us.
//
int VesaSetup(void)
{
	unsigned short int *checksum,*measured_checksum;
	void *biosinfo;
	LDR *l;
	WORD *bootdrv;
	static char bootstr[20];
	DWORD ad,ad2;
	BYTE *vesainfo;
	static BYTE *vb,*p;
	DWORD *ptr;
	int i,i2,i3,i4,
		x,y;

	//
	vesainfo =		0x4000; // VESA information structure.
	ptr =			0x4028; // PTR to VESA linear frame buffer.
	vb = *ptr;
	vesa_frame_buf = vb;

	//
	if( (*ptr&0xFFFFF000)==NULL )
	{
		vesa_enabled = FALSE;
		return FALSE;
	}

	// ------ VESA OKAY ----------------------------------------
	//
	outportb(0x3C8, 0);
	for(i=0,p=egapal; i<16; i++) 
	{
		outportb(0x3C9, p[i*3+2]>>2);
		outportb(0x3C9, p[i*3+1]>>2);
		outportb(0x3C9, p[i*3+0]>>2);
	}
	for(i=16; i<(256); i++) 
	{
		outportb(0x3C9, 63);
		outportb(0x3C9, i/4);
		outportb(0x3C9, i/4);
	}

/*	for(i2=0;; i2++)
	{
		for(i=0; i<(640*480); i++) { vesa_frame_buf[i] = i+i2; }
	}*/

	//
	vesa_enabled = TRUE;
	//
	xmemset(vesa_frame_buf, BLACK, 640*480);
	//
	VisibleTerminals_SetHWCopy(TRUE);
	//
#ifdef GRAOS
	DirectDrawText(vesa_frame_buf,640,480,
		"jtmos operating system",
		320-(12*8),240-4);
#endif

	//
	printk("%s: VESA ENABLED\n", __FILE__);
	printk("%s: vb = 0x%x\n", __FILE__, vb);

	// Dump VESA information block.
	for(i=0; i<0x30; i++)
	{
		if( (i&15)==0 ) { printk("\n"); }
		printf("%x,", vesainfo[i]);
	}
	printf("\n");

	//
	printk("VESA linear frame buffer at 0x%x.\n", vesa_frame_buf);

	//
	//BlitCurrentTerminalToVesa();

	//
	return TRUE;
}

//

