// mmBit.h - MM bitmap bit functions header file
#ifndef __INCLUDED_MMBIT_H__
#define __INCLUDED_MMBIT_H__


//
extern BYTE mmOrBits[];
extern BYTE mmAndBits[];

// Remove a single bit from the MM bitmap
#define mmBitRemove(WHICH) \
	if( (WHICH>>3)>=p_mm->l_bitmap )		panic("BIT CASE 1 ERROR"); \
	else p_mm->bitmap[WHICH>>3] &= mmAndBits[WHICH&7]

// Set a single bit on the MM bitmap
#define mmBitSet(WHICH) \
	if( (WHICH>>3)>=p_mm->l_bitmap )		panic("BIT CASE 2 ERROR"); \
	else p_mm->bitmap[WHICH>>3] |= mmOrBits[WHICH&7]

// Read the status of the specified page number #
#define mmBitGet(WHICH) \
	(p_mm->bitmap[WHICH>>3] & mmOrBits[WHICH&7])

// Set [WHICH 0-X] -bit to [STATE 0|1]
#define BIT(WHICH, STATE) \
	if( (WHICH>>3)>=p_mm->l_bitmap )		panic("BIT CASE 4 ERROR"); \
	else \
	if(STATE==0) \
		mmBitRemove(WHICH); \
	else \
		mmBitSet(WHICH)

//
int mmSetRegion(DWORD s, DWORD e, int STATE);
int mmFreeRegionAD(char *p_s, char *p_e);
int mmReserveRegionAD(char *p_s, char *p_e);
int mmFreeRegion(DWORD s, DWORD e);
int mmReserveRegion(DWORD s, DWORD e);


#endif


