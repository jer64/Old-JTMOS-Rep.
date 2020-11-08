/* ---------------------------------------------------------
 * TSS Descriptor
 *
 * D1(Double word #1)_______________________________________
 *
 * bits 0-15		Segment limit 	bits 0-15
 * bits 16-31		Base address  	bits 0-15
 *
 * D2_______________________________________________________
 * bits 0-7		Base          	bits 16-23
 * bit 8		1
 * bit 9		Busy bit(set to 0)
 * bit 10		0
 * bit 11		1
 * bit 12		0
 * bits 13-14		DPL	(Descriptor Privilege Level)
 * bit 15		Present bit(set to 1)
 *  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
 * bits 16-19		Segment limit	bits 16-19
 * bit 20		AVL (Available for use by system software)	(set to 1)
 * bit 21		0
 * bit 22		0
 * bit 23		G	(Granularity)		(set to 0)
 * bits 24-31		Base address	bits 24-31
 *
 *	Thats it.
 *
 *
 */
typedef struct
{
	DWORD d1,d2;
}SEGDES;

// ---------------------------------------------------------
// CODE/DATA SEGMENT STRUCTURE
typedef struct
{
	// Base / Limit
	WORD	seg0_15;	// 0
	WORD	base0_15;	// 2
	// Base / Limit II + access & flags
	BYTE	base16_23;	// 4
	BYTE	flags;		// 5
	BYTE 	access;		// 6
	BYTE	base24_31;	// 7
}CDSEG;
