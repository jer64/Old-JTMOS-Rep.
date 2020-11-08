/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/* SMALL UNIQUE EXECUTOR 11/97
 *
 * Executing routine, okay this routine calls different routines,
 * commanded by [.uni] formatted file
 *
 * Commands available at currect version:
 *
 * 1.Print 
 * 2.Input
 * 3.Beep
 * 4.End
 * 5.Insdrv     (f.e. insdrv population.drv 0)
 * 6.Driver     (driver 0 0x00 0x00 0x00 0x00)
 *                     fnc ax   bx   cx   dx
 * 7.GraphOn
 * 8.GraphOff
 * 9.waitkey
 *10.amenu: test.mnu
 *11.sys: dir
 *12.mode: [egatxt/vga16/mode256]
 *
 */
long          far u_stack[32]; // Space for 200 LOOP's
unsigned      far u_stackp;     // 16bit UStack pointer
unsigned      far windowhandle[32];
unsigned      far buttonhandle[32];

void uni_print(FILE *_fp)
{
   seektomark('"',_fp);
   do
   {
nexto:
    m1=fgetc(_fp);
    if(m1=='\\') /* Mark "\" allows special marks entering in text,
                    for example '\"' makes possible to enter '"' mark in
                    text, however it is very similiar to C-language(STDIO.H).*/
    {
     m1=fgetc(_fp);
     if(m1=='n')
     {
      printf("\n");
      goto nexto;
     }
     if(m1=='r')
     {
      printf("\r");
      goto nexto;
     }
     printf("%c",m1);
     goto nexto;
    }
    if(m1=='"')break;
    printf("%c",m1);
   }while(m1!='"');
   m1=fgetc(fp); if(m1!=';'){printf("\n"); goto nextone;}
   seektoeol(_fp);
nextone:
}

void uni_printNOENTER(FILE *_fp) // PRINTF printing, without color changes!
{
   seektomark('"',_fp);
   do
   {
    m1=fgetc(_fp); if(m1=='"')break;
    cprintf("%c",m1);
   }while(m1!='"');
   m1=fgetc(fp); if(m1!=';'){cprintf("\r\n"); goto nextone;}
   seektoeol(_fp);
nextone:
}

void uni_goto(FILE *_fp,unsigned char *_label)
{
 unsigned char _labeli[100];
 //
 fseek(_fp,0,SEEK_SET);
 strcpy(_labeli,_label);
 x=strlen(_label);
 _labeli[x]=  ':';
 _labeli[x+1]=0;
 do
 {
  fscanf(fp,"%s",&str);
  if(!stricmp(str,_labeli))
  {
   break;
  }
 }while(!feof(_fp));
}

int uniexecute(char *_fname) // << UNIQUE MAIN ! >>
{
 /* Huom!:      FP on ohjelman pointteri, ja
  *             komennot k„ytt„„ FP4:ta, tai FP3:ta (IN,OUT)*/
 FILE *_fp;
 char _cmd[128],_cmd2[128],_inputstr[256],curvar[255];
 unsigned _dax,_dbx,_dcx,_ddx; // Driver call time registers
 unsigned _reg1,_reg2,_reg3,_reg4; // Programming registers for Unique
 unsigned uad,uad2,uad3,uad4; // Unsigned address:es, variables
 unsigned char teksti[200];
 unsigned _par1,_par2,_par3,_par4,_par5,_par6,uni_fontcolor=14;
 long _ad,_ad2;
 //
 //
 fillbefore=0; // Do not fill windows empty before writing (1=FALSE)
 graphreset(SYSTEMON);
 textmode(C80); JtmGraphMode=0x03;
 sprintf(teksti,"UniExecuter V%1.2f  (C)1997 by Jari Tuominen",uni_version);
 jtm_headline(teksti);
 _fp=fopen(_fname,"rb");
 if(_fp==NULL)
 {
  jtm_messagebox(" Unique message ","Can't find requested source file");
  return -1;
 }
 do
 {
nextone:
  if(feof(fp))break;
  fscanf(_fp,"%s",&_cmd); // Read next command
  if(!stricmp(_cmd,"assign")) // Command includer! (NEW)
  {
   fscanf(fp,"%s",&str);

  }
  if(!stricmp(_cmd,"extractpcx:"))
  {
   fscanf(_fp,"%s",&_cmd);
   fscanf(_fp,"%s",&_cmd2);
   fscanf(_fp,"%u",&_par1);
   decode_pcx(_cmd,_cmd2,_par1);
   goto nextone;
  }
  if(!stricmp(_cmd,"viewraw:"))
  {
   fscanf(_fp,"%s",&_cmd);
    fp4=fopen(_cmd,"rb");
    uad=0;
    do
    {
     m1=fgetc(fp4);
     asm
     {
      push es
      push 0xa000
      pop es
      mov di,word ptr uad
      mov al,byte ptr m1
      mov byte ptr [es:di],al
      pop es
     }
     uad++; if(uad==0x00)break;
    }while(!feof(fp4));
    fclose(fp4);
   goto nextone;
  }
  if(!stricmp(_cmd,"updatedesktop"))
  {
   updatedesktop();
   goto nextone;
  }
  if(!stricmp(_cmd,"viewpcx:"))
  { // Okay, 320x??? + 256 colors expected
   if(jtm_vmode==0x13)
   {
    fscanf(_fp,"%s",&_cmd);
    decode_pcx(_cmd,"swapfile.$$$",1);
    fp4=fopen("swapfile.$$$","rb");
    uad=0;
    do
    {
     m1=fgetc(fp4);
     asm
     {
      push es
      push 0xa000
      pop es
      mov di,word ptr uad
      mov al,byte ptr m1
      mov byte ptr [es:di],al
      pop es
     }
     uad++; if(uad==0x00)break;
    }while(!feof(fp4));
    fclose(fp4);
   }   
   goto nextone;
  }
  if(!stricmp(_cmd,"rem")) // For example: rem "Just testing .."
  {
   seektomark('"',_fp); 
   seektomark('"',_fp);
  }
  if(!stricmp(_cmd,"sleep"))
  {
   fscanf(_fp,"%u",&_ad);
   sleep(ad);
   goto nextone;
  }
  if(!stricmp(_cmd,"delay"))
  {
   fscanf(_fp,"%u",&_ad);
   delay(ad);
   goto nextone;
  }
  //
  // => mode:   mcga256
  //            egatxt
  //            vga16
  

  if(!stricmp(_cmd,"mode:"))
  {
   fscanf(_fp,"%s",&_cmd);
   if(!stricmp(_cmd,"egatxt"))
   {
    jtm_vmode=0x03;
    asm
    {
     mov ax,0x03
     int 0x10
    }
   }
   if(!stricmp(_cmd,"vga16"))
   {
    jtm_vmode=0x12;
    asm
    {
     mov ax,0x12
     int 0x10
    }
   }
   if(!stricmp(_cmd,"mcga256"))
   {
    jtm_vmode=0x13;
    asm
    {
     mov ax,0x13
     int 0x10
    }
    goto nextone;
   }
   if(_cmd[0]=='0' && _cmd[1]=='x') // C stylish hex value catch
   {
    _cmd[0]==' ';
    _cmd[1]==' ';
    sscanf(_cmd,"%u",&_par4);
    asm
    {
     mov ah,0x00
     mov al,byte ptr _par4
     int 0x10
    }
   }
   goto nextone;
  }
  if(!stricmp(_cmd,"cls"))
  {
   if(jtm_vmode==0x03)
   {
    clrscr();
   }
   goto nextone;
  }
  if(!stricmp(_cmd,"message")) // Ansi color printing
  {
   textcolor      (jtm_messagefcolor);
   textbackground (jtm_messagebcolor);
   uni_printNOENTER(_fp);
   textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
   cprintf("\n\r");
  }
  if(!stricmp(_cmd,"headline")) // Ansi color printing
  {
   textcolor      (jtm_headlinefcolor);
   textbackground (jtm_headlinebcolor);
   uni_printNOENTER(_fp);
   textcolor(jtm_enterfcolor); textbackground(jtm_enterbcolor);
   cprintf("\n\r");
  }
  // DO/LOOP operation
  if(!stricmp(_cmd,"do")) // DO, pushes currect file position at "Do stack"
  {
   u_stack[u_stackp]=ftell(_fp);
   u_stackp++;
  }
  if(!stricmp(_cmd,"loop")) // loop, pulls last position from "Do stack"
  {
   u_stackp--;
   fseek(_fp,u_stack[u_stackp],SEEK_SET);
   goto nextone; 
  }              
  //
  if(!stricmp(_cmd,"print")) // For graphics resolution(s)
  {
   uni_print(_fp);
  }
  if(!stricmp(_cmd,"input"))
  {
   uni_print(_fp);
   strcpy(_inputstr,jinputstr());
  }
  if(!stricmp(_cmd,"screen_fadeout"))
  {
   jtm_scrfadeout();
   goto nextone;
  }
  if(!stricmp(_cmd,"screen_fadein"))
  {
   jtm_scrfadein();
   goto nextone;
  }
  if(!stricmp(_cmd,"beep"))
  {                            
   nicebeep();
  }
  if(!stricmp(_cmd,"insdrv"))
  {
   nextdrv++;
   fscanf(_fp,"%u",&ad);
   
  }
  if(!stricmp(_cmd,"end"))
  {
   break;
  }
  if(!stricmp(_cmd,"driver"))
  {
   fscanf(_fp,"%u",&ad);  
   fscanf(_fp,"%x",&ad2);  
   fscanf(_fp,"%x",&ad3);  
   fscanf(_fp,"%x",&ad4);  
   fscanf(_fp,"%x",&ad5);
   if(drivers[ad]==NULL)
   {
    unierror(0);
   }
   break;
  }
  if(!stricmp(_cmd,"mousecursor:"))
  {
   fscanf(_fp,"%s",&_cmd);
   if(!stricmp(_cmd,"on"))
   {
    mouse_cursor_on();
   }
   if(!stricmp(_cmd,"off"))
   {
    mouse_cursor_on();
   }
   goto nextone;
  }
  if(!stricmp(_cmd,"mousesystem")) // For example: mousesystem
                                   // actives the windowing system,
                                   // windows are really living, (=
  {
   mousesystem(); 
  }
  if(!stricmp(_cmd,"clearwindow")) // For example: clearwindow 1
  {
   fscanf(_fp,"%u",&_par1);
   jclearwind(_par1);
  }

  if(!stricmp(_cmd,"del")) // Usage: delete "[filename]"
  {
   uni_getstri(_fp);        
   jdelete(str);
   goto nextone;
  }
  if(!stricmp(_cmd,"ren")) // Usage: ren "[filename1]" "[filename2]"
  {
   uni_getstri(_fp);
   strcpy(str2,str);
   uni_getstri(_fp);        
   jrename(str2,str);  
   goto nextone;
  }

  if(!stricmp(_cmd,"drawbutton")) // For example: drawbutton 1 160 100 4 0
                                  //                       (ID)(X) (Y)(T)(?)
  {
   fscanf(_fp,"%u",&_par5); // Button handle
   fscanf(_fp,"%u",&_par1); // X position       
   fscanf(_fp,"%u",&_par2); // Y position
   fscanf(_fp,"%u",&_par3); // Size
   fscanf(_fp,"%u",&_par4); // Type
   uni_getstri(_fp);        // Get headline
   buttonhandle[_par5]=jdrawbutton(_par1,_par2,_par3,_par4,str);
  }

  if(!stricmp(_cmd,"copy"))
  {
   fscanf(_fp,"%s",&str);
   fscanf(_fp,"%s",&str2);
   xcopy(str,str2);
  }
  if(!stricmp(_cmd,"updatesystemblock"))
  {
   updatesystemblock();
  }
  if(!stricmp(_cmd,"record"))
  {
   
  }

  if(!stricmp(_cmd,"newvar")) // For example: newvar abc
  {                           //This function isn't very usable, but
   fscanf(_fp,"%s",&str);      //in future it may have some kind of use
   newvar(str);
   goto nextone;
  }

  if(!stricmp(_cmd,"$")) // For example: getvar abc
  {
   fscanf(_fp,"%s",&curvar);
   goto nextone;
  }

  if(!stricmp(_cmd,"add")) //   For example: add abc 123
  {
   fscanf(_fp,"%s",&ast);
   fscanf(_fp,"%u",&ad);
   sscanf(getvar(ast),"%u",&ad2);
   ad+=ad2;
   sprintf(ast2,"%u",ad);
   setvar(ast,ast2);
   goto nextone;   
  }

  if(!stricmp(_cmd,"dim")) //   For example: dim abc 123
  {
   fscanf(_fp,"%s",&str);
   fscanf(_fp,"%s",&str2);
   setvar(str,str2);
   goto nextone;
  }

  if(!stricmp(_cmd,"bar"))
  {
   fscanf(_fp,"%u",&_par1);
   fscanf(_fp,"%u",&_par2);
   fscanf(_fp,"%u",&_par3);
   fscanf(_fp,"%u",&_par4);
   fscanf(_fp,"%u",&_par5);
//   setcolor(_par5);
   fillarea(_par1,_par2,_par3,_par4,_par5);
   goto nextone;
  }

  if(!stricmp(_cmd,"rectangle"))
  {
   fscanf(_fp,"%u",&_par1);
   fscanf(_fp,"%u",&_par2);
   fscanf(_fp,"%u",&_par3);
   fscanf(_fp,"%u",&_par4);
   fscanf(_fp,"%u",&_par5);
   kehys(_par1,_par2,_par3,_par4,_par5);
   goto nextone;
  }
  if(!stricmp(_cmd,"textcolor"))
  {
   fscanf(_fp,"%u",&_par1);
   uni_fontcolor=_par1;
   goto nextone;
  }

  if(!stricmp(_cmd,"locate"))  // For example: locate 010 010
  {
   fscanf(_fp,"%u",&_par1);
   fscanf(_fp,"%u",&_par2);
   writex=_par1;
   writey=_par2;
   goto nextone;
  }

  if(!stricmp(_cmd,"writein")) // For example: writein 1
                               // defines that window #1 is the writing place
  {
   fscanf(_fp,"%u",&_par1);
   writex= jxw_x1[windowhandle[_par1]]+2;   
   writexs=jxw_x1[windowhandle[_par1]]+2;
   writey= jxw_y1[windowhandle[_par1]]+19;
   writeys=jxw_y1[windowhandle[_par1]]+19;
  }
  if(!stricmp(_cmd,"writeinbutton")) // For example: writeinbutton 1
                               // defines that button #1 is the writing place
  {
   fscanf(_fp,"%u",&_par1);
   writex= jxb_x1[buttonhandle[_par1]]+2;
   writexs=jxb_x1[buttonhandle[_par1]]+2;
   writey= jxb_y1[buttonhandle[_par1]]+2;
   writeys=jxb_y1[buttonhandle[_par1]]+2;
   goto nextone;
  }          
  if(!stricmp(_cmd,"winimage")) // For example: winimage 1 320 "jtmos.raw"
                                //                    (WIN#) (Width) (Name)
  {
   fscanf(_fp,"%u",&_par1); // Get window id
   fscanf(_fp,"%u",&_par2); // Get image width
   fscanf(_fp,"%s",&str);   // Get image fname
   jtm_winimage(windowhandle[_par1],_par2,str); 
  }
  if(!stricmp(_cmd,"winpaint")) // For example: winpaint 1 14             
                                //                   (WIN#) (Color#)
  {
   fscanf(_fp,"%u",&_par1); // Get window id
   fscanf(_fp,"%u",&_par2); // Get image width
   jtm_winpaint(_par1,_par2); 
  }
  if(!stricmp(_cmd,"wideimage")) // For example: wideimage "jtmos.raw"
  {
   uni_getstri(_fp);
   jtm_show640wideraw(str);
  }
  if(!stricmp(_cmd,"write")) // For example: write "teksti"
  {
   uni_getstr(_fp);
   fontcolor=uni_fontcolor;
   write(str);
   goto nextone;
  }
  if(!stricmp(_cmd,"openwindow"))
  {
   fscanf(_fp,"%u",&_par5); // _par5=Window handle (windowhandle[handle])
   fscanf(_fp,"%u",&_par1); // X1
   fscanf(_fp,"%u",&_par2); // Y1
   fscanf(_fp,"%u",&_par3); // X2
   fscanf(_fp,"%u",&_par4); // Y2
   seektomark('"',_fp); x=0;
   do
   {
    m1=fgetc(_fp); if(m1=='"')break;
    str[x]=m1; x++;
   }while(m1!='"');
   m1=fgetc(_fp); if(m1!=';'){goto seuraava;}
//   seektoeol(_fp);
seuraava:
   str[x]=0;
   fscanf(_fp,"%u",&_par6); // Icon
   windowhandle[_par5]=jopenwind(_par1,_par2,_par3,_par4,str,_par6);
  }
  if(!stricmp(_cmd,"flood"))
  {
   fscanf(_fp,"%u",&m1);
   if(JtmGraphMode==0x12)
   {
    flood(m1);
   }
   if(JtmGraphMode==0x13)
   {
    for(uad4=0; uad4<0xffe0; uad4+=32)
    {
     asm
     {
      push es
      mov ax,0xa000
      mov es,ax
      mov al,byte ptr m1
      mov ah,al
      mov di,word ptr uad4
      mov word ptr [es:di+0x00],ax
      mov word ptr [es:di+0x02],ax
      mov word ptr [es:di+0x04],ax
      mov word ptr [es:di+0x06],ax
      mov word ptr [es:di+0x08],ax
      mov word ptr [es:di+0x0a],ax
      mov word ptr [es:di+0x0c],ax
      mov word ptr [es:di+0x0e],ax
      mov word ptr [es:di+0x10],ax
      mov word ptr [es:di+0x12],ax
      mov word ptr [es:di+0x14],ax
      mov word ptr [es:di+0x16],ax
      mov word ptr [es:di+0x18],ax
      mov word ptr [es:di+0x1a],ax
      mov word ptr [es:di+0x1c],ax
      mov word ptr [es:di+0x1e],ax
      pop es
     }
    }
   }
  }
  if(!stricmp(_cmd,"goto"))
  {
   fscanf(_fp,"%s",&str);
   uni_goto(_fp,str);
  }
  if(!stricmp(_cmd,"GraphOn"))
  {
   JtmGraphMode=0x12;
   graphreset(SYSTEMON);
  }
  if(!stricmp(_cmd,"GraphOff"))
  {
   JtmGraphMode=0x03;
   graphreset(SYSTEMOFF);
  }
  if(!stricmp(_cmd,"getch"))
  {
   getch();
  }
  if(!stricmp(_cmd,"waitkey"))   
  {
   jtm_waitkey();
   goto nextone;
  }
  if(!stricmp(_cmd,"sys:"))
  {
   uni_getstr(_fp);
   system(str);
   goto nextone;
  }
  if(!stricmp(_cmd,"amenu:"))
  {
   fscanf(_fp,"%s",&str);
   _reg1=amenusys(str,0);
  }
  if(!stricmp(_cmd,"palette:"))
  {
   fscanf(_fp,"%s",&str);
   if(!stricmp(str,"greyscale256"))
   {
    jtm_greyscale256();
   }
   if(!stricmp(str,"bluescale256"))
   {
    jtm_bluescale256();
   }
   goto nextone;
  }
 }while(!feof(_fp));
 fclose(_fp);
}

