//
#ifndef __INCLUDED_SLIPCONFIG_H__
#define __INCLUDED_SLIPCONFIG_H__

//
#define SLIP_CFG_FNAME	"slip.cfg"

//
void slipReadConfiguration(void);

//
int slip_debug;
extern char user[100];
extern char passwd[100];

#endif




