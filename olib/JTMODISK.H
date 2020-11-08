/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
unsigned int writesector(unsigned char _drive,unsigned _sector,char *buf);
char *readsector(unsigned char _drive,unsigned _sector);
unsigned verifydisk(unsigned char _drive);
void jtm_formatdisk(char *_label);

unsigned int writesector(unsigned char _drive,unsigned _sector,char *buf)
{
 unsigned char _ret;
 asm
 {
  mov ax,cs
  mov es,ax
  mov al,1
  mov cx,word ptr _sector
  mov dl,byte ptr _drive
  mov bx,offset buf
  mov ah,0x03
  mov dh,0
  int 0x13
  mov al,0
  jnc zero
  mov al,1
zero:
  mov byte ptr _ret,al
 }
 return _ret;
}

char *readsector(unsigned char _drive,unsigned _sector)
{
 unsigned char buf[512]; // Stanard amount for 1.44MB disks
 asm
 {
  mov ax,cs
  mov es,ax
  mov al,1
  mov cx,word ptr _sector
  mov dl,byte ptr _drive
  mov bx,offset buf
  mov ah,0x02
  mov dh,0
  int 0x13
 }
 return buf;
}

unsigned verifydisk(unsigned char _drive)
{
 unsigned _return=0;
 asm
 {
  mov ah,0x10
  int 0x13
  jnc dset
  mov word ptr _return,0x01
dset:
/*  mov ax,0x00
  mov dl,byte ptr _drive
  int 0x13  
  
  mov al,1 // Sector(s) = 1
  mov ch,0 // Track
  mov cl,0 // Sector
  mov dh,0 // Head
  mov dl,byte ptr _drive // Drive     = 0(A:)
  mov ax,ds
  mov es,ax
  mov bx,offset buf
  mov ah,0x02
  int 0x13
  jnc dset
  mov word ptr _return,0x01
dset:
  jmp skip
buf: dw 0,0,0,0,0,0,0,0
skip:*/
 }
 return _return;
}

void jtm_formatdisk(char *_label) // Formats currect disk in drive 0(A:)
                                  // to RealFat format, which JTM-OS supports.
{
 // Okay lets start, first in case we have to create so called 'boot sector',
 // as we're creating now our very own disk format, we don't have to care
 // about those DOS/BIOS stanards, we can use just one time all the datas
 // available.
 // however this is part #1: "Control block creation:"
 //                          -------------------------
 float fa=0,fb=0;
 char buf[512];
 long ad,ad2;
 //
 cprintf("Formatting disk in drive A: (Press [ESC] to cancel)\n\r");
 //
 cprintf("Writing real header...\r");
 for(ad=0; ad<512; ad++)
 {
  if(ad<64)buf[ad]=_label[ad];
  else buf[ad]=0;
 }
 writesector(0,0,buf);

 //
 // And now this is the part #2: "Clear disk"
 //
 cprintf("Cleaning data area...         \n\r");
 for(ad=0; ad<512; ad++)buf[ad]=0;
 //
 for(ad=1, fa=0; ad<2880; ad++, fa+=100/2880)
 {
  m1=jgetchim();
  if(m1==27)
  {
   cprintf("\n\r?user break.\n\r");
   cprintf("ready.\n\r");
   break;
  }
  m1=writesector(0,ad,buf);
  if(m1!=0)
  {
   cprintf("\n\r?Disk error.\n\r");
   cprintf("ready.\n\r");
   break;
  }
  cprintf("%u/2880 completed.",ad); cprintf("\r");
 }
}
