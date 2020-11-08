#ifndef __INCLUDED_MSDOSMETADATA_H__
#define __INCLUDED_MSDOSMETADATA_H__

//
typedef struct
{
        //
        int mounted;
        int bufsiz; // buffer size in bytes
        char *buf; // buffer for the disk image(buffered)
        int dnr,db;
        DWORD *fat;
        int l_fat; // number of 32-bit
                   // FAT entries on the fat buffer.
	int l_bootsector;
	BYTE *bootsector; // 512 bytes
}MSDOSIO;

//
typedef struct
{
	// Realized (1) or not (0).
	int realized;

	// Realized data on a buffer, when realized != 0.
	char *buf;
	int l_buf;

	// Focus cursor.
	struct {
	int dnr,db;
	}focus;
}METADATA;

#endif


