/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * (M)library 1.00 (C)1998 by Jari Tuominen, all rights reserved.
 *            Mylly Software LTD in 1998.
 */

/*
 * void TaskMasterOFF(void);
 * void TaskMasterON(void);
 *
 * Task =    System task, for example copying a file, or moving files, etc.
 * Process = Process that runs in background, subroutine which will be
 *           called frequently.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define DONT_MASK_WHILE_RUNNING_PROCESSES

#ifndef __JTMTASK_H_INCLUDED
#define __JTMTASK_H_INCLUDED TRUE

#define jtmos_multitask TRUE
#include <jtminter.h>

// Defines
#define MTCMD_copy 0x0001
#define maxtasks         200
#define maxprocesses     200

//
void jtmtask_fixclockrate(void);

// Variables
void        *TempPointer;
long        ProcessesRunning=0; // 0=None
void        *ProcessPointer[maxprocesses];
int         *ProcessMode[maxprocesses];
long        TaskPower=0;
int         task_type[maxtasks];
char        *task_arg1[maxtasks];
char        *task_arg2[maxtasks];
long        task_par1[maxtasks];
long        task_par2[maxtasks];
long        task_par3[maxtasks];
long        task_par4[maxtasks];
long        task_counter[maxtasks];
long        task_counter2[maxtasks];
FILE        *task_fp[maxtasks];
FILE        *task_fp2[maxtasks];

int         ProcessFreezer=0; // If it is set to other than zero,
                              // then it causes that processes won't be called.

/*
   (!!: Future expansion, not yet available, not used in this version of TM)
   Usual possibilities :

                     (bit 0)
                         0= None
                         1= Call process immediatly

                     (bit 1)
                         0= None
                         1= Call process if mouse button pressed

                     (bit 2)
                         0= None
                         1= Call process if mouse cursor place changed

                     (bit 3)
                         0= None
                         1= Call process if new system message appeared

                     (bit 4)
                         0= None
                         1= Call process if new system error appeared

                     (bit 5)
                         0= None
                         1= Call process if system shutdown process flag set

                     (bit 6)
                         0= None
                         1= Call process if disk device accessed

                     (bit 7)
                         0= None
                         1= Call process if video device accessed

                     (bit 8)
                         0= None
                         1= Call process if audio device accessed

                     (bit 9)
                         0= None
                         1= Call process if jdisk device accessed (Virtual)

                     (bit 10)
                         0= None
                         1= Call process if a key pressed

                     (bit 11)
                         0= None
                         1= Call process if new process added

                     (bit 12)
                         0= None
                         1= Call process if new task added

                     (bit 13)
                         0= Slave mode  (Process can be terminated,
                                         process cannot terminate other
                                         processes or tasks that are above its
                                         own priority)
                         1= Master mode (Process cannot be terminated when
                                         it is still working,
                                         process itself can terminate
                                         other tasks or processes,
                                         because it is in "master" task mode)

                     (bit 14)
                         0= None
                         1= Terminate process immediatly (Suicide/Emergency exit)

                     (bit 15)
                         0= Normal priority (Called one time per interrupt)
                         1= Double priority (Called twice per interrupt)

 */


void te();

//
// Description: If you need to make own timer callback then
//              use system process adder. (TM_AddSystemProcess)
//
//              If you have some other need
//              which timing is not necceserily needed to be
//              called just in time, then use TM_AddProcess.
//

// Mode 1 = Application process             (Executed when CPU time left,
//                                           Application Process stops JMAIN() function,
//                                           but however Mode2 processes can stop mode2 processes as well)
// ..   2 = Executive/System driver process (Frequently executed, shouldn't take too much CPU power!)

// For processes that is needed to call frequently and
// but are very heavy CPU time eaters.
//
int TM_AddProcess(void *pointteri)
{
 if(jtmtask.taskmode==0)
 {
  ProcessMode[ProcessesRunning]=1; // <-- Application process signature
  ProcessPointer[ProcessesRunning]=pointteri;
  ProcessesRunning++;
  return (ProcessesRunning-1);
 }
 else
 {
  return -1;
 }
}

// For processes that is needed to call frequently and won't take too much
// CPU time to execute.
//
int TM_AddSystemProcess(void *pointteri)
{
 if(jtmtask.taskmode==0)
 {
  ProcessMode[ProcessesRunning]=2; // <-- System process signature
  ProcessPointer[ProcessesRunning]=pointteri;
  ProcessesRunning++;
  return (ProcessesRunning-1);
 }
 else
 {
  return -1;
 }
}

int TM_RemoveProcess(int ProcessID)
{
 ProcessMode[ProcessID]=0;
}

// TM_RUNPROCESSES function
// ------------------------
//
// This function executes specified group of processes.
//
// Group 1 = Application processes
// Group 2 = System processes
//
//
void TM_RunProcesses(int whichones)
{
 //
 long __a,__b,__a1;

 //
 if(ProcessesRunning!=0)
 {
  for(__a=0; __a<ProcessesRunning; __a++)
  {
   if( ((int)ProcessMode[__a])==whichones )
   {
    TempPointer=ProcessPointer[__a];
    __a1=(unsigned long int)TempPointer;
    (*(void (*)(void)) __a1)(); // *** X
   }
  }
 }
}

void TM_HandleProcesses() // Handle IRQ processes.
{
 //
 long __a,__b,__a1;


 //
 if(jtmtm.status==0 && jtmtask.taskmode==0)
 {
  //
//  jtm_releasesystemirq();
//  jtm_setsystemirq();

  //
  jtmtask.taskmode=0xff;
  enable();
  outportb(0x20,0x20);

  //
  jtmtm.status=0xff;
  TM_RunProcesses(1);
  TM_RunProcesses(2);
  jtmtm.status=0x00;

  //
  disable();
  jtmtask.taskmode=0;
 }

 if(jtmtask.taskmode==0xff)
 {
  // Blink background when interrupt occured while
  // another timer interrupt was still running.

  jtmscr_blinkbackground();
//  TM_RunProcesses(2);
 }
}

void TM_HandleTasks()
{
 long a,b;
 //    z
 // Task master IRQ handler -- Realtime multitasking, using IRQ's!
 //                 *** Task handler ***
 //
 // Notice: This is not processes handler, this is taskman,
 //         this is kind of device manager, console.
 //
 for(b=0; b<TaskPower; b++)
 {
  for(a=0; a<maxtasks; a++)
  {
   if(task_type[a]!=0)
   {
    // MT-COPY
    if(task_type[a]==MTCMD_copy)
    {
     if(task_fp[a]==NULL)
     {
//      cprintf("OS tasker: Copying file %s to %s\n\r",task_arg1[a],task_arg2[a]);
      //jtm_setpalette(0,rand()&255,0,0);
      task_counter[a]=0;
      task_fp[a]= fopen(task_arg1[a],"rb");
      if(task_fp[a]==NULL)
      {
       task_type[a]=0;
       goto NextTask;
      }
      task_fp2[a]=fopen(task_arg2[a],"wb");
      if(task_fp2[a]==NULL)
      {
       task_type[a]=0;
       goto NextTask;
      }
      fseek(task_fp[a],0,SEEK_END);
      task_counter2[a]=ftell(task_fp[a]);
      fseek(task_fp[a],0,SEEK_SET);
     }
     else
     {
      fputc(fgetc(task_fp[a]),task_fp2[a]); task_counter[a]++;
      if(task_counter[a]==task_counter2[a])
      {
       fclose(task_fp[a]);
       fclose(task_fp2[a]);
       task_type[a]=0;
      }
      goto NextTask;
     }
NextTask:
while(0!=0);
    }
   }
  }
 }
}

void jtm_userinterrupt()
{
 long a,b;
 //
 if(jtmtask.coverfakeOFF==0)
 {
  TM_HandleTasks();
  TM_HandleProcesses();
 }
}

void TaskMasterInitialize()
{
 long a;
 //
 jtm_shutdownflag=0;
 if(TempPointer==NULL)
 {
  TempPointer=malloc(10);
 }
 for(a=0; a<maxtasks; a++)
 {
  if(ProcessPointer[a]==NULL)
  {
   ProcessPointer[a]=malloc(10);
   ProcessMode[a]=malloc(10);
   task_arg1[a]=malloc(10);
   task_arg2[a]=malloc(10);
  }
  //
  task_type[a]=0;
  task_counter[a]=0;
  task_fp[a]=NULL;
  task_fp2[a]=NULL;
 }
 ProcessesRunning=0;
 for(a=0; a<ProcessesRunning; a++)
 {
  ProcessMode[a]=0; // 0 means not working, other values means working,
                    //                      and means the process state code.
 }
}

void jtm_SystemTimer(void)
{
 systemtimer.counterx++;
 systemtimer.counter1++;
 systemtimer.counter2++;
 systemtimer.counter3++;
 systemtimer.counter4++;
 systemtimer.counter5++;
 systemtimer.counter6++;
 systemtimer.counter7++;
 systemtimer.counter8++; // <-- Counter8 is reserved for SYSTEM only!
}

void TaskMasterON(void) // Never call this function if it's already on ..
{
 char str[255];
 //
// jtmtask.coverfakeOFF=0; // Fake mode is not resident as DEFAULT.
 jtm_addnotice("Installing task master ...\n");
// jtm_setclockrate(0);
 if(jtmtask.state==0)
 {
  TaskMasterInitialize();
  jtm_setsystemirq();
  jtmtask_fixclockrate();
  sprintf(str,"Tapio's task master 28/3/98 installed, f=%1.4x\n",0xf00); jtm_addnotice(str);
  jtm_showprocesslist();
  jtmtask.state=0xff;
 }
 else
 {
  sprintf(str,"TaskMasterON: ?? Task master already ON.\n"); jtm_addnotice(str);
 }
 systemtimer.counterx=0; // COUNTERX should NOT be changed.
 systemtimer.counter1=0;
 systemtimer.counter2=0;
 systemtimer.counter3=0;
 systemtimer.counter4=0;
 systemtimer.counter5=0;
 systemtimer.counter6=0;
 systemtimer.counter7=0;
 systemtimer.counter8=0;
 TM_AddSystemProcess(jtm_SystemTimer);
}

void TM_NewCommand(int ttype,char *arg1,char *arg2)
{
 long a;
 for(a=0; a<maxtasks; a++)
 {
  if(task_type[a]==0)
  {
   task_arg1[a]=arg1;
   task_arg2[a]=arg2;
   task_type[a]=ttype;
   goto loppu;
  }
 }
loppu:
 while(0!=0);
}

void TaskMasterOFF(void)
{
 if(jtmtask.state!=0)
 {
  jtm_releasesystemirq();
 }
}

void jtm_multitasker(void)
{
 unsigned char m1,m2,m3,m4,m5,m6,m7,m8;
 FILE *fp,*fp2,*fp3,*fp4;
 //
 if(fp!=NULL)
 {
  fputc(0x00,fp);
 }
}

int jtm_waitfornoMT() // Waits until there's no tasks left
{
 int retval=0;
 long a,key;
 //
 jtm_shutdownflag=1;
 do
 {
  for(a=0; a<maxtasks; a++)
  {
   if(task_type[a]!=0)goto TasksStillLeft;
  }
/*  for(a=0; a<ProcessesRunning; a++)
  {
   if(ProcessMode[a]!=0)goto TasksStillLeft;
  }*/
  break;
TasksStillLeft:
  key=jgetchim();
  if(key==27)
  {
   jtm_addnotice("*BREAK*\n");
   retval=1;
   break;
  }
 }while(retval==retval);
 return retval;
}


void jtm_showprocesslist()
{
 long a1;
 char ast[255],ast2[255],ast3[255],ast4[255],str[255];
 //
 sprintf(ast,"\n\rProcesses running : %u of %u max.\n",ProcessesRunning,maxprocesses);
 sprintf(ast2,"System task power : %u\n",TaskPower);
 sprintf(ast3,"Max. system tasks : %u\n",maxtasks);
 if(jtmscr.glsystem!=0)
 {
  write(ast);
  write(ast2);
  write(ast3);
 }
 else
 {
  sprintf(str,"%s%s%s",ast,ast2,ast3);
  jtm_addnotice(str);
 }
 //
 for(a1=0; a1<ProcessesRunning; a1++)
 {
  sprintf(ast4,"%1.4u %1.8lx %u\n",a1,ProcessPointer[a1],ProcessMode[a1]);
  write(ast4);
 }
}

int jtm_getscancode()
{
 // Returns: (ah) Scan code (al) ASCII code
 //
 //
 //
 __dpmi_regs regs;
 regs.x.ax=0x01;
 __dpmi_int(0x16,&regs);
 return regs.x.ax;
}

void jtm_systemkeyslooker(void)
{
 //
 char *tmp;
 long ad,ad2,ad3,ad4;

 //
 //
 if(jtmkeyb.ktab[0x19] && jtmkeyb.ktab[0x38])
 {
  if(jtm_vmode!=3)
  {
   //
   if(jtmkeyb.ktab[2])
   {
    jtm_greyscale256();
   }
   //
   if(jtmkeyb.ktab[3])
   {
    jtm_bluescale256();
   }
   //
   if(jtmkeyb.ktab[4])
   {
    jtm_lbluescale256();
   }
   //
   if(jtmkeyb.ktab[5])
   {
    jtm_redscale256();
   }
   //
   if(jtmkeyb.ktab[6])
   {
    jtm_lredscale256();
   }
   //
   if(jtmkeyb.ktab[7])
   {
    jtm_greenscale256();
   }
   //
   if(jtmkeyb.ktab[8])
   {
    jtm_lgreenscale256();
   }
  }
 }

 // Resets JTMOS if following key combinations pressed :
 //
 // 0x45 = NUM lock
 //
 // 0x1d = CTRL
 // 0x38 = LEFT ALT
 //
 // 0x2a = Left shift
 // 0x36 = Right shift
 //
 //
 //
 //
 if( jtmkeyb.ktab[0x45] )
 {
  jtmkeyb.ekeys.numlock=255-jtmkeyb.ekeys.numlock;
  jtmmouse.mousekeys=255-jtmmouse.mousekeys;
 }


 //
 // F12+SHIFT to show system meter sprite.
 // F12       to hide system meter sprite.
 //
 if(  (jtmkeyb.ktab[0x58]==0xff) && (jtmkeyb.ktab[0x2a]==0xff)
   )
 {
  jtmscr.graphictimer=0xff;
 }

 if(  (jtmkeyb.ktab[0x58]==0xff) && (jtmkeyb.ktab[0x2a]==0x00)
   )
 {
  jtmscr.graphictimer=0x00;
 }


 //
 // F11 to center mouse cursor
 //
 if(  (jtmkeyb.ktab[0x57]==0xff)
   )
 {
  jtmmouse.x=jtmscr.width/2;
  jtmmouse.y=jtmscr.height/2;
 }

 // CTRL+ALT+DEL
 // or either
 // Left shift+Right shift
 //
 if(  (jtmkeyb.ktab[0x33]==0xff &&
       jtmkeyb.ktab[0x34]==0xff)
   )
 {
  jtmmouse.arrowm=1;
 }
 else
 {
  jtmmouse.arrowm=0;
 }

 // C+T+ALT -> Force Color Text Test
 if(jtmkeyb.ktab[0x14] && jtmkeyb.ktab[0x2e] && jtmkeyb.ktab[0x38])
 {
  tmp=malloc(80*2*25);
  for(ad=0; ad<80*2*25; ad++)tmp[ad]=rand()&0xff;
  dosmemput(tmp, 80*2*25, 0xb8000);
  free(tmp);
 }

 //
 if(  (jtmkeyb.ktab[0x1d]==0xff &&
       jtmkeyb.ktab[0x38]==0xff &&
       jtmkeyb.ktab[0x53]==0xff)

       ||

      (jtmkeyb.ktab[0x36]==0xff &&
       jtmkeyb.ktab[0x2a]==0xff)
   )
 {
  // Resets system only if shutdown mode isn't yet active.
  if(jtm_shutdownmode==0)
  {
   jtm_setvmode(C80);
   textcolor(YELLOW); textbackground(BLUE);
   gotoxy(1,11);
   cprintf("                                                                              \n\r");
   cprintf("    Shutdown mode actived, press SHIFT+SHIFT+DEL to proceed emergency exit    \n\r");
   cprintf("                                                                              \n\r");
   textcolor(7); textbackground(BLACK); cprintf("\n\r");
   jtm_shutdownmode=0xff;
  }
 }
 //
 // Key combination to proceed exit request :
 //
 // Left shift+Right shift+Delete(Not backspace)
 //
 if(   (jtmkeyb.ktab[0x36]==0xff &&
        jtmkeyb.ktab[0x2a]==0xff &&
        jtmkeyb.ktab[0x53]==0xff)
   )
 {
  jtm_showprocesslist();
  jtmapp_shutdown();
  textmode(C80);
  textcolor(YELLOW); textbackground(RED);
  cprintf(" Emergency exit proceed.                                                     ");
  textcolor(7); textbackground(BLACK);
  cprintf("\n\r");
  outportb(0x20,0x20);
  exit(0xff);
 }
}

TM_SleepWhile(long longi)
{
 systemtimer.counter4=0;
 while(systemtimer.counter4<longi);
}

void jtmtask_fixclockrate(void)
{
#ifndef LEAVECLOCKRATESTANDARD
 jtm_setclockrate(JTM_DEFAULT_CLOCKRATE);
#endif
}
#endif

#ifdef __cplusplus
}
#endif

