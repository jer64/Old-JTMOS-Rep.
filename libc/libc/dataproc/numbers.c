// numbers.c - used by f.e. printf.c
//
#include <stdio.h>

//
void p8(char v)
{
	//
	char str[100];

	//
	shex8(str,v);
	puts(str);
}

//
void p16(int v)
{
	//
	p8((v>>8)&255);
	p8(v&255);
}

//
void p32(long v)
{
	//
	p16((v>>16)&65535);
	p16(v&65535);
}

//
void p32z(long v)
{
	//
	char str[100];

	//
	shex32z(str, v);
	//
	puts(str);
}

// Print Decimal Hex value
// (0x0100 -> 01.00);
void pdh32(long v)
{
	//
	p16(v>>16); printf(".");
	p16(v&0xffff);
}

// print decimal (in non-zero fashioned mode)
void pd32(long v)
{
	//
	char str[100];

	//
	sdec32(str,v);
	//
	puts(str);
}

// print decimal (in zero fashioned mode)
void pdz32(long v)
{
	//
	char str[100];

	//
	sdec32z(str,v);
	//
	puts(str);
}

//

