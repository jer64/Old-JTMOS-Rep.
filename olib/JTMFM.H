/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 *      JTMFM library.
 *      (C)1998 by Jari Tapio Tuominen, all rights reserved.
 */


/*
`FA_RDONLY'
`FA_HIDDEN'
`FA_SYSTEM'
`FA_LABEL'
`FA_DIREC'
`FA_ARCH'
*/
#define JTMFM_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif
void jtmfm_updatedir(char *path)
{
 //
 struct ffblk f;
 int i,i2;

 //
 done=findfirst(path,&f,FA_ARCH|FA_DIREC|FA_RDONLY);
 i=0;
 do
 {
  if(done)break;
  //
  if( jtmfm.dir1.fname[i]==NULL )
  {
   jtmfm.dir1.fname[i]=malloc(260);
  }
  strcpy(jtmfm.dir1.fname[i],f.ff_name);
  jtmfm.dir1.attrib[i]=f.ff_attrib;
  jtmfm.dir1.size[i]=f.ff_fsize;
  //
  done=findnext(&f);
  i++;
 }while(0==0);
 jtmfm.dir1.aof=i;
}

char *jtmfm_list(char *path,int yrestpoint,int LinesToShow,int highlightline)
{
 //
 int FolderCase;
 int done;
 int x,y,i,i2,ypoint;
 char str[255],str2[255],str3[255],str4[255],fname[255];

 //
 gotoxy(1,1); y=0; ypoint=0; FolderCase=0;
 while(0==0)
 {
  if(y>LinesToShow)break;
/*  if(done)
  {
   // Echo file name
   textcolor(7);
   textbackground(6);
   gotoxy(1,wherey());
   write("      ");
   // Echo file Type
   textcolor(14);
   textbackground(5);
   gotoxy(15,wherey());
   write("                ");
   // Next line
   cprintf("\n\r");
   y++;
  }
  else*/
  if(ypoint>(jtmfm.dir1.aof-1))
  {
   strcpy(str,"");
   textcolor(7); textbackground(RED);
   jtm_stretchstringlength(str,60); strcat(str,"\n");
   write(str);
   y++;
   goto ohi1;
  }
  if(ypoint>=yrestpoint)
  {
   // Construct file type string
   strcpy(str2,"("); i2=strlen(str2);
   if( (jtmfm.dir1.attrib[ypoint]&FA_RDONLY)!=0 ){strcat(str2,"R");}else{strcat(str2,".");}
   if( (jtmfm.dir1.attrib[ypoint]&FA_HIDDEN)!=0 ){strcat(str2,"H");}else{strcat(str2,".");}
   if( (jtmfm.dir1.attrib[ypoint]&FA_SYSTEM)!=0 ){strcat(str2,"S");}else{strcat(str2,".");}
   if( (jtmfm.dir1.attrib[ypoint]&FA_ARCH  )!=0 ){strcat(str2,"A");}else{strcat(str2,".");}
   jtm_stretchstringlength(str2,4+i2);

   strcat(str2,")");
   // Construct file name string
   sprintf(str,"%s",jtmfm.dir1.fname[ypoint]);
   jtm_stretchstringlength(str,16);

   // Construct file size string(str3)
   if((jtmfm.dir1.attrib[ypoint]&FA_DIREC)!=0)
   {
    sprintf(str3,"[Folder]");
    FolderCase=1;
   }
   else
   {
    FolderCase=0;
    sprintf(str3,"%u bytes",jtmfm.dir1.size[ypoint]);
   }
   jtm_stretchstringlength(str3,16);

   // Echo file name
   if(ypoint!=highlightline)
   {
    if(!FolderCase){textcolor(7);}else{textcolor(14);}
    textbackground(RED);
   }
   else
   {
    if(!FolderCase){textcolor(7);}else{textcolor(14);}
    textbackground(BLUE);
    strcpy(fname,jtmfm.dir1.fname[ypoint]);
   }
   gotoxy(1,wherey());
   write(str2);
   // Echo file Type
   gotoxy(15,wherey());
   write(str);
   // Echo file Length
   gotoxy(15+24,wherey());
   write(str3);
   // Next line
   cprintf("\n\r");

   //
   y++;
  }
ohi1:
  ypoint++;
 }
 textcolor(7);
 textbackground(RED);
 return fname;
}

void jtmfm_open(void)
{
 jtm_setvmode(3);
 jtm_setcursortype(_NOCURSOR);
 textcolor(7); textbackground(RED);
 drawansiwindow( 4,4,80-4,25-4, 3|0x80, " File manager - (C)1998 by JTT ", 1 );
}

void jtmfm_scrollup(int *line,int *afdentries)
{
 if(line[0]>0)
 {
  line[0]--;
 }
}

void jtmfm_scrolldown(int *line,int *afdentries)
{
 if(line[0]<(afdentries[0]-13)) //-24
 {
  line[0]++;
 }
}

void jtmfm_cursorup(int *higy,int *yloc,int *afdentries)
{
   if(higy[0]!=0)
   {
    higy[0]--;
   }
   else
   {
    jtmfm_scrollup(&yloc[0],&afdentries[0]);
   }
}

void jtmfm_cursordown(int *higy,int *yloc,int *afdentries)
{
   if(higy[0]<12)
   {
    if(higy[0]<afdentries[0]-1)higy[0]++;
   }
   else
   {

    jtmfm_scrolldown(&yloc[0],&afdentries[0]);
   }
}

char *jtmfm_filemenu(char *path)
{
 //
 int higy=0,higx=0; // Highlight position.
 char m1,m2,m3,m4;
 int key,pagepower,RELOAD;
 char str[255],str2[255];
 long ad,ad2;
 char fname[255],path1[255],dirri[255];

 //
 RELOAD=0;

 //
 strcpy(path1,path);
 jtmfm.yloc=0;

reload:
 //
 pagepower=12; // How many steps you can take with pressing PGDOWN/PGUP
 jtmfm.afdentries=5000; // Amount of files in directory.
 jtmfm_open();

 // Read directory
 jtmfm_updatedir(path1);
 jtmfm.afdentries=jtmfm.dir1.aof;

 //
 do
 {
  if(jtmfm.yloc>jtmfm.afdentries)
  {
   jtmfm_cursorup(&higy,&jtmfm.yloc,&jtmfm.afdentries);
  }
 }while(jtmfm.yloc>jtmfm.afdentries);

 do
 {
  //
  strcpy(fname,jtmfm_list(path1,jtmfm.yloc,pagepower, higy+jtmfm.yloc));
  sprintf(str,"%s (%1.6x, yloc=%1.6x)   ",path1,key,jtmfm.yloc);
  write(str);

  //
  do
  {
   key=jtmkeyb_read();
  }while(key==-1);
  // On return key press.
  if(key==0x1c)
  {
   break;
  }
  if(key==0x48)
  {
   jtmfm_cursorup(&higy,&jtmfm.yloc,&jtmfm.afdentries);
  }
  if(key==0x50)
  {
   jtmfm_cursordown(&higy,&jtmfm.yloc,&jtmfm.afdentries);
  }
  if(key==0x49)
  {
   for(ad=0; ad<pagepower+1; ad++) jtmfm_cursorup(&higy,&jtmfm.yloc,&jtmfm.afdentries);
  }
  if(key==0x51)
  {
   for(ad=0; ad<pagepower+1; ad++) jtmfm_cursordown(&higy,&jtmfm.yloc,&jtmfm.afdentries);
  }
  // 'C' -> Change directory
  if(key==0x2e)
  {
   jtm_inputstr(dirri,"Path:");
   jtm_chdir(dirri);
/*   if(ifstrcontainsmark(path1,'.'))
   {
    if(!(path1[strlen(path1)-1]=='/'  ||
         path1[strlen(path1)-1]=='\\')
      )
    {
     strcat(path1,"/");
    }
    strcat(path1,"*.*");
   }*/
   RELOAD=1;
   break;
  }
  if(key==0x52) // Insert key event
  {
   jtm_mkdir();
   RELOAD=1;
   break;
  }
  if(key==0x53) // Delete key event
  {
   jtm_removeicon(fname);
   RELOAD=1;
   break;
  }
  // On ESC key press.
  if(key==1)
  {
   strcpy(fname,"[escape]");
   break;
  }

 }while(0==0);
 if(RELOAD)
 {
  RELOAD=0;
  goto reload;
 }
 return fname;
}

int jtmfm_execute(char *fname)
{
 //
 struct ffblk f;
 int i;

 //
 jtm_getfileinfo(fname,&f);
 if( (f.ff_attrib&FA_DIREC)!=0 || !strcmp(fname,".") || !strcmp(fname,"..") )
 {
  jtm_chdir(fname);
  return 0;
 }

 //
 if(strlen(fname)>3 && fname[strlen(fname)-4]=='.')
 {
  if(fname[strlen(fname)-3]=='E' &&
     fname[strlen(fname)-2]=='X' &&
     fname[strlen(fname)-1]=='E'
    )
  {
   system(fname);
  }
 }
 else
 {
 }
 return 0;
}
#ifdef __cplusplus
}
#endif

