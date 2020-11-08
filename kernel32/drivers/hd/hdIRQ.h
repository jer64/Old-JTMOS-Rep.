#ifndef __INCLUDED_HDIRQ_H__
#define __INCLUDED_HDIRQ_H__

//
void hdIRQ14(void);
void hdIRQ15(void);
void dhd_setuphdirqs(void);
extern void hdLowIRQ14(void);
extern void hdLowIRQ15(void);

#endif


