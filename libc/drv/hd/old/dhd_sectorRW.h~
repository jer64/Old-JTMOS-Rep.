#ifndef __INCLUDED_DHD_SECTORRW_H__
#define __INCLUDED_DHD_SECTORRW_H__

// Double checks
#define VERIFY_READS
#define VERIFY_WRITES

//
int dhd_sectorRW1(HDSPEC *h,
                BYTE *buf,long headNr,long sectorNr,
                long cylinder,
                long driveNr, // 0=master,1=slave(I think)
                long rw, // 0=read, 1=write
                long sectorCount);

int dhd_sectorRWCached(HDSPEC *h,
                BYTE *buf, long headNr, long sectorNr,
                long cylinder,
                long driveNr, // 0=master,1=slave(I think)
                long rw);

int dhd_sectorRW(HDSPEC *H,
                BYTE *buf, long headNr, long sectorNr,
                long cylinder,
                long driveNr, // 0=master,1=slave(I think)
                long rw);

		
#endif
