/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * J-active area system v1.10
 *
 * (C)1998-99 by Jari Tapio Tuominen, all rights reserved.
 *
 */

#define ACTMOD_ON       1
#define ACTMOD_BUTTON1  2
#define ACTMOD_BUTTON2  4
#define ACTMOD_BUTTON3  8

void jtmact_remove(long jid)
{
 jareas.j[jid].state=0;
}

void jtmact_initializesystem(void)
{
 //
 jtmapp_wiper(sizeof(JAREAS),&jareas);

 //
 jareas.amount=0;
}

int awrite(char *texti,void *_handler)
{
 //
 long x1,y1,x2,y2;

 //
 x1=jtmscr.writex; y1=jtmscr.writey;
 write(texti);
 x2=jtmscr.writex; y2=jtmscr.writey+8;

 //
 return jtmact_createactivearea(x1,y1,x2,y2,_handler);
}

int jtmact_createactivearea(int x1,int y1,int x2,int y2,void *_handler)
{
 //

 //
 jareas.j[jareas.amount].x1=x1;
 jareas.j[jareas.amount].y1=y1;
 jareas.j[jareas.amount].x2=x2-1;
 jareas.j[jareas.amount].y2=y2-1;
 jareas.j[jareas.amount].mode=0;
 jareas.j[jareas.amount].handler=_handler;
 jareas.j[jareas.amount].state=1;

 //
 jareas.amount++;

 //
 return (jareas.amount-1);
}

void jtmact_keepuptodate(void)
{
 //
 int i,i2;

 //
 for(i=0; i<jareas.amount; i++)
 {
  //
  if( !jareas.j[i].state )goto nexti;

  // Init mode.
  jareas.j[i].mode=0;

  // Perform a check if Mouse arrow might currently fit
  // inside the active area.
  if( jareas.j[i].x1<=jtmmouse.x && jareas.j[i].y1<=jtmmouse.y &&
      jareas.j[i].x2>=jtmmouse.x && jareas.j[i].y2>=jtmmouse.y)
  {
   jareas.j[i].mode=jareas.j[i].mode|1; // Bit 0: Mouse arrow inside A.R. ?
   if(jtmmouse.button1)
   {
    jareas.j[i].mode=jareas.j[i].mode|2; // Bit 1: Inside and button1 down.
   }
   if(jtmmouse.button2)
   {

    jareas.j[i].mode=jareas.j[i].mode|4; // Bit 2: Inside and button2 down.
   }
   if(jtmmouse.button3)
   {
    jareas.j[i].mode=jareas.j[i].mode|8; // Bit 3: Inside and button3 down.
   }

  }
  // Call handler if one defined for the purpose.
  // (Remember: Bit0 of 'mode' tells if it's inside area).
  if(jareas.j[i].handler)jareas.j[i].handler(jareas.j[i].x1,
                                             jareas.j[i].y1,
                                             jareas.j[i].x2,
                                             jareas.j[i].y2,
                                             jareas.j[i].mode);
nexti:
 }
}
