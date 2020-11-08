#ifndef __INCLUDED_MSDOSIO_H__
#define __INCLUDED_MSDOSIO_H__

//
/*
0-10    File name (8 bytes) with extension (3 bytes)
11      Attribute - a bitvector. Bit 0: read only. Bit 1: hidden.
        Bit 2: system file. Bit 3: volume label. Bit 4: subdirectory.
        Bit 5: archive. Bits 6-7: unused.
12-21   Reserved (see below)
22-23   Time (5/6/5 bits, for hour/minutes/doubleseconds)
24-25   Date (7/4/5 bits, for year-since-1980/month/day)
26-27   Starting cluster (0 for an empty file)
28-31   Filesize in bytes
*/
//
typedef struct
{
	char filename[11];
	char attribute;
	char reserved[9];
	WORD time;
	WORD date;
	char starting_cluster[2];
	DWORD filesize;
}FAT12_FILE_ENTRY;

#endif

