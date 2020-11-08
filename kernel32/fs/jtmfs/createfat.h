//
#ifndef __INCLUDED_CREATEFAT_H__
#define __INCLUDED_CREATEFAT_H__

//
int jtmfat_createfat(int dnr);;
void WriteInformationBlock(int dnr, JTMFAT_INFOBLOCK *inf);
void ReadInformationBlock(int dnr, JTMFAT_INFOBLOCK *inf);
void SetKernelChecksum(int dnr, WORD checksum);
void WriteKernel(int dnr, void *ker);
int CopyKernel(const char *src, const char *dst);
WORD DetermineKernelChecksum(BYTE *buf, int len);
JTMFAT_INFOBLOCK *BuildInfoBlock(int dnr, int bootdrv);
static void InstallBootLoader(int dnr, int bdrv);
static void BuildBootRecord(int dnr, BOOTRECORD *br, int bdrv);

#endif




