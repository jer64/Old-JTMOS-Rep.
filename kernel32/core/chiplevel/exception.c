/*
 * JTMOS SYSTEM EXCEPTION HANDLING CODE.
 * (C) 2001-2006 by Jari Tuominen(jari@vunet.org).
 *
 * Because none of these exception handlers really don't
 * actually return to the code which generate the exception
 * we haven't made a stack pushing code which would
 * require a low level assembly code.
 *
 * 16.9.2003: Added a new handler for "COPROCESSOR NOT FOUND" error.
 * This handler attempts to recover new exceptions caused by
 * implementation of new TSS based task switching. 
 *
 * exception.c - C language portion of the exception handling code.
 * exc.inc - assembly language portion of the exception handling code.
 */

//
#include "kernel32.h"
#include "int.h"
#include "exception.h"

//
EXCEPTION exception;

//
void exception_0(void)
{
	exception.code=0;
	FATALEXCEPTION("(00) divide by zero");
}

//
void exception_1(void)
{
	exception.code=1;
	FATALEXCEPTION("(01) single step");
}

//
void exception_2(void)
{
	exception.code=2;
	FATALEXCEPTION("(02) non-maskable (NMI)");
}

//
void exception_3(void)
{
	exception.code=3;
	FATALEXCEPTION("(03) breakpoint");
}

//
void exception_4(void)
{
	exception.code=4;
	FATALEXCEPTION("(04) overflow trap");
}

//
void exception_5(void)
{
	exception.code=5;
	FATALEXCEPTION("(05) BOUND range exceeded");
}

//
void exception_6(void)
{
	exception.code=6;
	FATALEXCEPTION("(06) invalid opcode");
}

// (obsolete, handled 100% in the exc.inc)
void exception_7(void)
{
	exception.code=7;
	FATALEXCEPTION("(07) coprocessor not available");
}

//
void exception_8(void)
{
	exception.code=8;
	FATALEXCEPTION("(08) double fault exception");
}

//
void exception_9(void)
{
	exception.code=9;
	FATALEXCEPTION("(09) coprocessor segment overrun");
}

//
void exception_10(void)
{
	exception.code=10;
	FATALEXCEPTION("(10) invalid task state segment");
}

//
void exception_11(void)
{
	exception.code=11;
	FATALEXCEPTION("(11) segment not present");
}

//
void exception_12(void)
{
	exception.code=12;
	FATALEXCEPTION("(12) stack exception");
}

//
void exception_13(void)
{
	exception.code=13;
	FATALEXCEPTION("(13) general protection fault");
}

//
void exception_14(void)
{
	exception.code=14;
	FATALEXCEPTION("(14) page fault");
}

//
void exception_15(void)
{
	exception.code=15;
	FATALEXCEPTION("(15) reserved(??)");
}

// Coprocessor error.
void exception_16(void)
{
	exception.code=16;
	FATALEXCEPTION("(16) coprocessor error");
}

//
void setup_exception_handlers(void)
{
	//
	setint( 0,  ASMexception_0,  SEG_CODE32, 0x8E00 );
	setint( 1,  ASMexception_1,  SEG_CODE32, 0x8E00 );
	setint( 2,  ASMexception_2,  SEG_CODE32, 0x8E00 );
	setint( 3,  ASMexception_3,  SEG_CODE32, 0x8E00 );
	setint( 4,  ASMexception_4,  SEG_CODE32, 0x8E00 );
	setint( 5,  ASMexception_5,  SEG_CODE32, 0x8E00 );
	setint( 6,  ASMexception_6,  SEG_CODE32, 0x8E00 );
	setint( 7,  ASMexception_7,  SEG_CODE32, 0x8E00 );
	setint( 8,  ASMexception_8,  SEG_CODE32, 0x8E00 );
	setint( 9,  ASMexception_9,  SEG_CODE32, 0x8E00 );
	setint( 10, ASMexception_10, SEG_CODE32, 0x8E00 );
	setint( 11, ASMexception_11, SEG_CODE32, 0x8E00 );
	setint( 12, ASMexception_12, SEG_CODE32, 0x8E00 );

	// GPF
	setint( 13, ASMexception_13, SEG_CODE32, 0x8E00 );

	//
	setint( 14, ASMexception_14, SEG_CODE32, 0x8E00 );
	setint( 15, ASMexception_15, SEG_CODE32, 0x8E00 );
	setint( 16, ASMexception_16, SEG_CODE32, 0x8E00 );
}

//

