//
#ifndef __INCLUDED_DEBUG_H__
#define __INCLUDED_DEBUG_H__

// Lock IDs
#define ID_REPORT_MESSAGE	1
#define ID_DPRINTK		2
#define ID_PRINTF		3

//
//extern int debug2;
int ReportMessage(const char *fmt, ...);
void doch(int ch);

#endif



