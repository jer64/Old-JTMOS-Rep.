#ifndef __INCLUDED_SB_H__
#define __INCLUDED_SB_H__

// 256K should be enough.
#define SB_FIFO_BUFFER_SIZE 256*1024

//
#define WHEREAMI	ReportMessage("File %s, function %s, line %d.\n", __FILE__, __FUNCTION__, __LINE__);

//
#include "kernel32.h"
#include "macrodef.h"
#include "int.h"
#include "pit.h"
#include "keyb.h"
#include "syssh.h"
#include "cmos.h"
#include "floppy.h"
#include "buildnr.h"
#include "buffer.h" // FBUF

/* 
 * Mixer registers of SB Pro
 */
#define VOC_VOL		0x04
#define MIC_VOL		0x0A
#define MIC_MIX		0x0A
#define RECORD_SRC	0x0C
#define IN_FILTER	0x0C
#define OUT_FILTER	0x0E
#define MASTER_VOL	0x22
#define FM_VOL		0x26
#define CD_VOL		0x28
#define LINE_VOL	0x2E
#define IRQ_NR		0x80
#define DMA_NR		0x81
#define IRQ_STAT	0x82
#define OPSW		0x3C
// Cool controls
#define TREBLE_L	0x44
#define TREBLE_R	0x45
#define BASS_L		0x46
#define BASS_R		0x47
// Beep! Beep!
#define SPEAKER_VOL	0x3B // 00..C8


#define FREQ_HI         (1 << 3)/* Use High-frequency ANFI filters */
#define FREQ_LOW        0	/* Use Low-frequency ANFI filters */
#define FILT_ON         0	/* Yes, 0 to turn it on, 1 for off */
#define FILT_OFF        (1 << 5)

#define MONO_DAC	0x00
#define STEREO_DAC	0x02

// DSP PORTS
#define DSP_RESET       0x6
#define DSP_READ        0xA
#define DSP_WRITE       0xC
#define DSP_COMMAND     0xC
#define DSP_STATUS      0xC
#define DSP_DATA_AVAIL  0xE
#define DSP_DATA_AVL16  0xF
#define MIXER_ADDR      0x4
#define MIXER_DATA      0x5
#define OPL3_LEFT       0x0
#define OPL3_RIGHT      0x2
#define OPL3_BOTH       0x8

// DSP COMMANDS
#define DSP_CMD_SPEAKERON       0xD1
#define DSP_CMD_SPEAKEROFF      0xD3
#define DSP_CMD_DMAPAUSE	0xD0
#define DSP_CMD_DMARESUME	0xD4

//
typedef struct
{
	// 0x220,5,1
	WORD base,irq,dma;
	int version;
	char mixer;
	int length,frequency;
	// Is playing audio?
	int isPlaying;
	// Increase in this variable indicates that an interrupt has occured.
	int irqTick;
}SB;

// Define function prototypes
int sb_write_dsp(BYTE);
int sb_read_dsp(void);
int sb_get_version(void);
void sb_configure(int,int,int);
int sb_init(int);
int sb_reset(void);
int sb_init1(int,int,int,int);
void sb_set_mixer_reg(int,int);
int sb_get_mixer_reg(int);
void sb_get_sample(int length);
int sb_play_sample(WORD,WORD);
void sb_turn_speaker_on(void);
void sb_turn_speaker_off(void);
int sb_init_(int);

// Wave functions
void sb_direct_output(int amount);

// Volume controls
void sb_set_master_volume(int amount);
void sb_set_dsp_volume(int amount);
void sb_set_fm_volume(int amount);
void sb_set_cd_volume(int amount);
void sb_set_line_volume(int amount);

//
WORD sb_getplayposition(unsigned char _dmanumber);

// from sb_interrupt.asm
extern void sb_ISR(void);

// default SB IRQ handler
void sb_irqhandler(void);

//
int sb_set_treble(int power); // 0-15
int sb_set_bass(int power);
void sb_start_playing(void);
void sbProcess(void);
void init_sbThread(void);

//
#endif

//

