/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/**
 **JTMOS ansi library v1.00 (C)1997 by Jari Tuominen, all rights reserved.
 **                                                                     **
 ** JTMOS series -- DJGPP port.            Another fine                 **
 **                                        JTMOS library for DJGPP!     **
 ** ∞∞∞∞∞∞ ∞∞∞∞∞∞  ∞∞∞∞∞   ∞∞∞∞∞   ∞∞∞∞∞∞                               **
 **     ±±   ±±   ±± ± ±± ±±   ±± ±±                                    **
 ** ≤≤  ≤≤   ≤≤   ≤≤   ≤≤ ≤≤   ≤≤  ≤≤≤≤≤     Ansi      + Ansi           **
 ** €€  €€   €€   €€   €€ €€   €€      €€    Graphics    Windowing      **
 ** €€  €€   €€   €€   €€ €€   €€ €€   €€    Library     Routines       **
 **  €€€€    €€   €€   €€  €€€€€   €€€€€                                **
 */

#define jtm_djansisaddr 0xb8000

extern void jtm_colorsetbymark(unsigned char m0);
extern char *jinputstr(void);
extern void jtm_message(char *_message);
extern void jtm_headline(char *_headline);
extern void jtm_defwin7(char *headline); // For "System" messaging, doesn't clear screen
extern void jtm_defwin6(); // For "System" messaging, doesn't clear screen
extern void jtm_defwin5();
extern void jtm_defwin4(); // For "System" messaging
extern void jtm_defwin3();
extern void jtm_defwin2();
extern void jtm_defwin();

/*class jtm
{
 public:
 void waitkey(void)
 {
 }
} */


/*
class io
{
 class keyb
 {
  unsigned char read(void)
  {
   return getch();
  }

  void waitkey(void)
  {
   cprintf("[JTMOS/wkey]: Press any key to continue ...\n");
   return io.keyb.read();
  }
 };
};
*/

unsigned char jkey(void)
{
 return getch();
}

void wkey(void)
{
 char m1;
 //
 cprintf("[JTMOS/wkey]: Press any key to continue ...\r");
 m1=jkey();
 cprintf("                                           \r");
 return m1;
}

void ceprint(const char *_str)
{
 gotoxy(40-(strlen(_str)/2),wherey());
 cprintf("%s\n\r",_str);
}

void cewrite(const char *_str)
{
 //
 char str[500];

 //
 jtm_gotoxy(39-(strlen(_str)/2),jtm_wherey());
 sprintf(str,"%s\n\r",_str);
 write(str);
}

#ifdef __BORLANDC__
extern void setpalette(unsigned char _color,unsigned char _red,unsigned char _green,unsigned char _blue);
void flashup();
#endif

extern void jwindow(int _x1,int _y1,int _x2,int _y2,int _winid);

extern void lwindow(int _winid);
extern void writetowin(int _wid,char *_txt,int _line);

void chartowin(int _wid,char *_txt,int _line);
void drawblockbelow(char _mark,char _color);
extern void jtm_messagebox(char *_headline,char *_message);

#ifdef __BORLANDC__
void setpalette(unsigned char _color,unsigned char _red,unsigned char _green,unsigned char _blue)
{
 asm{
  mov dx,0x03c8
  mov al,_color
  out dx,al
  inc dx
  mov al,[_red]
  out dx,al
  mov al,[_green]
  out dx,al
  mov al,[_blue]
  out dx,al
 }
}
#endif

/////////////////////////////////////////////////////////////////////////////
//DRAWANSIWINDOW
//
//Description :
//
//This function is a very nice function to draw Windows,
//it can also setup window arrays, if requested.
//
//      ***************
//      ***************
//      *************** <-- First of all, if fill is set to TRUE (!=0),
//      ***************     it'll fill window area with empty character.
//      ***************
//
//
//   x1,y1
//      ---------------
//      |             |
//      |             | <-- Type&127
//      |             |
//      ---------------
//                  x2,y2
//
//
//      Type :
//
//              1 = Inactive window
//              2 = Active window
//              3 = Inactive window + headline
//              4 = Active window + headline
//
//      If you "OR" 128 to the TYPE then drawansiwindow function will
//      call function "window" to setup window borders.
//
//      Notice: Drawansiwindow moves cursor inside the window, if not
//              requested to not to do it.
//
//
void drawansiwindow(int _x1,int _y1,int _x2,int _y2,int _type,char *_headline,int _fill)
{
 char stringi[100];
 register unsigned _x,_y;
 //
 for(_x=0; _x<(_x2-_x1); _x++)stringi[_x]=' '; stringi[_x]=0;
 for(_x=0; _x<(_x2-_x1); _x++)stringi[_x]=' '; stringi[_x]=0;
 window(1,1,ScreenCols(),ScreenRows());
 if(_fill!=0)
 {
  for(_y=_y1; _y<_y2; _y++)
  {
   gotoxy(_x1,_y);
   cprintf("%s",stringi);
  }
 }
 if((_type&127)==0)
 {
  gotoxy(_x1,_y1); cprintf("⁄");
  gotoxy(_x2,_y1); cprintf("ø");
  gotoxy(_x1,_y2); cprintf("¿");
  gotoxy(_x2,_y2); cprintf("Ÿ");
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1); cprintf("ƒ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y2); cprintf("ƒ");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x1,_y); cprintf("≥");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x2,_y); cprintf("≥");}
  if((_type&128)!=0) window(_x1+1,_y1+1,_x2-1,_y2-1);
 }
 if((_type&127)==1)
 {
  gotoxy(_x1,_y1); cprintf("…");
  gotoxy(_x2,_y1); cprintf("ª");
  gotoxy(_x1,_y2); cprintf("»");
  gotoxy(_x2,_y2); cprintf("º");
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1); cprintf("Õ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y2); cprintf("Õ");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x1,_y); cprintf("∫");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x2,_y); cprintf("∫");}
  if((_type&128)!=0) window(_x1+1,_y1+1,_x2-1,_y2-1);
 }
 if((_type&127)==2)
 {
  gotoxy(_x1,_y1); cprintf("⁄");
  gotoxy(_x2,_y1); cprintf("ø");
  gotoxy(_x1,_y2); cprintf("¿");
  gotoxy(_x2,_y2); cprintf("Ÿ");
  gotoxy(_x1+1,_y1+1); cprintf("%s",_headline);
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1+2); cprintf("ƒ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1); cprintf("ƒ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y2); cprintf("ƒ");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x1,_y); cprintf("≥");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x2,_y); cprintf("≥");}
  gotoxy(_x1,_y1+2); cprintf("√");
  gotoxy(_x2,_y1+2); cprintf("¥");
  if((_type&128)!=0) window(_x1+1,_y1+3,_x2-1,_y2-1);
 }
 if((_type&127)==3)
 {
  gotoxy(_x1,_y1); cprintf("…");
  gotoxy(_x2,_y1); cprintf("ª");
  gotoxy(_x1,_y2); cprintf("»");
  gotoxy(_x2,_y2); cprintf("º");
  gotoxy(_x1+1,_y1+1); cprintf("%s",_headline);
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1+2); cprintf("Õ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1); cprintf("Õ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y2); cprintf("Õ");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x1,_y); cprintf("∫");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x2,_y); cprintf("∫");}
  gotoxy(_x1,_y1+2); cprintf("Ã");
  gotoxy(_x2,_y1+2); cprintf("π");
  if((_type&128)!=0) window(_x1+1,_y1+3,_x2-1,_y2-1);
 }
 if((_type&127)==4)
 {
  gotoxy(_x1,_y1); cprintf("⁄");
  gotoxy(_x2,_y1); cprintf("ø");
  gotoxy(_x1,_y2); cprintf("¿");
  gotoxy(_x2,_y2); cprintf("Ÿ");
  gotoxy(_x1+1,_y1+1); cprintf("                                                                           ");
  gotoxy(_x1+1,_y1+1); ceprint(_headline);
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1+2); cprintf("ƒ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y1); cprintf("ƒ");}
  for(_x=_x1+1; _x<_x2; _x++){gotoxy(_x,_y2); cprintf("ƒ");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x1,_y); cprintf("≥");}
  for(_y=_y1+1; _y<_y2; _y++){gotoxy(_x2,_y); cprintf("≥");}
  gotoxy(_x1,_y1+2); cprintf("√");
  gotoxy(_x2,_y1+2); cprintf("¥");
  if((_type&128)!=0) window(_x1+1,_y1+3,_x2-1,_y2-1);
 }
 if(((_type&127)==2 || (_type&127)==4) && (_type&128)==0 )gotoxy(_x1+1,_y1+3);
 if(((_type&127)==1 && (_type&128)==0))gotoxy(_x1+1,_y1+1);
 //
 if(((_type&127)==2 || (_type&127)==4) && (_type&128)!=0 )gotoxy(1,1);
 if(((_type&127)==1 && (_type&128)!=0))gotoxy(1,1);
 printf("\0  >(C)1997 by Jari Tuominen, all rights reserved.<  ");
}

void sumbackground(char _style,char _mark,char _mark2,char _color,char _color2)
{
 unsigned _ad;
 for(_ad=0; _ad<80*50*2; _ad+=2)
 {
#ifdef __BORLANDC__ 
  asm{
  push es
  mov ax,0xb800
  mov es,ax
  mov di,word ptr _ad
  mov al,byte ptr _mark
  mov [es:di+0],al
  mov al,byte ptr _color
  mov [es:di+1],al
  pop es
  }  
#endif
 }
 if(_style==win95stylish)
 {
  drawblockbelow(_mark2,_color2);
 }
}

void writetowin(int _wid,char *_txt,int _line)
{
 window(1,1,80,50);
 jtmscr.awin.wx1_b=wherex();
 jtmscr.awin.wy1_b=wherey();
 gotoxy(jtmscr.awin.wx1,jtmscr.awin.wy2-1);
 lwindow(_wid); gotoxy(1,_line); cprintf("\r\n%s",_txt);
 window(1,1,80,50);
 gotoxy(jtmscr.awin.wx1_b,jtmscr.awin.wy1_b);
}

void chartowin(int _wid,char *_txt,int _line)
{
 window(1,1,80,50);
 jtmscr.awin.wx1_b=wherex();
 jtmscr.awin.wy1_b=wherey();
 gotoxy(jtmscr.awin.wx1,jtmscr.awin.wy2-1);
 lwindow(_wid); gotoxy(1,_line); cprintf("\r\n%c",_txt[0]);
 window(1,1,80,50);
 gotoxy(jtmscr.awin.wx1_b,jtmscr.awin.wy1_b);
}

void drawblockbelow(char _mark,char _color)
{
 unsigned _ad;
 for(_ad=80*49*2; _ad<80*50*2; _ad+=2)
 {
#ifdef __BORLANDC__
  asm{
  push es
  mov ax,0xb800
  mov es,ax
  mov di,word ptr _ad
  mov al,byte ptr _mark
  mov [es:di+0],al
  mov al,byte ptr _color
  mov [es:di+1],al
  pop es
  }  
#endif
 }
}

void jtm_defwin()
{
 unsigned x,y;
 //
 textmode(C4350);
 // DEFAULT background:
 sumbackground(win95stylish,'∞','∞',0x08,0x08);
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,2,78,48,1," ",0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,3,77,47, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin2()
{
 unsigned x,y;
 //
 textmode(C4350);
 // DEFAULT background:
 sumbackground(win95stylish,'∞','∞',0x08,0x08);
 // DEFAULT window:
 textcolor(default_window2_color); textbackground(default_window2_bcolor);
 drawansiwindow(2,2,78,6,1," ",0);
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,7,78,48,1," ",0);
 // DEFAULT system window:
 textcolor(default_window2_tcolor); textbackground(default_window2_tbcolor);
 jwindow(3,3,77,5, default_window2); for(y=0; y<60; y++)cprintf("\r\n");
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,8,77,47, default_window); for(y=0; y<200; y++)cprintf("\r\n");
}

void jtm_defwin3()
{
 unsigned x,y;
 //
 textmode(C4350);
 // DEFAULT background:
 sumbackground(win95stylish,' ',' ',0x08,0x08);
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,10,78,30,1," ",0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,11,77,29, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin4() // For "System" messaging
{
 unsigned x,y;
 //
 textmode(C80);
 // DEFAULT background:
 sumbackground(win95stylish,' ',' ',0x08,0x08);
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,8,78,18,2,
 "                               System message                         ",0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,11,77,17, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin5()
{
 unsigned x,y;
 //
 textmode(C4350);
 // DEFAULT background:
 sumbackground(win95stylish,'∞','∞',0x08,0x08);
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,2,78,48,1," ",0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,3,77,47, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin6() // For "System" messaging, doesn't clear screen
{
 unsigned x,y;
 //
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,8,78,18,2,
 "                               System message                         ",0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,11,77,17, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin7(char *headline) // For "System" messaging, doesn't clear screen
{
 unsigned x,y;
 //
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,8,78,18,2,headline,0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,11,77,17, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin8(char *headline) // For "System" messaging, doesn't clear screen
{
 unsigned x,y;
 //
 // DEFAULT window:
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(2,12,78,16, 4,headline,0);
 // DEFAULT system window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 jwindow(3,15,77,15, default_window); for(y=0; y<60; y++)cprintf("\r\n");
}

void jtm_defwin9(char *headline)
{
 unsigned x,y;
 //
 textcolor(YELLOW); textbackground(GREEN);
 drawansiwindow(4,6,76,18,4,headline,0);
        jwindow(5,9,75,17,default_window);
 for(y=0; y<20; y++)cprintf("\r\n");
}

void jtm_defwin10(char *headline)
{
 unsigned x,y;
 //
 textmode(C80);
 textcolor(7); textbackground(BLUE);
 clrscr();
 // DEFAULT window:
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 textcolor(LIGHTRED); textbackground(RED);
 drawansiwindow(2,4,78,22,4,headline,0);
 // DEFAULT system window:
 textcolor(7); textbackground(BLACK);
 jwindow(3,7,77,21, default_window); for(y=0; y<60; y++)cprintf("\r\n");
 textcolor(7); textbackground(BLACK);
}

void jtm_defwin11(char *headline)
{
 unsigned x,y;
 //
 textcolor(YELLOW); textbackground(GREEN);
 window(1,1,80,25);
 drawansiwindow(2,2,79,24,4,headline,0);
         window(3,5,78,23); //,default_window
 textcolor(YELLOW); textbackground(BLACK);
 for(y=0; y<25*2; y++)cprintf("\r\n");
}

#ifdef __BORLANDC__  /* Cut out all those Borland C/C++ palette routines
                      * (Replaced at JTMDJGFX.H DJGPP GFX LIBRARY          */
void pal_fadedown(int _color,int _delay)
{
 unsigned x,y;
 //
 for(x=63; x>0; x--){setpalette(_color,x,x,x); delay(_delay); }
}

void pal_fadeup(int _color,int _delay)
{
 unsigned x,y;
 //
 for(x=0; x<63; x++){setpalette(_color,x,x,x); delay(_delay); }
}

void flashup()
{
 pal_fadeup(0,4);
 pal_fadedown(0,4);
}

void savepalette(char *_fname)
{
 setpalette(15,0xff,0xff,0xff);
 fp=jtm_fopen(_fname,"wb");
 for(x=0; x<palette_maxcolours; x++)
 {
#ifdef __BORLANDC__
  asm
  {
   mov ah,0x10
   mov al,0x15
   mov bx,word ptr x
   int 0x10
   mov byte ptr m1,dh
   mov byte ptr m2,ch
   mov byte ptr m3,cl
  }
#endif
  textcolor(15);
  fputc(m1,fp);
  fputc(m2,fp);
  fputc(m3,fp);
 }
 jtm_fclose(fp);
}

void loadpalette(char *_fname)
{
 fp=jtm_fopen(_fname,"rb");
 for(x=0; x<palette_maxcolours; x++)
 {
  m1=fgetc(fp);
  m2=fgetc(fp);
  m3=fgetc(fp);
#ifdef __BORLANDC__
  asm
  {
   mov dx,0x03c8
   mov al,byte ptr x
   out dx,al
   inc dx
   mov al,byte ptr m1
   out dx,al
   mov al,byte ptr m2
   out dx,al
   mov al,byte ptr m3
   out dx,al
  }
#endif
 }
 jtm_fclose(fp);
}

blackpalette()
{
 for(x=0; x<palette_maxcolours; x++)setpalette(x,0,0,0);
}

void floadpalette(char *_fname)
{
 char palette[768];
 char palette1[768];
 //
 savepalette("pal0.swp");
 fp=jtm_fopen("pal0.swp","rb");
 for(x=0; x<palette_maxcolours; x++)
 {
  palette1[x*3+0x00]=fgetc(fp);
  palette1[x*3+0x01]=fgetc(fp);
  palette1[x*3+0x02]=fgetc(fp);
 }
 jtm_fclose(fp);
 //
 fp=jtm_fopen(_fname,"rb");
 for(ad=0; ad<768; ad+=3)
 {
  palette[ad+0x00]=fgetc(fp);
  palette[ad+0x01]=fgetc(fp);
  palette[ad+0x02]=fgetc(fp);
 }
 jtm_fclose(fp);

 for(x2=0; x2<63; x2++)
 {
  for(x=0; x<palette_maxcolours; x++)
  {
   m1=palette1[x*3];
   m2=palette1[x*3+1];
   m3=palette1[x*3+2];
#ifdef __BORLANDC__
   asm
   {
    mov dx,0x03c8
    mov al,byte ptr x
    out dx,al
    inc dx
    mov al,byte ptr m1
    out dx,al
    mov al,byte ptr m2
    out dx,al
    mov al,byte ptr m3
    out dx,al
   }
#endif
  }
  for(x=0; x<768; x++)
  {
   if(palette1[x]<palette[x])palette1[x]++;
   if(palette1[x]>palette[x])palette1[x]--;
  }
  delay(4);
 }
}
#endif

void jwindow(int x1,int y1,int x2,int y2,int winid)
{
 // Function: "Define window"
 // Description: Changes window area using window id code and registers it.
 //

 // Allocate database space for window
 if(jwin.awin.dbase[winid]==NULL)
 {
  jwin.awin.dbase[winid]=malloc(64); // 64 bytes should be enough for one
 }                                   // "ansi" window.

 // Store information to database
 jwin.awin.dbase[winid][0*2+0]=x1&255;
 jwin.awin.dbase[winid][0*2+1]=x1/256;

 jwin.awin.dbase[winid][1*2+0]=y1&255;
 jwin.awin.dbase[winid][1*2+1]=y1/256;

 jwin.awin.dbase[winid][2*2+0]=x2&255;
 jwin.awin.dbase[winid][2*2+1]=x2/256;

 jwin.awin.dbase[winid][3*2+0]=y2&255;
 jwin.awin.dbase[winid][3*2+1]=y2/256;

 jtm_window(x1-1,y1-1,x2-1,y2-1);
}

void lwindow(int winid)
{
 // Function: "Load window"
 // Description: Loads information of a specified window to ansi screen registry
 //
 jtmscr.awin.wx1=jwin.awin.dbase[winid][0*2+0];
 jtmscr.awin.wy1=jwin.awin.dbase[winid][1*2+0];
 jtmscr.awin.wx2=jwin.awin.dbase[winid][2*2+0];
 jtmscr.awin.wy2=jwin.awin.dbase[winid][3*2+0];

 jtm_window(
        jtmscr.awin.wx1-1,
        jtmscr.awin.wy1-1,
        jtmscr.awin.wx2-1,
        jtmscr.awin.wy2-1
       );
}

void jtm_thanks(void)
{
 //
 char str[255],str2[255];

 sprintf(str, "Thank you for using JTMOS v%1.2f\n",jtm_stationversion);
 sprintf(str2,"      (C)1997-98 by Jari Tapio Tuominen, all rights reserved.\n");
 write(str); write(str2);
}


void jtm_messagebox(char *_headline,char *_message)
{
 // (Kesken erÑinen !)
 jtm_defwin6();
 jtm_headline(_headline);
 jtm_message(_message);
 wkey();
/* textmode(C80);
 textcolor(7); textbackground(BLACK);*/
 jwindow(3,1,1,80,25);
 clrscr();
}

void jtm_message(char *_message)
{
 textcolor      (jtm_messagefcolor);
 textbackground (jtm_messagebcolor);
 if(wtype==wtype_dos5)
 {
  cprintf("%s",_message);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
 if(wtype==wtype_mbox)
 {
  cprintf("%s",_message);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
 if(wtype==wtype_windows)
 {
  cprintf("%s",_message);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
}

void jtm_headline(char *_headline)
{
 textcolor      (jtm_headlinefcolor);
 textbackground (jtm_headlinebcolor);
 if(wtype==wtype_dos5)
 {
  cprintf("%s",_headline);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
 if(wtype==wtype_mbox)
 {
  cprintf("%s",_headline);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
 if(wtype==wtype_windows)
 {
  cprintf("%s",_headline);
  textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
  cprintf("\n\r");
 }
}

void jputscrn(int _winid)
{
 // 80x50 screen restore from c:\screen.swp
 FILE *_fp;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;
 unsigned _ad;
 int _x,_y;
 unsigned char _data;
 char screen[0xffff],str[255],str2[255];
 // Create file name
 strcpy(str,cfg_swap);
 strcat(str,"screen");
 sprintf(str2,"%x",_winid);
 strcat(str,str2);
 strcat(str,".swp");
 //
 _fp=jtm_fopen(str,"rb");
 fscanf(_fp,"%d",&_x);
 fscanf(_fp,"%d",&_y);
 lwindow(_winid);
 gotoxy(_x,_y);
 fgetc(_fp);
 #ifdef __BORLANDC__
  for(_ad=0; _ad<80*50*2; _ad++) //80x50*2, "*2" means two parts,1=ASC 2=color
  {
   _data=fgetc(_fp);
   asm{
    push es
    mov ax,0xb800
    mov es,ax
    mov di,_ad
    mov al,byte ptr _data
    mov byte ptr [es:di],al
    pop es
   }
  }
 #else
  for(ad=0; ad<80*50*2; ad++)
  {
   screen[ad]=fgetc(_fp);
  }
  dosmemput(screen,80*50*2,jtm_djansisaddr);
 #endif
 jtm_fclose(_fp);
}

void jgetscrn(int _winid)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 // 80x50 screen backuping to c:\screen.swp
 FILE *_fp;
 unsigned _ad;
 unsigned char _data;
 char screen[0xffff],str[255],str2[255];
 // Create file name
 strcpy(str,cfg_swap);
 strcat(str,"screen");
 sprintf(str2,"%x",_winid);
 strcat(str,str2);
 strcat(str,".swp");
 //
 _fp=jtm_fopen(str,"wb");
 fprintf(_fp,"%d ",wherex());
 fprintf(_fp,"%d ",wherey());
 #ifdef __BORLANDC__
  for(_ad=0; _ad<80*50*2; _ad++) //80x50*2, "*2" means two parts,1=ASC 2=color
  {
   asm{
    push es
    mov ax,0xb800
    mov es,ax
    mov di,_ad
    mov al,byte ptr [es:di]
    mov byte ptr _data,al
    pop es
   }
   fputc(_data,_fp);
  }
 #else
  dosmemget(jtm_djansisaddr,80*50*2,&screen);
  for(ad=0; ad<80*50*2; ad++)
  {
   fputc(screen[ad],_fp);
  }
 #endif
 jtm_fclose(_fp);
}

void jedit(char *_filename)
{
 char ast[255];
 //
 strcpy(ast,"edit ");
 strcat(ast,_filename);
 system(ast);
}

void *jtm_inputstr(char *str,char *_headline)
{
 //
 char ast[255];

 //
 jtm_defwin8(_headline);
 strcpy(str,jinputstr());
}

void *jtm_msgbox(char *str,char *_headline)
{
 //
 jtm_defwin8(_headline);
 write(str);
 return jgetch();
}

/*
 * JTMOS smart input routine (C)by JTT in 1997-99.
 */
char *jinputstr(void)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2;
 char jlegalmarks[]=" QWERTYUIOPASDFGHJKLZXCVBNMôéèqwertyuiopasdfghjklzxcvbnm0123456789.,-+/\\*ÑÜî|<>!\"#œ%&/()=@ú${[]}ı´?:;_?~˘^Ô`";
 char stringi[0x1fff];
 int key;
 unsigned _temp,curx;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 char str[0x1fff];
 //
 /** Empty space for key storages **/
 jtm_setcursortype(_NORMALCURSOR);
 _temp=jtmscr.immediatly_update_on_function_write;
 jtmscr.immediatly_update_on_function_write=0xff;
 strcpy(str,"                                                                                     ");
 curx=0;
 str[curx]=0;
 do
 {
nextkey:
  key=jgetchim(); m1=key;
  // Skip extra keys
  if(key==-2)
  {
   while(jgetchim()!='#');
   goto nextkey;
  }
  jtmscr_action();
  if(jtmscr.about_button!=0)
  {
   strcpy(str,"ver");
   curx=strlen(str);
   goto enterit;
  }
  if(m1==27 && escapingbyesc==1)
  {
   goto closingtime;
  }
  if(closeimmediatly==1)
  {
closingtime:
   strcpy(str,"leave"); 
   goto returntime;
  }
//  if(m1==77)m1==' ';
  if(m1==8)  //Del (or CRSR left .. m1==75 || [??])
  {
   if(curx==0)
   {
    /*
     * Check string counter, if zero then reject mark erasing..
     */
    goto nextkey;
   }
   write(" \b\b");
   curx--;
   str[curx]=0;
   goto nextkey;
  }
  if(m1==13) //Enter
  {
enterit:
   if(enterafterinput==1)write("\n");
   /*
    * Enter pressed, exiting, adding zero to string end, to calculate
    * correct string length.
    */
   break;
  }
  for(x2=0; x2<strlen(jlegalmarks); x2++)
  {
   if(jlegalmarks[x2]==m1)
   {
    /*
     * Found legal mark, hey user has brains, shit, how can computer
     * sometimes imagine something that people won't...
     */
    goto stringbanking;
   }
  }
  goto nextkey;
stringbanking:
  str[curx]=m1;
  // Print mark
  sprintf(stringi,"%c",m1);
  write(stringi);
  curx++;
  str[curx]=0;
 }while(m1!=13);
// Legal string:
 str[curx]=0;
returntime:
 jtm_setcursortype(_NOCURSOR);
 jtmscr.immediatly_update_on_function_write=_temp;
 return str;
}

// Input routine #2
//
// Notice:
//  -Input string will be returned !
//  -Argument can be used to specify what input buffer should contain
//   as default when input routine starts to ask string from user.
//
//
char *jtm_input(char *DataToBuffer)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2;
 char jlegalmarks[]=" QWERTYUIOPASDFGHJKLZXCVBNMôéèqwertyuiopasdfghjklzxcvbnm0123456789.,-+/\\*ÑÜî|<>!\"#œ%&/()=@ú${[]}ı´?:;_?";
 char stringi[0x1fff];
 unsigned _temp,curx;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 char str[0x1fff];
 int key;
 //
 /** Empty space for key storages **/
 _temp=jtmscr.immediatly_update_on_function_write;
 jtmscr.immediatly_update_on_function_write=0xff;
 strcpy(str,DataToBuffer);
 write(str);
 curx=strlen(str);
 jtm_setcursortype(_NORMALCURSOR);
 do{
nextkey:
  key=jgetchim(); m1=key;
  // Skip extra keys
  if(key==-2)
  {
   while(jgetchim()!='#');
   goto nextkey;
  }
  jtmscr_action();
  if(jtmscr.about_button!=0)
  {
   strcpy(str,"ver");
   curx=strlen(str);
   goto enterit;
  }
  if(m1==27 && escapingbyesc==1)
  {
   goto closingtime;
  }
  if(closeimmediatly==1)
  {
closingtime:
   strcpy(str,"leave"); 
   goto returntime;
  }
  if(m1==77)m1==' ';
  if(m1==8)  //Del (or CRSR left .. m1==75 || [??])
  {
   if(curx==0)
   {
    /*
     * Check string counter, if zero reject mark erasing..
     */
    goto nextkey;
   }
   write(" \b\b");
   curx--;
   str[curx]=0;
   goto nextkey;
  }
  if(m1==13) //Enter
  {
enterit:
   if(enterafterinput==1)write("\n");
   /*
    * Enter pressed, exiting, adding zero to string end, to calculate
    * ordinary string.
    */
   break;
  }
  for(x2=0; x2<strlen(jlegalmarks); x2++)
  {
   if(jlegalmarks[x2]==m1)
   {
    /*
     * Found legal mark, hey user has brains, shit, how can computer
     * sometimes imagine something that people won't...
     */
    goto stringbanking;
   }
  }
  goto nextkey;
stringbanking:
  str[curx]=m1;
  // Print mark
  sprintf(stringi,"%c",m1);
  write(stringi);
  curx++;
 }while(m1!=13);
// Legal string:
 str[curx]=0;
returntime:
 jtmscr.immediatly_update_on_function_write=_temp;
 jtm_setcursortype(_NOCURSOR);
 return str;
}

/*void DumpAscFile(char *filename,void SelectedOutputConsole())
{
 FILE *f,*f2;
 f=jtm_fopen(filename,"rb");
 do
 {
  SelectedOutputConsole(fgetc(f));
 }while(!feof(f));
 jtm_fclose(f);
}

void jtm_putc(char tavu)
{
 switch(jtm_vmode)
 {
  case 0x03:
  cprintf("%c",tavu);
  break;
 }
}*/





/*
  Data expressing library 97, version 1.00 (C)1997 by Jari Tuominen,
                                        all right reserved.
 */

 /*
#define showtype_txt    0       // Normal text file. #
#define showtype_c      1       // C source. *
#define showtype_jtx    2       // JTMOS advanced text file format. *
#define showtype_bin    3       // Binary file. *
#define showtype_pcx    4       // PCX image.   *
#define showtype_gif    5       // GIF image.   #
#define showtype_jpg    6       // JPG image.   #
#define showtype_mpg    7       // MPG video.   #
#define showtype_avi    8       // AVI video.   *
*/

//
// * = Future expansion.
// # = Coming soon.
//
//
//

void show(char *_fname)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 textcolor(BLACK);
 fp=jtm_fopen(_fname,"rb"); y=0;
 textcolor(YELLOW);
 do
 {
again:
  if(feof(fp))break;
  m1=fgetc(fp);
  if(m1==0x0d)
  {
   textcolor(YELLOW);
   fgetc(fp);
   cprintf("\n\r");
   y++;
   if(y>11)
   {
    textcolor(BLACK);
    cwaitkey();
    textcolor(YELLOW);
    y=0;
   }
   goto again;
  }
  if(isprint(m1)==0)
  {
   textcolor(RED);
   m1='.';
  }
  jtm_colorsetbymark(m1);
  m1=tolower(m1);
  cprintf("%c",m1);
 }while(!feof(fp));
 jtm_fclose(fp);
 textcolor(7); cprintf("\n\r");
 cwaitkey();
}

#define FILETYPE_TXT    0       // Normal text file. #
#define FILETYPE_C      1       // C source. *
#define FILETYPE_CPP    2       // C/C++ source. *
#define FILETYPE_PAS    3       // Pascal source. *
#define FILETYPE_BAS    4       // Basic source. *
#define FILETYPE_JTX    5       // JTMOS advanced text file format. *
#define FILETYPE_BIN    6       // Binary file. *
#define FILETYPE_PCX    7       // PCX image.   *
#define FILETYPE_GIF    8       // GIF image.   #
#define FILETYPE_JPG    9       // JPG image.   #
#define FILETYPE_MPG    10      // MPG video.   #
#define FILETYPE_AVI    11      // AVI video.   *
#define FILETYPE_BAT    12      // BAT video.   *

struct
{
// char *filetypes[]={"txt","c","cpp","pas","bas","jtx","bin","pcx",
//                    "gif","jpg","mpg","avi","bat"};
}FILETYPES;

/////////////////////////////////////////////////////////////////////////////
//
// jtm_show
//
// Displays file to user in graphical/text/numerical form.
//
//
//
//
void jtm_show(int type,char *_fname)
{
 char str[255],str2[255];
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //

 switch(type)
 {
  ///////////////////////////////////////////////////////////////////////////
  // Text file
  //
  case FILETYPE_TXT:
  textcolor(BLACK);
  fp=jtm_fopen(_fname,"rb"); y=0;
  textcolor(YELLOW);
  do
  {
   if(feof(fp))break;
   m1=fgetc(fp);
   if(m1==0x0d)
   {
    textcolor(YELLOW);
    fgetc(fp);
    cprintf("\n\r");
    y++;
    if(y>11)
    {
     textcolor(BLACK);
     cwaitkey();
     textcolor(YELLOW);
     y=0;
     m1=0;
    }
   }
   else
   if(isprint(m1)==0)
   {
    textcolor(RED);
    m1='.';
   }
   if(m1!=0)
   {
    jtm_colorsetbymark(m1);
    m1=tolower(m1);
    cprintf("%c",m1);
   }
  }while(!feof(fp));
  jtm_fclose(fp);
  textcolor(7); cprintf("\n\r");
  cwaitkey();
  break;

  default:
  sprintf(str,"JTM_SHOW: Unknown/Unsupported file format.\n");
  write(str);
  jtm_messagebuffer(str);
 }
}

/////////////////////////////////////////////////////////////////////////////
//
// ConvertToLowerCase function
// ---------------------------
//
// This function does exact what the name tells you.
//

void ConvertToLowerCase(char *fname1,char *fname2)
{
 FILE *fp1,*fp2;
 fp1=jtm_fopen(fname1,"rb"); if(fp1==NULL)exit(1);
 fp2=jtm_fopen(fname2,"wb"); if(fp2==NULL)exit(2);
 do
 {
  fputc(toupper(fgetc(fp1)),fp2);
 }while(!feof(fp1));
 jtm_fclose(fp1);
 jtm_fclose(fp2);
 textbackground(BLACK);
}

void cls()
{
 window(1,1,80,25);
 clrscr();
}

void jtmansi_screensystemon(void)
{
/*

  This function is usually called when JTMOS is just startting.
  It initializes all screens.

*/
 long a1,a2,a3,a4;
 //
 jtmansi.screen.nextscreen=0; // Default setting, which means that no screens
                              // yet installed.
 for(a1=0; a1<MaxScreens; a1++)
 {
  jtmansi.screen.colors[a1]=16;
  jtmansi.screen.bits[a1]=8;
  jtmansi.screen.mode[a1]=1;
  jtmansi.screen.allocated[a1]=0;
  jtmansi.screen.cursorx[a1]=0;
  jtmansi.screen.cursory[a1]=0;
  jtmansi.screen.cursormode[a1]=0;
  jtmansi.screen.vmode[a1]=0x03;
  jtmansi.screen.width[a1]=80;
  jtmansi.screen.height[a1]=25;
  jtmansi.screen.dat[a1]=malloc(jtmansi.screen.width[a1]*jtmansi.screen.height[a1]*2);
 }
}

void jtmansi_cls(unsigned ScreenID)
{
 //dosmemput(jtmansi.screen.data[ScreenID, 80*50*2, 0xb8000);
 jtmansi.screen.mode[ScreenID]=0;
}

void jtmansi_clearline(int lnum)
{
 gotoxy(1,lnum);
 cprintf("                                                                              ");
 gotoxy(1,lnum);
}

void jtm_colorsetbymark(unsigned char m0)
{
#ifndef JTMOS_Linux
  if(m0==' ')
  {
   textcolor(YELLOW);
  }
  if(m0=='/')
  {
   textcolor(7);
  }
#endif
}

void showmnu(char *_fname,int _ymenu)
{
 FILE *_fp;
 unsigned _x,_y;
 unsigned char _m1;
 unsigned char _str[255];
 _fp=jtm_fopen(_fname,"rb");
 _y=0;
 do
 {
nextone:
  if(feof(_fp))break;
  _m1=fgetc(_fp);
  if(_m1=='^')
  {
   fscanf(_fp,"%s",&_str);
   textcolor(am_message); textbackground(am_messagebak);
   ceprint(_str);
   goto nextone;
  }
  if(_m1=='>' || _m1=='?')
  {
   fscanf(_fp,"%s",&_str);
   if(_m1=='?')
   {
    textcolor(am_edit); textbackground(am_editbak);
    if(_y==_ymenu)
    {
     textcolor(am_actedit); textbackground(am_acteditbak);
    }
    goto printit;
   }
   else
   {
    textcolor(am_normal); textbackground(am_normalbak);
    if(_y==_ymenu)
    {
     textcolor(am_active); textbackground(am_activebak);
    }
printit:
    ceprint(_str);
    _y++;
    goto nextone;
   }
  }
 }while(!feof(_fp));
 jtm_fclose(_fp);
}

void jtm_setcursortype(int type)
{
 if(jtm_vmode==3)
 {
  _setcursortype(type);
 }
 else
 {
  if(type==_NOCURSOR)
  {
   jtmscr.cursor.state=0;
  }
  else
  {
   jtmscr.cursor.state=1;
  }
 }
}

// get: 80x50 ega textmode
jtmansi_getscreen(char *Screen)
{
 dosmemget(0xb8000, 80*50*2, Screen);
}

// put: 80x50 ega textmode
jtmansi_putscreen(char *Screen)
{
 dosmemput(Screen, 80*50*2, 0xb8000);
}
/**
 ** DIALOG FUNCTION !
 **
 **/
int jtm_dialog(char *sumtxt[],long activeitem1)
{
 //
 long select,ad,ad2,ad3,ad4,x,y,items,items1,activeitem,col,xx,yy;
 char str[255],str2[255],scr[(80*50*2)];
 int key;

 //
 jtmansi_getscreen(scr);

 //
 activeitem=activeitem1;

 //
 for(ad=0; ad<65535; ad++)
 {
  if(!sumtxt[ad])return 1; // huh huh :>~
  if(!strcmp(sumtxt[ad],"*"))break;
 }
 items=ad;

 //
 items1=items;

 // Empty dialog?!
 if(!items)return;

 // Draw window outfit
 textcolor(7); textbackground(BLUE);
 window(1,1,80,50);
 drawansiwindow( 10,2,70,2+items1+1, 0, "", 1);

 // Define new text output area
 jwindow(11,3,69,4+items, 1);

 //
 jtm_gotoxy(11,3);

 //
 jtm_setcursortype(_NOCURSOR);

 //
 systemtimer.counter1=0;

 //
 do
 {
  // Draw dialog text
  jtm_gotoxy(0,0);
  yy=3; xx=11;
  for(y=0,ad=0; y<items1; y++)
  {
   if(!strcmp(sumtxt[y],"*")){ad=1; break;}
 //  if(y)write("\n");
   if(y==activeitem)
   {
//    textcolor(7); textbackground(GREEN);
    col=0x71;
   }
   else
   {
//    textcolor(7); textbackground(BLUE);
    col=0x17;
   }
   sprintf(str,"%s",sumtxt[y]);
   jtm_stretchstringlength(str,59);
   jtm_writestringonscreen(str,xx,yy,col);
   yy++;
//   write(str);
  }

  //
  if(systemtimer.counter1>1)
  {
   if(jtmkeyb.ktab[0x2a])
   {
    //
    systemtimer.counter1=0;

    // CRSR UP
    if(jtmkeyb.ktab[0x48] && activeitem)
    {
     activeitem--;
    }
    // CRSR DOWN
    if(jtmkeyb.ktab[0x50] && activeitem<(items-1))
    {
     activeitem++;
    }
   }
   // ENTER
   if(jtmkeyb.ktab[0x1c])
   {
    //
    systemtimer.counter1=0;
    break;
   }
   // ESC
   if(jtmkeyb.ktab[1])
   {
    //
    systemtimer.counter1=0;
    activeitem=-1;
    goto retu;
   }
  }
 }while(!0);
retu:
 //
 jtmansi_putscreen(scr);
 jtmkeyb.place=0;
 textcolor(7); textbackground(BLACK);
 return activeitem;
}

