#ifndef __INCLUDED_LPT_H__
#define __INCLUDED_LPT_H__

//
typedef struct
{
 // Non-zero if the port does work
 char isWorking;
 // I/O address for the port
 int ad;
 // IRQ for the port
 int irq;
 // DMA for the port
 int dma;
}LPTPORT;

//
void lpt_transmit_byte(unsigned int val);
signed int lpt_receive_byte(void);
unsigned int lpt_input_value(void);
int lpt_get(void);
int lpt_put(int);

//
extern LPTPORT lptport[8];

//
extern int lpt_base;

//
#endif

//
