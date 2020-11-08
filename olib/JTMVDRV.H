/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
//
//      JTMOS virtual device's library 1.00
//                    (C)1998 by Jari Tapio Tuominen, all rights reserved.
//
// Features :
//
//      - Mounting of virtual devices
//      - TAPFS support for all supported mass-storage devices(including virtual disk image)
//
//
#ifdef __cplusplus
extern "C" {
#endif
#define __DEFAULT_VD_SIZE 1024*512 // 512K VD image as default

typedef struct
{ 
 // 
 char imagepath[255];
 long type;             // 1=Virtual Disk Image, 2=Ramdisk image,
                        // 3=Host FS access.
 long sizeinbytes;
 long sectorsize;
 long amountofsectors;
 // Optional / Additional
 long filesystem;       // 1=TAPFS,2=No file system(sectors only)
 char label[255];
 char description[255];
 // For external device driver support only!
 char extdevfname[255]; // Specifies what external device driver should be
                        // loaded in memory.
 void *extdevptr;    // VOID PTR to the device driver.
 // Additional parameters
 unsigned long int apar[25]; // When using RAMDISK, APAR[0] is a 32bit offset
                             // to the current ramdisk data storage.
}DEVICE;

// JTM_DAS32DIRECTSET function
// ---------------------------
//
// This function direct sets sector in
// the specified mode on the drive using DAS32.
// Mode might be f.e. 0xffffffff = reserved, 0x00000000 = free.
//
//
jtm_das32directset(DEVICE *ds,unsigned long sectorta,unsigned long int reqval)
{
 //
 unsigned long ad,ad2,ad3,ad4,sec,losec,*dasbuf;

 //
/* dasbuf=malloc(ds[0].sectorsize);

 //
 sec=((sectorta*4)/ds[0].sectorsize);

 // Read a part of the DAS32 table in memory for modifying.
 jtm_readsector(&ds,sec,dasbuf);

 //
// dasbuf[(sectorta*4)%ds[0].sectorsize]=reqval;

 // Write the part of the DAS32 back on drive.
 jtm_writesector(&ds,sec,dasbuf);

 //
 free(dasbuf);*/
}

// JTM_CREATEDAS32 function
// ------------------------
//
// This function createsa a simple file allocation table for the entire
// disk space. The file allocation table is 32bit, and each 32bit value
// means same as one sector.
// The first 18 sectors on the disk are not reserved,
// the ones comes after the first 18 sectors are the DAS32 itself,
// after that DAS32 sectors there'll be only wide variant of data sectors.
// As default, DAS32 allocates itself and first sector.
// First sector should not be freed unless the drive is not
// used for booting up the system. The 17 other first sectors are usually
// set as free sectors.
//
jtm_createdas32(DEVICE *ds)
{
 //
 long ad,ad2,ad3,ad4,str[255];

 // First of all, clear the DAS32 allocation map.
/* for(ad=0; ad<ds[0].amountofsectors; ad++)
 {
  jtm_das32directset(&ds,ad,0x00000000);
 }*/
/* // Then allocate sector 0
 // 0xffffffff=System reserved, not for remapping.
 write("First sector ...\n");
 jtm_das32directset(&ds,0,0xffffffff);
 write("Self allocation ...\n");
 for(ad=18; ad<(18+((ds[0].amountofsectors*4)/ds[0].sectorsize)+1); ad++)
 {
  jtm_das32directset(ds,ad,0xffffffff);
 }*/
}

// JTM_WRITESECTOR function
// ------------------------
//
// This function writes data on the specified drive.
//
//
//
jtm_writesector(DEVICE *ds,long sector,unsigned char *somedata)
{
 //
 unsigned long int ad,ad2,ad3,ad4,a1,a2,a3,a4,ramoff;
 unsigned char *ramo;
 char str[255];

 //
 sprintf(str,"Amount of sectors = %u.\n",ds[0].amountofsectors);
 write(str);

 // Check drive type and proceed with the propel data read method.
 //
 // <RAMDISK>
 if(ds[0].type==2)
 {
  // Get the ramdisk memory location (32bit LINEAR)
  ramoff=ds[0].apar[0];
  ramo=ramoff;

  // Compute linear address from the specified sector number
  ad=sector*ds[0].sectorsize;
  for(a1=ad,a2=0; a1<ad+ds[0].sectorsize; a1++,a2++)
  {
   ramo[a1]=somedata[a2];
  }
 }
}

// JTM_READSECTOR function
// ------------------------
//
// This function reads data from the specified drive. (By amount of sectors)
//
//
//
jtm_readsector(DEVICE *ds,long sector,unsigned char *somedata)
{
 //
 unsigned long int ad,ad2,ad3,ad4,a1,a2,a3,a4,ramoff;
 unsigned char *ramo;

 // Check drive type and proceed with the propel data read method.
 //
 // <RAMDISK>
 if(ds[0].type==2)
 {
  // Get the ramdisk memory location (32bit LINEAR)
  ramoff=ds[0].apar[0];
  ramo=(char*)ramoff;

  // Compute linear address from the specified sector number
  ad=sector*ds[0].sectorsize;
  for(a1=ad,a2=0; a1<ad+ds[0].sectorsize; a1++,a2++)
  {
   somedata[a2]=ramo[a1];
  }
 }
}

// JTM_FORMATDRIVE function
// ------------------------
//
// This function formats specified drive.
//
//
//
jtm_formatdrive(DEVICE *ds)
{
 //
 long ad,ad2,ad3,ad4;
 unsigned char *das,*somedata;

 //
 somedata=malloc(ds[0].sectorsize);
 for(ad=0; ad<ds[0].sectorsize; ad++)somedata[ad]=0;

 // We first have to indentify the drive type.
 // <RAMDISK>
 if( ds[0].type==2 )
 {
  das=malloc(ds[0].sizeinbytes);
  for(ad=0; ad<ds[0].amountofsectors; ad++)jtm_writesector(ds,ad,somedata);
  ds[0].apar[0]=(unsigned long int)das;
  jtm_createdas32(ds);
 }

 //
 free(somedata);
}


// JTM_MOUNTDRIVE function
// -----------------------
//
// This function mounts drive.
// The function will be prolly moved to other library since
// mounting is possible with other devices also.
// When the function starts mounting a device,
// it first checks for the device availability,
// if the device is available it'll continue if it isn't then it'll return 1.
unsigned long jtm_mountdevice(char *dev,DEVICE *ds)
{
 //
 long ad,ad2,ad3,ad4;
 char str[255],str2[255],str3[255],str4[255];
 FILE *f,*f2,*f3,*f4;
 long _VALUE_;

 // Initialize DEVICE structure.
 ds[0].sizeinbytes=0;
 ds[0].type=0;
 ds[0].sizeinbytes=0;
 ds[0].sectorsize=0;
 ds[0].amountofsectors=0;

 ds[0].filesystem=0;
 ds[0].label[0]=0;
 ds[0].description[0]=0;

 ds[0].extdevfname[0]=0;
 ds[0].extdevptr=(void*)NULL;

 // First of all it is neccesery to receive information about
 // the device existing, and its properties.
 sprintf(str,"c:\\jtm%s",dev);
 f=fopen(str,"rb");
 if(f==NULL)return 2;
 do
 {
next:
  if(feof(f))break;
  fscanf(f,"%s",&str2);
  if(!strcmp(str2,"[device]"))
  {
   goto next;
  }
  if(!strcmp(str2,"image:"))
  {
   fscanf(f,"%s",&str2);
   strcpy(ds[0].imagepath,str2);
   goto next;
  }
  if(!strcmp(str2,"type:"))
  {
   fscanf(f,"%u",&ds[0].type);
   goto next;
  }
  if(!strcmp(str2,"sizeinbytes:"))
  {
   fscanf(f,"%u",&ds[0].sizeinbytes);
   goto next;
  }
  if(!strcmp(str2,"sizeofsector:"))
  {
   fscanf(f,"%u",&ds[0].sectorsize);
   goto next;
  }
  if(!strcmp(str2,"amountofsectors:"))
  {
   fscanf(f,"%u",&ds[0].amountofsectors);
   goto next;
  }
  if(!strcmp(str2,"filesystem:"))
  {
   fscanf(f,"%u",&ds[0].filesystem);
   goto next;
  }
  if(!strcmp(str2,"label:")) // Device label (Not drive!!)
  {
   fscanf(f,"%s",&ds[0].label);
   goto next;
  }
  if(!strcmp(str2,"description:"))
  {
   fscanf(f,"%s",&ds[0].description);
   goto next;
  }
  if(!strcmp(str2,"extdevfname:")) // Filename of an extrnal device driver
                                   // which'll handle device propely.
  {
   fscanf(f,"%s",&ds[0].extdevfname);
   goto next;
  }
  if(!strcmp(str2,"extdevptr:")) // Direct call(??)
  {
   fscanf(f,"%x",&ds[0].extdevptr);
   goto next;
  }
  if(!strcmp(str2,"/*"))
  {
   do
   {
    fscanf(f,"%s",&str);
    if(!strcmp(str,"*/"))break;
   }while(0==0);
   goto next;
  }
  // ERROR !!
  fclose(f);
  return 1;
 }while(0==0);
 fclose(f);

 // Return OK!
 return 0;
}









// JTMFN_SYSCFG
int jtm_vdsysteminit()
{
 //
 long ad,ad2;
 char str[255],str2[255];

 //
 if(jtmvdsys.state)
 { // Already started?
  return 1; // Cannot start twice.
 }
 else
 {
  if(!fexist(jtmfn_syscfg))
  {
   return 2; // JTMOS configuration file doesn't exist.
  }
  else
  {
   strcpy(str,jgetenv("preferences","virtualdrive="));
   if(str[0]==0)return 3; // VirtualDrive variable not defined in preferences
   sscanf(str,"%u",&jtmvdsys.state);
   if(!jtmvdsys.state)
   {
    return 4; // VirtualDrive(s) not allowed in this system.
   }
   else
   {
    strcpy(str,jgetenv("preferences","vdpath="));
    if(str[0]==0)return 5; // VirtualDrive's path not defined.
    strcpy(jtmvdsys.vdpath,str);
    return 0; // Ok, initialization completed without any errors!
   }
   //
  }
 }
}

int jtm_createvdrive(char *fname,char *label,long size)
{
 //
 FILE *f,*f2;
 long ad,ad2,ad3,ad4;
 char str[255],str2[255];

 //
 if(size<0)
 {
  size=__DEFAULT_VD_SIZE;
 }

 //
 sprintf(str,"%s/%s",jtmvdsys.vdpath,fname);
 if(fexist(str))
 {
  return 0;
 }

 // Create dump VD image.
 f=fopen(str,"wb");
 if(f==NULL)
 {
  return 2; // Cannot create VD image!
 }
 for(ad=0; ad<size; ad++)
 {
  fputc(0x00,f);
 }
 fclose(f);
}






// JTM_VDFORMAT function
// ---------------------
//
// This functions creates basic bootstrap/data allocating system/file system.
// The first sector of VD image contains same information as most of
// floppy disks might contain, information about disk size, used space,
// drive label, format, and so on.
// Next sectors after the first sector usually contain the DAS,
// data allocation system space, which is the data which defines
// how the drive space is allocated.
//
//
jtm_vdformat(char *fname,char *label)
{
 //
 char str[255],str2[255],str3[255],str4[255];
 FILE *f,*f2;
 long ad,ad2,ad3,ad4,len;

 //
 sprintf(str,"%s/%s",jtmvdsys.vdpath,fname);
 if(!fexist(str))return 1; // VD image doesn't exist.

 // Create valid 22 marks label string
 for(ad=0; ad<22; ad++)
 {
  if(ad>=strlen(label))
  {
   str2[ad]=' ';
  }
  else
  {
   str2[ad]=label;
  }
 }
 str2[ad]=0;

 //
 len=fsizeof(str);

 //
 f=fopen(str,"wb");
 /**** Write first sector ****/
 // Write "JMP 0140", opcode.
 fputw(0x3eeb,f);
 // Write indentify string
 fprintf(f,"JTMFS100");
 // Write label
 fprintf(f,"%s",str2);
 // Write format description
 fputw(0x0001,f);
 // Write length of the drive
 fputd(len,f);
 // Lets suppose that we're using clusters of 1024 byte size.
 // SizeOfDAS32=((TotalSizeOfHDD)/1024)*4  (1024 -> Cluster)  ("*4" -> DWORD)
 write(" ");
 for(ad=0; ad<(len/1024); ad++)
 {
  //
  if((ad&1))
  {
   write("\b±");
  }
  else
  {
   write("\b°");
  }

  // Bootstrap at first cluster
  if(ad==0)fputd(0xffffffff,f); // 0xffffffff = (Reserved)Bootstrap
  // Next
  if(ad>=1 || ad<=(1+((len/1024)/256)) )
  {
   fputd(0xfffffffe,f); // 0xfffffffe = (Reserved)DAS32
   goto stored1;
  }
  fputd(0x00000000,f);          // 0x00000000 = Free cluster
stored1:
  ad=ad;
 }
 write("\b");

 //
 sprintf(str3,"Data area begins from 0x%1.8x.\n",ftell(f));
 write(str3);

 // Rest of the image can be dumpped with 0's.
 for(ad=ftell(f); ad<len; ad++)
 {
  fputc(0x00,f);
 }
 fclose(f);
}

#ifdef __cplusplus
}
#endif

