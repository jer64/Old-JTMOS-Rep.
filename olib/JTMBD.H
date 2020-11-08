/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*** BINDISK ***/
typedef struct
{
 long sectors;
 long secsiz;
 char *b;
 long *bam;
}BINDISK;

bd_savedisk(BINDISK *d,char *fname)
{
 //
 long i,i2;
 char *tmp;
 FILE *f;

 //
 tmp=calloc(1,d[0].secsiz);

 //
 f=fopen(fname,"wb");

 //
 fputd(d[0].sectors,f);                 // Sectors
 fputd(d[0].secsiz,f);                  // Sector size
 fputd(d[0].secsiz*d[0].sectors,f);     // Total diskette size

 //
 for(i=0; i<d[0].sectors; i++)
 {
  fputd(d[0].bam[i],f);
 }

 //
 for(i=0; i<d[0].sectors; i++)
 {
  bd_readsector(i,tmp,d);
  for(i2=0; i2<d[0].secsiz; i2++)fputc(tmp[i2],f);
 }

 //
 fclose(f);

 //
 cfree(tmp);
}

bd_loaddisk(char *fname,BINDISK *d)
{
 //
 long i,i2;
 char *tmp;
 FILE *f;

 //
 tmp=calloc(1,d[0].secsiz);

 //
 f=fopen(fname,"rb");

 //
 d[0].sectors=fgetd(f);                 // Sectors
 d[0].secsiz=fgetd(f);                  // Sector size
 fgetd(f);

 //
 d[0].b=calloc(1,d[0].sectors*d[0].secsiz);

 //
 d[0].bam=calloc(1,d[0].sectors*4);

 //
 for(i=0; i<d[0].sectors; i++)
 {
  d[0].bam[i]=fgetd(f);
 }

 //
 for(i=0; i<d[0].sectors; i++)
 {
  for(i2=0; i2<d[0].secsiz; i2++)tmp[i2]=fgetc(f);
  bd_writesector(i,tmp,d);
 }

 //
 fclose(f);

 //
 cfree(tmp);
}

bd_readsector(long sector,char *buf,BINDISK *d)
{
 //
 long i,i2,i3,i4,offs;

 //
 offs=d[0].secsiz*sector;

 // Copy data from buffer to BINDISK(OFFSET:SECTOR)
 for(i=offs+0,i2=0; i<offs+d[0].secsiz; i++,i2++)
 {
  buf[i2]=d[0].b[i];
 }
}

bd_writesector(long sector,char *buf,BINDISK *d)
{
 //
 long i,i2,i3,i4,offs;

 //
 offs=d[0].secsiz*sector;

 // Copy data from buffer to BINDISK(OFFSET:SECTOR)
 for(i=offs+0,i2=0; i<offs+d[0].secsiz; i++,i2++)
 {
  d[0].b[i]=buf[i2];
 }
}

bd_writesectors(BINDISK *d,char *buf,long *ordtab,long sectors)
{
 //
 long i;

 //
 for(i=0; i<sectors; i++)
 {
  bd_writesector(ordtab[i],buf+(i*d[0].secsiz),d);
 }
}

//
// Returns sector number
long bd_sectoralloc(BINDISK *d,long sectors,long *ordtab)
{
 //
 long i,i2,i3,i4,i5,i6;

 //
 for(i=0,i2=0,i3=0,i4=0,i5=0; i<d[0].sectors; i++)
 {
  if(!d[0].bam[i])
  {
   if(!i4){i4=1; i5=i;}
   if(i3)d[0].bam[(i3-1)]=i+1;
   ordtab[i2]=i;
   i3=i+1;
   i2++;
   if(i2>=sectors)
   {
    d[0].bam[i]=0xffffffff; // Means 'end sector'.
    // Enough sectors found and successfully allocated.(RETVAL=0)
    return i5;
   }
  }
 }
 // Not enough free sectors to complete allocation.(RETVAL=1)
 return -1;
}

// Version without ORDTAB.
//
// Returns sector number
long bd_sectorallok(BINDISK *d,long sectors)
{
 //
 long i,i2,i3,i4,i5,i6;

 //
 for(i=0,i2=0,i3=0,i4=0,i5=0; i<d[0].sectors; i++)
 {
  if(!d[0].bam[i])
  {
   if(!i4){i4=1; i5=i;}
   if(i3)d[0].bam[(i3-1)]=i+1;
   i3=i+1;
   i2++;
   if(i2>=sectors)
   {
    d[0].bam[i]=0xffffffff; // Means 'end sector'.
    // Enough sectors found and successfully allocated.(RETVAL=0)
    return i5;
   }
  }
 }
 // Not enough free sectors to complete allocation.(RETVAL=1)
 return -1;
}

int bd_sectorfree(BINDISK *d,long ssec)
{
 //
 long i,i2,i3,i4;

 //
 i=ssec;
 while(!0)
 {
  //
  if(!d[0].bam[i])return -1;
  if(d[0].bam[i]==0xffffffff)return 0;
  i2=d[0].bam[i];
  d[0].bam[i]=0;
  i=i2;
 }
}

bd_writedata(BINDISK *d,char *sbuf,long *ordtab,long length)
{
 //
 char *tmp;
 long i,i2,i3,i4,truesize;

 //
 truesize=d[0].secsiz*((length/d[0].secsiz)+1);
 tmp=calloc(1,truesize);

 //
 for(i=0; i<truesize; i++)tmp[i]=0;
 for(i=0; i<length; i++)tmp[i]=sbuf[i];

 //
 bd_writesectors(d,tmp,ordtab,truesize/d[0].secsiz);

 //
 cfree(tmp);
}

bd_makefs(BINDISK *d)
{
 //
 long ordtab[100];
 char *tmp;

 //
 tmp=calloc(1,256);

 //
 tmp[0]=0;

 // Allocate space for first directory entries and files.
 bd_sectoralloc(d,10,ordtab);

 //
 bd_writedata(d,tmp,ordtab,256);

 //
 cfree(tmp);
}

bd_newdisk(BINDISK *d,long secsiz,long secamo)
{
 //
 long i;

 // CREATE 1024K BINDISK
 d[0].sectors=secamo;
 d[0].secsiz=secsiz;

 //
 d[0].b=calloc(1,d[0].sectors*d[0].secsiz);

 //
 d[0].bam=calloc(1,secamo*4);

 //
 for(i=0; i<secamo; i++)d[0].bam[i]=0;

 //
 bd_makefs(d);
}

bd_getfree(BINDISK *d)
{
 //
 long i,i2,i3,i4;

 //
 for(i=0,i2=0; i<d[0].sectors; i++)
 {
  if(!d[0].bam[i])
  {
   i2+=d[0].secsiz;
  }
 }
 return i2;
}

bd_freedisk(BINDISK *d)
{
 free(d[0].bam);
 free(d[0].b);
}

long bd_getcap(BINDISK *d)
{
 //
 return (d[0].secsiz*d[0].sectors);
}

long bd_getallocated(BINDISK *d)
{
 return bd_getcap(d)-bd_getfree(d);
}

long bd_getsectors(BINDISK *d)
{
 return d[0].sectors;
}

long bd_getsecsiz(BINDISK *d)
{
 return d[0].secsiz;
}

bd_informme(BINDISK *d)
{
 //
 char str[255];

 //
 write("'bindisk' has ...\n");
 sprintf(str,"total %u sectors\neach sector has %u bytes information\n%8u bytes space allocated\n%8u bytes free space left\n%8u bytes total space\n",bd_getsectors(d),bd_getsecsiz(d),bd_getallocated(d),bd_getfree(d),bd_getcap(d));
 write(str);
}
