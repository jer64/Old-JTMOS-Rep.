/*
 * ============================================================
 * sb.c - Sound Blaster support.
 *
 * (C) 2001-2003 by Jari Tuominen(jari@vunet.org)
 *
 * This driver does not yet work, please avoid using it.
 *
 * See drivers/audio/audio.c for GENERIC AUDIO DRIVER.
 * (contains audio data buffer FBUF aud;)
 *
 * SB base is usually located between 0x0210-0x0260
 * Init first searches for 0x220, then 0x210,0x230-0x260
 * if none of these base addresses contain a standard/supported
 * soundblaster card, the conclusion will be that there
 * are no detectable nor supported soundblaster card.
 *
 * Credits note: some code borrowed partly from
 * various places, f.e. Linux kernel.
 *
 * Notification:
 *
 * Requires atleast Sound Blaster 2.00.
 * Sound Blaster 2.00 came with enhanced DMA features
 * that made programming easier, and cleaner job to do.
 * If you want SB 1.00 support, mail me any time :-)
 * Mail me for any documents, etc. what you might need.
 *
 * Added some Logitech SoundMan specific code.
 *
 * ============================================================
 */
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "dma.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "sb.h"
#include "sb_dev.h"
#include "buffer.h" // FBUF
#include "audio.h"
//
#include "driver_sys.h"

// Thread structure
THREAD sbThread;

/* Define a useful macro for writing data to the DAC - Note: No time-out. */
#define writedac(x) { while(inportb(sb.base+0x0C) & 0x80); \
		      outportb(sb.base+0x0c,(x)); }

//#define __BLASTER220__
#define SB_DMA_BUFFER_SIZE		(1024*4)
#define SB_DMA_BUFFER_OFFSET		(0xA0000-(SB_DMA_BUFFER_SIZE))
#define SB_DMA_BUFFER2_OFFSET		(0xA0000-(SB_DMA_BUFFER_SIZE*2))
// 0 = play from buffer 0, render at buffer 1
// 1 = play from buffer 1, render at buffer 0
static int buffer_swap=0;

/*
 * This [see below] is an idea, and is not actually implemented!
 *
 * The idea is that, there is two audio buffers,
 * first DMA buffer at 0x9000,
 * second DMA buffer at 0x9800.
 * first one one of those are used for rendering
 * another one of those are used for playing
 * on each IRQ the places of those buffers are swapped
 * DMA setupped to play another one of these,
 * and rendering made on the other one.
 */

// Create soundblaster structure
SB sb;

//
WORD sb_getplayposition(BYTE _dmanumber)
{
	WORD uad;

	//
	uad = inportb(sb.dma*2+0) | (inportb(sb.dma*2+1)<<8);
	return uad;
}

//
void sb_reset_mixer(void)
{
	int i;

	// Standard soundblaster mixer init:
	// Turn speaker on, volume up, store the default values.
	sb_turn_speaker_on();
	sb_set_master_volume(0xFF);
	sb_set_dsp_volume(0xFF);
	sb_set_fm_volume(0xFF);
	sb_set_cd_volume(0xFF);
	sb_set_line_volume(0xFF);

	// Logitech SoundMan specific mixer init:
//	sb_set_mixer_reg(0x00, 0x18);	/* Mute unused (Telephone) line */
//	sb_set_mixer_reg(0x10, 0x38);	/* Config register 2 */

	// Set TREBLE & BASS
	// Emphasize bass, normalize treble.
	// Note:	Max. bass value usually leads into
	//		audio dataloss, so it is not used,
	//		a moderate 12 is better than 15.
//	sb_set_treble(8);
//	sb_set_bass(12);
}

// Detects presence of SB Mixer(>=PRO)
int sb_detect_mixer(void)
{
	/*
	 * Detect the mixer by changing parameters of two volume channels. If the
	 * values read back match with the values written, the mixer is there (is
	 * it?)
	 */

	// Set it
	sb_set_mixer_reg(FM_VOL,0xFF);
	sb_set_mixer_reg(VOC_VOL,0x33);

	// Got it?
	if(sb_get_mixer_reg(FM_VOL)!=0xFF)return 0;
	if(sb_get_mixer_reg(VOC_VOL)!=0x33)return 0;
  
	// Found mixer!
	return 1;
}

// returns the mixer register's value that usually
// represents the current IRQ
int sb_get_irq_nr(void)
{
	//
	sb_get_mixer_reg(IRQ_NR);
}

// returns the mixer register's value that usually
// represents the current DMA
int sb_get_dma_nr(void)
{
	//
	sb_get_mixer_reg(DMA_NR);
}

// 0-15
int sb_set_treble(int power)
{
	//
	sb_set_mixer_reg(TREBLE_L, power<<8);
	sb_set_mixer_reg(TREBLE_R, power<<8);
}

// 0-15
int sb_set_bass(int power)
{
	//
	sb_set_mixer_reg(BASS_L, power<<8);
	sb_set_mixer_reg(BASS_R, power<<8);
}

// Copy a piece of sample(at most 64K)
// to the DMA buffer area from the SB FIFO buffer.
void sb_get_sample(int length)
{
	static BYTE *p;
	static int i,l,d1;

	WHEREAMI

	//
	if(!buffer_swap)
		p = (DWORD)SB_DMA_BUFFER2_OFFSET;
	else
		p = (DWORD)SB_DMA_BUFFER_OFFSET;

	// Truncate length to fit inside the limit.
	if(length>SB_DMA_BUFFER_SIZE)
	{
		l=SB_DMA_BUFFER_SIZE;
	}
	else
		l=length;

	WHEREAMI

	// Copy the data
	for(i=0; i<l; i++)
	{
		if( (d1 = GetBuffer(&aud))==-1 )
		{
			// Output default wafeform if no data available in buffer
			d1 = 0x80;
		}
		//d1 = i; // TEST AUDIO
		p[i] = d1;
	}

	WHEREAMI
}

// Sets up a DMA transfer
void sb_dma(int channel,BYTE *buf,int length,char direction)
{
	static DWORD ad;

	//
	ad=buf;

	// DMA
	outportb(0x0A, 0x05);
	outportb(0x0C, 0x00);
	outportb(0x0B, 0x49);
	// Address
	outportb(0x02, ad&255);
	outportb(0x02, (ad>>8)&255);
	outportb(0x83, (ad>>16)&255);
	// Length
	outportb(0x03, length&255);
	outportb(0x03, (length>>8)&255);
	// 
	outportb(0x0a, 0x01);
}

// Sets playback frequency
int sb_set_frequency(WORD _freq)
{
	static BYTE timeconstant;

	//
	if(_freq<=1)
		panic("sb_set_frequency was called with <=1 frequency");
	timeconstant = 256-(1000000/_freq);
	if( !sb_write_dsp(0x40) )
	{
		sb_write_dsp(timeconstant);
	}
	else
	{
		printk("sb.c: setting timeconstant failed\n");
		sb_reset();
		return 1;
	}
	return 0;
}

// 0x14 => non-auto DMA
// 0x1C => auto DMA(SB >=2.00), recommended.
//	Note: use command 0x48 first.
int sb_set_playback(WORD fmt, WORD _size)
{
/*	// Send size
	if(sb.version>=0x200)
	{
		sb_write_dsp(0x48);
		sb_write_dsp(_size&0xFF);
		sb_write_dsp(_size>>8);
	}*/

	//
	if( !sb_write_dsp(fmt) )
	{
	//	if(sb.version<0x200)
	//	{
			sb_write_dsp(_size&0xFF);
			sb_write_dsp(_size>>8);
	//	}
	}
	else
	{
		printk("%s: setting playback block's size failed\n", __FUNCTION__);
		sb_reset();
		return 1;
	}
	return 0;
}

//
int sb_set_dma_blocklength(WORD length)
{
	//
	if( !sb_write_dsp(0x48) )
	{
		// Send size
		sb_write_dsp(length&0xff);
		sb_write_dsp(length>>8);
	}
	else
	{
		return 1;
	}
}

// This function plays sample with soundblaster
// New: we use auto init DMA mode here.
int sb_play_sample(WORD _size,WORD _freq)
{
	static DWORD offs1,offs2,page1,page2;
	static int ad;
	static BYTE timeconstant;

	//
	sb.isPlaying=1;

	// Send the audio on the way to SB
	// SB_DMA_BUFFER_ADDRESS
	if(buffer_swap==0)
		sb_dma(sb.dma, SB_DMA_BUFFER_OFFSET, _size, TRUE);
	else
		sb_dma(sb.dma, SB_DMA_BUFFER2_OFFSET, _size, TRUE);

	// Set the playback frequency
	sb_set_frequency(_freq);

	// Set playback to 8-bit (0x14 non-auto, 0x1C auto init DMA(recom.))
	if(sb.version>=0x200)
		// SB >=2.xx
		sb_set_playback(0x1C, _size);
	else
		// SB 1.xx
		sb_set_playback(0x14, _size);
 
	//
	return 0;
}


//
void sb_turn_speaker_on(void)
{
	//
	sb_write_dsp(DSP_CMD_SPEAKERON);
}

//
void sb_turn_speaker_off(void)
{
	//
	sb_write_dsp(0xd3);
}

// Sets a SB's mixer register
// (you can control the mixer this way..)
void sb_set_mixer_reg(int which,int value)
{
	//
	outportb(sb.base+4,which);
	outportb(sb.base+5,value);
}

// 0x10 - Output sample in direct mode.
// Sends 8-bit audio sample at blaster to play.
void sb_direct_output(int amount)
{
        //
        sb_set_mixer_reg(0x10,amount);
}

// 8-bit value, L/H = left/right channel 0xC0 0x0C
void sb_set_master_volume(int amount)
{
	//
	sb_set_mixer_reg(0x22,amount);
}

// 8-bit value, L/H = left/right channel 0xF0 0x0F
void sb_set_dsp_volume(int amount)
{
        //
        sb_set_mixer_reg(0x04,amount);
}

// 8-bit value, L/H = left/right channel 0xF0 0x0F
void sb_set_fm_volume(int amount)
{
	//
	sb_set_mixer_reg(0x26,amount);
}

// 8-bit value, L/H = left/right channel 0xC0 0x0C
void sb_set_cd_volume(int amount)
{
	//
	sb_set_mixer_reg(0x28,amount);
}

// 8-bit value, L/H = left/right channel 0xC0 0x0C
void sb_set_line_volume(int amount)
{
	//
	sb_set_mixer_reg(0x2E,amount);
}


// Reads a SB's mixer register current value
// (you can read the mixer this way.. and do various conclusions)
// Max. 0xC0 0x0C Left/Right
int sb_get_mixer_reg(int which)
{
	//
	outportb(sb.base+4,which);
	return inportb(sb.base+5);
}

// 8-bit value, L/H = left/right channel 0xF0 0x0F
void sb_set_speaker_volume(int amount)
{
	//
	sb_set_mixer_reg(SPEAKER_VOL, amount);
}

/* 
 * SB DSP commands available:
 *	D0	Pause 8-bit DMA transfer
 *	D1	Enable speaker
 *	D3	Disable speaker
 *	D4	Resume 8-bit DMA transfer
 *	E1	Determine DSP version number
 */
// Writes to DSP
int sb_write_dsp(BYTE the_data)
{
	int i,t;

	// Wait until ready
	for(t=GetSeconds(); (inportb(sb.base+DSP_STATUS)&0x80); i++)
	{
		//
		SwitchToThread();

		//
		if( (GetSeconds()-t)>=4 )
		{
			printk("%s: sb DSP_STATUS read time-out at sb_write_dsp\n", __FUNCTION__);
			return 1;
		}
//		printk("secs=%d, t=%d     \r", GetSeconds(), t);
	}
 
	// Write to the data port
	outportb(sb.base+DSP_WRITE, the_data);
 
	//
	return 0;
}

// sb_read_dsp
//
// Reads from DSP
// return value: returns -1 on failure(possible failure: time-out)
// (8-Bit value)
//
int sb_read_dsp(void)
{
	int i,t;

	// Wait first
	for(t=GetSeconds(); !(inportb(sb.base+DSP_DATA_AVAIL)&0x80); )
	{
		//
		SwitchToThread();

		//
		if( (GetSeconds()-t)>=2 )
		{
			printk("%s: sb DSP_DATA_AVAIL read time-out at sb_read_dsp\n", __FUNCTION__);
			return -1;
		}
	}
 
	//
	return inportb(sb.base+DSP_READ);
}

// Receives SB version
int sb_get_version(void)
{
	int major,minor;
 
	// version command = 0xe1
	sb_write_dsp(0xE1);
 
	//
	if( (major = sb_read_dsp()) == -1 )return -1;
	if( (minor = sb_read_dsp()) == -1 )return -1;
 
	//
	return minor&0xFF | ((major&0xFF)<<8);
}

// Configure blaster
void sb_configure(int base,int irq,int dma)
{
	// Setup the default blaster preferences
	sb.base=base;
	sb.irq=irq;
	sb.dma=dma;
}

void sb_found(void)
{
	printk("%s: found soundblaster\n", __FUNCTION__);
}

// be quiet if nothing found
void sb_not_found(void)
{
//	printk("%s: no soundblaster card detected\n", __FUNCTION__);
}

// SB INIT MAIN
// ------------
// Detects blaster, and registers device if one detected
int sb_init(int hardscan)
{
	int rval,t;

	// Init main structure
	memset(&sb, 0, sizeof(SB));
	memset(SB_DMA_BUFFER_OFFSET,  0x80, SB_DMA_BUFFER_SIZE);
	memset(SB_DMA_BUFFER2_OFFSET, 0x80, SB_DMA_BUFFER_SIZE);

	// Detect device
	rval = sb_init_(hardscan);

	// Device detected?
	if(!rval)
	{
		// Device detected OK
		// ------------------
		printk("Setting up interrupt handler.\n");

		// Setup interrupt handler
		setint(M_VEC+sb.irq,
			sb_ISR,
			GIMMIE_CS(),
			D_PRESENT + D_INT + D_DPL3);
		enable_irq(sb.irq);

		// Register device
		printk("Registering device.\n");
		sb_register_device();

		// Start loop.
		printk("Starting loop.\n");
		audio.frequency = 11025;
		audio.length = SB_DMA_BUFFER_SIZE;
		sb_start_playing();
		printk("Done!\n");

		// Wait until SB interrupt occurs.
		for(t=GetSeconds(); !sb.irqTick; )
		{
			if( (GetSeconds()-t)>=4 )
			{
				//
				printk("SB IRQ failed, IRQ%d not valid?\n",
					sb.irq);

				//
				return 1;
			}
		}

		//
		printk("Detected SB interrupt, interrupts are apparently working correctly.\n");

		// Start DMA monitor(clip hack).
		init_sbThread();
	}
	return rval;
}

// Creates SB thread
void init_sbThread(void)
{
	//
	ThreadCreateStack(&sbThread, 1024*16);
	sbThread.pid =
		create_thread(sbThread.stack,
			sbThread.l_stack, sbProcess);

	//
	SetThreadTerminal(sbThread.pid, 6);

	// max. 31 characters
	IdentifyThread(sbThread.pid, "KSB");
}

//
void moreData(void)
{
	// Send more audio data
	disable();
	sb_start_playing();
	enable();
}

// Predictive chunk changer.
// (clip hack)
void sbProcess(void)
{
	int pos,lpos,stopTime,stopCounter,t,t2,tt,key,try;

	//
	for(lpos=0,pos=0,lpos=0,stopTime=-1,try=0,t=GetSeconds(); ; lpos=pos)
	{
		//
		key = getch1();

		// Get position
		pos = sb_getplayposition(sb.dma);

		//
		ReportMessage("SB DMA position=0x%x, time=%d, counter=%d, alength=%d, afreq=%d.\n",
			pos,
			stopTime, stopCounter,
			audio.length, audio.frequency);

		//------------------------------------------------------
		// Detect DMA stops and trigger autoresume to fix accidents.
		//
		if(lpos==pos)
		{
			if(stopTime==-1)
				stopTime = GetSeconds();
		}
		else	stopTime = -1;

		//
		if(stopTime!=-1 && (GetSeconds()-stopTime)>=2)
		{
			//
			stopTime=-1;
			moreData();
			//
			printk("Resumed stopped SB DMA.\n");
			continue;
		}

		//------------------------------------------------------
		// Playing fix.
		//
		if( (pos&0xFF00)==0 )
		{
			//
			sleep(1);
			if( (try++)<2 )
				continue;
			try = 0;

			//
			moreData();
			
			//
			printk("Sent more audio data via DMA.\n");
		}
		else	try = 0;

		//------------------------------------------------------
		//
		SwitchToThread();
	}
}

/*
 * sb_init
 *
 * Note:	In future we need to enable detection of multiple
 *		soundblaster cards, currently we just detect
 *		the first one that came in our way.
 */
int sb_init_(int hardscan)
{
	int i;

	//
	printk("sound blaster driver version 1.0 by Jari Tuominen\n");

#ifdef __BLASTER220__
	// We just set the base at as 0x220 if specified so . .
	sb.base=0x220;
	sb.irq=5;
	sb.dma=1;
	return 0;
#else
	// First look at 0x220
	if( !(i=sb_init1(0x220,5,1, 1)) ) { return 0; }

	// If not yet detected, and hardscan was adviced:
	if(hardscan)
	{ 
		// Then walkthrough all possible base addresses
		printk("%s: hunting through various addresses for the sb\n", __FUNCTION__);
  
		//
		for(i=0x210; i<0x270; i+=0x10)
		{
			// 0x220? We already tried that -> skip
			if(i==0x220)continue;
			// Lets try this base address.....
			if( !sb_init1(i,5,1, 1) ) { return 0; }
		}
	}
  
	// No success? Too bad :}
	return 1;
#endif
}

// We will init SB here
int sb_init1(int base, int irq, int dma,
		int quiet)
{
	int i,i2;

	// Not playing now(ceases DMA loop in IRQ)
	sb.isPlaying=0;
 
	//
	sb.mixer = 0;
 
	// Set preferences
	sb_configure(base,irq,dma);
 
	//
	if(!quiet)
	{
		print("trying with following values: ");
		print("base=0x"); p16(sb.base); print(" ");
		print("irq="); pd32(sb.irq); print(" ");
		print("dma="); pd32(sb.dma);
		print("\n");
	}

	// try to reset for awhile
	for(i=0; i<4; i++) if( !(i2=sb_reset()) ) break; // 10
	sb_turn_speaker_on();
 
	//
	if(i2)
	{
		return 1;
	}
 
	// Get SB version
	if( (sb.version = sb_get_version()) == -1 )
	{
		//
		if(!quiet)
			print("sb.c: sb version query failed, giving up sb_init\n");
		return 1;
	}

	//
	printk("SB version 0x%x.\n",
		sb.version);
 
	//
	if(sb.version==0xFFFF)return 1;
 
	//
	if(!quiet)
	{
		print("sb.c: version = ");
		p16(sb.version); print("\n");
	}
 
	// Attempt to reset & detect mixer
	if( sb.mixer=sb_detect_mixer() )
	{
		// Found mixer:
		sb_reset_mixer();
	}
 
	// Report
	sb_found();
	printk("%s: Base=0x%x, irq=%d, dma=%d.\n", __FUNCTION__, sb.base, sb.irq, sb.dma);
 
	// Report mixer
	if(sb.mixer)
	{
		//
		printk("%s: mixer detected(SB pro & newer)\n", __FUNCTION__);
	}
	else
	{
		printk("%s: no mixer detected (<=2.0 sb?)\n", __FUNCTION__);
		printk("%s: Warning: JTMOS Sound Blaster driver REQUIRES atleast SB version 2.0.\n", __FUNCTION__);
	}

	//
	printk("\rTurning speaker on       \r");
	sb_turn_speaker_on();
	sb_turn_speaker_on();
	sb_turn_speaker_on();
	sb_turn_speaker_on();

	// Report returning
	printk("\r%s: OK done here, returning back.         \n", __FUNCTION__);
	return 0;
}

// sb_reset
//
// This code resets the SoundBlaster
// Note: returns nonzero on error, zero on success
//
int sb_reset(void)
{
	int i;

	//
	printk("%s: resetting Sound Blaster ", __FUNCTION__);
	//
	enable();
	// Reset
	outportb(sb.base+DSP_RESET, 1);
	for(i=0; i<20000; i++) { inportb(0x80); }
	outportb(sb.base+DSP_RESET, 0);
	for(i=0; i<10000; i++) { inportb(0x80); }
	printk("\r                                          \r");
 
	// Get status
	i=sb_read_dsp();

	//
	printk("%s: After reset, DSP_READ = 0x%x\n",	__FUNCTION__, i);
	if(i!=0xAA)
	{
		// Not found
		return 1;
	}

	//
	return 0;
}

// SB interrupt handler
void sb_irqhandler(void)
{
	char *scr = (0xB8000+(80*2));
	int i;

	// Let's play if desired so
	//if(sb.isPlaying)
	sb.irqTick++;
	*scr++;
	sb_start_playing();

	// Clear interrupt.
	inportb(sb.base + DSP_DATA_AVAIL);

	// Flag IRQ.
	outportb(0x20, 0x20);
	outportb(0xA0, 0xA0);
}

// Begin playing
// - inputs 16K of audio data from the buffer
// - starts SB DMA
void sb_start_playing(void)
{
	// Start/continue SB playing.
	sb_play_sample(audio.length, audio.frequency);
	sb_get_sample(audio.length);

	// Swap between buffers.
	buffer_swap ^= 1;
}



