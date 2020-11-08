/*
 * llexception.c & exc.asm - low level backup-exception handlres.
 */

//
#include "kernel32.h"
#include "int.h"
#include "exception.h"
#include "llexception.h"
#ifdef GRAOS
#include "graos.h"
#endif
#include "vesa.h"

//
EXCEPTION exception;

//
void SafeExcReport(const char *msg)
{
	static char str[100];
	static int i,i2,l;
	static DWORD *ptr;
	static TSSEG *ts;
	static char *excmsg = "Exception caught";
	static char *excdea = "                ", *p;
	static int x;

	//
        if( (vesa_enabled && vesa_frame_buf!=NULL) )
        {
#ifdef GRAOS
		//
		DirectFillRect(vesa_frame_buf, 640,480, 0,0, 640,8*4, 0);
		//
		DirectDrawText(vesa_frame_buf,640,480,
			excmsg,
			0,8*0);
		DirectDrawText(vesa_frame_buf,640,480,
			msg,
			0,8*1);
#endif
	}
	else
	{
		// Tell about exception.
		p = 0xB8000;

		// Wipe off some space.
		for(i=0; i<(80*2*5); i+=2)
		{
			p[i+0] = ' ';
			p[i+1] = 0x0F;
		}

		//
		l = strlen(excmsg);
		for(i=0,i2=0; i<l; i++,i2+=2)
		{
			p[i2+0] = excmsg[i];
			p[i2+1] = 0x1F;
		}
		l = strlen(msg); p+=160;
		for(i=0,i2=0; i<l; i++,i2+=2)
		{
			p[i2+0] = msg[i];
			p[i2+1] = 0x1F;
		}
	}

	//
	for(x=0; ; x++)
	{
		outportb(0x3c8, 0);
		outportb(0x3c9, x);
	}
	HaltNow();
}

//
void llexception_0(void)
{
	exception.code=0;
	
	SafeExcReport("(00) divide by zero");
}

//
void llexception_1(void)
{
	exception.code=1;
	SafeExcReport("(01) single step");
}

//
void llexception_2(void)
{
	exception.code=2;
	SafeExcReport("(02) non-maskable (NMI)");
}

//
void llexception_3(void)
{
	exception.code=3;
	SafeExcReport("(03) breakpoint");
}

//
void llexception_4(void)
{
	exception.code=4;
	SafeExcReport("(04) overflow trap");
}

//
void llexception_5(void)
{
	exception.code=5;
	SafeExcReport("(05) BOUND range exceeded");
}

//
void llexception_6(void)
{
	exception.code=6;
	SafeExcReport("(06) invalid opcode");
}

// (obsolete, handled 100% in the exc.asm)
void llexception_7(void)
{
	exception.code=7;
	SafeExcReport("(07) coprocessor not available");
}

//
void llexception_8(void)
{
	exception.code=8;
	SafeExcReport("(08) double fault exception");
}

//
void llexception_9(void)
{
	exception.code=9;
	SafeExcReport("(09) coprocessor segment overrun");
}

//
void llexception_10(void)
{
	exception.code=10;
	SafeExcReport("(10) invalid task state segment");
}

//
void llexception_11(void)
{
	exception.code=11;
	SafeExcReport("(11) segment not present");
}

//
void llexception_12(void)
{
	exception.code=12;
	SafeExcReport("(12) stack exception");
}

//
void llexception_13(void)
{
	exception.code=13;
	SafeExcReport("(13) general protection fault");
}

//
void llexception_14(void)
{
	static char str[256];

	//
	sprintf(str, "(14) page fault - CR2 = 0x%x", GETCR2());

	//
	exception.code=14;
	SafeExcReport(str);
}

//
void llexception_15(void)
{
	exception.code=15;
	SafeExcReport("(15) reserved(??)");
}

// Coprocessor error.
void llexception_16(void)
{
	exception.code=16;
	SafeExcReport("(16) coprocessor error");
}

//
void setup_llexception_handlers(void)
{
	//
	setint( 0,  ASMllexception_0,  SEG_CODE32, 0x8E00 );
	setint( 1,  ASMllexception_1,  SEG_CODE32, 0x8E00 );
	setint( 2,  ASMllexception_2,  SEG_CODE32, 0x8E00 );
	setint( 3,  ASMllexception_3,  SEG_CODE32, 0x8E00 );
	setint( 4,  ASMllexception_4,  SEG_CODE32, 0x8E00 );
	setint( 5,  ASMllexception_5,  SEG_CODE32, 0x8E00 );
	setint( 6,  ASMllexception_6,  SEG_CODE32, 0x8E00 );
	setint( 7,  ASMllexception_7,  SEG_CODE32, 0x8E00 );
	setint( 8,  ASMllexception_8,  SEG_CODE32, 0x8E00 );
	setint( 9,  ASMllexception_9,  SEG_CODE32, 0x8E00 );
	setint( 10, ASMllexception_10, SEG_CODE32, 0x8E00 );
	setint( 11, ASMllexception_11, SEG_CODE32, 0x8E00 );
	setint( 12, ASMllexception_12, SEG_CODE32, 0x8E00 );

	// GPF
	setint( 13, ASMllexception_13, SEG_CODE32, 0x8E00 );

	//
	setint( 14, ASMllexception_14, SEG_CODE32, 0x8E00 );
	setint( 15, ASMllexception_15, SEG_CODE32, 0x8E00 );
	setint( 16, ASMllexception_16, SEG_CODE32, 0x8E00 );
}

//

