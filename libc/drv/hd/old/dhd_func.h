#ifndef __INCLUDED_DHD_FUNC_H__
#define __INCLUDED_DHD_FUNC_H__

int dhd_waitcompletion(HDSPEC *h);
int dhd_waituntilready(HDSPEC *h);
void dhd_delay1(void);
BYTE dhd_readerror(void);
int dhd_readstatus(void);
void dhd_waitideirq(int idenr);
int dhd_isDriveBusy(void);
int dhd_isError(void);
int dhd_GetStatus(void);

#endif
