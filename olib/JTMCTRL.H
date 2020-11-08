/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS control library (C)1997 by Jari Tuominen, all rights reserved.
 ** °°°°°° °°°°°°  °°°°°   °°°°°   °°°°°°                               **
 **     ±±   ±±   ±± ± ±± ±±   ±± ±±                                    **
 ** ²²  ²²   ²²   ²²   ²² ²²   ²²  ²²²²²     Control   + Control        **
 ** ÛÛ  ÛÛ   ÛÛ   ÛÛ   ÛÛ ÛÛ   ÛÛ      ÛÛ    Functions   Functions      **
 ** ÛÛ  ÛÛ   ÛÛ   ÛÛ   ÛÛ ÛÛ   ÛÛ ÛÛ   ÛÛ    Library     Library        **
 **  ÛÛÛÛ    ÛÛ   ÛÛ   ÛÛ  ÛÛÛÛÛ   ÛÛÛÛÛ                                **
 *                                                                      **
 * (DOS features)
 *      + PLUS most needed DOS routines like keyb(),
 *      + Useful system control/communicating functions
 *      + Poweful drive hardling functions.
 *      + Base multitasking possibilities (Requires JTMTASK.H or comptable)
 *
 * (Global features)
 *      + ANSI C libraries memory management system
 *        replacement functions (by JTMOS functions).
 *     -> This is still kind of "TODO" thing, still.
 *
 * - Features f.e. miscellious control routines for mouse, keyboard, etc..
 *
 * Notice:
 *
 *      This source code documenting is a combination of Finnish'n'English
 *      languages. I will try to fix this problem by
 *      writting more text in English.
 *      A seperate document for all JTMOS libraries might come out someday,
 *      well atleast if I'll make an official release of it.
 *      But source documenting will be included in all JTMOS libraries
 *      programmed in future too, I think it is better than nothing :)
 */

#define JTMCTRL_INCLUDED
#define __JTMCTRL_H_INCLUDED TRUE

/** Needed libraries **/
#include        <jtmwreg.h>  // Window registry handling functions library.
#include	<jtmfile.h>
#include	<jtmvie.h>
#include	<jtmdisk.h>	// Sector based low level disk operating functions

/*#ifndef JTMOS_Linux
 #include <bios.h>
#else
 #define cprintf           //
 #define textcolor         //
 #define textbackground    //
#endif*/

extern void	jdelay(long	_time);
extern unsigned char	jgetscode();
extern void	clearkeybuf();

extern void	jtm_readtime(JTIME *table);
extern void	jtm_none();	// It's empty, and it should be like that.
void jtm_multitasker();
extern void	poorbeep();
extern void	nicebeep();
extern void	jtm_killprocess();

void jtm_hookirqs();
void checkos();

/*** Mouse routines -- Disabled under Linux ***/
#ifndef __BORLANDC__
 __dpmi_regs regs;
#endif

void jtm_sethold(unsigned holdi)
{
#ifdef __BORLANDC__
 asm{
  mov	ax,0x13
  int	0x33
 }
#else
 regs.x.ax=0x13;
 regs.x.dx=holdi;
 __dpmi_int(0x33,&regs);
#endif
}

void jtm_setmickey(unsigned horiz,unsigned verti)
{
#ifdef __BORLANDC__
 asm{
  mov	ax,0x0f
  mov	cx,horiz
  mov	dx,verti
  int	0x33
 }
#else
 regs.x.ax=0x0f;
 regs.x.cx=horiz;
 regs.x.dx=verti;
 __dpmi_int(0x33,&regs);
#endif
}

int get_mouse_version()
{
 int _return;
 _return=0;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x24
  int	0x33
  mov	word ptr	_return,bx
 }
#else
 regs.x.ax=0x24;
 __dpmi_int(0x33,&regs);
 _return=regs.x.bx;
#endif
 return _return;
}

int get_mouse_type()
{
 int _return;
 _return=0;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x24
  int	0x33
  mov	byte ptr	_return,ch
 }
#else
 regs.x.ax=0x24;
 __dpmi_int(0x33,&regs);
 _return=((regs.x.cx)>>8)&0xff;
#endif
 return _return;
}

int get_mouse_irq()
{
 int _return;
 _return=0;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x24
  int	0x33
  mov	byte ptr	_return,cl
 }
#else
 regs.x.ax=0x24;
 __dpmi_int(0x33,&regs);
 _return=(regs.x.cx)&0xff;
#endif
 return _return;
}

void mouse_cursor_on()
{
 if(mousecursor!=1)
 {
  mousecursor=1;
#ifdef __BORLANDC__
  asm{
	mov ax,0x01
	int 0x33
  }
#else
 regs.x.ax=0x01;
 __dpmi_int(0x33,&regs);
#endif
 }
}

void mouse_cursor_off()
{
 if(mousecursor!=0)
 {
  mousecursor=0;
#ifdef __BORLANDC__
  asm{
	mov ax,0x02
	int 0x33
  }
#else
 regs.x.ax=0x02;
 __dpmi_int(0x33,&regs);
#endif
 }
}

void jtmmouse_setxy(int	_x,int _y)
{
 int _return;
 //
#ifdef __BORLANDC__
 // Argh :9, Borland C/C++ DOS16 3<8-D ?
#else
 if(jtmmouse.state==0)
 {
  regs.x.ax=0x04;
  regs.x.cx=_x;
  regs.x.dx=_y;
  __dpmi_int(0x33,&regs);
  _return=regs.x.cx;
 }
#endif
}

int mousexstat()
{
 int _return;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x03
  int	0x33
  mov	word ptr	_return,cx
 }
#else
 if(jtmmouse.state==0)
 {
  regs.x.ax=0x03;
  __dpmi_int(0x33,&regs);
  _return=regs.x.cx;
 }
 else
 {
  _return=jtmmouse.x;
 }
#endif
 return _return;
}

int mouseystat()
{
 int _return;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x03
  int	0x33
  mov	word ptr	_return,dx
 }
#else
 if(jtmmouse.state==0)
 {
  regs.x.ax=0x03;
  __dpmi_int(0x33,&regs);
  _return=regs.x.dx;
 }
 else
 {
  _return=jtmmouse.y;
 }
#endif
 return _return;
}

int mousebstat() // Get mouse button press information (Bits: 0,1,2)
{
 int _return;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x05
  int	0x33
  mov	word ptr	_return,ax
 }
#else
 regs.x.ax=0x05;
 __dpmi_int(0x33,&regs);
 _return=regs.x.ax;
#endif
 return _return;
}

int mousebrstat()	// Get mouse button release information (1,2,4,..)
{
 int _return;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x06
  int	0x33
  mov	word ptr	_return,ax
 }
#else
 regs.x.ax=0x06;
 __dpmi_int(0x33,&regs);
 _return=regs.x.ax;
#endif
 return _return;
}

unsigned	detect_mouse()
{
 unsigned _result;
 //
 goto	bugfix;
#ifdef __BORLANDC__
 asm{
  mov	ax,0x00
  int	0x33
  mov	word ptr	_result,ax
 }
#else
 regs.x.ax=0x00;
 __dpmi_int(0x33,&regs);
 _result=regs.x.ax;
#endif
bugfix:
 return _result;
}
/*** End of mouse routines, disabled under Linux ! ***/











/** Routines that needs CONIO.H **/
#ifndef JTMOS_Linux
void waitfornokey()
{
 unsigned char	_m1,meter;
 //
waitnk:
#ifdef __BORLANDC__
 asm{
  push es
  push 0x0040
  pop	es
  mov	di,0x17
  mov	al,byte ptr	[es:di]
  mov	byte ptr	meter,al
  pop	es
 }
#endif
 if(meter!=0)
 {
  getch();
  goto waitnk;
 }
}

int amenusys(char	*_fname,int	_starting) // Advanced menu system
{
 int _ymenu=0,_lymenu=1,_ymaxmenu,_mux,_muy;
 unsigned char	_m2;
 FILE	*_fp;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 //
 _ymenu=_starting;
 clearkeybuf();
 _mux=wherex();
 _muy=wherey();

 // Search for menu file
 _fp=fopen(_fname,"rb");
 // Case: NOT FOUND
 if(_fp==NULL)
 {
  return	-1; // Return error report
 }
 // Case: Found !
 seektomark('#',_fp);
 fscanf(_fp,"%u",&_ymaxmenu);
 fclose(_fp);
 gotoxy(_mux,_muy);
 showmnu(_fname,_ymenu);
// jdelay(50);
// waitfornokey();
// disable();
 do
 {
  textcolor(am_normal);	textbackground(am_normalbak);
  if(_ymenu!=_lymenu)
  {
	gotoxy(_mux,_muy);
	showmnu(_fname,_ymenu);
  }
  _lymenu=_ymenu;
/*  _m2=jgetscode();
  if(_m2==28)break;
  if(_m2==1){_ymenu=-1; break;}
  if(_m2==72 && _ymenu!=0)_ymenu--;              //Up
  if(_m2==80 && _ymenu<_ymaxmenu-1 )_ymenu++;    //Down
  jdelay(jtm_keydelaytime);
  waitfornokey();
  clearkeybuf();*/
	m2=jgetchim();
	if(m2==43)
	{
	 if(_ymenu<_ymaxmenu-1)_ymenu++;
	}
	if(m2==45)
	{
	 if(_ymenu!=0)_ymenu--;
	}
	if(m2==27)
	{
	 _ymenu=-1;
	 break;
	}
	if(m2==13)
	{
	 break;
	}
 }while(m2!=27);
// enable();
// clearkeybuf();
// waitfornokey();
 return _ymenu;
}

void clearkeybuf()
{
#ifdef __BORLANDC__
 asm{
  push es
  push 0x0040
  pop	 es
  mov	di,0x0014
  mov	al,0
  mov	byte ptr	[es:di],al
  pop	es
 }
#endif
}

void waitforscode()
{
 disable();
waiting:
#ifdef __BORLANDC__
 asm{
  in	al,0x64
  and	al,0x01
  jz	waiting
 }
#endif
 enable();
}

unsigned	char jgetscode()
{
 unsigned char	_scancode;
#ifdef __BORLANDC__
 asm{
  in al,0x60
  mov	byte ptr	_scancode,al
 }
#endif
 return _scancode;
}

void jdelay(long _time)
{
#ifdef __BORLANDC__
 asm{
  cli
 }
#else
 asm("cli");
#endif
 //
 delay(_time);
 //
#ifdef __BORLANDC__
 asm{
  sti
 }
#else
 asm("sti");
#endif
}

int jtm_getch(void)
{
 int key;
 //
 do
 {
  key=jgetchim();
  if(key!=-1)break;
 }while(0==0);
 return key;
}

int jgetche()
{
 unsigned char	bufstat;
 if(bioskey(1)==0)
 {
  return	-1;
 }
 return getch();
}

void jtm_waitfornokey()
{
 do
 {
 }while(bioskey(1)!=0);
}
#endif /*** End of non-Linux supportted area */

char cwaitkey(void)
{
 char	key;
 cprintf("\rPress any key to continue ...\r");
 key=jgetch();
 cprintf("\r                             \r");
 return key;
}

#define WAITKEY_FUNCTION
char waitkey(void)
{
 unsigned char	__key;
#ifndef JTMOS_Linux
 /*** DOS specified operating ***/
 if(jtm_vmode!=3)
 {
  write("\rPress any key to continue ...\r");
 }
 else
 {
  if(jtmansi.conoutput==0)
  {
	write("\rPress any key to continue ...\r");
  }
  if(jtmansi.conoutput==1)
  {
	cprintf("\rPress any key to continue ...\r");
  }
 }
 if(jtmkeyb.state==0)
 {
  __key=getch();
 }
 else
 {
  __key=jgetch();
 }
 if(jtm_vmode!=3)
 {
  write("                             \r");
 }
 else
 {
  if(jtmansi.conoutput==0)
  {
	write("                             \r");
  }
  if(jtmansi.conoutput==1)
  {
	cprintf("                             \r");
  }
 }
#else
 /*** Linux specified operating ***/
 printf("\rPress any key to continue ...\r");
 __key=getch();
 printf("                             \r");
#endif
 return __key;
}

char jtm_waitkey()
{
 char	__key;
#ifndef JTMOS_Linux
 if(jtmkeyb.state==0)
 {
  __key=jgetch();
 }
 else
 {
  __key=jgetch();
 }
#else
  __key=getch();
#endif
 return __key;
}









#ifndef JTMOS_Linux /*** Doesn't work under Linux ***/
// ** jgetchr **
char jgetch()
{
 int key;
 char	__m1,__m2;
 unsigned		 _ap;
 unsigned char	 _m1;
 unsigned char	 __wayout;
 //
 if(jtmkeyb.state!=0)
 {
  do
  {
   jtmscr_action();
   key=jgetchim();
  }while( key==-1 || key==0 );
  __m1=key;
  goto returnimmediatly;
 }
 if(waits==0)
 {
  __m1=-1;
  goto returnimmediatly;
 }
 __wayout=0;
 if(m_input==0)
 {
  if(jtmkeyb.state==0)
  {
	__m1=getch();
  }
  goto returnimmediatly;
 }

 do{
#ifdef jtmos_multitask
/*  if(j_manage_systemtasks()==-2 && jtm_shutdownmode==0)
  {
   closeimmediatly=1;
   __m1=-1;
   goto returnimmediatly;
  }*/
  for(_ap=0; _ap<multitaskpower;	_ap++)
  {
	jtm_multitasker();
  }
/*  if( (jtm_spkey()&(0x01|0x02|0x04|0x08))==(0x01|0x02|0x04|0x08)
       && jtm_wayout!=0 && jtm_shutdownmode==0)
  {
   jtm_wayout=0;
   __wayout=1;
   break;
  }*/
#endif
 }while(bioskey(1)==0);
 if(__wayout!=1)__m1=getch();
#ifdef jtmos_multitask 
/* if( (jtm_quitbykey==1 && jtm_quitkey==__m1 && jtm_shutdownmode==0)
      || (__wayout==1 && jtm_shutdownmode==0))
 {
  jtm_killprocess();
 }*/
#endif
 //
returnimmediatly:
 return __m1;
}

char jgetchim() // !JGETCHIM
{
 int _key;
 unsigned _ap;
/*#ifdef jtmos_multitask
 if(m_input==0)return getch();
 #endif*/
 char	_m1,_m2;

 if(jtmio.forceallspaces!=0){return " ";}
 if(jtmkeyb.state==0)
 {
  if(bioskey(1)==0)return -1;
  _key=getch();
 }
 else
 {
  _key=jtmkeyb_jgetchim();
 }
 if(esctoexit!=0 && _key==27)
 {		
//  jtmerror(14,_exit);
//  nicebeep(); // Heyh, somethings wrong!
#ifdef __BORLANDC__
  asm
  {
	mov ax,0x03
	int 0x10
  }
#else
 textmode(C80); clrscr();
#endif
  jtmapp_shutdown();
  exit(0); // Confirm exiting !
 }
 return _key;
}
#endif /**************************************/













void flongscan_str(FILE	*_fp)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255];
 //
 x=0;
 do
 {
  m1=fgetc(_fp); if(feof(_fp))break;
  str[x]=m1; x++;
  if(m1==13	||	m1==0)break;
  x++;
  if(x>255)
  {
	fclose(_fp);
	jtm_killprocess();
  }
 }while(!feof(_fp));
 fgetc(_fp);
 str[x]=0;
}

void flongscan_2str(FILE *_fp)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char	m1;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255];
 //
 x=0;
 do
 {
  m1=fgetc(_fp);
  if(m1!=0x20)
  {
	fseek(_fp,-1,SEEK_CUR);
	break;
  }
  if(m1==0 || fgetc(_fp) || m1==0x0d)
  {
	strcpy(str,NULL);
	goto skip;
  }
 }while(m1==0x20);
 do
 {
  m1=fgetc(_fp);	if(feof(_fp))break;
  m1=toascii(m1);
  str[x]=m1;
  if(m1==13	||	m1==0)break;
  x++;
  if(x>255)
  {
	fclose(_fp);
	jtm_killprocess(); 
  }
 }while(!feof(_fp));
 fgetc(_fp);
 str[x]=0;
skip:
 while(0!=0);
}

void jtm_killprocess()
{
 FILE	*_fp;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255];
 //
#ifndef JTMOS_Linux
 jtm_shutdownmode=1;	/* Tell system that system is already going down !
                        (So that it understand that it doesn't have to
                        start freezing display and sound cart again ..)*/
 jtm_defwin4();
 cprintf("JTMOS: Current program terminated\n\r");
 if(fexist("c:/jtmos.msb"))
 {
  _fp=fopen("c:/jtmos.msb","rb");
  do
  {
	if(feof(_fp))break;
	fscanf(_fp,"%s",&str);
	if(!stricmp(str,"message:"))
	uni_getstr(_fp);
	cprintf("%s",str);
  }while(0==0);
  fclose(_fp);
 }
 waitkey();
 textmode(C80);
 textcolor(7);	textbackground(BLACK);
 jwindow(3,1,1,80,25);
 clrscr();
#else
 jtm_shutdownmode=1;	/* Tell system that system is already going down !
                        (So that it understand that it doesn't have to
                        start freezing display and sound cart again ..)*/
 printf("JTMOS: Current program terminated\n\r");
 if(fexist("c:/jtmos.msb"))
 {
  _fp=fopen("c:/jtmos.msb","rb");
  do
  {
	if(feof(_fp))break;
	fscanf(_fp,"%s",&str);
	if(!stricmp(str,"message:"))
	uni_getstr(_fp);
	printf("%s",str);
  }while(0==0);
  fclose(_fp);
 }
 waitkey();
#endif
 exit(0);
}

void jdirectmessage(const char *__format)
{
#ifndef JTMOS_Linux
 if(jtm_vmode==0x03)	// ADOS, 80x25, etc.
 {
  cprintf("%s\n\r",__format);
 }
 if(jtm_vmode==0x12)	// JTMOS, 640x480, etc.
 {
  textmode(C80);
  cprintf("%s\n\r",__format);
  exit(-1);
 }
#endif
}

char *jstrval(unsigned _value) // Creates string based value of unsigned variable
{
 char	*_stringi;
 sprintf(_stringi,"%s",_value);
 return _stringi;
}

/*
 *      40:94   Drive 0 current cyliner
 *      40:95   Drive 1 -||-
 *      40:96   Keyboard mode state, type flags.
 *      40:97   Keyboard LED flags
 *
 *
 *
 *
 *
 *
 *
 */

// CHECKOS
// -------
//
//
// Description:
//
// CHECKOS is a kind of system update function,
// it autodetects things for Application/System/User usage.
// It also restarts JTMOS's computer messaging system,
// plus a lot of things maybe neccesery to do before
// running the system itself.
//
// Notice:
//
//  If after system start up process system founds
//  application which is meant to be executed after
//  system start up then system will execute the application
//  right behind.
//
//  The application is possible to be located in
//  same memory region as System is,
//  this is so called System+Application fusion,
//  which is usually used only when the JTMOS is
//  not running as basement on the computer and
//  when it is recommended that the OS control will be
//  returned to previous OS.
void checkos(void)
{
 //
 char	str[255];
 int cursorstate=0;

 //
#ifndef __BORLANDC__
 jtm_vmmavailable=_go32_dpmi_remaining_virtual_memory();
 jtm_phyavailable=_go32_dpmi_remaining_physical_memory();
#endif
 if(fexist(jtmos_cfg))
 {
  jtmos_cfgloaded=1;
 }
 else
 {
  jtmos_cfgloaded=0;
 }

// OLD STUFF :
/*#ifdef __MODERNGRAPHICS__
 for(ad=0; ad<jtm_sizeofbitmap; ad++)
 {
  jtm_bitmap[ad]=BLUE;
 }
#else
 #ifdef modern
  jgetscrn(jtm_modernwindow);
  jtm_defwin10(jtm_applicationname);
 #endif
#endif*/

 // Check JTM-system main configuration file "C:\JTMOS.CFG"
 // ...
 if(hookirqs==1)
 {
  jtm_hookirqs();
 }
 // Check PC model, simple bios memory location read operation
 jtm_pcmodel1=0;
 jtm_pcmodel2=0;
#ifdef __BORLANDC__
 asm
 {
  push es
  mov	ax,0xf000
  mov	es,ax
  mov	di,0xfffe
  mov	al,byte ptr	[es:di]
  mov	byte ptr	m1,al
  mov	al,byte ptr	[es:di+1]
  mov	byte ptr	m2,al
  pop	es
 }
 jtm_pcmodel1=m1;
 jtm_pcmodel2=m2;
#else // DJGPP only !!!!!
 jtmpcmodel.id=0;
 dosmemget(0xffffe, 2, &jtmpcmodel.id);
#endif

 // Remove existing message buffer/log.
 //
 if( fexist("c:\\jtmos.msb") )jdelete("c:\\jtmos.msb");

 // Update internal system variables
 //
 //
 if(jtmscr.screencapturepath==NULL)jtmscr.screencapturepath=malloc(256);
 strcpy(jtmscr.screencapturepath,jgetenv("shortcuts","screencapturepath="));
 // If variable not set then set up default.
 if(jtmscr.screencapturepath[0]==0)
 {
  strcpy(jtmscr.screencapturepath,"./");
 }

 //
 strcpy(str,jgetenv("preferences","soundblaster="));
 sscanf(str,"%x",&sndcarts.blaster);
 /*
 //ELSE

 if(!getenv("BLASTER")==NULL)
 {
  sndcarts.blaster=         1;
  sndcarts.blaster_port=    0x220;
  sndcarts.blaster_irq=     5;
  sndcarts.blaster_irq2=    0xff;
  sndcarts.blaster_dma=     1;
  sndcarts.blaster_dma2=    0xff;
 }
 else
 {
  sndcart.blaster=0;
 }
 */

 // - Find out mouse COM port specification from JTMOS configuration file
 strcpy(str,jgetenv("preferences","mousecomport="));
 str[0]=' ';
 sscanf(str,"%x",&jtmmouse.mousecomport);
 jtmmouse.mouseirqnum=0x0b+(2-jtmmouse.mousecomport);	// Mouse IRQ handler, cheese C:>*
 // - Find out graphics mode from JTMOS configuration file

 // ** MOUSE ARROW STATE (0=OFF, !=ON) **
 jtmupdater.screenrefreshrate=9;
 strcpy(str,jgetenv("preferences","mousearrowstate="));
 sscanf(str,"%u",&cursorstate);
 if(cursorstate==0)
 {
  jtmgl_arrowoff();
 }

 // ** MOUSE ARROW LOCK (0=OFF, !=ON) **
 jtmupdater.screenrefreshrate=9;
 strcpy(str,jgetenv("preferences","mousearrowlock="));
 sscanf(str,"%d",&jtmmouse.cursorlock);

 // ** SCREEN REFRESH RATE **
 jtmupdater.screenrefreshrate=9;
 strcpy(str,jgetenv("preferences","screenrefreshrate="));
 sscanf(str,"%u",&jtmupdater.screenrefreshrate);

 // ** GRAPHICS MODE **
 jtmscr.defaultgfxmode=0x101;
 strcpy(str,jgetenv("preferences","defaultgfxmode="));
 if( str[0]==0 )
 {
  // Set Invalid Videomode warning flag
  jtmworkstation.WARNINGS=jtmworkstation.WARNINGS|1;
 }
 str[0]=' ';
 sscanf(str,"%x",&jtmscr.defaultgfxmode);

 // ** BITS PER PIXEL **
 jtmscr.bpp=8;
 strcpy(str,jgetenv("preferences","glsystem_bpp="));
 sscanf(str,"%u",&jtmscr.bpp);

// cprintf("%x\n\r",jtmscr.defaultgfxmode);
// waitkey();
 //
 // - Find out default system icon image library file
 strcpy(jtmscr.fn_systemimage_raw,jgetenv("shortcuts","sysimage="));
 //
 // - Find out default system font image file
 strcpy(jtmscr.fn_systemfont_raw, jgetenv("shortcuts","sysfont="));
 //
 if(getenv("TEMP")!=NULL)
 {
  strcpy(_dostemp,getenv("TEMP"));
 }
 else
 {
  strcpy(_dostemp,"Undefined");
 }
 if(getenv("JTEMP")!=NULL)
 {
  strcpy(tempdrive,getenv("JTEMP"));
  jxwvmm=checktemp(); 
 }
 else
 {
  jxwvmm=0;
 }
 // Update ..
 jtm_underwindows=jtm_detectwindows();
 if(!getenv("ULTRASND")==NULL)
 {
  sndcarts.gus=		 1;
  sndcarts.gus_port=	 0x240;
  sndcarts.gus_irq=	 7;
  sndcarts.gus_irq2=	 0xff;
  sndcarts.gus_dma=	 5;
  sndcarts.gus_dma2=	 7;
  strcpy(str,getenv("ULTRASND"));
 }
 else
 {
  sndcarts.gus=0;
 }
 if(getenv("JTMOS")==NULL)
 {
  jtmos=0; // Off
 }
 else
 {
  jtmos=1; // On
 }
 if(getenv("JTMOS")==NULL && warnings==1)
 {
  printf("Warning!: This program requids atleast JTMOS 1.0\n");
 }
 // Warning, if this machine is running under Windows
 if(jtm_underwindows!=0	&&	warnings==1)
 {
  printf("Warning!: This PC is Running under Microsoft Windows,\n");
  printf("JTMOS(ViE) is designed to work under clean DOS.\n");
  printf("Please exit Windows and run JTMOS from clean DOS, thank you.\n");
 }
}

unsigned	checktemp()	// Old foolish virtual memory server code (Do not use)
{
 unsigned tfree;
 //
 if(warnings!=0)printf("Querying internal JXW-VMM memory manager...");
 tfree=ftempfree();
 if(warnings!=0)
 {
  printf("\r%u kb's VMM free(Atleast), ",tfree);
  if(tfree<512)
  {
	printf("Low memory.          \n\r");
  }
  if(tfree>511)
  {
	printf("Good.                \n\r");
  }
 }
nothing:
 return tfree;
}

int ftempfree()
{
 unsigned co;
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	ast[255],ast2[255];
 //
 for(ad=0, ad3=0;	ad<0xfff; ad++)
 {
  m1=jgetchim();
  if(m1==27)break;
  strcpy(ast,tempdrive);
  strcat(ast,_tempname);
  sprintf(ast2,"%1.2x",ad);
  strcat(ast,ast2),
  fp=fopen(ast,"wb");
  for(co=0;	co<16384/8;	co++)
  {
	fprintf(fp,"        ");
 //  fputc(0x00,fp);
  }
  fclose(fp);
  if(fsizeof(ast)<16384)break;
  ad3+=16;
#ifdef __BORLANDC__
  jpingpong(1);
#endif
 }
#ifdef __BORLANDC__
 jpingpong(0);
#endif
 for(ad=0; ad<0xfff;	ad++)
 {
  strcpy(ast,tempdrive);
  strcat(ast,_tempname);
  sprintf(ast2,"%1.2x",ad);
  strcat(ast,ast2);
  if(fexist(ast))
  {
	jdelete(ast);
  }
  else
  {
	break;
  }
#ifdef __BORLANDC__
  jpingpong(1);
#endif
 }
#ifdef __BORLANDC__
 jpingpong(0);
#endif
 return ad3;
}

void jtm_hookirqs()
{
 // setvect( 0x60, jtm_control );
}

void jtm_loadjtmoscfg(char *buffer)
{
 jtm_loadbin(jtmfn_syscfg,buffer);
}

void nicebeep() // =)
{
 sound(1000); delay(25); nosound();
}

void poorbeep() // :P
{
 sound(100); delay(125); nosound();
}

void jsystem(char	*syscmd)
{
// mouse_cursor_off();
// delay(moff_delay);
 char	str[255];
 //
 jtmmouse_delaymode();
// strcpy(_str,"Executing dos command \"");
// strcat(_str,syscmd);
// strcat(_str,"\"");
 //
// textcolor(YELLOW);
// writetowin(system_window,_str,system_window_line);
// textcolor(7);
 //
// jgetscrn(0);
 //
// jputscrn(64);
// textmode(PRVMODE);
 system(syscmd);
// jgetch();
// jgetscrn(64);
 //
 // Return to ADOS screen
// textmode(ADOSMODE);
// lwindow(0);
// jputscrn(0);
// mouse_cursor_on();
 jtmmouse_normalmode();
}

void jtm_fastsinus(unsigned char	*_sinustable)
{
 register uad;
 float fa;
 for(uad=0,fa=0; uad<0xffff; uad++,fa+=.025)
 {
  _sinustable[uad]=128+(127*sin(fa));
 }
}

void jtm_colorprint(char *texti)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;
 //
 for(ad=0; ad<strlen(texti); ad++)
 {
  jtm_colorsetbymark(texti[ad]);
  cprintf("%c",texti[ad]);
 }
}

int ifstrcontainsmark(unsigned char	*str,unsigned char mark)
{
 unsigned char	m1;
 unsigned long	ad;
 //
 for(ad=0; ad<strlen(str);	ad++)
 {
  if(	str[ad]==mark )return 0; // 0=Mark found, no error.
 }
 return 1; // 1=Mark not found.
}

// Reads string.
// Example maybe:
//
// freadstringuntilmark(f,str,'!');
// FILE *f contains following data "ueiruhriehuierg!hjjhoi".
// str will contain following marks after this function call "ueiruhriehuierg".
// Where this can be used?
// You can use this function f.e. at compilers/parsers what ever connering
// string operations.
//
void freadstruntilmark(FILE *f,unsigned char	*str,unsigned char umark)
{
 register
 unsigned
 long	ad;
 register
 unsigned char	m1;
 //
 ad=0;
 do
 {
  if(	feof(f) )break;
  m1=fgetc(f);
  if(	m1==umark )break;
  str[ad]=m1;
  ad++;
 }while(0==0);
 str[ad]=0;
}

void jtm_osmess(char	*texti)
{
 char	str[255],str2[255];
 //
 strcpy(str,texti);
 strcpy(str2,"JTMOS: ");
 strcat(str2,str),
 printf("%s",str2);
}

void jtm_none(void) // It's empty, and it should be like that.
{
}

void jtm_queryerrorlist()
{
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 //
 goto	noqueries;
 write("Application returned some error messages,\n");
 write("do you want to display the report(y/n)?\n");
 m1=jtmkeyb_getch();
 if(m1=='y')
 {
  show("c:\\jtmos.msb");
 }
noqueries:
 while(0!=0);
}










/*** START OF VIRTUAL MEMORY CODE ***/
#ifndef NO_JTMVM
// Virtual memory library
#define MAXcaches	524288
typedef struct
{
 unsigned long	cache_active[MAXcaches]; // If active status is zero, its then inactive.
 unsigned long	cache_sector[MAXcaches]; // Sectors currently in caches
 unsigned char	*cache_pointer[MAXcaches];	// Pointer to caches
 unsigned char	tempdir[256]; // Temp directory path
 unsigned cs; // Cache size, notice: Cache size must come from 2^x.
				  // Legal sizes are for example following: 1,2,4,8,16,32,64,128,
				  // 256,512,1024,2048,4096,8192,16384,32768 and so on.
				  // So if the cache value size won't come from formula 2^x,
				  // it'll be illegal, there is no check for that, so
				  // be careful and set correct cache size.
 long	totalsectors; // Total number of sectors
 long	totalcaches;  // Total number of caches
 long	lastsector,lastaddress,last_a1,last_a2,last_a3,last_a4;
}JTMVM;

JTMVM	jtmvm;

char *jtmvm_makefname(unsigned long	secci)
{
 unsigned char	charri[1000];
 sprintf(charri,"%sjtm386.%1.3x",jtmvm.tempdir,secci);
 return charri;
}

void jtmvm_clearvmm(void) // Also allocates memory
{
 long	aa1,aa2;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255];
 //
 for(aa1=0;	aa1<jtmvm.totalsectors;	aa1++)
 {
  strcpy(str,jtmvm_makefname(aa1));
  fp=fopen(str,"wb");
  for(aa2=0; aa2<jtmvm.cs;	aa2++)
  {
	fputc(0x00,fp);
  }
  fclose(fp);
 }
}

/** > This function initializes virtual memory < **/
void jtmvm_turnon(void)
{
 long	aa1,aa2;
 char	str[255];

 // -1 means "No last sector/address"
 jtmvm.lastsector=-1;
 jtmvm.lastaddress=-1;

 // <*** DO NOT CHANGE VALUE BELOW! ***>
 // Set new cache size, default/good cache sizes: 1024*16,..
 // If cache size is smaller than swap disk cluster size
 // it'll be extermely slow.
 jtmvm.cs=1024*8;

 // memory for caches,
 // which actually means that
 // it is possible to store
 // 16MB memory one time in memory,
 // but since all memory isn't
 // used, there won't be any
 // problems accessing
 // over 16MB, like 32MB or 64MB.
 jtmvm.totalcaches=(1024*256)/jtmvm.cs;

 // Define amount of VMM to allocate.
 jtmvm.totalsectors=(1024*1024)/jtmvm.cs;
 strcpy(str,"c:/"); // <== SWAP FILES PATH
 strcpy(jtmvm.tempdir,str);
 jtm_addnotice("JTMVM - Installing memory driver.\n");
 sprintf(str,"Total sectors: %u\n",jtmvm.totalsectors); jtm_addnotice(str);
 sprintf(str,"Cache size: %u KB(s), amount of caches: %u couples.\n",jtmvm.cs/1024,jtmvm.totalcaches); jtm_addnotice(str);
 //
 jtm_addnotice("Allocating caches ... ");
 for(aa1=0;	aa1<jtmvm.totalcaches; aa1++)
 {
  if(jtmvm.cache_pointer[aa1]==NULL)jtmvm.cache_pointer[aa1]=malloc(jtmvm.cs);
  jtmvm.cache_active[aa1]=0; // Inactive all caches ~ Setup start up defaults
  jtmvm.cache_sector[aa1]=0;
 }
 jtm_addnotice("Done.\n");
 //
 jtm_addnotice("Allocating VMM ... ");
 jtmvm_clearvmm(); // As the name tells you, it clears memory,
						 // but it also allocates the memory.
 jtm_addnotice("Done.\n");
}

void jtmvm_turnoff(void)
{
 long	aa1,aa2;
 char	str[255];
 //
 jtm_addnotice("JTMVM - Removing memory swap files.\n");
// jtmvm.cache=malloc(jtmvm.cs);
 for(aa1=0;	aa1<jtmvm.totalsectors;	aa1++)
 {
  strcpy(str,jtmvm_makefname(aa1));
  jdelete(str);
 }
 jtm_addnotice("Done.\n");
}

void jtmvm_savesector(long	sector,long	cacheid)
{
// sprintf(ast,"Savesector((S)%u to: (CacheID) %u)",sector,cacheid);
// jtm_messagebuffer(ast);
 jtm_savebin(jtmvm_makefname(sector),jtmvm.cache_pointer[cacheid],jtmvm.cs);
 jtmscr_blinkbackground();
}

void jtmvm_loadsector(long	sector,long	cacheid)
{
// sprintf(ast,"Loadsector((S)%u to: (CacheID) %u)",sector,cacheid);
// jtm_messagebuffer(ast);
 jtm_loadbin(jtmvm_makefname(sector),jtmvm.cache_pointer[cacheid]);
// jtmscr_blinkbackground();
}

void jtmvm_poke(long	location,unsigned	char dataa)
{
 unsigned long	a1,a2,a3,a4,a5,a6,a7,a8;
 // LOG
// sprintf(ast,"jtmvm_poke %u,%u",location,dataa);
// jtm_messagebuffer(ast);
 //
 // Ensin selvitet„„n mihin sektoriin muisti osoite kuuluu.
 a1=(location/jtmvm.cs); // <= Sektori
 // Sitten lasketaan AND operaatiolla ala muisti osoite (?? :) )
 a2=(location&(jtmvm.cs-1)); // <= Muisti osoite LOW

 // Tsekkaa ett„ jos osoite olis ollut „skeinen
// jtmvm.last_a4=a4;
// jtmvm.last_a2=a2;
 if(a1==jtmvm.lastsector)
 {
  a2=jtmvm.last_a2;
  a4=jtmvm.last_a4;
  goto muistikirjoitus;
 }

 // þ  Check if sector is already loaded in cache.
 //    K„yd„„n kaikki cachet l„pi, ja tarkistetaan onko pyydetty muistin kohta
 //    jo v„li muistista vai tarvitaanko se hakea muistista.
 for(a3=0; a3<jtmvm.totalcaches;	a3++)
 {
//  sprintf(ast,"%u,",jtmvm.cache_sector);
//  jtm_messagebuffer(ast);
  if(	jtmvm.cache_active[a3]!=0 )
  {
//   printf("%u. CMP %u,%u\n",a3,jtmvm.cache_sector[a3],a1);
	if( jtmvm.cache_sector[a3]==a1 )
	{
//    printf("*SAME*\n");
//    sprintf(ast,"Cache %u includes sector %u information",a3,a1);
//    jtm_messagebuffer(ast);
	 a4=a3;
	 goto	muistikirjoitus;
	 // Breikin j„lkeen ohjelman suoritus hypp„„ muistikirjoitus kohtaan
	}
  }
 }
 //þ T„ss„ kohdassa ollaan jo huomattu ett„ sektoria ei ollut cache muistissa
 //  siisp„ on ryhdytt„v„ toimiin ja ladattava se cache muistiin,
 //  jos vapaata cache paikkaa ei ole niin joku cache paikka on dumpattava
 //  diskille v„li aikaisesti.
 for(a3=0; a3<jtmvm.totalcaches;	a3++)
 {
  if(jtmvm.cache_active[a3]==0x00) // Tsekkaa ett„ jos cache oliskin k„ytett„viss„.
  {
//   sprintf(ast,"Free cache found (%u), locking cache to sector %u",a3,a1);
//   jtm_messagebuffer(ast);
	// Joo, ja jos vapaata l”yty niin varaa se heti!
	a4=a3; // A4 tulee sis„lt„m„„n k„ytett„viss„ olevan cachen avaimen
	jtmvm.cache_sector[a4]=a1;
	goto loyty;
  }
 }
 // Otetaan cache[0] k„ytt””n uudestaan
 a4=rand()&(jtmvm.totalcaches-1);
// sprintf(ast,"Reallocating (%u)",a4);
// jtm_messagebuffer(ast);
 // Tallenetaan sektori cachesta diskille
 jtmvm_savesector(jtmvm.cache_sector[a4],a4);
 // Ladataan uusi sektori cacheen tilalle
 jtmvm_loadsector(a1,a4);
 //
 jtmvm.cache_sector[a4]=a1;
 //
loyty:
 // þ Muistin kirjoitus
muistikirjoitus:
 if(jtmvm.cache_active[a4]<64000)
 {
  jtmvm.cache_active[a4]++;
 }
 jtmvm.cache_pointer[a4][a2]=dataa;
 jtmvm.last_a4=a4;
 jtmvm.last_a2=a2;
}

unsigned	char jtmvm_peek(long	location)
{
 unsigned long	a1,a2,a3,a4,a5,a6,a7,a8;
 // LOG
// sprintf(ast,"jtmvm_peek %u",location);
// jtm_messagebuffer(ast);
 //
 // Ensin selvitet„„n mihin sektoriin muisti osoite kuuluu.
 a1=(location/jtmvm.cs); // <= Sektori
 // Sitten lasketaan AND operaatiolla ala muisti osoite (?? :) )
 a2=(location&(jtmvm.cs-1)); // <= Muisti osoite LOW

 if(a1==jtmvm.lastsector)
 {
  a2=jtmvm.last_a2;
  a4=jtmvm.last_a4;
  goto muistikirjoitus;
 }

 // þ  Check if sector is already loaded in cache.
 //    K„yd„„n kaikki cachet l„pi, ja tarkistetaan onko pyydetty muistin kohta
 //    jo v„li muistista vai tarvitaanko se hakea muistista.
 for(a3=0; a3<jtmvm.totalcaches;	a3++)
 {
//  sprintf(ast,"%u,",jtmvm.cache_sector);
//  jtm_messagebuffer(ast);
  if(	jtmvm.cache_active[a3]!=0 )
  {
//   printf("%u. CMP %u,%u\n",a3,jtmvm.cache_sector[a3],a1);
	if( jtmvm.cache_sector[a3]==a1 )
	{
//    printf("*SAME*\n");
//    sprintf(ast,"Cache %u includes sector %u information",a3,a1);
//    jtm_messagebuffer(ast);
	 a4=a3;
	 goto	muistikirjoitus;
	 // Breikin j„lkeen ohjelman suoritus hypp„„ muistikirjoitus kohtaan
	}
  }
 }
 //þ T„ss„ kohdassa ollaan jo huomattu ett„ sektoria ei ollut cache muistissa
 //  siisp„ on ryhdytt„v„ toimiin ja ladattava se cache muistiin,
 //  jos vapaata cache paikkaa ei ole niin joku cache paikka on dumpattava
 //  diskille v„li aikaisesti.
 for(a3=0; a3<jtmvm.totalcaches;	a3++)
 {
  if(jtmvm.cache_active[a3]==0x00) // Tsekkaa ett„ jos cache oliskin k„ytett„viss„.
  {
//   sprintf(ast,"Free cache found (%u), locking cache to sector %u",a3,a1);
//   jtm_messagebuffer(ast);
	// Joo, ja jos vapaata l”yty niin varaa se heti!
	a4=a3; // A4 tulee sis„lt„m„„n k„ytett„viss„ olevan cachen avaimen
	jtmvm.cache_sector[a4]=a1;
	goto loyty;
  }
 }
 // Otetaan cache[0] k„ytt””n uudestaan
 a4=0;
// sprintf(ast,"Reallocating (%u)",a4);
// jtm_messagebuffer(ast);
 // Tallenetaan sektori cachesta diskille
 jtmvm_savesector(jtmvm.cache_sector[a4],a4);
 // Ladataan uusi sektori cacheen tilalle
 jtmvm_loadsector(a1,a4);
 //
 jtmvm.cache_sector[a4]=a1;
 //
loyty:
 // þ Muistin kirjoitus
muistikirjoitus:
 if(jtmvm.cache_active[a4]<64000)
 {
  jtmvm.cache_active[a4]++;
 }
 jtmvm.last_a4=a4;
 jtmvm.last_a2=a2;
 return jtmvm.cache_pointer[a4][a2];
}
#endif

/*** THE END OF VIRTUAL MEMORY CODE ***/









void jtm_system(unsigned char	*_string)
{
 /*

      Since JTMOS keyboard functions won't work when executing MS-DOS shell,
      mainly because MS-DOS keyboard handler is replaced with JTMOS own
      keyboard handler routine.

 */
 textmode(C80); clrscr();
 jtmkeyb_off();
 system(_string);
}

void jtm_addnotice(unsigned char	*joku) // ASCII + 13 + 0
{
 if(strlen(jtmscr.systemnotice)<256)strcat(jtmscr.systemnotice,joku);
}

void jtm_mergenoticestomessagebuffer(void)
{
 jtm_messagebuffer(jtmscr.systemnotice);
}

void jtm_newbin(unsigned char	*fnoutput,unsigned char	*fnsource,char	scrout)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;
 int done;
 struct ffblk fdir;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255],str2[255],ast[255],ast2[255];
 //
 time_t now;
 time(&now);
 //
 strcpy(ast, fnsource);
 strcpy(str, fnoutput);	strcat(str,	".bin");
 strcpy(str2,fnoutput);	strcat(str2,".ctr");
 fp2=fopen(str, "wb");
 fp3=fopen(str2,"wb");
 if(fp2==NULL || fp3==NULL)
 {
  fclose(fp2);	fclose(fp3);
  goto loppu;
 }
 done=findfirst(ast,&fdir,0);
 fprintf(fp3,";JTMOS binary archive v1.2\n");
 fprintf(fp3,"Source= \"%s\"\n",ast);
 fprintf(fp3,"Archiver= JTMOS %f\n",jtm_stationversion);
 fprintf(fp3,"Date= %s\n",asctime(localtime(&now))	);
 fprintf(fp3,"Subtype= none\n");
 fprintf(fp3,"^list\n");
 do
 {
  fp=fopen(fdir.ff_name,"rb");
  if(fp==NULL)
  {
	goto closeall;
  }
  // ** Logi tiedoston kirjaus prosessi **
  // Kirjoita tiedoston nimi luetteloon
  if(scrout==JTM_TRUE)
  {
	sprintf(str,"%s\n",fdir.ff_name);
	write(str);
  }
  fprintf(fp3,"%s ",fdir.ff_name);
  // Kirjoita tiedoston sijainti binaari paketissa (Esim. 0x0123abcd)
  fprintf(fp3,"%x ",ftell(fp2));
  // Kirjoita tiedoston pituus tavuina (Esim. 00100f00)
  fseek(fp,0,SEEK_END);	fprintf(fp3,"%x\n",ftell(fp)); fseek(fp,0,SEEK_SET);

  // Kopio tiedoston sis„lt„m„ tieto binaari paketti tiedostoon
  do
  {
	if(feof(fp))break;
	fputc(fgetc(fp),fp2);
  }while(ad==ad);
  fclose(fp);

  // Seuraavan tiedoston etsint„
  done=findnext(&fdir);
 }while(!done);
 fprintf(fp3,"^liste\n");
 fprintf(fp3,";End of archive file.\n");
 //
closeall:
 fclose(fp2);
 fclose(fp3);
 //
loppu:
 while(0!=0);
}

void jtm_extbin(unsigned char	*fnarc,char	textoutput)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255],str2[255],ast[255],ast2[255],ast3[255];
 //
 sprintf(ast, "%s.ctr",fnarc);
 sprintf(ast2,"%s.bin",fnarc);
 fp= fopen(ast, "rb");
 fp2=fopen(ast2,"rb");
 f1wordseek(fp,"^list");
 do
 {
  sprintf(ast3,"%s\n",str);
  write(ast);
  fscanf(fp,"%s",&str);
  if(!stricmp(str,"^liste"))break;
  fscanf(fp,"%x",&ad2);
  fscanf(fp,"%x",&ad3);
  fseek(fp2,ad2,SEEK_SET);
  fp3=fopen(str,"wb");
  while(ad==ad)
  {
	if(ad3==0)break;
	ad3--;
	fputc(fgetc(fp2),fp3);
  }
  fclose(fp3);
 }while(ad==ad);
 fclose(fp);
 fclose(fp2);
}

void jtm_lstbin(unsigned char	*fnarc,char	textoutput)
{
 unsigned x,y,xx,yy,x1,y1,x2,y2,ad,ad2,ad3,ad4,ad5,ad6,ad7;
 unsigned char	m1,m2,m3,m4,m5,m6,m7,m8;
 FILE	*fp,*fp2,*fp3,*fp4;
 char	str[255],ast[255],ast2[255];
 //
 sprintf(ast, "%s.ctr",fnarc);
 sprintf(ast2,"%s.bin",fnarc);
 fp=fopen(ast,"rb");
 f1wordseek(fp,"^list");
 do
 {
  fscanf(fp,"%s",&str);
  if(!stricmp(str,"^liste"))
  {
	break;
  }
  fscanf(fp,"%x",&ad);
  fscanf(fp,"%x",&ad2);
  sprintf(str,"%s %u bytes\n",str,ad2);
  if(textoutput==JTM_TRUE)write(str);
 }while(ad==ad);
 fclose(fp);
loppu:
 while(0!=0);
}

/*
 * This function detects Windows by calling 'interrupt 0x2f, function ax=0x1600'.
 */
unsigned	char jtm_detectwindows() // al=0x00/No windows detected
											 //    0x80/Windows detected
{
 unsigned char	_result=0;
#ifdef __BORLANDC__
 asm
 {
  mov	ax,0x1600
  int	0x2f
  mov	byte ptr	_result,al
 }
#else
 __dpmi_regs regs;
 regs.x.ax=0x1600;
 __dpmi_int(0x2f,&regs);
 _result=regs.x.ax&0xff;
#endif
 return _result;
}
/*
          INT 1A,2 - Read Time From Real Time Clock  (XT 286,AT,PS/2)


        AH = 02


        on return:
        CF = 0 if successful
           = 1 if error, RTC not operating
        CH = hours in BCD
        CL = minutes in BCD
        DH = seconds in BCD
        DL = 1 if daylight savings time option


        - on AT with BIOS before 6/10/85, DL is not returned
*/

int jtm_bcdtodec(int val)
{
 return (val&0x0f)+((val/16)*10);
}

void jtm_readtime(JTIME	*table)
{
 __dpmi_regs regs;
 //
 regs.x.ax=0x0200;
 __dpmi_int(0x1a,&regs);
 table[0].hours=jtm_bcdtodec(regs.x.cx/256);
 table[0].minutes= jtm_bcdtodec(regs.x.cx&255);
 table[0].seconds= jtm_bcdtodec(regs.x.dx/256);
 table[0].daylight=jtm_bcdtodec(regs.x.dx&255);
}

void jtm_readdate(JDATE	*table)
{
 __dpmi_regs regs;
 //
 regs.x.ax=0x0400;
 __dpmi_int(0x1a,&regs);
 table[0].century=jtm_bcdtodec(regs.x.cx/256);
 table[0].year= jtm_bcdtodec(regs.x.cx&255);
 table[0].month= jtm_bcdtodec(regs.x.dx/256);
 table[0].day=jtm_bcdtodec(regs.x.dx&255);
}

// jtm_parsedirstring
// ------------------
//
// Returns: 0=Parsing success. !=Couldn't change a thing.(Not supposed to be an error sign)
//
// This function parses directory string to usable state that it
// will be legal pointer
//
// -Simple explain :
//
// Source: "d:/jtm/bin/program.exe"
//
// Result: "d:/jtm/bin"
//
int jtm_parsedirstring(char *str)
{
 long	ad;
 //
 for(ad=strlen(str)-1; ad!=0;	ad++)
 {
  if(	str[ad]=='/' )
  {
	str[ad+1]=0;
	return 0;
  }
 }
 return 1;
}


/*** ANSI C memory management function replacements ***/

int jtm_freesomething(void)
{
 long	ad;
 //
 for(ad=0; ad<65536;	ad++)
 {
  if(	jmem.allocated[ad]!=NULL )
  {
	return jtm_free(jmem.allocated[ad]);
  }
 }
 return -1;	// Couldn't free anything.
}

void *jtm_malloc(size_t	_size)
{
 void	*huh;
 long	ad;
 //
 for(ad=0; ad<65536;	ad++)
 {
  if(jmem.allocated[ad]==NULL)goto okay;
 }
 return NULL;
okay:
 if( (jmem.allocated[ad]=malloc(_size))==NULL )
 {
  if(jtm_freesomething()==0)
  {
	goto okay;
  }
  else
  {
	return NULL;
  }
 }
 return jmem.allocated[ad];
}

int jtm_free(void	*_ptr)
{
 long	ad;
 //
 for(ad=0; ad<65536;	ad++)
 {
  if(	jmem.allocated[ad]==_ptr)goto okay;
 }
 return -1;	// No such memory region allocated(huh?).
okay:
 free(_ptr);
 return 0; // Memory successfully freed.
}

void jtm_callmlcode(unsigned long int address)
{
 //
 char str[255];

 //
 sprintf(str,"CALLING 0x%1.8x\n",address);
 jtm_messagebuffer(str);
 (*(void (*)(void)) address)();
}

////////////////////////////////////////////////////////////////////////////
//JTM_READSTRAR
//
//Read next string and restore file pointer.
//
char *jtm_readstrar(FILE *f)
{
 //
 char str[500];
 unsigned long int ad;

 //
 ad=ftell(f);
 fscanf(f,"%s",&str);
 fseek(f,ad,SEEK_SET);
}

////////////////////////////////////////////////////////////////////////////
//JTM_IFCONTAINSCHAR
//
//Returns 1 if the string contains specified mark,
//else it returns zero.
//
int jtm_ifcontainschar(char *str,char mark)
{
 //
 long ad;

 //
 for(ad=0; ad<strlen(str); ad++)
 {
  if( str[ad]==mark ) return 1; // Returns 1(String did contain asked mark)
 }
 return 0; // Return 0(String didn't contain asked mark)
}

////////////////////////////////////////////////////////////////////////////
//JTM_IFJTMOSCFGSMO
//
// Description:
//
// Performs a check at JTMOS.CFG, system configuration file.
// It checks if the configuration is automatically generated or not.
//
// Returns:
//
// System Configuration is ..
//   0= not automatically generated.
//   != automatically generated. (Means that JTMOS.CFG didn't exist and
//                                JTMOS had to generate SAFEMODE one.)
//
int jtm_ifjtmoscfgsmo(void)
{
 //
 FILE *f,*f2;
 int i;

 //
 f=fopen(jtmfn_syscfg,"rb");
 i=f3wordseek(f,"signature","fce2e37be38b","boulder");
 fclose(f);
 return i;
}

// JTM_LOGSTDOUT2FILE ** Log STDOUT to file.
//
// Creates specified file and
// starts logging STDOUT to specified file created.
//
void jtm_logstdout2file(char *fname)
{
 //
 jtmio.extfp=fopen(fname,"wb");
 jtmio.stdout2file=0xff;
}

// JTM_CLOSESTDOUTLOG
//
// Closes STDOUT log file.
//
void jtm_closestdoutlog(void)
{
 fclose(jtmio.extfp);
 jtmio.stdout2file=0;
}

/*
  ad4=0;
  ad3=0;
  do
  {
   i=f1wordseek(f,"end");
   if(i!=0)break;
   ad3=ad4;
   ad4=ftell(f);
  }while(0==0);
  ad=ad3; // ad=ftell(f);

*/

// JTM_RM FUNCTION
// ---------------
//
// Removes a file.
// This function actually calls MS-DOS function,
// JTMOS file system support will be added later on,
// so it is very recommended to using it already right now,
// to prove support for future expansions like other file systems, etc.
//
int jtm_rm(char *fname)
{
 return remove(fname);
}

// JTM_RMDIR FUNCTION
// ------------------
//
// Removes a directory.
//
int jtm_rmdir(char *fname)
{
 return rmdir(fname);
}

// JTM_GETFILEINFO FUNCTION
// ------------------------
//
// Receives information about the specified file,
// if file not found then it returns non zero answer.
//
// Example: jtm_getfileinfo("myfile.txt",&f);
//
//
int jtm_getfileinfo(char *fname,struct ffblk *f)
{
 return findfirst(fname,f,0xff);
}

//
//
//
int jtm_moreinfo(struct ffblk *f)
{
 return findnext(f);
}

// JTM_REMOVEICON FUNCTION
// -----------------------
//
// Does same as jtm_removeicon1 function but
// asks before removing the icon.
//
int jtm_removeicon(char *fname)
{
 //
 struct ffblk f;
 char str[255],str2[255];

 //
 if(!jtm_getfileinfo(fname,&f))
 {// Found
  //
  if((f.ff_attrib&FA_DIREC)!=0)
  {
   sprintf(str,"Removing folder %s",fname);
  }
  else
  {
   sprintf(str,"Removing file %s",fname);
  }
  if(jtm_msgbox("Are you sure(Yes/No)?",str)=='y')
  {
   jtm_removeicon1(fname);
  }
 }
}

// JTM_REMOVEICON1 FUNCTION
// ------------------------
//
// This function removes icon(file or folder).
//
// Example: jtm_removeicon1("myfolder");
//
int jtm_removeicon1(char *fname)
{
 //
 struct ffblk f;

 //
 if(!jtm_getfileinfo(fname,&f))
 {// Found
  //
  if((f.ff_attrib&FA_DIREC)!=0)
  {
   jtm_rmdir(fname);
  }
  else
  {
   jtm_rm(fname);
  }
 }
 return 0;
}

// JTM_MKDIR
//
//
//
int jtm_mkdir()
{
 //
 char fname[255];

 //
 jtm_inputstr(fname,"Creating new folder - Please enter new folder name");
 if(fname[0]!=0)
 {
  if(jtm_mkdir1(fname)!=0)
  {
   jtm_msgbox("Couldn't create folder !","Error");
  }
 }
}

//
//
//
//
//
int jtm_mkdir1(char *fname)
{
 return mkdir(fname);
}

//
//
//
//
//
//
long jtm_getpmemfree(void)
{
 return _go32_dpmi_remaining_physical_memory();
}

//
//
//
//
//
//
long jtm_getvmemfree(void)
{
 return _go32_dpmi_remaining_virtual_memory();
}

// For Applications
//
JTMKEYB *jtm_getjtmkeyb()
{
 return &jtmkeyb;
}

JTMSND1 *jtm_getjtmsnd()
{
 return &jtmsnd;
}

JTMSCR *jtm_getjtmscr()
{
 return &jtmscr;
}

// Generates 64bit timecode and returns it.
unsigned long long jtm_get64timecode(void)
{
 //
 long long lad,lad2,lad3,lad4,lanswer;
 JTIME jtime;
 JDATE jdate;

 /*MSG00000000
  *   ^^^^^^^^
  *   ||||||||
  *   ||||T||U
  *   |||MHT|n
  *   ||D  MTu
  *   |YL   Ss
  *  YH      e
  *          d
  *
  */

 //
 jtm_readtime(&jtime); jtm_readdate(&jdate);

 //
 lanswer=((jdate.year&0xffff) <<48)|
         ((jdate.day&0xff)    <<40)|
         ((jdate.month&0xff)  <<32)|
         ((jtime.hours&0xff)  <<24)|
         ((jtime.minutes&0xff)<<16)|
         ((jtime.seconds&0xff)<<8);

 //
 return lanswer;
}

inline int CTO16(int val1,int val2)
{
 return (val1&0xff)|((val2&0xff)<<8);
}

inline int CRLO(int valx)
{
 return valx&0xff;
}

inline int CRHI(int valx)
{
 return (valx>>8)&0xff;
}

// If you give for example value 0x100 it tells which bit is set in the
// specified value(Well this function expects that the value has only
// one bit set).
inline int CRBIT(int valx)
{
 //
 register long ad,ad2,ad3;

 //
 for(ad2=valx,ad3=0; ad2!=0; ad2=ad2>>1,ad3++){}
 return ad3;
}

void jtm_swap(char *poi1,char *poi2,long length)
{
 //
 register long i,i2,i3;

 //
 for(i=0; i<length; i++)
 {
  i2=poi1[i];
  i3=poi2[i];
  poi1[i]=i3;
  poi2[i]=i2;
 }
}

// CyanScreen: When exception/error happends this screen appears.
jtm_cyanscreen(const char *STR_ERROR)
{
 //
 long key;
 char str[255];

 //
 jtmgl_closegas();

 //
 jtm_textcolor(CYAN); jtm_textbackground(BLUE);
 jtm_setcursortype(_NOCURSOR);
 jtm_clrscr();

 //
 jtm_textcolor(WHITE); jtm_textbackground(BLUE);
 //
 jtm_gotoxy(2,2);
 sprintf(str," JTMOS %f ",jtm_stationversion);
 write(str);

 //
 jtm_gotoxy(0,12);
 //
 jtm_textcolor(WHITE); jtm_textbackground(CYAN);
 cewrite(STR_ERROR);

 //
 key=jgetch();

 //
 jtm_textcolor(7); jtm_textbackground(BLACK); jtm_clrscr();
 return key;
}


