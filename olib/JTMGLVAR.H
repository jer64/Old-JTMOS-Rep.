/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * 	JTMOS global variables header file version 1.09
 *              (C)1997-99 by Jari Tuominen, all rights reserved.
 */
#ifndef __JTMGLVAR_H__
#define __JTMGLVAR_H__
#ifndef ACOF
#define ACOF 0xb8000+(80*50*2)
#endif

#define JTM_DEFAULT_CLOCKRATE 0x0000 // 0x000 = default, 0xf00 = quick

#define JTM_DEFAULT_TXT_VMODE 7

#ifndef __dj_include_conio_h_
enum COLORS {
    /*  dark colors     */
    BLACK,          
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    /*  light colors    */
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif

#define __JTMGLVAR_H_INCLUDED TRUE
#define jtm_copyright      "Jari Tapio Tuominen"
#define jtm_stationyear    1999
#define jtm_stationversion 0.6250 // See VERSIONS.TXT for more details.
/*
 */

/*** JTMOS wave device handles --(Future expansion)-- ***/
#define jtm_devicenone      0 // None
#define jtm_devicesb        1 // Sound Blaster 2.00 (To play 44100hz 8bit)
#define jtm_devicegus       2 // Gravis ultrasound Plug & Play + compatible.
#define jtm_devicedw        3 // Disk writer

/*** JTMOS compresser devices ***/
#define jtm_devicerlpack    4
#define jtm_devicejcomplex  5

/*** JTMOS data base devices ***/
#define jtm_devicehd        6 // Default device, C: and D:, so on.
#define jtm_devicecdrom     7 // When MSCDEX loaded
#define jtm_devicefloppy    8 // Drive A: or B:
#define jtm_devicevjfat     9 // Virtual JFAT, for example: "HD1:"
#define jtm_devicerjvat    10 // Real JFAT, for example "HD1:"

/*** JTMDJGFX ***/
#define MaxScreens 16 // Maximum amount of screens possible open same time

#define maxwin 25   // . Windows max. in memory
#define maxhot 25   // . Hot's   max. in memory
#define maxbut 25   // . Buttons max. in memory

#ifdef skipsomejob // If thats set, include libraries need immediatly.
#include <stdio.h>
#include <math.h>
#include <string.h>
#endif

#define JTM_TRUE  1
#define JTM_FALSE 0

#define JTM_ABORT 0
#define JTM_RETRY 1
#define JTM_FAIL  2

#define CLOCKON         1
#define CLOCKOFF        2
#define PERFECT         1
#define SYSPALETTE_FN   "system.pal"
#define SYSTEMON        1
#define SYSTEMOFF       0
#define pi              3.1415927      // This is often used value, isn't it?
#define pi2             pi*2

#ifndef WORD
 #define WORD unsigned
 #define BYTE unsigned char
#endif

#define pcx_header 128

#define btype_undefined 0
#define btype_button    1
#define btype_sprite    2
#define wtype_dos5    1
#define wtype_mbox    2
#define wtype_windows 3
#define wtype_modern  4
#define jtm_enterfcolor         7
#define jtm_enterbcolor         BLACK
#define jtm_messagefcolor       7
#define jtm_messagebcolor       BLACK
#define jtm_headlinefcolor      WHITE
#define jtm_headlinebcolor      BLUE
#define jtm_headlinefcolor      WHITE
#define jtm_headlinebcolor      BLUE

#define CLOCKON         1
#define CLOCKOFF        2

#define jtmos_cfg "c:/jtmos.cfg"


int wtype=wtype_dos5;

unsigned long int jtm_xmax,jtm_ymax,jtm_xmin,jtm_ymin;

/** Audio device preferences **/
/* Notice: In irq's + dma's, value 0xff means not available or not set. */
                // sndcarts.blaster

typedef struct {
                char            blaster;
                unsigned        blaster_port;
                unsigned char   blaster_irq;
                unsigned char   blaster_irq2;
                unsigned char   blaster_dma;
                unsigned char   blaster_dma2;
                // sndcarts.gus . .
                char            gus;
                unsigned        gus_port;
                unsigned char   gus_irq;
                unsigned char   gus_irq2;
                unsigned char   gus_dma;
                unsigned char   gus_dma2;
               }SNDCARTS;
SNDCARTS sndcarts;
/**                           **/

unsigned      jtm_vmode=   0x03;// Base graphic mode:
                                // ------------------
                                //
                                // 0x13 = 320x200

unsigned      jtm_emode=   0x00;// Emulation mode:
                                // ---------------
                                //
                                // 0x40 -- Emulates SVGA 32bit mode with
                                //         B/W VGA256 mode.(Not recommended)
                                //
                                //
                                //
unsigned char jtm_vbank=0xff; // 0xff means not defined yet (Start up default)

int           jtmos_cfgloaded=0;  // 0=Nope, 1=Yes

unsigned long jtm_errors=0; // Counts how many errors occured while program
                            // run, this variable is used by f.e. function
                            // jtm_messagebuffer();

// *** CONFIGURATION ***
#define cfg_doseditor "edit"
char cfg_jtxfilelnk[64];
int  waits=1; // Waitkey/Jgetch really waits something, 1=True, 0=False!
int  cfg_exejtx=0;
int  cfg_waitretscr=0;  // 1=Wait 0=No waits
int  cfg_autoexec_efl=1;// 1=Load autoexec.efl at startup 0=Don't load
int  cfg_autexetime=0;  // 1=After screen setup 0=Before ..
char cfg_swap[64];

char  m_input=1; //Multitasking input routine 0=OFF! Other=ON

 unsigned char esctoexit=0; // Allow process be stopped by escape key

 int palette_maxcolours=16;

unsigned char um1,um2,um3,um4;
char kbuf[0xff]={250,0};
 //
 int wymin=4;
 int wymax=28;
 #define def_wymin wymin
 #define def_wymax wymax
 //
unsigned commandlineinterface=0; // ADOS=0


#define moff_delay 20
#define ADOSMODE        C4350 // 80x50, great resolution for ADOS kind of OS's.
#define PRVMODE         C80   // Previous shell textmode, 80x25=default without det.
#define errormsgbox_window              56
    int errormsgbox_window_line=4;
    int errormsgbox_color =7;
    int errormsgbox_window_bcolor=BLUE;
    int errormsgbox_window_tcolor =7;
    int errormsgbox_window_tbcolor=BLACK;
#define sup_window              57
    int sup_window_line=4;
    int sup_color =7;
    int sup_window_bcolor=BLUE;
    int sup_window_tcolor =7;
    int sup_window_tbcolor=BLACK;
#define status_window           58
    int status_window_line=4;
    int status_window_color =7;
    int status_window_bcolor=BLUE;
    int status_window_tcolor =7;
    int status_window_tbcolor=BLACK;
#define default_window          60
    int default_window_line=44;
    int default_window_color =7;
    int default_window_bcolor=BLUE;
    int default_window_tcolor =7;
    int default_window_tbcolor=BLACK;
#define default_window2         59
    int default_window2_line=44;
    int default_window2_color =7;
    int default_window2_bcolor=1;
    int default_window2_tcolor =7;
    int default_window2_tbcolor=BLACK;
#define driver_window           61
    int driver_window_line=6;
    int driver_window_color =7;
    int driver_window_bcolor=BLUE;
    int driver_window_tcolor =7;
    int driver_window_tbcolor=BLACK;
#define system_window           63
    int system_window_line=4;
    int system_window_color =7;
    int system_window_bcolor=BLUE;
    int system_window_tcolor =7;
    int system_window_tbcolor=BLACK;
#define temp_window             58
#define dos_window              64
    int dos_window_line=50;
    int dos_window_color =7;
    int dos_window_bcolor=BLUE;
    int dos_window_tcolor =7;
    int dos_window_tbcolor=BLACK;
#define user_window             0
//    int user_window_line=wymax-wymin-3;
    int user_window_color =7;
    int user_window_bcolor=BLUE;
    int user_window_tcolor =7;
    int user_window_tbcolor=BLACK;
#define win95stylish 1
    int crash_prepare=0;

unsigned char jtm_waitsdALLWAYS=1;

#ifndef jtm_aname
 char *jtm_applicationname="Application 1                                                                      ";
#else
 char *jtm_applicationname=jtm_aname;
#endif

int done=0,dirline=0,dircolorset,ados_actdisk=0,ados_cdrive,ados_actualdisk=0;
#define statab 40

unsigned long jtm_vmmavailable,jtm_phyavailable,jtm_maxphyavailable;
#define jtm_doswindow    64
#define jtm_modernwindow jtm_doswindow-1
        // If 64=DOS window, so modern can be 64-1=63 ..

#define __updatebytime 1
// 256 color vesa modes:
#define _vesa640x400 0x0100
#define _vesa640x480 0x0101
#define _vesa800x600 0x0103
#define _vesa16m     0x010f // 320x200 + 16M colors

#ifdef JTMOS_VesaSupport
#define JTMOS_DefaultGfxMode 0x101      // (When using Graphics mode)
#else
#define JTMOS_DefaultGfxMode 0x12
#endif

 // Here's some icons defined in one table, in 4 word form, x1,y1,x2,y2(4dws)
 // (Only for JTMOS SYSIMAGER), so SYSIMAGE.RAW contains following icons:
unsigned     jtm_icons[]={290,70,302,77,  // 00.House 
                          304,79,316,86,  // 01.Fade
                          304,70,316,77,  // 02.)S(
                          318,79,330,86,  // 03.Finland
                          318,88,330,95,  // 04.Sweden
                          318,97,330,104, // 05.Denmark
                          304,97,316,104, // 06.USA
                          304,88,316,95,  // 07.Germany
                          1,157,45,195,   // 08.TV
                          165,30,187,53,  // 09.DOS icon
                          132,32,155,52,  // 10.Text icon
                          108,59,115,66,  // 11.[X] Button Released
                          108,68,115,75,  // 12.[X] Button Pressed
                          99,59,106,66,   // 13.[=] Button Released
                          99,68,106,75,   // 14.[=] Button Pressed
                          440,20,471,53,  // 15.Disk (Closed)
                          480,20,511,53,  // 16.Disk (Open)
                          244,26,271,53,  // 17.Folder
                          403,27,431,50,  // 18.[ADOS]
                          261,106,317,130,// 19.[START]
                          176,95,199,117, // 20.[JTMOS]
                          440,56,472,88,  // 21.Archive icon
                          397,91,427,119, // 22.[C:\>]
                          428,91,458,119, // 23.[U100]
                          459,91,489,119, // 24.[@]
                          397,120,427,148,// 25.[VMM]
                          428,120,458,148,// 26.[!]
                          459,120,489,148,// 27.[?]
                          51,158,152,212, // 28.[JTMOS] logo
                          1,157,46,196,   // 29.Monitor/Television screen
                          165,5,188,29,   // 30.WIN icon
                          10,86,97,103,   // 31.[WorkStation] logo
                          127,5,164,29,   // 32.[IMAGE] icon
                          68,11,127,55,   // 33.My computer !!
                          518,20,552,45,  // 34.Drive off
                          518,55,551,79,  // 35.Drive on
                          560,20,593,44,  // 36.HD off
                          560,55,593,79,  // 37.HD on
                          0,217,19,241,   // 38.Number zero (Medium size)
                          36,217,52,241,  // 39.Number one
                          69,217,87,241,  // 40.Number two
                          102,217,121,241,// 41.Number three  ... So you can
                          135,217,156,241,// 42.Number four       try this:
                          171,217,189,241,// 43.Number five       number=
                          204,217,223,241,// 44.Number six        38+number
                          238,217,257,241,// 45.Number seven
                          272,217,291,241,// 46.Number eight
                          306,217,325,241,// 47.Number nine
                          0,243,19,267,   // 48.Point mark // Sized in medium
                          117,59,125,67,  // 49.Minimize button released
                          117,68,125,76   // 50.Minimize button pressed
                          };

char *jtm_sysimage;

//#ifdef DJGPP // Not for Borland C/C++, b'coz it requids     typingz..

unsigned char     lsystxt[0x7f]; // Last system text appeared at system block
unsigned char     csystxt[0x7f]; // Currect system text appears at system block

unsigned char jtm_winflag=0;     // JTM windows flag,small flag in the corner
unsigned char jtm_systemfired=0; // SystemBlock fired?, 1=True!, 0=False..
unsigned char jtm_systemblock=0; // Default=Off(0)
unsigned char fillbefore=1;
unsigned        jxw_ic[maxwin];           // Close button location on
unsigned        jxw_cx[maxwin];           // Close button location on
unsigned        jxw_cy[maxwin];           // screen
unsigned        jxw_x1[maxwin];           // Window place in x-co. I
unsigned        jxw_y1[maxwin];           //                 y-co. I
unsigned        jxw_x2[maxwin];           // Window place in x-co. II
unsigned        jxw_y2[maxwin];           //                 y-co. II
unsigned        jxw_ty[maxwin];           // Window type 0=Normal 1=Bold
unsigned char   jxw_hd[40][maxwin];       // Window headline f.e. " Window #1 "
unsigned  jxw_mo[maxwin];           /* Window headline mode,
                                 *
                                 * so if mouse pointer is located on
                                 * currect window headline then
                                 * JXW_MO will be update, and other wayz,
                                 * same thing happends,
                                 *
                                 * So actions:
                                 *
                                 * jxw_mo[n]=0  : Cursor not there
                                 * jxw_mo[n]=!0 : No where else, but there.
                                 *                                      */
long  nextwind=0;      // 0=No windows defined yet, >0=of windows defined

unsigned char JtmGraphMode=0x12; /*
                                        0x01=320x200 4 colors
                                        0x12=640x480 16 colors
                                        0x13=320x200 256 colors
                                 */

unsigned  jxb_cx[maxbut];           // Draw position at 
unsigned  jxb_cy[maxbut];           // screen (x,y)
unsigned  jxb_x1[maxbut];           // Button place in x-co. I
unsigned  jxb_y1[maxbut];           //                 y-co. I
unsigned  jxb_x2[maxbut];           // Button place in x-co. II
unsigned  jxb_y2[maxbut];           //                 y-co. II
unsigned  jxb_sz[maxbut];           /* If type=1(Button) then it's size.
                                     * If type=2(Sprite) then this variable
                                     * defines which sysimage sprite uses.
                                     */
unsigned  jxb_mo[maxbut];           /* Button/Sprite mode, 0=released,
                                                           1=pressed */
unsigned  jxb_ty[maxbut];           /* Button/Sprite type:
                                       0= Undefined
                                       1= Button
                                       2= Sprite
                                    */
unsigned char           jxb_hd[64][maxbut];       // Button headline f.e. " Window #1 "
unsigned                jxb_hdmo[maxbut];         /* Button headline mode,
                                 *
                                 * so if mouse pointer is located on
                                 * currect window headline then
                                 * JXW_MO will be update, and other wayz,
                                 * same thing happends other way.
                                 *
                                 * So actions:
                                 *
                                 * jxb_mo[n]=0  : Cursor not on it
                                 * jxb_mo[n]!=0 : Cursor on it.
                                 *                                      */
long  nextbutt=0;      // 0=No buttons defined yet, >0=of buttons defined

unsigned char TITLECOLOR =CYAN;
unsigned char TITLECOLOR2=7;             
unsigned char TITLECOLOR2FIRED=14;
unsigned char SPRBACKCOLOR =8;
unsigned char SPRBACKCOLOR2=14;
unsigned      mousex=0,mousey=0,lmousex=0xffff,lmousey=0xffff;

/** WorkStation font routine **/
unsigned char     fontcolor=0x08;
unsigned          fontx1[255];   // 255 marks per font should be enough,
unsigned          fonty1[255];   // because even normaly only 30-50 marks
unsigned          fontx2[255];   // will be located in.
unsigned          fonty2[255];
unsigned          graphstate=0;
//
unsigned char   jgetenvth=0; /*
                                Global variable for jgetenv function
                                0=First string, 1=... 2=...,
                                64=Unique string, C stylish text inside '"'.
                             */ 
//#endif

// Screen variables
char  closeimmediatly=0; /*If this variable is set, system should close
                                                                 immediatly*/

// Windowing:
BYTE distance=16;

// Some define's:
#define _err_tasksoverflow 11
char mt_src[statab];
char mt_out[statab];
int system_busy=0;
int system_buttons=0; // If this variable is set to 1, it will set buttons on
int escapingbyesc=1;
long multitaskpower=1000;
#define p0 Page0_Offs
#define p1 Page1_Offs
#define _dontexit -1
#define _exit     -1
#define _jtmexit  -2
#define _jtmcont  -1
#define _none     0
#define _waitkey  -1
//
unsigned char jtm_currentwavedevice=jtm_devicenone;
unsigned char jtm_currentadvanceddatadevice=jtm_devicehd;
unsigned char jtm_currentpackdevice=jtm_devicerlpack;

#define vmode_320x200_32k 0x10d
#define vmode_320x200_64k 0x10e
#define vmode_320x200_16m 0x10f
#define vmode_640x480_16m 0x112
#define vmode_640x480_64k 0x111
#define vmode_640x480_32k 0x110
#define vmode_640x480_256 0x101
#define vmode_640x400_256 0x100

long jtm_xres;
long jtm_yres;
long jtm_cres;
int         jtm_shutdownflag;   // When this flag is set to other value than
                                // zero, it means that system is going off
                                // soon, all processes should now finish
                                // their works quickly.
                                // Possibilities are:
                                //  0=No shutdown mode (normal)
                                //  1=Shutdown mode (Returning to MS-DOS)
                                //  2=Shutdown mode (Switching off computer)
                                //  3=Restart JTMOS
                                //  4=Proceed reboot

#define MaxKeyBuffer 1024*64

// Keyboard
typedef struct
{
 unsigned keypressed;
 int key;  // Pressed key
 int rkey; // Released key
 unsigned place,aplace;
 unsigned char *buffer;
 unsigned char *ascbuf;
 unsigned char state;
 unsigned char *ktab;
 int ExtraKey,ExtraKeyCode;
 long counter;
 char scantoascbusy;

 //
 struct
 {
  int numlock;
 }ekeys;

 //
 char macro[255]; long macrooffs;
 int specialkeyconvert2asc;
}JTMKEYB;

JTMKEYB jtmkeyb;

// Mouse
typedef struct
{
 // Memory to store cursors/pieces of bitmap temporarily.
 unsigned char *buf1,*buf2;
 // Sprites for arrow modes,
 // like normal(sprarrow),delay(sprdelay) and sleep(sprsleep).
 unsigned char *sprarrow,*sprdelay,*sprsleep,*sprfinger;
 unsigned long signal;
 long viewx,viewy,x,y,lbx,lby,lx,ly,llx,lly;
 unsigned char buttons;
 unsigned char state;
 unsigned char arrowstate;
 char combytes[10];
 char bytenum;
 char dx,dy;
 unsigned char button1,button2,button3;
 unsigned long arrowm;
 unsigned long arrowmode;
 long count,countdown;
 char cursorlock;
 int mouseirqnum;
 int mousecomport;
 int mousekeys;
}JTMMOUSE;

JTMMOUSE jtmmouse;

/*

  JTMOS virtual screen device :
  -----------------------------

      Requirements :

      -> One 80x25 16 color EGA ANSI based screen with screen information
         datas takes about ~4KB memory.
      -> One 256 color 1600*1200 sized bitmap takes about 1.92MB's memory,
         if you are running two screens of that size, it'll eat memory
         about 4MB's, but including Windoes and all other stuff it'll take much
         more memory, memory management system will probaly have somekind of
         problems, because of having not enough memory left,
         it'll swap most unused(inactive) memory to disk, and
         user maybe wonder whats going on if system cannot respond because
         it is busy with handling all that big amount of memory.

         Overall: It is good to have somekind of drive cache running before
                  you execute any JTMOS programs because it'll speed up
                  disk handling a lot, good amount of disk cache to allocate
                  is 4MB's, but if you want real good and smooth working
                  it is better install drive cache sized atleast 16MB's.

                  So how much total RAM all this is going to take.
                  All I can say is that 32M is actually smallest size of
                  RAM when requiring enjoyable working.
                  <16MB's is for masokists *<8-)>-|O=

      Introduction:

      -> STDIO and CONIO libraries is now decicated to replace with this JTMOS
         virtual screen library,
         word "virtual" means that screens are very flexible.
         Screens can be tiled like Windowses(Well, no routines to do that yet,
         well keep on dreaming *<8-), seriously it'll be soon possible, honestly.)

      Screens are not same thing as Windows, Windows will be drawn
      on screen(s)

      Rules:

        -One screen has one cursor.
        -Screen can be formatted ASCII or either BITMAP.
         -> If screen is in bitmap format, variable "bits" tells how many
            bits are used for one pixel in bitmap.
        -"colors" variable tells which is maximum amount of colors can be used

      Problems :

        When opening more windows than there is memory available,
        it'll cause serious problems.


*/
typedef struct
{
 unsigned char conoutput;

 struct
 {
  unsigned char *dat[MaxScreens];
  unsigned width[MaxScreens];
  unsigned height[MaxScreens];
  unsigned bits[MaxScreens];  // þ This is usually set to 8, which means 8bit.
  unsigned colors[MaxScreens];
  unsigned mode[MaxScreens];
  unsigned cursorx[MaxScreens];   // þX-location of cursor in current screen
  unsigned cursory[MaxScreens];   // þY-location of cursor in current screen
  unsigned cursormode[MaxScreens];// þCursor mode, thats it.
  unsigned allocated[MaxScreens]; //  If variable is set to 1 if screen is
                                  //  allocated.
  unsigned vmode[MaxScreens];     // þVideomode that is wished to be setupped
                                  //  if possible, and if videomode changing
                                  //  is not allowed system tries to
                                  //  emulate videomode with nearest possible
                                  //  choice.
  /*------------------------------------------------------------------------*/
  unsigned nextscreen;
 }screen;
 unsigned char palette[(256*4)];
}JTMANSI;

JTMANSI jtmansi;

/*

 Here's some short documentation about structure above:
 ----------------------------------------------------------------------------
 jtmansi.screen.mode =       bit 0 : Screen scrolling enabled, 1=TRUE 0=FALSE
                             bit 1 : Bitmap mode, 1=TRUE 0=FALSE
 ----------------------------------------------------------------------------
 jtmansi.screen.cursormode = bit 0 : Insert mode, 1=TRUE 0=FALSE
 ----------------------------------------------------------------------------

*/

typedef struct
{
 int action,a,b,c,d,e,f,z,x,y,state;
 unsigned screenrefreshrate;
}JTMUPDATER;

JTMUPDATER jtmupdater;

struct
{
 unsigned char status;
}jtmtm;

typedef struct
       {
        unsigned long int headlinebackgroundcolor;
        unsigned long int headlineforgegroundcolor;
        unsigned long int defaultbackgroundcolor;
        unsigned long int backgroundcolor;
        unsigned long int forgegroundcolor;
        unsigned long int textbackgroundcolor;
        unsigned long int textforgegroundcolor;
        //
        long width,height;
        long colors;
        long lwidth,lheight;
        long lcolors;
        unsigned char *glarrow;
        unsigned long int vbrefresh;
        unsigned char *vbuffer;
        unsigned char vbufferupdatemode;
        unsigned char vbufferBUSY;
        unsigned char vbufferDELAY;
        long          sizeofvbuffer;
        /** WorkStation font routine **/
        unsigned long int fontcolor,fontbcolor;
        unsigned char     *font; // Bitmap data storage for bitmap fonts
        unsigned          fontx1[255];// 255 marks per font should be enough,
        unsigned          fonty1[255];// because even normaly only 30-50 marks
        unsigned          fontx2[255];// will be located in,
        unsigned          fonty2[255];// US ASCII has 7bits and european 8bits,
        int ma;                       // I prefer to use max. 256 characters
                                      // font since no graphic marks included.
        int writex,writey,writexs,writeys,writexm,writeym;
        int lwritex,lwritey,lwritexs,lwriteys,lwritexm,lwriteym;
        unsigned long clipnumber; // For screen clips
        unsigned char glsystem; // GLsystem state
        unsigned char srsignal;// Screen redraw signal

        // Information about 2048 lines redraw.
        unsigned char srtab[2048];
        // F.e., screen contains 480 lines, and you want to draw circle in
        // mindle of the screen, drawing of circle itself is quite fast
        // peration, but poking video buffer to video ram is really slow,
        // but it can be speeded up, atleast if not all lines are neccesery
        // to write in video memory, this operation is now possible with this
        // table above, which contains information about these lines which must
        // poke in video memory.

        //System notice, features textdata, which usually fits in 8000 marks.
        unsigned char systemnotice[(1024*64)];
        //
        unsigned char forcedelaysr;
        //
        struct
        {
         unsigned char *bm,*tmp; // Pointer to start button bitmap
         unsigned char state;
         unsigned char mode;
        }sbutton;
        //
        struct
        {
         unsigned char mode; // 0 or either 0xff
         long step,step2,step3,step4,cont,cont2,cont3,cont4,
                                           lcont,lcont2,lcont3,lcont4,
                                           delay,delay2,delay3,delay4;
         unsigned char state; // !0 ON & 0 OFF
        }blinker;
        //
        unsigned char bsignal;
        //
        unsigned sbegin;
        //
        unsigned char sin1[0x1ff];
        //
        unsigned char immediatly_update_on_function_write;
        //
        struct
        {
         unsigned char busy;
        }write;
        //

        // (For ANSI windows)
        struct
        {
         int wx1,wy1,wx2,wy2;
         int wx1_b,wy1_b,wx2_b,wy2_b;
        }awin;

        // Default file names for system image and system font.
        unsigned char fn_systemimage_raw[64];
        unsigned char fn_systemfont_raw[64];

        // Default graphics mode
        unsigned defaultgfxmode;

        //
        unsigned char bwmode; // Black'n'white mode feature (0=OFF,..)

        //
        unsigned char bpp;    // Bits per pixel (8,15,16,24)
        unsigned char planes; // Number of planes (?)

        //
        unsigned long int drawround;
        unsigned long int drawexpection; // 0=No expection
                                         // 1=Immediatly redraw
                                         // 2
        //
        unsigned char srsignallock; // 0=Unlocked, !=Locked.

        //
        unsigned char graphictimer;

        //
        char *screencapturepath; // Path where to save screen captures.

        //
        int makingmovie;

        //
        long extparamlock; // Set this variable befoer using,
                           // and please don't allow your program
                           // to use extparam while extparamlock is set.
        long extparam[20]; // Some extra parameters

        unsigned char *extpointer[20];

        // Wipe SRTAB after UPDATEVRAM ? 1=TRUE and 0=FALSE.
        int wipesr;

        struct
        {
         int shutdown;
         int state;
        }gas;

        //
        struct
        {
         int state;
         unsigned char *b,*b2,*b3,*b4;
         unsigned char *icon;
         int look; // 1=C64 like
                   // 2=3D cursor
                   // 3=GridBox
                   // 4=Transparent GridBox
        }cursor;

        //
        void *bgrd_function; // Background redraw function
        void *fgrd_function; // Forgeground redraw function

        //
        int busy;

        //
        int psetup;
        /*
                Valid values for PSETUP :

                0x00 = Multiscale palette MP 256/64=4 scales
                0x01 = Grey
                0x02 =       Red
                0x03 = Light Red
                0x04 =       Green
                0x05 = Light Green
                0x06 =       Blue
                0x07 = Light Blue
         */

       // Write also font background to target bitmap? 1=TRUE, 0=FALSE..
       int wfb;

       //
       int arrowsystempid;

       //
       char  about_button;
       char  start_button;
       char   quit_button;

       //
       char defaultff[100];
       int noimmedwu;

       // (Used by the jtmscr_plinkbackground function,
       //  can be also used for any other purposes if needed.)
       int plink;

       }JTMSCR;

JTMSCR jtmscr;

// Amount of graphic windows allowed to be open sametime. (default=1024)
#define AOGFXWIN 1024

typedef struct
{
 unsigned char s[255];
}JSTRING;


typedef struct {
        long nextwin;
        long x1[AOGFXWIN];
        long y1[AOGFXWIN];
        long x2[AOGFXWIN];
        long y2[AOGFXWIN];
        long writexs[AOGFXWIN],writeys[AOGFXWIN],
             writexm[AOGFXWIN],writeym[AOGFXWIN];
        char *text[AOGFXWIN];
        char *bitmap[AOGFXWIN];
        unsigned long int fcolor[AOGFXWIN],bcolor[AOGFXWIN];
        struct
        {
         unsigned long fcolor[AOGFXWIN],bcolor[AOGFXWIN];
         long x1[AOGFXWIN];
         long y1[AOGFXWIN];
         long x2[AOGFXWIN];
         long y2[AOGFXWIN];
         long writexs[AOGFXWIN],writeys[AOGFXWIN],
              writexm[AOGFXWIN],writeym[AOGFXWIN];
         char *text[AOGFXWIN];
         char *bitmap[AOGFXWIN];
        }headline;

        // Variables for ansi windows usage
        // (Special casing because ansi graphic mode is character based,
        //  no bitmaps and other such huge space taking structures are needed)
        struct
        {
         int *dbase[AOGFXWIN];
        }awin;
       }JWIN;
JWIN jwin;

typedef struct {
        unsigned long int fontcolor;
        unsigned long int fontbcolor;
        }NEWWINDOW;
NEWWINDOW NewWindow;

struct
{
 unsigned char timer;
}jtmclipper;

unsigned char jtm_bmpheader8[]={
  0x42,0x4d,0x38,0xfe,
  0x0,0x0,0x0,0x0,0x0,0x0,0x36,0x4,0x0,0x0,0x28,0x0,0x0,0x0,
  0x40,0x1,0x0,0x0, // x-width  (offset 0x12)
  0xc8,0x0,0x0,0x0, // y-heigth (offset 0x16)
  0x1,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x12,0xb,0x0,0x0,0x12,0xb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff
  };

unsigned char jtm_bmpheader24[]={
  0x42,0x4d,0x38,0xee,
  0x0,0x0,0x0,0x0,0x0,0x0,0x36,0x0,0x0,0x0,0x28,0x0,0x0,0x0,
  0x40,0x1,0x0,0x0, // x-width  (offset 0x12)
  0xc8,0x0,0x0,0x0, // y-heigth (offset 0x16)
  0x1,0x0,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x12,0xb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff
  };

char jtmspr_arrow[]=          // 16x16
     {
      1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
      1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
      1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
      1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
      1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
     };

typedef struct
{
 unsigned char state;
 unsigned char taskmode;
 unsigned char coverfakeOFF;
}JTMTASK;

JTMTASK jtmtask;

typedef struct
        {
         unsigned char hours;
         unsigned char minutes;
         unsigned char seconds;
         unsigned char daylight;
        } JTIME;

typedef struct
        {
         unsigned char century;
         unsigned char year;
         unsigned char month;
         unsigned char day;
        } JDATE;

//unsigned char _JTMOS_DETECTIVE_[255]="[E37B] JTMOS tunnistus merkkijono [FCE2]"

unsigned char hookirqs=1; // Install OS to work from interrupts.
unsigned _nvmmhand=0; // Next VMM handle
char _dostemp[100]; // The path of DOS temp
FILE *_vmmfp[25]; // Reserves space for 25 memory regions.

char     _jcols[]={BLACK,BLUE,LIGHTBLUE,CYAN,LIGHTCYAN,YELLOW,WHITE,
                   YELLOW,LIGHTCYAN,CYAN,LIGHTBLUE,BLUE,0xff};

char     _jmarks[]={124,47,45,92,0xff};

char     tempdrive[100]={99,58,92,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define _tempname "tempfile."

#define warnings 0     // 0=Off, 1=On
#define delaytime1 2500

/* Some variables */
WORD hand[maxwin];
unsigned char jtm_pcmodel1=0,jtm_pcmodel2=0;

unsigned jxwvmm=0;  // VMM memory available
                    // (Automatically set to zero, if JTEMP not set,
                    //  which means that JXW-VMM memory not available)
int quickwin=0; /* 0= No Quick Windows, 1= Draw Quick Windows
                   will be used at f.e. error message telling 2 user */
unsigned char jtmos=            1; // As the default it's 0 (=Off),
                                   // checkos(); changes it to On(1),
                                   // if environment variable "JTMOS" is set.
unsigned char jtm_shutdownmode= 0;
unsigned char jtm_quitbykey=    0;  // 1=Allow system terminate processes
                                    //   if jgetchim()==JTM_QUITKEY ...
unsigned char jtm_quitkey=      27;

/* Pointers for 16 JTMOS drivers */
char *drivers[16];
char enterafterinput=1;
int nextdrv=0; // => 0 means no drivers installed yet(Only if nextdrv=0)
int nextstate=1; /* 0 Nextdrv ON 1 Nextdrv OFF(Default),
		  * 1 is normal state, and 0 is used at special cases
		  */
unsigned char jtm_underwindows=0; //0=False, 0x80=True

unsigned char am_normal=        7;
unsigned char am_normalbak=     BLACK;
unsigned char am_active=        WHITE;
unsigned char am_activebak=     BLUE;
unsigned char am_message=       CYAN;
unsigned char am_messagebak=    BLACK;
unsigned char am_edit=          7;
unsigned char am_editbak=       0;
unsigned char am_actedit=       14;
unsigned char am_acteditbak=    4;

unsigned char jtm_keydelaytime=100;

#ifndef JTMOS_Linux /*** !!!! ***/
/** Mouse configurations **/
int mousecursor=0; //0=Off, 1=On
#define j_mousetype_busmouse        1
#define j_mousetype_serialmouse     2
#define j_mousetype_inportmouse     3
#define j_mousetype_ps2mouse        4
#define j_mousetype_hpmouse         5
#define j_mouselanguage_english         0
#define j_mouselanguage_french          1
#define j_mouselanguage_dutch           2
#define j_mouselanguage_german          3
#define j_mouselanguage_swedish         4
#define j_mouselanguage_finnish         5
#define j_mouselanguage_spanish         6
#define j_mouselanguage_portuguese      7
#define j_mouselanguage_italian         8
int waitkey_mouseexit=JTM_TRUE; // Left mouse button exit from WAITKEY function.
int waitkey_mouseexit_button=0x01;
#endif

typedef struct
{
 long counter1,counter2,counter3,counter4,
      counter5,counter6,counter7,counter8,
      counterx;
 unsigned char *temp,*temp2;
}SYSTEMTIMER;
SYSTEMTIMER systemtimer;

#ifdef JTMSND_STATE_ON
#define WPLAY_STOP     0  // Sample playing is stopped.
#define WPLAY_NORMAL   1  // Sample playing will be automatically stopped.
#define WPLAY_REPEAT   2  // Sample playing will be looped.
#define WPLAY_ABNORMAL 4  // Sample playing will be abnormal :) ?

#ifdef DJGPP
#include <dpmi.h>
typedef struct
{
 struct
 {
  int state;
  _go32_dpmi_seginfo seginfo;
  int bits;
  unsigned playmode;
  unsigned frequency;
  long cplen; // Clip Playing Length
  long wavelength;
  char *clip1,*wave;
  long wpp; // Wave Playing Place (Can used only once by time!!).
  int notice; // Kuittaus muuttuja.
  int noticetimeout;
  int soundused; // <-- Is set if sound system used.
                 //     This variable will be checked allways before
                 //     exiting to DOS to prevent crash!
 }sbsys;
}JTMSND1;
JTMSND1 jtmsnd;
#endif
#endif


/*unsigned char jtm_currentvbank();
char jgetchim();
char jgetch();
void mouse_cursor_off();
void mouse_cursor_on();
int mousexstat();
int mouseystat();
int get_mouse_irq();
int get_mouse_type();
int get_mouse_version();*/

unsigned char JTM_EGAPALETTE[]={
 0, 0, 0,
 0, 0, 168,
 0, 168, 0,
 0, 168, 168,
 168, 0, 0,
 168, 0, 168,
 168, 84, 0,
 168, 168, 168,
 84, 84, 84,
 84, 84, 252,
 84, 252, 84,
 84, 252, 252,
 252, 84, 84,
 252, 84, 252,
 252, 252, 84,
 252, 252, 252
};

// Application configuration structure.
// Notice: This configuration structure is not allways used.
//         It is allways loaded from configuration
//         information located at the disk,
//         but only if jtm_aconfig is defined.
// F.e. #define jtm_aconfig "program.cfg"
typedef struct
{
 unsigned char *fname_cfg;
 unsigned char *data;
}APPCONFIG;
APPCONFIG appconfig;

#ifdef __DJGPP__
// For MS-DOS & DJGPP
char jtmfn_syscfg[]="c:/jtmos.cfg\09438547357934749753759543478478938747554757892547572572752397853";
#else
// For Linux / UN*X systems
char jtmfn_syscfg[]="/etc/jtmos.cfg\0569567567579796776763786392967236767468346784637946372498698634";
#endif

//
// JTMOS Memory Management Structure
//
// Benefit: When system runs out of memory resources, JMM starts
//          freeing first allocated memory.
//          FIFO = First in, first out.
//
// Notice!: All nom-swappable memory must be marked with
//          non-swappable marking.
//
// Expections: System can manage to put its own memory regions
//             to non-swappable state which is really
//             recommended to prove system working more comfortable.
//
// Structure:  jmem
//
typedef struct
{
 void *allocated[65536];
 void *swapped[65536];
 int state;
 int error;
 int warning;
 int caution;
}JMEM;
JMEM jmem;


typedef struct
{
 int LowRule;
 long a1,a2,a3,a4,a5,a6,a7,a8;
 unsigned char *bin1,*bin2;
}ICOMP;
ICOMP icomp;

typedef struct
{
 unsigned long int id;
}JTMPCMODEL;
JTMPCMODEL jtmpcmodel;

/*
                         * 0x0000 = State OK.
                           0x0001 = Fatal error occured,
                                    system is ordered to exit immediatly.
                           0x0002 = ...*

                         * (Bits)
                           0x0000 = State OK.
                           0x0001 = Video Mode Unknown
                           0x0002 =
                           0x0004
                           0x0008
                           0x0010 */



typedef struct
{
 long ERRORSTATE;
 unsigned WARNINGS;
}JTMWORKSTATION;
JTMWORKSTATION jtmworkstation;

#define JID_ACTIVEAREA 0x0000

typedef struct
{
 int x1,y1,x2,y2,mode,state;
 void (*handler)(int x1,int y1,int x2,int y2,int mode);
}JAREA;

typedef struct
{
 JAREA j[1000];
 int amount,state;
}JAREAS;
JAREAS jareas;

typedef struct
{
 int logstdout,
     logstdin,
     stdout2file;
 int forceallspaces;

 FILE *extfp,*extfp2;
}JTMIO;
JTMIO jtmio;

typedef struct
{
 char *fname[100000];
 unsigned char attrib[100000];
 unsigned long size[100000];
 long files;
 int aof; // Amount of files
}JDIRSTRUCT;

typedef struct
{
 int yloc,afdentries;
 JDIRSTRUCT dir1;
}JTMFM;
JTMFM jtmfm;

// Window registry - Here all information about "windows" will be stored.
#define JTMWREG_AOW 500
typedef struct
{
 int amount; // Amount Of active windows.
 long x1[JTMWREG_AOW],y1[JTMWREG_AOW],x2[JTMWREG_AOW],y2[JTMWREG_AOW]; // Coordinate database for "windows".
 char *headline[JTMWREG_AOW];
 unsigned long int type[JTMWREG_AOW];
}JTMWREG;
JTMWREG jtmwreg;
// These definations are only needed when using Tapio superior keyboard system
// under MS-DOS. Tapio keyboard gives possibility to use keyboard in new ways.
#ifdef DJGPP
#include <jtmkfi.h>
#endif

typedef struct
{
 int xlen,ylen,lxlen,lylen;
}JTMLPCXINFO;
JTMLPCXINFO jtmlpcxinfo;

typedef struct
{
 long filesopen;
 long filesclosed;
 long aohandles;
 FILE *handles[5000];
}JTMSTDIO;
JTMSTDIO jtmstdio;

typedef struct // JTMOS virtual disk system
{
 int drivesattached; // Virtual drives attached.
 int drivesfound;    // Virtual drives found(detected).
 char vdpath[255];   // Virtual drives physical location(PATH) in HOST system.
 int state;
}JTMVDSYS;
JTMVDSYS jtmvdsys;

typedef struct // JTMOS history system / System diagnostic system
{
 unsigned long int AofmAtStartUp; // Amount of free memory at start up
 unsigned long int AofmAfStartUp; // Amount of free memory after start up
 unsigned long int AofmAtShutDown; // Amount of free memory at shutdown
 unsigned long int AofmAfShutDown; // Amount of free memory after shutdown
}JTMHISTORY;
JTMHISTORY jtmhistory;

// JTMEXTFIL main structure
// Special note: Drive cannot be mirrored to another mirrored drive !
typedef struct
{
 int state; // Set to non zero when active
 int drvamount; // Amount of drives (Updated at system boot up time)
 char *drvlst[200]; // List of drives available(In addition to normal C: D: E:)
 struct DLL *drvdll[200];  // Pointer to a DLL that handles drive
                           // (When not just mirroring directories).
                           // DLL usually contains functions like
                           // dll_jfopen dll_jfclose replacement functions.
 char *drvinf[200];  // Pointer to specified information about drive(ASCII).
                     // If information string begins with '"'
                     // then the string is a filename which points to
                     // the device driver which should be loaded to
                     // 'drvexe' else it is a mirroring path.
 long drvtyp[200];  // Drive type :
                    // 0x00 = None (Not allocated? or freed?)
                    // 0x01 = Path mirror (F.e. mir: mirrors to c:/dos)
                    // 0x02 = Device driver based drive
}JTMEXTFIL;
JTMEXTFIL jtmextfil; // JTMOS extended capacities for filing
                     // (Abilities that goes beyond DOS)

// JTMEXTFIL JFILE structure
typedef struct {

 //
 //  Extended Parameters
 //

 // Usual use for 'extpoi':
 //
 // Pointer 0: Pointer to file data buffer.
 // Param.  0: Offset of pointer 0 (ftell returns similiar value).
 //
 char *extpoi[10];
 long extpar[10];

 // Type :
 // 0x00=Undefined
 // 0x01=DOS handled(Calls will be automatically redirected to DOS)
 // 0x02=DLL handled(Calls will be automatically redirected to specified DLL)
 long type;

 // Cool compressions available:
 //      0x00=Undefined
 //      0x01=Normal, Uncompresssed RAW stream, DOS like.
 //   *% 0x02=Compressed stream(Saves couple bytes of space on disk)
 //
 // * = Compressed streams act like a large buffers,
 //     so if you write data into a compressed stream,
 //     the system first sucks all the data to a temporary buffer(f.e. file)
 //     until FCLOSE function is called. When FCLOSE is called,
 //     JTMOS compresses the temporary buffer into final file,
 //     and removes the temporary file.
 //
 // % = Requires DLL !
 //
 long compression;

 // STDIO FILE structure :
 FILE *file;
} JFILE;

// JTMOS dll registry. - System keeps diary of all DLL's installed on system
// with following structure.
#define JMAXDLL 200
typedef struct
{
 int amount;
 char *fname[JMAXDLL];
 struct DLL *funct[JMAXDLL];
}JDLLREG;

typedef struct
{
 long x1,y1,x2,y2;
}LINESTR;

/*
 *  Polygon contains three points: A(x1,y1),B(x2,y2),C(x3,y3)
 *
 */
typedef struct
{
 long x1,y1,x2,y2,x3,y3;
}POLG;

#endif
