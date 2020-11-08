/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*       JTMOS interrupt handler/server center
         -------------------------------------

         This is the main code of JTMOS keyboard handler,
         it is the only thing which really controls the heart of keyboard
         management system. At the moment it's fully functioning on Finnish
         keyboard layout, I'll fix it since I'll recognize that enough
         important thing to do in my programming priority.

   JTMOS interrupt server version 1.00
   (C)1997-98 by Jari Tapio Tuominen, all rights reserved.
   Borland C/C++ users shoudn't include this library, this is only 4 DJGPP v2.x.
   Forget Borland, forget Windows choose real life instead,
   which is of course => JTMOS <= ayeah!

   Notice:

   -Sound Blaster support -- IRQ #5 (0x0d) handler
 */

#ifndef __JTMINTER_H__
#define __JTMINTER_H__
#define __JTMINTER_H_INCLUDED TRUE
#define JTM_IntSysVer 1.00

/*
Alternatively you may wish to write your own interrupt handler to process
mouse events as they happen. When a mouse event occurs, 3 interrupts are
generated and the bytes are availble via the COM port.

                  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                  ³        Interrupt    Port ³
                  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
                  ³ COM1      $0C       $3F8 ³
                  ³ COM2      $0B       $3F8 ³
                  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

The three bytes sent are formatted as follows:


               1st byte        2nd byte         3rd byte
          ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
          ³-³1³?³?³X³X³Y³Y³³-³0³X³X³X³X³X³X³³-³0³Y³Y³Y³Y³Y³Y³
          ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙ
               ³ ³ ÀÂÙ ÀÂÙ      ÀÄÄÄÄÂÄÄÄÄÙ      ÀÄÄÄÄÂÄÄÄÄÙ
               ³ ³  ³   ³            ³                ³
               ³ ³  ³   ÀÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄ¿       ³
               ³ ³  ÀÄÄÄÄÄÄÄÄ¿       ³        ³       ³
               ³ ³          ÚÁ¿ ÚÄÄÄÄÁÄÄÄÄ¿  ÚÁ¿ ÚÄÄÄÄÁÄÄÄÄ¿
               ³ ³         ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
               ³ ³         ³ ³ ³ ³ ³ ³ ³ ³ ³³ ³ ³ ³ ³ ³ ³ ³ ³
 Left Button ÄÄÙ ³         ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙ
Right Button ÄÄÄÄÙ            X increment      Y increment


The X and Y increment values are in 2's compliment signed char format. (BTW
thanks go to Adam Seychell for posting this info to comp.os.msdos.programmer).
*/
                       /*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                         ³ IRQ Jumper             ³
                         ³  Setting     Interrupt ³
                         ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
                         ³    2            0Ah    ³
                         ³    3            0Bh    ³
                         ³    5            0Dh    ³
                         ³    7            0Fh    ³
                         ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

int     ___i8work=0;
int     keybirqnum=0x09; // Keyboard IRQ handler ! Oh boy, what a start!
int     blasterirqnum=0x0d; // in priority it's 5
int     hookirqnum=0x08;// = Timer, which is quite good choice for
                        //   all kind of standard operations, like mouse, counters, etc..
extern void jtm_userinterrupt(void);
extern void jtm_usersbirq(void);
extern void jtm_userkeyb(void);
extern void uusi_usermouse(void);
_go32_dpmi_seginfo si_oldinterrupt,            si_newinterrupt,
                   si_oldsbirq,                si_newsbirq,
                   si_oldkeybinterrupt,        si_newkeybinterrupt,
                   si_oldmouseinterrupt,       si_newmouseinterrupt;

void uusi_sbirq(void)
{
 //
 enable();
 jtm_rdsp();
#ifdef ALLOWSBINT
 jtm_usersbirq();
#endif
 outportb(0x20,0x20);
}

// UUSI_I8 function
//
// Timer trigger function.
//
//
//
void uusi_i8(void)
{
 if(___i8work==0)
 {
  ___i8work=0xff;
#ifndef NOTI8
  jtm_userinterrupt();
#endif
  //Finish interrupt, return to normal works.
  outportb(0x20,0x20);
  ___i8work=0;
 }
}

// JTMKEYB_OUTASCBUFFER
//
// Notice: String ascbuf is ASCIIZ based.
//
//
int jtmkeyb_outascbuffer(int key)
{
 int akey;
 //
 if(key==-1 || key==0)return 0;
 if(jtmkeyb.ascbuf[0]==0){jtmkeyb.aplace=0;}
 if(jtmkeyb.aplace>MaxKeyBuffer)
 {
  jtmkeyb.aplace=0;
 }
 akey=jtmkeyb_scantoasc(key);
 if(akey==0)return 0;
 if(akey=='\b')
 {
  if(jtmkeyb.aplace!=0)
  {
   jtmkeyb.aplace--;
   jtmkeyb.ascbuf[jtmkeyb.aplace]=0;
   return 0; // 0=No error.
  }
  else
  {
   return 1;
  }
 }
 if(akey==13)
 {
  jtmkeyb.aplace=0;
  jtmkeyb.ascbuf[jtmkeyb.aplace]=0;
  return 0; // 0=No error.
 }
 if(jtmkeyb.aplace<MaxKeyBuffer)
 {
  jtmkeyb.ascbuf[jtmkeyb.aplace]=akey;
  jtmkeyb.aplace++;
  jtmkeyb.ascbuf[jtmkeyb.aplace]=0;
  return 0;
 }
 return 1;
}

long jtmkeyb_translatektabasoutput()
{
  if(jtmkeyb.specialkeyconvert2asc && !jtmkeyb.ExtraKey)
  {
   // CRSR_UP? -> Return -2(Means macro string)
   if(jtmkeyb.ktab[72] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[crup]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // CRSR_LEFT? -> Return -2(Means macro string)
   if(jtmkeyb.ktab[75] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[crleft]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // CRSR_DOWN? -> Return -2(Means macro string)
   if(jtmkeyb.ktab[80] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[crdown]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // CRSR_RIGHT? -> Return -2(Means macro string)
   if(jtmkeyb.ktab[77] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[crright]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // Dialog(W9x) -> Return -2(Means macro string)
   if(jtmkeyb.ktab[93] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[dialog]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // TAB -> Return -2(Means macro string)
   if(jtmkeyb.ktab[15] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[tab]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // CAPSLOCK -> Return -2(Means macro string)
   if(jtmkeyb.ktab[58] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[capslock]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // INSERT -> Return -2(Means macro string)
   if(jtmkeyb.ktab[82] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[insert]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // DELETE -> Return -2(Means macro string)
   if(jtmkeyb.ktab[83] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[delete]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // HOME -> Return -2(Means macro string)
   if(jtmkeyb.ktab[71] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[home]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // END -> Return -2(Means macro string)
   if(jtmkeyb.ktab[79] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[end]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // PGUP -> Return -2(Means macro string)
   if(jtmkeyb.ktab[73] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[pgup]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }

   // PGDOWN -> Return -2(Means macro string)
   if(jtmkeyb.ktab[81] && jtmkeyb.ktab[42])
   {
    strcpy(jtmkeyb.macro,"[pgdown]#");
    jtmkeyb.macrooffs=0;
    jtmkeyb.ExtraKey=0xfe;
    return -2;
   }
  }
}

// This handler is based on hardware interrupts and is
// very low left stuff, so beware of writting delaying code.
//
void KeyboardHandler(void)
{
 jtmkeyb.keypressed=1;
 jtmkeyb.key=inportb(0x60);
 if((jtmkeyb.key&128)==128)      // Mark released keys (bit 7 set)
 {
  jtmkeyb.ktab[jtmkeyb.key&0x7f]=0;
  jtmkeyb.rkey=jtmkeyb.key&0x7f;
  goto keyokay;
 }
 jtmkeyb.ktab[jtmkeyb.key&0x7f]=0xff;
 if(jtmkeyb.place<MaxKeyBuffer)  // Update
 {
  jtmkeyb.buffer[jtmkeyb.place]=jtmkeyb.key;
  jtmkeyb_outascbuffer(jtmkeyb.key);
  jtmkeyb.place++;
 }

 //
 jtmkeyb_translatektabasoutput();

 //
keyokay:
 outportb(0x20,0x20);
}

// This function reads keyboard buffer written by the keyboardhandler
// function.
//
int jtmkeyb_read(void)
{
 //
 int key;

 //
 if(jtmkeyb.place!=0)
 {
  jtmkeyb.place--;
  key=jtmkeyb.buffer[jtmkeyb.place];
 }
 else
 {
  key=-1;
 }
 return key;
}

// JTMKEYB_SCANTOASC function
//
int jtmkeyb_scantoasc(int mark)
{
 //
 long ad,ad2;

 //
 // Check out keys that does have more than one keys pressed
 //
 // Handle extra key exception
 //

repeatcheck:
 //
 if(jtmkeyb.ExtraKey==0xfe)
 {
  jtmkeyb.ExtraKey=0xfd;
  return -2;
 }
 if(jtmkeyb.ExtraKey==0xfd)
 {
  if( !(ad=jtmkeyb.macro[jtmkeyb.macrooffs]) )
  {
   jtmkeyb.ExtraKey=0;
   goto repeatcheck;
  }
  else
  {
   jtmkeyb.macrooffs++;
   return ad;
  }
 }

 //
 if(jtmkeyb.ExtraKey)
 {
  jtmkeyb.ExtraKey=0;
  jtmkeyb.counter=0;
  return jtmkeyb.ExtraKeyCode;
 }

 //

 //
 if(!jtmkeyb.ExtraKey)
 {
  // <>SHFT+ - -> '_'
  if(((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) && jtmkeyb.ktab[0x35]))
  {
   //
   jtmkeyb.ktab[0x35]=0;
   //
   jtmkeyb.ExtraKeyCode='_';
   jtmkeyb.ktab[0x02]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // ALT + ù -> '~'
  if(jtmkeyb.ktab[0x38] && jtmkeyb.ktab[0x1b])
  {
   //
   jtmkeyb.ktab[0x1b]=0;
   //
   jtmkeyb.ExtraKeyCode='~';
   jtmkeyb.ktab[0x02]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // ^
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) && jtmkeyb.ktab[0x1b])
  {
   //
   jtmkeyb.ktab[0x1b]=0;
   //
   jtmkeyb.ExtraKeyCode='^';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // ù
  if(jtmkeyb.ktab[0x1b])
  {
   //
   jtmkeyb.ktab[0x1b]=0;
   //
   jtmkeyb.ExtraKeyCode='ù';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+1 -> '!'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x02])
  {
   jtmkeyb.ExtraKeyCode='!';
   jtmkeyb.ktab[0x02]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+2 -> '\"'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x03])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ktab[0x3]=0;
   //
   jtmkeyb.ExtraKeyCode='\"';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+3 -> '#'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x04])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='#';
   jtmkeyb.ktab[0x04]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+4 -> 'Ï'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x05])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='Ï';
   jtmkeyb.ktab[0x05]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+5 -> '%'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x06])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='%';
   jtmkeyb.ktab[0x06]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+6 -> '&'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x02])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='&';
   jtmkeyb.ktab[0x07]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+7 -> '/'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x02])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='/';
   jtmkeyb.ktab[0x08]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+8 -> '('
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x09])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode='(';
   jtmkeyb.ktab[0x09]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+9 -> ')'
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x0a])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ExtraKeyCode=')';
   jtmkeyb.ktab[0x0a]=0; jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  // Shift+0 -> '='
  if((jtmkeyb.ktab[0x2a] || jtmkeyb.ktab[0x36]) &&
     jtmkeyb.ktab[0x0b])
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   //
   jtmkeyb.ktab[0xb]=0;
   //
   jtmkeyb.ExtraKeyCode='=';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }

  //
  if(
     jtmkeyb.ktab[0x33]==0xff && jtmkeyb.ktab[0x2a]==0xff
    )
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   jtmkeyb.ktab[0x33]=0;
   //
   jtmkeyb.ExtraKeyCode=';';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  //
  if(
     jtmkeyb.ktab[0x34]==0xff && jtmkeyb.ktab[0x2a]==0xff
    )
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   jtmkeyb.ktab[0x34]=0;
   //
   jtmkeyb.ExtraKeyCode=':';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  //
  if(
     jtmkeyb.ktab[0x08]==0xff && jtmkeyb.ktab[0x2a]==0xff
    )
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   jtmkeyb.ktab[0x08]=0;
   //
   jtmkeyb.ExtraKeyCode='/';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  //
  if(
     jtmkeyb.ktab[0x0c]==0xff && jtmkeyb.ktab[0x38]==0xff
    )
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   jtmkeyb.ktab[0x0c]=0;
   //
   jtmkeyb.ExtraKeyCode='\\';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  //
  if( jtmkeyb.ktab[0x2a]==0xff && jtmkeyb.ktab[0x2b]==0xff
      ||
      jtmkeyb.ktab[0x37]==0xff
    )
  {
   if(jtmkeyb.place>=2)jtmkeyb.place--;
   jtmkeyb.ktab[0x2b]=0;
   jtmkeyb.ktab[0x37]=0;
   //
   jtmkeyb.ExtraKeyCode='*';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  //
  if( jtmkeyb.ktab[0x0c]==0xff &&
     (jtmkeyb.ktab[0x2a]==0xff ||
      jtmkeyb.ktab[0x36]==0xff)
    )
  {
   jtmkeyb.ktab[0x2a]=0;
   jtmkeyb.ktab[0x36]=0;
   //
   jtmkeyb.ExtraKeyCode='?';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
  // |
  if( jtmkeyb.ktab[0x56] &&
      jtmkeyb.ktab[0x38]
    )
  {
   jtmkeyb.ktab[0x56]=0;
   //
   jtmkeyb.ExtraKeyCode='|';
   jtmkeyb.ExtraKey=0xff;
   return 0;
  }
 } // <--- END OF EXTRA KEYS!

 // Check out keys that does have less than two keys pressed at once.
 if(jtmkeyb.ExtraKey==0)
 {
  switch(mark)
  {
   // Rivi 1
   case 1:
   return 27;
   case 0x33:
   return ',';
   case 0x34:
   return '.';
   case 0x39:
   return ' ';
   case 0x10:
   return 'q';
   case 0x11:
   return 'w';
   case 0x12:
   return 'e';
   case 0x13:
   return 'r';
   case 0x14:
   return 't';
   case 0x15:
   return 'y';
   case 0x16:
   return 'u';
   case 0x17:
   return 'i';
   case 0x18:
   return 'o';
   case 0x19:
   return 'p';
   // Rivi 2
   case 0x1e:
   return 'a';
   case 0x1f:
   return 's';
   case 0x20:
   return 'd';
   case 0x21:
   return 'f';
   case 0x22:
   return 'g';
   case 0x23:
   return 'h';
   case 0x24:
   return 'j';
   case 0x25:
   return 'k';
   case 0x26:
   return 'l';
   // Rivi 3
   case 0x2c:
    return 'z';
   case 0x2d:
   return 'x';
   case 0x2e:
   return 'c';
   case 0x2f:
   return 'v';
   case 0x30:
   return 'b';
   case 0x31:
   return 'n';
   case 0x32:
   return 'm';
   // Numerot+muut
   case 0x02:
   return '1';
   case 0x03:
   return '2';
   case 0x04:
   return '3';
   case 0x05:
   return '4';
   case 0x06:
   return '5';
   case 0x07:
   return '6';
   case 0x08:
   return '7';
   case 0x09:
   return '8';
   case 0x0a:
   return '9';
   case 0x0b:
   return '0';
   case 0x0e:
   return '\b';
   case 0x1c:
   return 0x0d;
   case 0x0c:
   return '+';
   case 0x35:
   return '-';
   case 41:
   return 'õ';
  }
 }
loppu:
 return 0;
}

extern int jtmkeyb_getch(void)
{
 //
 int jee,key;

 //
 if(jtmkeyb.state==0)
 {
  return getch();
 }
 do
 {
  jee=jtmkeyb_read();
 }while(jee==-1);
 //
 key=jtmkeyb_scantoasc(jee);
 if(key==-1)key=0;
 return key;
}

extern int jtmkeyb_jgetchim(void)
{
 //
 int key,key2;

 //
 if(jtmkeyb.state==0)
 {
  return jgetchim();
 }
 key=jtmkeyb_read();
 key2=jtmkeyb_scantoasc(key);
 if(key2==-1)key2=0;
 return key2;
}

void jtmprocess_keyboardsyncher(void)
{
 jtmkeyb.counter+=12;
}

void jtmkeyb_on(void)
{
 long a1;
 //
 jtmkeyb.scantoascbusy=0;
 jtmkeyb.counter=0;
 jtmkeyb.ExtraKey=0;
 jtmkeyb.ExtraKeyCode=0;
 TM_AddProcess(jtmprocess_keyboardsyncher);
 //
 if(jtmkeyb.state==0)
 {
  jtm_addnotice("Installing Tapio keyboard system driver ...\n");
  jtmkeyb.keypressed=0; // First of all, make sure that keypressed variable
                        // shows that key isn't pressed.
  jtmkeyb.aplace=0;
  jtmkeyb.place=0;
  jtmkeyb.key=0;

  // Allocate
  if(jtmkeyb.buffer==NULL){ jtmkeyb.buffer=malloc(MaxKeyBuffer); }
  if(jtmkeyb.ascbuf==NULL){ jtmkeyb.ascbuf=malloc(MaxKeyBuffer); }
  if(jtmkeyb.ktab==NULL){ jtmkeyb.ktab=malloc(0x1000); }

  for(a1=0; a1<0xfff; a1++)jtmkeyb.ktab[a1]=0;
  for(a1=0; a1<MaxKeyBuffer; a1++)jtmkeyb.buffer[a1]=0;
  for(a1=0; a1<MaxKeyBuffer; a1++)jtmkeyb.ascbuf[a1]=0;
  jtm_setkeyboardhandler();
  jtmkeyb.state=1;
  jtm_addnotice("Tapio's keyboard system driver 26/3/98 installed.\n");
 }
 else
 {
  jtm_addnotice("JTMKEYB_ON: Keyboard system driver already installed.\n");
 }
}

void jtmkeyb_off(void)
{
 if(jtmkeyb.state!=0)
 {
  jtm_addnotice("JTMKEYB_OFF: Keyboard system driver successfully uninstalled.\n");
  jtm_releasekeyboardhandler();
  jtmkeyb.state=0;
 }
 else
 {
  jtm_addnotice("JTMKEYB_OFF: Keyboard system driver already uninstalled.\n");
 }
}

void jtm_setmousehandler(void) // Mouse handler setup, hooks Mouse's IRQ.
{
 //
 /* Store old interrupt vector to memory for later restoring */
 _go32_dpmi_get_protected_mode_interrupt_vector(jtmmouse.mouseirqnum,&si_oldmouseinterrupt);
 /* Get new interrupt offset */
 si_newmouseinterrupt.pm_offset=(long unsigned int)uusi_usermouse;
 /* Allocate wrapper */
 _go32_dpmi_allocate_iret_wrapper(&si_newmouseinterrupt);
 /* Codesegment ... */
 si_newmouseinterrupt.pm_selector=_go32_my_cs();
 /* Interrupt vector set */
 _go32_dpmi_set_protected_mode_interrupt_vector(jtmmouse.mouseirqnum,&si_newmouseinterrupt);
}

void jtm_releasemousehandler(void)
{
 /* Interrupt vector update phase */
 _go32_dpmi_set_protected_mode_interrupt_vector(jtmmouse.mouseirqnum,&si_oldmouseinterrupt);
 /* Release wrapper */
 _go32_dpmi_free_iret_wrapper(&si_newmouseinterrupt);
}

void jtm_setkeyboardhandler(void) // Keyboard handler setup! Hooks irq 9.
{
 //
 /* Store old interrupt vector to memory for later restoring */
 _go32_dpmi_get_protected_mode_interrupt_vector(keybirqnum,&si_oldkeybinterrupt);
 /* Get new interrupt offset */
 si_newkeybinterrupt.pm_offset=(long unsigned int)KeyboardHandler;
 /* Allocate wrapper */
 _go32_dpmi_allocate_iret_wrapper(&si_newkeybinterrupt);
 /* Codesegment ... */
 si_newkeybinterrupt.pm_selector=_go32_my_cs();
 /* Interrupt vector set */
 _go32_dpmi_set_protected_mode_interrupt_vector(keybirqnum,&si_newkeybinterrupt);
}

void jtm_releasekeyboardhandler(void)
{
 /* Interrupt vector update phase */
 _go32_dpmi_set_protected_mode_interrupt_vector(keybirqnum,&si_oldkeybinterrupt);
 /* Release wrapper */
 _go32_dpmi_free_iret_wrapper(&si_newkeybinterrupt);
}

char ___savepic2;

void jtm_setsbirq(void)
{
 char m1,m2,m3,m4;
 //
/*
L11:    MOV DX,021H
L12:    MOV CX,CS:SB_IRQ        ;Sound Blaster IRQ
	AND CL,07H
	MOV AH,01
	SHL AH,CL
	NOT AH
	IN AL,DX
	MOV CS:SAVEPIC2,AL
	AND AL,AH
	OUT DX,AL
	*/

 /* Store old interrupt vector to memory for later restoring */
 _go32_dpmi_get_protected_mode_interrupt_vector(blasterirqnum,&si_oldsbirq);
 /* Get new interrupt offset */
 si_newsbirq.pm_offset=(long unsigned int)uusi_sbirq;
 /* Allocate wrapper */
 _go32_dpmi_allocate_iret_wrapper(&si_newsbirq);
 /* Codesegment ... */
 si_newsbirq.pm_selector=_go32_my_cs();
 /* Interrupt vector set */
 _go32_dpmi_set_protected_mode_interrupt_vector(blasterirqnum,&si_newsbirq);

 m2=1 << 5;
 m2=255-m2;
 m1=inportb(0x21);
 ___savepic2=m1;
 m1=m1 & m2;
 outportb(0x21,m1);
}

void jtm_releasesbirq(void)
{
 /* Interrupt vector update phase */
 _go32_dpmi_set_protected_mode_interrupt_vector(blasterirqnum,&si_oldinterrupt);
 /* Release wrapper */
 _go32_dpmi_free_iret_wrapper(&si_newsbirq);

 /*
L31:    MOV DX,0021H
L32:    MOV AL,CS:SAVEPIC2
	OUT DX,AL
 */
 outportb(0x21,___savepic2);
}

// JTM_SETSYSTEMIRQ function
// -------------------------
//
// This function setups a 'system timer vector' (Interrupt(0x09).
//
//
// See also: JTM_RELEASESYSTEMIRQ
//
void jtm_setsystemirq(void)
{
 /* Store old interrupt vector to memory for later restoring */
 _go32_dpmi_get_protected_mode_interrupt_vector(hookirqnum,&si_oldinterrupt);
 /* Get new interrupt offset */
 si_newinterrupt.pm_offset=(long unsigned int)uusi_i8;
 /* Allocate wrapper */
 _go32_dpmi_allocate_iret_wrapper(&si_newinterrupt);
 /* Codesegment ... */
 si_newinterrupt.pm_selector=_go32_my_cs();
 /* Interrupt vector set */
 _go32_dpmi_set_protected_mode_interrupt_vector(hookirqnum,&si_newinterrupt);
}

// JTM_RELEASESYSTEMIRQ function
// -----------------------------
//
// This function releases current system timer interrupt.
//
//
// See also: JTM_SETSYSTEMIRQ
//
void jtm_releasesystemirq(void)
{
 /* Interrupt vector update phase */
 _go32_dpmi_set_protected_mode_interrupt_vector(hookirqnum,&si_oldinterrupt);
 /* Release wrapper */
 _go32_dpmi_free_iret_wrapper(&si_newinterrupt);
}

/*    cli
	   mov	al,00110110b  ; bit 7,6 = (00) timer counter 0
			      ; bit 5,4 = (11) write LSB then MSB
			      ; bit 3-1 = (011) generate square wave
			      ; bit 0 = (0) binary counter
	   out	43h,al	      ; prep PIT, counter 0, square wave&init count
	   jmp	$+2
	   mov	cx,countdown  ; default is 0x0000 (65536) (18.2 per sec)
			      ; interrupts when counter decrements to 0
	   mov	al,cl	      ; send LSB of timer count
	   out	40h,al
	   jmp	$+2
	   mov	al,ch	      ; send MSB of timer count
	   out	40h,al
	   jmp	$+2
           sti*/
void jtm_setclockrate(unsigned _rate) // If you use interrupt 0x08,
                                      // use this function to setup
                                      // it's frequence.
{
 //
 outportb(0x43,0x02|0x04|0x10|0x20);
 outportb(0x40,(_rate)&0xff);
 outportb(0x40,(_rate/256)&0xff);
}

void jtm_setclockrateb(unsigned _rate) // Dunno if this works?
{
 //
 outportb(0xc3,0x02|0x04|0x10|0x20);
 outportb(0xc0,(_rate)&0xff);
 outportb(0xc0,(_rate/256)&0xff);
}

/*


      JTMOS mouse device driver version 1.00
      (C)1998 by Jari Tapio Tuominen, all rights reserved.
      Type: Standard mouse


*/

void jtmmouse_on(void)
{
 long a1;
 char str[255];
 //
 if(jtmmouse.state==0)
 {
  jtm_addnotice("Initializing mouse ... result = ");
  sprintf(str,"%1.2x\n",detect_mouse()); jtm_addnotice(str);
  jtm_addnotice("Installing Tapio's mouse device driver ...\n");
  jtm_setmousehandler();
  jtmmouse.state=0xff;
  jtm_addnotice("Tapio's mouse device driver 14/4/98 installed.\n");
 }
 else
 {
  jtm_addnotice("JTMMOUSE_ON: Mouse device driver already installed.\n");
 }
}

void jtmmouse_off(void)
{
 if(jtmmouse.state!=0)
 {
  jtm_addnotice("JTMMOUSE_OFF: Mouse device successfully uninstalled.\n");
  jtm_releasemousehandler();
  jtmmouse.state=0;
 }
 else
 {
  jtm_addnotice("JTMMOUSE_OFF: Mouse device driver already uninstalled.\n");
 }
}

void uusi_usermouse(void)
{
  unsigned char inbyte;
  //
  /* Get the port byte */
  jtmmouse.signal=0xff;
  inbyte=inportb(0x3f8);

  /* Make sure we are properly "synched" */
  if( (inbyte & 64)==64 )jtmmouse.bytenum=0;

  /* Store the byte and adjust bytenum */
  jtmmouse.combytes[jtmmouse.bytenum]=inbyte;
  jtmmouse.bytenum++;

  /* Have we received all 3 bytes? */
  if(jtmmouse.bytenum==3)
  {
      /* Yes, so process them */
      jtmmouse.dx= ((jtmmouse.combytes[0] & 3) << 6) + jtmmouse.combytes[1];
      jtmmouse.dy= ((jtmmouse.combytes[0] & 12) << 4) + jtmmouse.combytes[2];
      if(jtmmouse.dx>=128)jtmmouse.dx=jtmmouse.dx-256;
      if(jtmmouse.dy>=128)jtmmouse.dy=jtmmouse.dy-256;
      jtmmouse.x=jtmmouse.x+jtmmouse.dx;
      jtmmouse.y=jtmmouse.y+jtmmouse.dy;
      if(jtmmouse.x<0)jtmmouse.x=0;
      if(jtmmouse.y<0)jtmmouse.y=0;
      jtmmouse.button1=(jtmmouse.combytes[0] & 32);
      jtmmouse.button2=(jtmmouse.combytes[0] & 16);
      jtmmouse.button3=(jtmmouse.combytes[0] & 8);


      /* And start on first byte again */
      jtmmouse.bytenum=0;
  }

/*
 inportb(0x3f8);
 nicebeep();
 jtm_setpalette(0,rand()&0x3f,0,0);*/
 // Kuittaa keskeytys
 outportb(0x20,0x20);
}

void jtmmouse_waitforclick(void)
{
 while(jtmmouse.button1!=0)
 {
  jtmscr_action();
 }
 while(jtmmouse.button1==0)
 {
  jtmscr_action();
 }
}
#endif
/** END OF JTMINTER_H **/
