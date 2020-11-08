//
#ifndef __INCLUDED_DEVAPI_RW_H__
#define __INCLUDED_DEVAPI_RW_H__

//
#define ENABLE_DEVAPI_HDLED

//
int _writeblock(int dnr, int b, BYTE *buf);
int _readblock(int dnr, int b, BYTE *buf);
int readblock(int dnr, int b, BYTE *buf);
int writeblock(int dnr, int b, BYTE *buf);
int readblock1(long device_nr,
                long block_nr,
                BYTE *buf);
int readblockCached(long device_nr,
		long block_nr,
		BYTE *buf);
int writeblock1(long device_nr,
                long block_nr,
                BYTE *buf);
int writeblockCached(long device_nr,
                long block_nr,
                BYTE *buf);

#endif



