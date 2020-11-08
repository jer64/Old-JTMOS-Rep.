/* (c)Copyright 1997-1999 by Jari Tapio Tuominen, ABSOLUTLY NO WARRANTY!
   See file COPYING for more information! */
/*
 * Quick RAW audio player 1.00 --
 *
 * (When using SB,                Plays audio waves that fits in current DPMI,
 *  Addonics Sound Vision)        8bit unsigned + mono is the limit,
 *                                44100hz shouldn't be the limit, I guess,
 *                                I tried 65535hz, it worked out, I think (=,
 *                                atleast my ears didn't hear any noise.
 *
 * jtm_currentwavedevice=jtm_devicegus;
 * jtm_currentwavedevice=jtm_devicesb;
 *
 * jtm_qplay("drumfill.raw",22050);
 *
 * Notice!: Do not allocate convertional memory before the audio has played,
 *          because the player frees memory that wave allocated before
 *          returning, if you don't wait until audio has played,
 *          that may cause that current audio data in memory
 *          will be corrupted while it's playing(Random noise !! ): ).
 *
 * You don't need Windows to make such game as Command & Conquer and
 * Outpost II, you may just need some freeware utilities like DJGPP,
 * and libraries like JTMOS is.
 *
 */

#ifndef JTMSND_INCLUDED
#define JTMSND_INCLUDED

#define SBBSIZE (1024*9) // size of SB DMA buffer
#define __dspbase 0x220

unsigned __dspreset =__dspbase+0x6;
unsigned __dspread  =__dspbase+0xa;
unsigned __dspwrite =__dspbase+0xc;
unsigned __dspwritestatus=__dspbase+0xc;
unsigned __dspdavail=__dspbase+0xe;

//long     sbdos_place=0x500000; //(unsigned long int)&SB_BUFFER; //DOS memory location, needed for 24bit DMA (0x90000?)

// Sound blaster reserved:
 unsigned jtm_sbdos_segm;
 unsigned jtm_sbdos_memoryselector;
 char *wavedata;
 int bufsbsegm;

extern int      jtm_playsample(long sbdos_segment,unsigned _size,unsigned _freq);
extern int      jtm_writedsp(unsigned char _value);
extern int      jtm_qplay(char *filename,unsigned frequency); // Very simple paremeters!
extern void     jtm_filteraudio(unsigned char *audio,unsigned repeat);
extern int      jtm_getdmacounter(unsigned char _dmanumber);
extern void     jtm_filteraudio(unsigned char *audio,unsigned repeat);

int jtm_getdmacounter(unsigned char _dmanumber)
{
 unsigned uad;
 //
 uad=inportw(_dmanumber*2);
 return uad;
}

void jtmsnd_speakeron()
{
 jtm_writedsp(0xd1);
}

void jtmsnd_speakeroff()
{
 jtm_writedsp(0xd3);
}

void jtmsnd_stopdma()
{
 jtm_writedsp(0xd0);
}

void jtmsnd_continuedma()
{
 jtm_writedsp(0xd4);
}

int jtmsnd_dspreset(void)
{
 int retval;
 long ad,ad2;
 //
 for(ad=0; ad<1024; ad++)
 {
  retval=jtmsnd_dspreset1();
  if(retval==JTM_TRUE)break;
 }
 return retval;
}

int jtmsnd_dspreset1(void)
{
 long ad,retval;
 //
 outportb(__dspreset,1);
 outportb(__dspreset,0);

 if( (inportb(__dspdavail)&0x80)==0x80 &&
     inportb(__dspread)==0xaa )
 {
  retval=JTM_TRUE;
 }
 else
 {
  retval=JTM_FALSE;
 }
 return retval;
}

int playing=0;

int jtm_playsample(long sbdos_offset,unsigned _size,unsigned _freq)
{
 unsigned char timeconstant;
 unsigned long int offs1,offs2,page1,page2;
 long ad;
 //
//* jtmsnd_dspreset();
//* jtmsnd_speakeron();

 _size=_size-1;

 // Address
 offs1=sbdos_offset; //sbdos_segment; //_sound;
 page1=offs1>>16;

 // Setup the DMA chip
 outportb(0x0a,0x01);
 outportb(0x0c,0x00);
 outportb(0x0b,0x49);
 outportb(0x02, offs1&0xff);
 outportb(0x02, offs1>>8  );
 outportb(0x83,page1);
 outportb(0x03,_size&0xff);
 outportb(0x03,(_size>>8)&0xff);
 outportb(0x0a,0x01);

 // Set the playback frequency
 timeconstant=256-(1000000 / _freq);
 jtm_writedsp(0x40);
 jtm_writedsp(timeconstant);

 // Set playback to 8bit
 jtm_writedsp(0x14);
 jtm_writedsp(_size&0xff);
 jtm_writedsp(_size>>8);
}

int jtm_writedsp(unsigned char _value)
{
 while( (inportb(__dspwritestatus)&0x80)!=0x00 );
 outportb(__dspwrite,_value);
 //
 return JTM_TRUE;
}

int jtm_readdsp()
{
 char m1,m2;
 //
 do
 {
  m1=inportb(__dspdavail);
 }while((m1&0x80)==0);
 return inportb(__dspread);
}

void jtm_writedac(char byteri)
{
 jtm_writedsp(0x10);
 jtm_writedsp(byteri);
}

void jtm_readdac(char byteri)
{
 jtm_writedsp(0x20);
 return jtm_readdsp();
}

void jtmsnd_sbsys_on()
{
 // Allocate memory for SB buffer
 //
 long want_size=100*1024;

 if(jtmsnd.sbsys.state==0)
 {
  jtmsnd.sbsys.state=0xff;
  jtmsnd.sbsys.seginfo.size = (want_size+15) / 16;
  _go32_dpmi_allocate_dos_memory(&jtmsnd.sbsys.seginfo);
 }
// dosmemput(buffer, want_size, info.rm_segment*16);
// --> LinearAddressForDMA=jtmsnd.sbsys.seginfo.rmsegment*16
}

void jtm_sbsys_off()
{
 // Release memory allocated for SB buffer
 if(jtmsnd.sbsys.state!=0)
 {
  jtmsnd.sbsys.state=0;
  _go32_dpmi_free_dos_memory(&jtmsnd.sbsys.seginfo);
 }
}

int jtm_qplay(char *filename,unsigned frequency) // Very simple arguments!
{
 char sample[0xffff];
 //
 if(jtmsnd_dspreset()==JTM_FALSE)return JTM_FALSE;
 //
 if(fsizeof(filename)>65535) return -2;
 if(jtm_loadbin(filename,sample)!=0)
 {
  return -1;
 }
 return jtm_playclip(sample,frequency,fsizeof(filename));
}

int jtm_playclip(char *sample,unsigned frequency,unsigned length)
{
 register long ad;
 register char m1;
 //
 dosmemput(sample, length, jtmsnd.sbsys.seginfo.rm_segment*16);
 return jtm_playsample((jtmsnd.sbsys.seginfo.rm_segment*16),length,frequency);
}

int jtm_rdsp(void)
{
 char m1,m2,m3,m4;
 //
 m1=0x200;
 while(m1!=0)
 {
  if( (inportb(__dspdavail)&80)==0x80 ) break;
  m1--;
 }
 return inportb(__dspread);
}
/*
  ;Read DSP with timeout

	PUSH CX
	PUSH DX
	MOV DX,CS:SB_IO_ADDR
	ADD DL,0EH
	MOV CX,0200H
L4:     IN AL,DX
	JMP $+2
	OR AL,AL
	JS L5
	LOOP L4
	STC
	JMP L6
L5:     SUB DL,04H
	IN AL,DX
	CLC
L6:     POP DX
	POP CX
	RET
*/














void jtm_usersbirq(void)
{
 char m1,m2,m3,m4;
 //
 inportb(0x22e);

 /* Tell SB to accept next block */
// jtm_writedsp(0x14);
// jtm_writedsp(0xff);
// jtm_writedsp(0xff);

 jtmsnd_playit();
}

extern jtmsnd_playit()
{
 register long ad,ad2;
 //
 // Kuittaa "notice", eli huomautus, mik„ kertoo toiselle prosessille ett„
 // SB ajuri ymm„rsi huomautuksen tai ainakin kuttasi sen, v„h„n niin kuin
 // "Life sign" m„isesti.
CheckAgain:
 jtmsnd.sbsys.notice=0;
 if( jtmsnd.sbsys.playmode==WPLAY_STOP )
 {
  goto STOPPI;
 }

 if( jtmsnd.sbsys.wpp>=jtmsnd.sbsys.wavelength )
 {
  /* Kirjoita se koodi mik„ tuottaa k„sittely menetelm„n mit„
     k„ytet„„n samplen loppuun soimisessa
   */
  switch( jtmsnd.sbsys.playmode )
  {
   case WPLAY_STOP:
   goto STOPPI;
   case WPLAY_NORMAL:
   jtmsnd_stopplay1();
   break;
   case WPLAY_ABNORMAL:
   break;
   case WPLAY_REPEAT:
   jtmsnd.sbsys.wpp=0;
   break;
  }
 }

 if( (jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp)<1 )
 {
  jtmsnd.sbsys.playmode=WPLAY_STOP;
  goto CheckAgain;
 }

 //
 // Onko „„nt„ viel„ j„ljell„ t„yden puskurin verran ?
 if( (jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp)>=SBBSIZE )
 {
  // Siirr„ „„nt„ bufferiin
  for(ad=jtmsnd.sbsys.wpp,ad2=0; ad2<SBBSIZE; ad++,ad2++)
  {
   jtmsnd.sbsys.clip1[ad2]=jtmsnd.sbsys.wave[ad];
  }
  // Jos loput „„nest„ ei viel„ mahdu kokonaan SB DMA bufferiin niin
  // soitetaan bufferin verran sit„ ainakin.
  jtm_playclip(jtmsnd.sbsys.clip1,jtmsnd.sbsys.frequency,SBBSIZE);
  // Siirr„ pointteria eteen p„in ett„ se olisi seuraavalla kerralla
  // tarvittaessa siin„ kohdassa miss„ sijaitsee seuraava
  // DMA bufferin kokoinen osa „„nest„.
  jtmsnd.sbsys.wpp+=SBBSIZE;
  jtmsnd.sbsys.cplen=SBBSIZE;
 }
 else
 {
  // Siirr„ „„nt„ bufferiin, mutta vain sen verran kun on j„lell„.
  for(ad=jtmsnd.sbsys.wpp,ad2=0; ad2<(SBBSIZE-(jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp)); ad++,ad2++)
  {
   if( ad>=jtmsnd.sbsys.wavelength ) break;
   jtmsnd.sbsys.clip1[ad2]=jtmsnd.sbsys.wave[ad];
  }
  // Jos loput „„nest„ mahtuu kokonaan SB DMA bufferiin niin soitetaan vain
  // sen verran eik„ koko bufferin verran.
  jtm_playclip(jtmsnd.sbsys.clip1,jtmsnd.sbsys.frequency,
  SBBSIZE-(jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp));
  // Siirr„ pointteria viel„ viimeisen kerran eteen p„in ett„ se saavuttaa
  // „„ni virran lopun.
  jtmsnd.sbsys.cplen=SBBSIZE-(jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp);
  jtmsnd.sbsys.wpp+=(SBBSIZE-(jtmsnd.sbsys.wavelength-jtmsnd.sbsys.wpp));
  jtmsnd.sbsys.playmode=WPLAY_STOP;
 }
STOPPI:
 while(0!=0);
}

int jtmsnd_startplay(char *fname,int moodi)
{
 jtmsnd.sbsys.playmode=moodi;
 if(jtmsnd_startplay1(fname)!=0)return 1;
 return 0;
}

int jtmsnd_startplay1(char *fname)
{
 char m1,m2;
 long ad,ad2,x,y,ret;
 //
 if( !fexist(fname) )
 {
  jtm_addnotice("Cannot play: "); jtm_addnotice(fname); jtm_addnotice(", file not found!\n");
  return 1;
 }
 else
 {
  jtm_addnotice("JTMSND: Playing \""); jtm_addnotice(fname); jtm_addnotice("\".\n");
 }
 jtmsnd.sbsys.soundused=0xff;

 jtmsnd_dspreset();
 jtmsnd_speakeron();
 jtmsnd_sbsys_on();

 jtmsnd.sbsys.wavelength=fsizeof(fname);
 jtmsnd.sbsys.clip1=malloc(1024*64);
 if(jtmsnd.sbsys.wave!=NULL)free(jtmsnd.sbsys.wave);
 jtmsnd.sbsys.wave=malloc(fsizeof(fname));
 jtm_loadbin(fname,jtmsnd.sbsys.wave);

 jtmsnd_playit();
 return 0;
}

int WaitDMA(void) // -1=Timeout occured, 0="Notice" noticed.
{
 systemtimer.counter1=0;
 //
 jtmsnd.sbsys.notice=0xff;
 while(jtmsnd.sbsys.notice==0xff)
 {
  if( systemtimer.counter1>jtmsnd.sbsys.noticetimeout ) // Long time out = 200*8
  {
   return -1;
  }
 }
 return 0;
}

void jtmsnd_stopplay(void)
{
 jtm_messagebuffer("jtmsnd_stopplay: Attempting to stop audio.\n");
 jtmsnd_stopdma();
 jtmsnd.sbsys.playmode=WPLAY_STOP;
 outportb(0x02,0);
 outportb(0x02,0);
 outportb(0x83,0);
 WaitDMA();
 jtmsnd_speakeroff();
 jtmsnd_stopplay1();
 jtm_messagebuffer("jtmsnd_stopplay: Done.\n");
}

void jtmsnd_stopplay1(void)
{
 jtmsnd.sbsys.playmode=WPLAY_STOP;
 jtmsnd_stopdma();
 // Freing not required, because freeing can be done before allocating new
 // audio. Especially why not to free wave is that it is not recommended to
 // free audio when it maybe still in use.
 jtm_sbsys_off();
  // free(jtmsnd.sbsys.wave);
}

void jtmsnd_setfrequency(unsigned freq)
{
 jtmsnd.sbsys.frequency=freq;
}

unsigned long int jtmsnd_getvolume16fromraw(char *fname)
{
 // Some locals
 FILE *f;
 register
 unsigned long int ad,ad2,ad3,ad4,ad5,ad6,ad7,ad8,
                   Isoin,Pienin,Eka;

 // Initialize locals
 Isoin=0;
 Pienin=0xffff;
 //
 Eka=0xff;

 // Start process
 f=jtm_fopen(fname,"rb");
 do
 {
  //
  if(feof(f))break;
  ad=ad2;
uudestaa:
  ad2=fgetw(f);
  if(Eka==0xff){Eka=0; ad=ad2; goto uudestaa; }
  if(ad>ad2)
  {
   ad3=ad-ad2;
  }
  else
  {
   ad3=ad-ad2;
  }
  //
  if(ad3>Isoin)Isoin=ad3;
  if(ad3<Pienin)Pienin=ad3;
  //
 }while(0==0);
 jtm_fclose(f);
 return (Isoin-Pienin);
}
#endif
