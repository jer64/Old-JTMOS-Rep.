//
#include <stdio.h>

//
typedef unsigned long int DWORD;
typedef unsigned char BYTE;
typedef unsigned short int WORD;

//
typedef struct
{
        // Base / Limit
        WORD    seg0_15;        // 0
        WORD    base0_15;       // 2
        // Base / Limit II + access & flags
        BYTE    base16_23;      // 4
        BYTE    flags;          // 5
        BYTE    access;         // 6
        BYTE    base24_31;      // 7
}CDSEG;

//
int main(int argc, char **argv)
{
	DWORD d[2];
	CDSEG *s;

	// Check arg. count
	if(argc<3)
	{
		printf("Usage: tssdes [TSS DESC. DWORD 1/2] [TSS DESC. DWORD 2/2]\n");
		return 0;
	}

	// Read args
	sscanf(argv[1], "%x", &d[0]);
	sscanf(argv[2], "%x", &d[1]);

	// Get segment PTR
	s = &d;

	// Print base
	printf("segment base =  %x\n",
		s->base0_15 | (s->base16_23<<16) | (s->base24_31<<24));	

	// Print limit
	printf("segment limit = %x\n",
		s->seg0_15);

	//
	printf("Segment type = %d\n",
		s->flags & 0xF);

	//
	if(s->flags & 0x80)
	{
		printf("Segment is present.\n");
	}

	//
	if(s->access & 0x80)
	{
		printf("Granularity is set.\n");
	}

	//
	return 0;
}
