#ifndef __INCLUDED_SERIALMOUSE_H__
#define __INCLUDED_SERIALMOUSE_H__

#include "SerialMouseDev.h"

void SerialMouse_irqhandler(int nCom);
void SerialMouse_irqhandler4(void);
void SerialMouse_irqhandler3(void);
void SerialMouse_setupirqs(void);
extern void SerialMouse_asmirqhandler4(void);
extern void SerialMouse_asmirqhandler3(void);
int SerialMouse_SetDivisor(int nCom, int div);
void LimitToRectArea(int *x, int *y,
                int x1,int y1,int x2,int y2);

#endif

