#ifndef __INCLUDED_GUIRECT_H__
#define __INCLUDED_GUIRECT_H__

#include <stdio.h>
#include "graos.h"

int fillRect(VMODE *v, int x1,int y1, int x2, int y2,  int vari);
int drawRect(VMODE *v, int x1,int y1, int x2, int y2,  int vari);
int eraseArea(VMODE *v, int x1,int y1, int x2,int y2);

#endif


