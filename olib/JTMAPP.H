/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS Application Programming Interface - JTMOS API
 *
 * (C)1998-99 by Jari Tapio Tuominen, all rights reserved.
 *
 * Provides easy programming times for those who are interested about to
 * make powerful/flexible applications with JTMOS.
 *
 */


#define __JTMAPP_H_INCLUDED TRUE
#include <jtmtask.h>
#include <jtmacfg.h>
struct
{
 int exitim; // Exit immediatly
}jtmapp;

/*
   "Easy programming kit" from Jari Tuominen.
   JTMOS application programming library v1.00
   We made it as easy as possible to use, now we just hope you enjoy it!

   ** Console programming module version 1.00 alpha **

  -Features :

   -Multitasking disk operating, like:
     -Copying files
     -Deleting files
     -Compression

   -And other cool features like:
     -Changing video modes
     -Audio playing
     -Video playing

   -Everything is possible to make run while the application is doing
    something else meanwhile.

   -Possibility to create processes that runs while the application is running
   -DJGPP compatible

        Copyright by MS Limited =)
*/

void jtmapp_ansibasereset()
{
 jtm_setvmode(JTM_DEFAULT_TXT_VMODE);
 textcolor(default_window_color); textbackground(default_window_bcolor);
 drawansiwindow(1,1,80,24, 2, jtm_applicationname, 1); jwindow(2,4,79,23,default_window);
 textcolor(default_window_tcolor); textbackground(default_window_tbcolor);
 clrscr();
}

void jtmapp_gfxbasereset()
{
 jtm_glsystemon();
}

void jtmapp_scripthoststartup(void)
{
 int i1,i2;
 char str[512],str2[512];
 //
 strcpy( str,jgetenv("startup","startscript=") );
/* if( !strcmp(str,"") || !strcmp(str,"Nope") )
 {
  jtm_messagebuffer("JTMOS Scripting Host error: Start up script not found, continuing without start up script.\n");
 }
 else
 {*/
  i1=jtmjsc_executescript(str);
  if(i1)
  {
   sprintf(str,"JTMOS Scripting Host: Error number %u occured.\n",i1);
   write(str);
   jtm_addnotice(str);
  }
  else
  {
   jtm_addnotice("JTMOS Scripting Host: Script successfully executed.\n");
  }
/* }*/
}

void jtm_warnwindows(void)
{
 if(jtm_detectwindows())
 {
  if(jtm_detectwindows()==0x80)
  {
   jtm_addnotice("Warning: MS-Windows is \"probaly\" running..\n");
  }
  else
  {
   jtm_addnotice("Warning: This progam runs best in pure DOS or similiar non-multitasking\n");
   jtm_addnotice("operating system. All JTMOS programs usually run good also in DOS prompts,\n");
   jtm_addnotice("but f.e. in video&audio quality and smoothness it may cause some bad flickers.\n");
  }
  jtm_addnotice("I prefer quiting Windows, OS/2 and any other multitasking operating system\n");
  jtm_addnotice("DOS 3.3 or newer required to run, you're probaly using 80386 or newer CPU.\n");
 }
}

// Init Code
void jtmapp_startup(unsigned char *sm1,unsigned char *sm2)
{
 char m1,m2,m3,m4;
 char str[2550],str2[2550];
 unsigned long int ad;
 //
#ifndef NO_HOOKS_BY_JTMOS
/*** START UP ***/

 //
 ExceptionHandlerSystemOn();

#ifndef NO_JTMTASK
 TaskMasterON();
#endif

#ifndef NO_JTMKEYB
 jtmkeyb_on();
#endif

#ifndef NO_JTMVM
 jtmvm_turnon();
#endif

#ifdef JTMSND_INCLUDED
#ifndef NO_JTMSND
 #ifndef PLEASE_DONT_RESET_DSP
  jtm_setsbirq();
 #endif PLEASE_DONT_RESET_DSP
#endif
 #ifndef PLEASE_DONT_RESET_DSP
  jtmsnd_dspreset();
 #endif PLEASE_DONT_RESET_DSP
#endif
/****************************************************************************
 ****************************************************************************/

 // Don't remove document remark unless you want
 // Windows warnings appear to your screen.
  //jtmapp_warnwindows();

 //
 jtmwr_initialize();

 // Initialize JTMOS extended fs
 jtm_initextfil(&jtmextfil);

#ifndef nosystemprocesses
 TM_AddProcess(jtm_systemkeyslooker);
#endif
// If graphics system is allowed then start it.
#ifndef nograos     // Opens Display Device -> Allows screen output
 if( (jtmworkstation.WARNINGS&1) )
 {
  // Set Exit Request.
  jtmworkstation.ERRORSTATE=1;
  // Warn user about the confusion.
  jtm_messagebuffer("Fatal error: Cannot open display device, needed configuration doesn't exist.\n");
  textcolor(7); textbackground(BLACK);
  cprintf("Fatal error: Cannot open display device, needed configuration doesn't exist.\n\r");
  cprintf("Cannot continue executing application ...\n\r");
  waitkey();
 }
 else
 {
  jtmapp_gfxbasereset();
  jtmapp_scripthoststartup();
  jtmscr.srsignal=0xff;
 }
#else
// printf("%s",jtmscr.systemnotice);
 jtmscr.systemnotice[0]=0;
#endif
#ifndef NO_JTMMOUSE
again:
#ifdef EXPECT_MOUSE_ON
 if(detect_mouse()!=0xffff)
 {
  write("DOS mouse driver not installed, load it now(y/n)?");
  m1=jgetch();
  sprintf(str,"%c\n",m1);
  write(str);
  if(m1=='y')
  {
   sprintf(str,"Executing ext mouse driver (%s)\n",jgetenv("preferences","emousedrv=") );
   write(str);
   system( jgetenv("preferences","emousedrv=") );
   waitkey();
  }
 }
#endif
 jtmmouse_on();
#endif
 jtmmouse.arrowm=0x00;

 // Load configuration if needed.
// #ifdef jtm_aconfig
  // Load application configuration at the start up and
  // if configuration file doesn't exist create default configuration.
  if(jtmconfig_loadconfig())
  {
   for(ad=0; ad<256; ad++)appconfig.data[ad]=0;
  }
// #endif
#endif
}

void jtmapp_shutdown(void)
{
 // ** Quit to DOS **
 // Remove all reserved resources and reset screen to default
 // Setup previous keyboard handler (Interrupt 0x09) to handle tasks ahead
 //
 // Notice: All drivers will be removed from memory, and normal interrupt
 //         handlers will be returned, sends OFF message to all drivers,
 //         even for those which are not ON, just to make sure all resources
 //         will be back to "normal" after shutdown process.

 /** Do this allways first! **/
 jtmhistory.AofmAtShutDown=_go32_dpmi_remaining_physical_memory();
 /** Then all the rest .... **/

 //
 if(jtmio.stdout2file)
 {
  jtm_closestdoutlog();
 }

 // Request "SB reset" before exiting.
 // Notice: This operation must be done before turning off JTMOS heart.
 if(jtmsnd.sbsys.soundused)
 {
  write("jtmapp_shutdown: Turn SBSYS off...");
  while(WaitDMA()!=-1);
  write("\rjtmapp_shutdown: SBSYS off state.\n");
 }

 // Turning off extfil (DO NOT USE EXTFIL AFTER THIS HAVE BEEN DONE !)
 jtm_shutdownextfil(&jtmextfil);

 //
 //   --> TURNING OFF JTMOS HEART <-- "The most critical parts"
 //

 // Turn mouse'n'keyboard controllers OFF
 disable();
 //
 jtmmouse_off();
 jtmkeyb_off();
#ifdef JTMSND_INCLUDED
#ifndef NO_JTMSND
 #ifndef PLEASE_DONT_RESET_DSP
  jtm_releasesbirq();
 #endif
#endif
#endif
 //
 if(jtm_vmode!=0x03)jtm_setvmode(JTM_DEFAULT_TXT_VMODE);
 if(jtm_errors)
 {
  jtm_queryerrorlist();
 }
 jtm_addnotice("\nPlease wait, completing all tasks first. (Press ESC to ignore)\n");
 jtm_waitfornoMT();
 jtm_addnotice("Releasing system resources.\n");
 TaskMasterOFF();
#ifndef NO_JTMVM
 jtmvm_turnoff();
#endif
 jtm_addnotice("Returning to MS-DOS ...\n");
 jtmscr.glsystem=0;

 // Free application configuration information, since it is not needed
 // when exiting to DOS :)
 free(appconfig.data);
 free(appconfig.fname_cfg);

 // Merge notice book to JTMOS Message Book.
 jtm_mergenoticestomessagebuffer();

 fflush(stdout);
 enable();

 if(jtm_vmode!=3)
 {
  jtm_vmode=3;
  textmode(C80);
//  jtm_setvmode(JTM_DEFAULT_TXT_VMODE);
 }
}

void jtmjicli_command(int argc,char *argv[])
{
 //
 char *image,*stringi,*stringi2,*stringi3,*stringi4,*stringi5,
      *stringi6,*stringi7,str[2550];
 unsigned long int ad,ad2;
 char *basejam;
 int i,i2;

 /*

 */

 stringi= malloc(512);
 stringi2=malloc(512);
 stringi3=malloc(512);
 stringi4=malloc(512);
 stringi5=malloc(512);
 stringi6=malloc(512);
 stringi7=malloc(512);
 #ifdef jtm_aname
 strcpy(stringi,jtm_aname);
 #else
 strcpy(stringi,"no name");
 #endif

 #ifdef jtm_aversion
 sprintf(stringi2,"%1.2f",jtm_aversion);
 #else
 sprintf(stringi2,"no version information available");
 #endif

 #ifdef jtm_acopyright
 sprintf(stringi3,"%s",jtm_acopyright);
 #else
 sprintf(stringi3,"no copyright information available");
 #endif

 #ifdef jtm_ausage
 sprintf(stringi6,"%s",jtm_ausage);
 #else
 sprintf(stringi6,"no usage information available");
 #endif

 #ifdef jtm_adescription
 sprintf(stringi5,"%s",jtm_adescription);
 #else
 sprintf(stringi5,"no description information available");
 #endif

 strcpy(stringi4,"Flags defined: ");
 #ifdef nograos
 strcat(stringi4,"nograos,");
 #endif
 #ifdef graos
 strcat(stringi4,"graos,");
 #endif
 #ifdef NO_SWINS
 strcat(stringi4,"NO_SWINS,");
 #endif
 #ifdef allownotices
 strcat(stringi4,"allownotices,");
 #endif
 #ifdef NO_SBUTTON
 strcat(stringi4,"NO_SBUTTON,");
 #endif
 #ifdef NO_JTMMOUSE
 strcat(stringi4,"NO_JTMMOUSE,");
 #endif
 #ifdef SAFEMODE
 strcat(stringi4,"SAFEMODE,");
 #endif
 #ifdef EMULATETEXT
 strcat(stringi4,"EMULATETEXT,");
 #endif
 #ifdef NO_UPDATER
 strcat(stringi4,"NO_UPDATER,");
 #endif
 #ifdef NO_BACKGROUNDPICTURE
 strcat(stringi4,"NO_UPDATER,");
 #endif
 #ifdef NO_UPDATER
 strcat(stringi4,"NO_UPDATER,");
 #endif
 #ifdef __NOMODERNGRAPHICS__
 strcat(stringi4,"__NOMODERNGRAPHICS__,");
 #endif
 #ifdef modern
 strcat(stringi4,"modern,");
 #endif
 #ifdef DONT_EXECUTE
 strcat(stringi4,"DONT_EXECUTE,");
 #endif

 if( stringi4[strlen(stringi4)-1]==',' )
 {
  stringi4[strlen(stringi4)-1]='.';
 }

 /*

 */

#ifndef NO_INTERNALARGUMENTS
 // INTERNAL ARGUMENTS //

 if(argc>1)
 {
  for(i=0; i<argc; i++)
  {
   if(i!=argc)
   {
    if(!strcmp(argv[i],"(logtomsb)"))
    {
     jtmio.logstdout=0xff;
    }
    if(!strcmp(argv[i],"(fas)"))
    {
     jtmio.forceallspaces=0xff;
    }
    if(!strcmp(argv[i],"(logto)"))
    {
     jtm_logstdout2file(argv[i+1]);
    }
   }
   if(!strcmp(argv[i],"(makingmovie)"))
   {
    jtmscr.makingmovie=1;
   }
  }
 }

 //
 if(argc>=3)
 {
  //
  if( !strcmp(argv[1],"viewraw?(gs16)") )
  {
   if( fexist(argv[2]) )
   {
    image=malloc( fsizeof(argv[2]) );
    jtm_loadbin(argv[2],image);
    jtm_setvmode(0x13);
    for(ad=0; ad<256; ad++)
    {
     jtm_setpalette(ad,ad,ad,ad);
    }
    jtm_vf(image);
    while(jgetchim()!=27);
    jtmapp.exitim=0xff;
   }
  }

  //
  if( !strcmp(argv[1],"viewraw?(vgapalette)") )
  {
   if( fexist(argv[2]) )
   {
    image=malloc( fsizeof(argv[2]) );
    jtm_loadbin(argv[2],image);
    jtm_setvmode(0x13);
    jtm_vf(image);
    while(jgetchim()!=27);
    jtmapp.exitim=0xff;
   }
  }

  //
  if( !strcmp(argv[1],"viewraw?") )
  {
   if( fexist(argv[2]) )
   {
    image=malloc( fsizeof(argv[2]) );
    jtm_loadbin(argv[2],image);
    jtm_setvmode(0x13);
    jtm_greyscale256();
    jtm_vf(image);
    while(jgetchim()!=27);
    jtmapp.exitim=0xff;
   }
  }

  //
  if( !strcmp(argv[1],"viewpcx?") )
  {
   if( fexist(argv[2]) )
   {
    image=malloc( 1024*128 );
    decode_pcxto(argv[2],image,0);
    jtm_setvmode(0x13);
    jtm_greyscale256();
    jtm_vf(image);
    while(jgetchim()!=27);
    jtmapp.exitim=0xff;
   }
  }
 }

 // For configurating application via commandline.
 // Configurating is supposted to be completed with entering
 // right kind of arguments, arguments can only numbers(decimal values).
 if(argc>=3) // configure? + one argument required.
 {
  if(
     !strcmp(argv[1],"configure?") || !strcmp(argv[1],"config?")
    )
  {
   for(ad=2; ad<argc; ad++)
   {
    sscanf(argv[ad],"%u",&appconfig.data[ad-2]);
   }
   jtmconfig_saveconfig();
   sprintf(stringi,"Application successfully configured (%s).\n",appconfig.fname_cfg);
   write(stringi);
   waitkey();
   jtmapp.exitim=0xff;
   goto loppu;
  }
 }
 if(argc>=2)
 {
  if(
     !strcmp(argv[1],"configure?") || !strcmp(argv[1],"config?")
    )
  {
   write("JTMOS internal configurating utility\n");
   write("Usage: program.exe config? [argument1] [argument2] ...\n");
   if(jtm_dumpfiletowrite(appconfig.fname_cfg))
   {
    write("No configuration file found for current application, OK.\n");
   }
   jtmapp.exitim=0xff;
   goto loppu;
  }
 }

 if(argc>=2)
 {
  //
  //
  if(!strcmp(argv[1],"rmcaps?"))
  {
   sprintf(str,"%s*.*",jtmscr.screencapturepath);
   jtm_removeicon(str);
   jtmscr_clrscr();
   jtmapp.exitim=0xff;
   goto loppu;
  }

  // Virtual Memory Test
  //
  if(!strcmp(argv[1],"memorytest?"))
  {
   write("Testing memory ...");
   basejam=malloc(_go32_dpmi_remaining_physical_memory());
   if( basejam==NULL )
   {
    write("\nCouldn't allocate maximum amount of memory.\n");
    jtmapp.exitim=0xff;
    goto loppu;
   }
   for(ad=0; ad<_go32_dpmi_remaining_physical_memory(); ad++)
   {
    basejam[ad]=ad&255;
   }
   for(ad=0; ad<_go32_dpmi_remaining_physical_memory(); ad++)
   {
    if(basejam[ad]!=(ad&255))
    {
     write("\rFatal error: Memory test failed!\n");
     jtmapp.exitim=0xff;
     goto loppu;
    }
   }
   write("\rMemory test completed - Memory working fine.\n");
   jtmapp.exitim=0xff;
   goto loppu;
  }
 }

 // Virtual Memory Test 2
 //
 if(argc>=2)
 {
  //
  if(!strcmp(argv[1],"vmsb?"))
  {
   jdelete("c:\\jtmosmsb.tmp");
   xcopy("c:\\jtmos.msb","c:\\jtmosmsb.tmp");
   system("type c:\\jtmosmsb.tmp");
   jdelete("c:\\jtmosmsb.tmp");
   jtmapp.exitim=0xff;
   goto loppu;
  }

  //
  if(!strcmp(argv[1],"delmsb?"))
  {
   write("DELMSB? = Delete Message Buffer\n");
   write("JTMOS.MSB -> Trashcan\n");
   mkdir("c:\\trashcan");
   xcopy("c:\\jtmos.msb","c:\\trashcan\\jtmos.msb");
   jdelete("c:\\jtmos.msb");
   jtmapp.exitim=0xff;
   goto loppu;
  }

  //
  if(!strcmp(argv[1],"memorytest2?"))
  {
   write("Testing memory ...");
   basejam=malloc(_go32_dpmi_remaining_virtual_memory());
   if( basejam==NULL )
   {
    write("\nCouldn't allocate maximum amount of memory.\n");
    jtmapp.exitim=0xff;
    goto loppu;
   }
   for(ad=0; ad<_go32_dpmi_remaining_virtual_memory(); ad++)
   {
    basejam[ad]=ad&255;
   }
   for(ad=0; ad<_go32_dpmi_remaining_virtual_memory(); ad++)
   {
    if(basejam[ad]!=(ad&255))
    {
     write("\rFatal error: Memory test failed!\n");
     jtmapp.exitim=0xff;
     goto loppu;
    }
   }
   write("\rMemory test completed - Memory working fine.\n");
   jtmapp.exitim=0xff;
   goto loppu;
  }
 }

 // For CONOUT output of all internal JTMOS commands available
 // via command line.
 if(argc>=2)
 {
  ////////////////////////////////////////////////////////////////////////
  // SYSTEM HELP
  //
  if(
     !strcmp(argv[1],"syshelp?") || !strcmp(argv[1],"systemhelp?")
    )
  {
   //
   jtm_viewdocpage("jicli/allcmds.txt");

   //
   jtmapp.exitim=0xff;
   goto loppu;
  }

  ////////////////////////////////////////////////////////////////////////
  // VIEW DOCUMENT
  //
  if(
     !strcmp(argv[1],"cat?")
    )
  {
   //
   jtm_catdoc(argv[2]);

   //
   jtmapp.exitim=0xff;
   goto loppu;
  }

  ////////////////////////////////////////////////////////////////////////
  // VIEW JTMOS DOCUMENT
  //
  if(
     !strcmp(argv[1],"page?")
    )
  {
   //
   jtm_viewdocpage(argv[2]);

   //
   jtmapp.exitim=0xff;
   goto loppu;
  }
 }

 // For CONOUT output version information option via commandline.
 if(argc>=2)
 {
  if(
     !strcmp(argv[1],"version?") || !strcmp(argv[1],"copyright?") ||
     !strcmp(argv[1],"ver?") || !strcmp(argv[1],"system?")
    )
  {
#ifdef graos
   jtmscr_opendump();
#endif
   sprintf(str,"JTMOS version %1.4f (C)%u by %s, all rights reserved.\n",jtm_stationversion,jtm_stationyear,jtm_copyright);
   write(str);
   sprintf(str,"Application name: \"%s\"\nVersion %s\n(C)%s, all rights reserved.\n",stringi,stringi2,stringi3);
   write(str);
   sprintf(str,"Application main file: %s, size: %u kilobytes.\n",argv[0],fsizeof(argv[0])/1024);
   write(str);
   sprintf(str,"%s\n",stringi4);
   write(str);
   sprintf(str,"Application description: %s\n",stringi5);
   write(str);
   sprintf(str,"Application usage: %s\n",stringi6);
   write(str);
   sprintf(str,"Maximum size of video buffer: %u kilobytes.\n",jtmscr_maxvbuffersize/1024);
   write(str);
   sprintf(str,"Remaining virtual memory = %u MB(s).\n",_go32_dpmi_remaining_virtual_memory()/(1024*1024));
   write(str);
   sprintf(str,"Remaining physical memory = %u MB(s).\n",_go32_dpmi_remaining_physical_memory()/(1024*1024));
   write(str);
   sprintf(str,"AofmAtStartUp = %u MB(s).\n",jtmhistory.AofmAtStartUp/(1024*1024));
   write(str);
   #ifdef graos
    write("** GraOS active - Extra information about GraOS available **\n");
    sprintf(str,"Screen width  = %u\n",jtmscr.width);
    write(str);
    sprintf(str,"Screen height = %u\n",jtmscr.height);
    write(str);
    sprintf(str,"Screen BPP    = %u\n",jtmscr.bpp);
    write(str);
   #endif
   write("If you would like to get some help about Internal System Commands then\n");
   write("simple write \"systemhelp?\" or either \"syshelp?\" after program name.\n");
   write("Remember: To emergency exit hold down LSHIFT+RSHIFT+DEL or either CTRL+ALT+DEL.\n");
   //
   write("\"c:\\jtmos.cfg\", JTMOS system configuration file ");
   if(!fexist(jtmfn_syscfg))
   {
    write("doesn't exist.\n");
   }
   else
   {
    write("exist, ");
    sprintf(str,"size=%u bytes.\n",fsizeof(jtmfn_syscfg));
    write(str);
   }
   //
   if( jtm_ifjtmoscfgsmo()==0 )
   {
    write("Configuration state [SAFEMODE].\n");
    write("Warning: Current configuration is so called safemode configuration,\n");
    write("         that is generated only when no configuration found at\n");
    write("         system start up time, either you are using JTMOS very first time,\n");
    write("         or JTMOS isn't yet configured for this computer.\n");
   }
   else
   {
    write("Configuration state [OK].\n");
   }
   //
   sprintf(str,"You are currently using drive %c:\n",getdisk()+'A');
   write(str);
   //
   //
   // *** ADD NEW ?SYSINFO CODE HERE: ***
   //
   sprintf(str,"jtmkeyb.state=%u.\n",jtmkeyb.state);
   write(str);

   //
   sprintf(str,"fchksum32(argv[0])=0x%1.8x\n",fchksum32(argv[0]));
   write(str);

   //
   jtmapp.exitim=0xff;
   waitkey();
   goto loppu;
  }
 }
loppu:
 asm("nop");
#endif
}

void jtmapp_cmdlinecheck(int argc,char *argv[])
{
 //
 char *image,*stringi,*stringi2,*stringi3,*stringi4,*stringi5,
      *stringi6,*stringi7,str[2550];
 unsigned long int ad,ad2;
 char *basejam;
 int i,i2;

 //
 jtmjicli_command(argc,argv);
loppu:
 while(0!=0);
}

/////////////////////////////////////////////////////////////////////////////
//JTM_HEALJTMOSCFG
//----------------
//
// This function fixes/heals JTMOS configuration as needed.
// First of all, if the configuration file doesn't exist then
// it creates "a safemode" one, which is not very long configuration
// file but it fixes some problems which appear when running
// some applications/functions without any configuration file at all.
//
//
void jtm_healjtmoscfg(void)
{
 //
 FILE *f,*f2;

 //
 if(!fexist(jtmfn_syscfg))
 {
  jtm_messagebuffer("C:\\JTMOS.CFG doesn't exist, creating default.\n");
  f=jtm_fopen(jtmfn_syscfg,"wb");
  fprintf(f,"rem \"C:\\JTMOS.CFG\"\n");
  fprintf(f,"rem \"This file was created by JTMOS, application programming interface.\"\n");
  fprintf(f,"rem \" >>> signature fce2e37be38b boulder <<< \"\n");
  jtm_fclose(f);
 }
}

jtmapp_wiper(long length,char *pointer)
{
 //
 register long i;

 //
 for(i=0; i<length; i++)
 {
  pointer[i]=0;
 }
}

jtmapp_initallglvarstructures()
{
 jtmapp_wiper(sizeof(JTMSCR),         &jtmscr);
 jtmapp_wiper(sizeof(JTMSND1),        &jtmsnd);
 jtmapp_wiper(sizeof(APPCONFIG),      &appconfig);
 jtmapp_wiper(sizeof(SYSTEMTIMER),    &systemtimer);
 jtmapp_wiper(sizeof(JMEM),           &jmem);
 jtmapp_wiper(sizeof(ICOMP),          &icomp);
 jtmapp_wiper(sizeof(JTMPCMODEL),     &jtmpcmodel);
 jtmapp_wiper(sizeof(JMEM),           &jmem);
 jtmapp_wiper(sizeof(JTMWORKSTATION), &jtmworkstation);
 jtmapp_wiper(sizeof(JMEM),           &jmem);
 jtmapp_wiper(sizeof(JAREAS),         &jareas);
 jtmapp_wiper(sizeof(JTMWREG),        &jtmwreg);
 jtmapp_wiper(sizeof(JTMSTDIO),       &jtmstdio);
 jtmapp_wiper(sizeof(JTMVDSYS),       &jtmvdsys);
 jtmapp_wiper(sizeof(JTMHISTORY),     &jtmhistory);
 jtmapp_wiper(sizeof(JTMEXTFIL),      &jtmextfil);
}

/*** MAIN FUNCTION
 *
 * Notice: Application programmers use int jmain (int arg,char *argv[]);
 *
 *
 ***/
int main(int argc,char *argv[])
{
 char sm1[1000],sm2[1000];
 char str[255];
 int retval,i,i2,
#ifdef _FRUN_
 FORCERUN=1;
#else
 FORCERUN=0;
#endif

 //
 jtmapp_initallglvarstructures();

 //
 atexit(jtmapp_shutdown);

 //
#ifndef REMOVE_CHECKHOST
 if(jtm_checkhost())
 {
  // If JTMOS is not yet ran :)
 }
 else
 {
  // If JTMOS is already running :/
  jtm_messagebuffer("An attempt to reload JTMOS heart detected: JTMOS is already running.");
  printf("You are already running JTMOS.\n");
  if(!FORCERUN){return 0;}else{printf("OPT/FORCERUN: Forcing execution.\n");}
 }
#endif

 //
 // Setup system variables in right condinition before initializing device drivers.
 //
 jtmhistory.AofmAtStartUp=_go32_dpmi_remaining_physical_memory();

 //
 jtmscr.graphictimer=0;

 //
#ifdef GS256
 jtmscr.psetup=1;
#else
 // Multiscale palette
 jtmscr.psetup=0;
#endif

//#ifdef JTMACFG_INCLUDED
 appconfig.data=malloc(256);
 appconfig.fname_cfg=malloc(256);
 //
 strcpy(str,argv[0]); str[strlen(str)-4]=0;
 sprintf( appconfig.fname_cfg,"%s.cfg",str );
//#endif

#ifdef JTMSND_INCLUDED
 jtmsnd.sbsys.wave=NULL;
 jtmsnd.sbsys.soundused=0;
 jtmsnd.sbsys.noticetimeout=30*8; // Long time out value = 200*8
 jtmsnd.sbsys.playmode=WPLAY_NORMAL;
 jtmsnd.sbsys.frequency=22025;
 jtmsnd.sbsys.bits=8;
 jtmsnd.sbsys.wavelength=0;
 jtmsnd.sbsys.wpp=0;
 jtmsnd.sbsys.state=0;
#endif

 //
 strcpy(jtmscr.defaultff,"sysfontb.raw");
 jtmscr.cursor.look=2;

 // Initialize DLL registry
 jdllreg.amount=0;

 // Initialize JTMOS I/O variables
 jtmio.logstdout=0;
 jtmio.logstdin=0;
 jtmio.stdout2file=0;

 //
 jtmkeyb.scantoascbusy=0;
 jtmkeyb.counter=0;
 jtmkeyb.ExtraKey=0;
 jtmkeyb.ExtraKeyCode=0;
 jtmkeyb.specialkeyconvert2asc=1;

 jtmmouse.mousekeys=0xff; // Mouse keys are ment to be allowed usually.
 jtmmouse.countdown=0;
 jtmmouse.count=0;
 jtmmouse.cursorlock=0;

 // 'SYSTEM' buttons
 jtmscr.about_button=0;
 jtmscr.start_button=0;
 jtmscr.quit_button;
 //
 jtmscr.plink=0;
 //
 jtmscr.noimmedwu=0; // Write function can call screen update if needed.
 jtmscr.wfb=1; // Write 'text' background to target as well? (0,1)

 jtmscr.makingmovie=0;
 jtmscr.wipesr=0; // WIPE SRTAB after UPDATEVRAM is completed?

 jtmscr.blinker.cont=  0;
 jtmscr.blinker.cont2= 0;
 jtmscr.blinker.cont3= 0;
 jtmscr.blinker.cont4= 0;
 jtmscr.blinker.delay= 0;
 jtmscr.blinker.delay2=0;
 jtmscr.blinker.delay3=0;
 jtmscr.blinker.delay4=0;

 // JTMOS STDIO initialization
 jtmstdio.filesopen=0;
 jtmstdio.filesclosed=0;
 jtmstdio.aohandles=0;

 // Reset active are system
 jtmact_initializesystem();

 // Setup clean state
 jtmworkstation.ERRORSTATE=0;
 jtmworkstation.WARNINGS=0;

 jtmtask.taskmode=0;
 jtmscr.immediatly_update_on_function_write=0;
 jtm_maxphyavailable=32;
 jtmapp.exitim=0;
 jtmtask.state=0;
 jtmscr.glsystem=0;
 ___i8work=0;
 jtmtm.status=0;
 jtmupdater.state=0;
 jtmkeyb.state=0;
 jtmmouse.state=0;
 jtmmouse.x=0; jtmmouse.y=0;
 jtmmouse.dx=0; jtmmouse.dy=0;
 jtmmouse.button1=0; jtmmouse.button2=0;

 // JTMOS VDSYSTEM - Virtual Disk
 jtmvdsys.state=0;

 // Check out JTMOS.CFG state.
 jtm_healjtmoscfg();
 // Initialize consoles to work fine before executing application itself
 checkos();

 /*(int _x1,int _y1,int _x2,int _y2,int _type,char *_headline,int _fill)*/
 //
 if(jtm_vmode!=0x03)jtm_setvmode(JTM_DEFAULT_TXT_VMODE);
#ifndef __NOCLRATSTARTUP
 clrscr();
 jtmansi.conoutput=0; // Defines that no special output required
// fflush(stdout);
 jwindow(1,1,80,25,default_window);
#endif
 sprintf(jtmscr.systemnotice,
          "jtmos %1.2f, released in %u by %s, all rights reserved, vmm: %1.2um, ram: %1.2um.\n",
          jtm_stationversion,jtm_stationyear,jtm_copyright,
          jtm_vmmavailable/(1024*1024),
          jtm_phyavailable/(1024*1024)
         );

 //
 jtmapp_startup(sm1,sm2);

 //
 if(jtmworkstation.ERRORSTATE==1)
 {
  goto exitti;
 }

 //
 //
#ifndef NOGL
 jtmgl_opengas();
#ifndef NOSWINS
 jtmgl_openwindow("stdout");
#endif
#endif

 //
 jtmapp_cmdlinecheck(argc,argv);
 if(jtmapp.exitim)goto exitti;
 #ifdef jtm_areqarg
 if( argc < jtm_areqarg )
 {
  jtmapp_baseinformation();
  waitkey();
  goto exitti;
 }
 #endif

 /** Do this allways ''just'' before executing the application itself **
  ** to keep history book updated propely.                            **/
 jtmhistory.AofmAtStartUp=_go32_dpmi_remaining_physical_memory();

 // þ þþ Execute application þþ þ
#ifndef DONT_EXECUTE
 retval=jmain(argc,argv);
#endif
 jtmhistory.AofmAfStartUp=_go32_dpmi_remaining_physical_memory();
 // þ þþ End of execution þþ þ

exitti:
 asm("nop");
quittaaheti:
 return retval;
}

void jtmapp_info(void)
{
 unsigned char str[2550];
 #ifdef jtm_aversion
  #ifdef jtm_aname
   #ifdef jtm_acopyright
    #ifdef jtm_adescription
     #ifdef jtm_ausage
      #ifdef jtm_areqarg
      sprintf(str,"%s version %1.4f, (c)Copyright by %s.\nDescription: %s\nUsage: %s\nArguments required = %u\n",jtm_aname,jtm_aversion,jtm_acopyright,jtm_adescription,jtm_ausage,jtm_areqarg);
      write(str);
      #endif
     #endif
    #endif
   #endif
  #endif
 #endif
}

void jtmapp_baseinformation(void)
{
 char str[2550];
 #ifdef jtm_aversion
  #ifdef jtm_aname
   #ifdef jtm_acopyright
    #ifdef jtm_adescription
     #ifdef jtm_ausage
      #ifdef jtm_areqarg
      sprintf(str,"%s version %1.4f, (c)Copyright by %s.\nDescription: %s\nUsage: %s\nArguments required = %u\n",jtm_aname,jtm_aversion,jtm_acopyright,jtm_adescription,jtm_ausage,jtm_areqarg);
      write(str);
      #endif
     #endif
    #endif
   #endif
  #endif
 #endif
}

char *sysver()
{
 char str[255];
 sprintf(str,
          "jtmos %1.2f, copyright %u by %s, all rights reserved.\n",
          jtm_stationversion,jtm_stationyear,jtm_copyright);
 return str;
}
