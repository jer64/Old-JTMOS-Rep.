/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * JTMOS / ADOS command line executer & handler
 * (Indepent library, can be included within other libraries)
 *
 * Features:
 *      þ Command executer v1.10, 1997 model.
 *      þ ADOS batch files executer
 *
 * Returns integer:
 *
 * 0   Means no error report
 * -1  Close shell immediatly
 * -2  Syntax error
 *
 *
 */
 char far _messy[255];
 char far _messy2[255];
 char far _messy3[255];
 int _c;
 //
int jados_execute(char far *_adoscmd)
{
 mouse_cursor_off(); delay(moff_delay);
  if(!stricmp(_adoscmd,"edit"))
  {
   jsystem(cfg_doseditor);
   goto nextcom;
  }
  if(!stricmp(_adoscmd,"free"))
  {
   sprintf(ast,"free: %u bytes",jgetdiskfree());
   writetowin(system_window,ast,system_window_line);
   goto nextcom;
  }
  if(!stricmp(_adoscmd,"time"))
  {
   sprintf(ast, "Currect time is %u:%u:%u",jtm_timehour(),jtm_timemin(),jtm_timesec());
   writetowin(system_window,ast,system_window_line);
   goto nextcom;
  }
  if(!stricmp(_adoscmd,"date"))
  {
   sprintf(ast, "Currect date is %u.%u.%u",jtm_dateday(),jtm_datemonth(),jtm_dateyear());
   writetowin(system_window,ast,system_window_line);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"tasks"))
  {
   writetowin(system_window,"List of tasks",system_window_line);   
   for(_c=0; _c<tasks; _c++)
   {
    strcpy( _messy, "#");
    sprintf(_messy2,"%d",_c); strcat(_messy2,":  ");
    strcat(_messy,_messy2);
    sprintf(_messy2,"%d",task_tpe[_c]); strcat(_messy2,"(Job), P(1)(2):");
    strcat(_messy,_messy2); 
    strcat(_messy,task_pr1[_c]); 
    strcat(_messy,task_pr2[_c]); 
    writetowin(user_window,_messy,user_window_line);   
   }
   goto nextcom;
  }

  if(!stricmp(adoscom0,"task"))
  {
   writetowin(system_window,"Multitasking power changed",system_window_line);
   sscanf(adoscom1,"%d",&multitaskpower);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"cd"))
  {
   chdir(adoscom1);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"dir"))
  {
   writetowin(system_window,"Loading directory",system_window_line);
   strcpy(ast,_adoscmd);
   x=0; y=0;
   do
   {
    if(ast[x]==' '){x++; y=x+1;}
    x++;
   }while(x!=strlen(ast));
   if(y==0)strcpy(ast,"*.*");
   else
   {
//    for(x=y, x2=0; x<strlen(ast); x2++, x++)ast2[x2]=ast[x]; ast2[x2]=0;
//    strcpy(ast,ast2);
   }
   done = findfirst( ast, &ados_dirblock, FILENAME|DIRECTORY);

   dirline=0;
   do
   {
    dircolorset=0;
    textcolor(LIGHTGREEN);
    strcpy(ast,ados_dirblock.ff_name);
    if(strlen(ast)<20)
    {
     for(x=strlen(ast); x<20; x++)ast[x]=' '; ast[x]=0;
    }

    if(ados_dirblock.ff_fsize<1024)
    {
     if(ados_dirblock.ff_fsize==0)
     {
      sprintf(str," -> EMPTY");
      goto setdone;
     }
     sprintf(str," -> Size: %u bytes",ados_dirblock.ff_fsize);
     goto setdone;
    }
    else
    {
     sprintf(str," -> Size: %uKB(s)",ados_dirblock.ff_fsize/1024);
     goto setdone;
    }
setdone:

    strcat(ast,str);
    
     for(x=strlen(ast); x<37; x++)ast[x]=' '; ast[x]=0;

    if((ados_dirblock.ff_attrib&FA_ARCH)!=0x00)
    {
     textcolor(GREEN);
     strcat(ast,"(A)");
    }
    if((ados_dirblock.ff_attrib&FA_RDONLY)!=0x00)
    {
     textcolor(LIGHTRED);
     strcat(ast,"(R)");
    }
    if((ados_dirblock.ff_attrib&FA_HIDDEN)!=0x00)
    {
     textcolor(CYAN);
     strcat(ast,"(H)");
    }
    if((ados_dirblock.ff_attrib&FA_SYSTEM)!=0x00)
    {
     textcolor(RED);
     strcat(ast,"(S)");
    }
    if((ados_dirblock.ff_attrib&FA_DIREC)!=0x00)
    {
     textcolor(LIGHTBLUE);
     strcat(ast,"(Directory)");
    }
    if((ados_dirblock.ff_attrib&FA_LABEL)!=0x00)
    {
     textcolor(YELLOW);
     strcat(ast,"(Label)");
    }
    writetowin(user_window,ast,user_window_line);
    done = findnext(&ados_dirblock);
    if(done!=0)break;
    dirline++; if(dirline==20){dirline=0; jgetch();}
   }while(ad==ad);
   writetowin(user_window,"\r",user_window_line);
   goto nextcom;
  }


  if(!stricmp(adoscom0,"/"))
  {
   if(adoscom1[0]>'z' || adoscom1[0]<'a')
   {
    textcolor(RED);
    writetowin(system_window,"Invalid drive letter, or capital letter used.",system_window_line);
    textcolor(7);
    goto nextcom;
   }
   ados_actdisk=getdisk();

   sprintf(ast,"%c:",adoscom1[0]);

   ados_cdrive=convdletter(adoscom1[0]);
   if( chkdiskready(ados_cdrive)==0 )
                        // Notice: 0x00=drive a:, 0x80 = first hd
   {
    setdisk( convdletterback(ados_cdrive) );
    writetowin(user_window,ast,user_window_line);
   }
   else
   {

    writetowin(user_window,"?Cannot access that drive",user_window_line);
    setdisk(ados_actdisk);
   }
   goto nextcom;
  }

  if(!stricmp(adoscom0,"catch"))
  {
   writetowin(system_window,"Catching files",system_window_line);
   done = findfirst( adoscom1, &ados_dirblock, FILENAME|DIRECTORY);

   do
   {
    if(
       !(ados_dirblock.ff_attrib&FA_DIREC!=0  &&
         ados_dirblock.ff_attrib&FA_LABEL!=0)
      )
    {
     strcpy(str,adoscom0);
     strcat(str,ados_dirblock.ff_name); 
     strcpy(mt_src,str);
     strcpy(mt_out,ados_dirblock.ff_name);
     mt_fcopy(".","..");
     strcpy(str,"Copying ");
     strcat(str,mt_src);
     strcat(str," to ");
     strcat(str,mt_out);
     cprintf("%s\r\n",str);
    }
    if(done!=0)goto nextcom;
    done = findnext(&ados_dirblock);
   }while(ad==ad);

   goto nextcom;
  }

  if(!stricmp(adoscom0,"setdate"))
  {
   cprintf("Currect date is %d.%d.%d\n\r",j_getdate_day(),j_getdate_month(),j_getdate_year());

   cprintf("new day:");
   strcpy(dump,jinputstr());
   sscanf(dump,"%d",&ad);

   cprintf("new month:");
   strcpy(dump,jinputstr());
   sscanf(dump,"%d",&ad2);

   cprintf("new year:");
   strcpy(dump,jinputstr());
   sscanf(dump,"%d",&ad3);

   j_setdate_day(ad);
   j_setdate_month(ad2);
   j_setdate_year(ad3);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"copy"))
  {
   strcpy(mt_src,adoscom1);
   strcpy(mt_out,adoscom2);
   strcpy(_messy,"Copying ");
   strcat(_messy,adoscom1);
   strcat(_messy," to ");
   strcat(_messy,adoscom2);
   writetowin(system_window,_messy,system_window_line);
   mt_fcopy(adoscom1,adoscom2);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"cls"))
  {
   for(ad=0; ad<50; ad++)
   {
    writetowin(user_window,"\r",user_window_line);
   }
   goto nextcom;
  }

  if(!stricmp(adoscom0,"clsa"))
  {
   for(ad=0; ad<50; ad++)
   {
    writetowin(user_window,"\r",user_window_line);
    writetowin(system_window,"\r",system_window_line);
    writetowin(driver_window,"\r",driver_window_line);
   }
   goto nextcom;
  }

  if(!stricmp(adoscom0,"ver"))
  {
   textcolor(14);
   writetowin(system_window,"  ADOS 6/97",system_window_line);
   writetowin(system_window,"  80386 6/97 update, (C)1996,1997 by",system_window_line);
   writetowin(system_window,"  Jari Tuominen, all rights reserved.",system_window_line);
   textcolor(WHITE);
   strcpy(dump,NULL);
   goto nextcom;
  }

  if(!stricmp(adoscom0,"shutdown"))
  {
   jsystem("shutdown");
   goto nextcom;
  }

  if(!stricmp(adoscom0,"dos?"))
  {
   jgetscrn(3);
   textmode(PRVMODE);
   jputscrn(64);
   jgetch();
   textmode(ADOSMODE);
   lwindow(0);
   jputscrn(3);
   clrscr();
   goto nextcom;
  }

  if(adoscom0[strlen(adoscom0)-1]=='!' )
  {
   for(x=0; x<strlen(_adoscmd); x++)
   {
    if(_adoscmd[x]=='!')
    {
     _adoscmd[x]=' ';
     break;
    }
   }
 //  adoscom0[strlen(adoscom0)-1]=0;
//   strcpy(ast,adoscom0); strcat(ast," ");
//   strcat(ast,adoscom1); strcat(ast," ");
//   strcat(ast,adoscom2); strcat(ast," ");
//   strcat(ast,adoscom3);
   jsystem(_adoscmd);
   goto nextcom;
  }

  if(adoscom0[strlen(adoscom0)-1]=='\\' )
  {
   adoscom0[strlen(adoscom0)-1]=0;
   chdir(adoscom0);
   goto nextcom;
  }
  
  if( (fp=fopen(getlink("c:\\links\\adosfil.lin"),"rb"))==NULL)
  {
   jtmerror(6,_dontexit);
  }
  do
  {
   fscanf(fp,"%s",&ast);
   fscanf(fp,"%s",&ast2);
   if(ast[strlen(ast)-1]=='#')
   {
    strcpy(str,ast);
    str[strlen(str)-1]=0;
    if(!stricmp(str,adoscom0))
    {
     jsystem(ast2);
     goto nextcom;
    }
   }
   if(adoscom0[strlen(adoscom0)-3]==ast[0] &&adoscom0[strlen(adoscom0)-2]==ast[1] &&str[strlen(adoscom0)-1]==ast[2])
   {
    strcpy(str2,adoscom0);
    strcpy(str3,ast2);
    strcat(str3,str2);
    writetowin(system_window,"Connecting . . .",user_window_line);
//    cprintf("Connecting:      %s\n\r",str3);
    jsystem(str3);
    fclose(fp);
    goto nextcom;
   }
  }while(stricmp(ast,".end"));
  fclose(fp);

  if(!stricmp(adoscom0,"leave"))
  {
   close_ados();  
   clearscreen(); clrscr();
   textmode(PRVMODE);
   textcolor(7); textbackground(BLACK);
   jwindow(1,1,80,25,1); 
   mouse_cursor_off();
   goto closeshell;
  }
  mouse_cursor_on();
  return -2;
closeshell:
  mouse_cursor_on();
  return -1;
nextcom:
 mouse_cursor_on();
 return 0;
}

void execute_efl(char *_fname)
{
 FILE *_fp;
 _fp=fopen(_fname,"rb");
 if(_fp==NULL)
 {
  textmode(ADOSMODE);
  cprintf("%s file not found, optimize ADOS.CFG !\r\n",_fname);
  nicebeep();
  exit(1);
 }
 do{
  fscanf(_fp,"%s",&str); if(!stricmp(str,".end"))break;
  for(x=0; x<strlen(str); x++)if(str[x]=='_')str[x]=' ';
  jados_execute(str);
 }while(stricmp(str,".end") && !feof(_fp));
 fclose(_fp);
}


