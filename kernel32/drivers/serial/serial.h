//
#ifndef __INCLUDED_SERIAL_H__
#define __INCLUDED_SERIAL_H__

//
#include "basdef.h"
#include "buffer.h" // FBUF

// Option
#define DISABLE_FIFO	& (~1)
// (Last bits 7,6 define FIFO buffer size/triggerer level)
// Bits      RCVR FIFO
// (serial port base+2, write)
//  76     Trigger Level
//  00        1 byte
//  01        4 bytes
//  10        8 bytes
//  11       14 bytes
// 0xC7	=> 14 bytes FIFO (DOES NOT WORK)
// 0x87 => 8 bytes FIFO (OK)
// 0x47 => 4 bytes FIFO (OK)
// 0x07	=> 1 bytes FIFO (DOES NOT WORK)
// Bit 0:  0=FIFO disabled, 1=FIFO enabled
// Bit 6,7: FIFO RCV buffer length
#define DEF_SERFIFO	((0xC7)&0xFF)

// 0x07 also doesn't work (corruption...)	1 bytes
// 0x47 seems to work without data corruption*	4 bytes
// 0x87 seems to work without data corruption*	8 bytes
// 0xc7 doesn't work (causes corruption)	14 bytes
// * although in the login prompt of my Linux shell
// there was some corruption, but that might've occured
// because of terminal program which doesn't fully
// understand the VT220/etc. codes.

/*
^Baud Rate Divisor Table

%		      Baud Rate			     Baud Rate
%    Baud Rate	       Divisor	       Baud Rate      Divisor

	  50		900h		  2400		30h
	 110		417h		  3600		20h
	 150		300h		  4800		18h
	 300		180h		  7200		10h
	 600		 C0h		  9600		0Ch
	1200		 60h		 19200		06h
	1800		 40h		 38400		03h
	2000		 3Ah		115200		01h

	- Baud rate divisors can be calculated by taking the oscillating
	  frequency (1,843,200) and dividing by the quantity of the
desired
	  baud rate times the UART clocking factor (16).  Use the
following
	  formula:
^divisor = 1843200 / (BaudRate * 16);
*/

// 0x01 = 115,200 bps
// 0x02 = 56,700 bps
// 0x03 = 38,400 bps
// 0x0C = 9,600 bps
// 0x30 = 2,400 bps
#define SERIAL_TIMEOUT_VALUE	100

// Larger for faster connection
// 9600 bps:
//#define SERIAL_CPUMODER		64*2
// 38400 bps:
#define SERIAL_CPUMODER		64*6

#define SERIAL_1200BPS		0x60 // For serial mouses
#define SERIAL_2400BPS		0x30
#define SERIAL_9600BPS		0x0c
#define SERIAL_38400BPS		0x03
#define SERIAL_56700BPS		0x02
#define SERIAL_115200BPS	0x01
#define SERIAL_MOUSE_RATE	SERIAL_1200BPS
#define SERIAL_VERYSLOW		SERIAL_2400BPS
#define SERIAL_SLOW		SERIAL_9600BPS
#define SERIAL_MEDIUM		SERIAL_38400BPS
#define SERIAL_FAST		SERIAL_115200BPS
// Defaults to 1200 BPS for serial mouses
#define SERIAL_LINESPEED_BPS	115200
//#define SERIAL_LINESPEED	SERIAL_1200BPS

//
#define SERIAL_RECBUF			0
#define SERIAL_INTENABLE		1
#define SERIAL_FIFO_CONTROL		2
#define SERIAL_INTIDENT			2
#define SERIAL_LINE_CONTROL		3
#define SERIAL_MODEM_CONTROL		4
#define SERIAL_LINE_STATUS		5
#define SERIAL_MODEM_STATUS		6
#define SERIAL_SCRATCH_REGISTER		7

//
typedef struct
{
	// Is it working?
	char isWorking;
	// I/O address
	int ad;
	// IRQ
	int irq;
}SERIALPORT;

//
extern SERIALPORT serialport[8];

//
void serial_init(void);
// portnr = 0-3(0=COM1, 1=COM2, ...)
// serial_get returns -1 if no data available
int serial_get(int portnr);
int serial_get1(int base);
int serial_put(int portnr, int c);
void serial_puts(int portnr, const char *s);
void init_serial_portEx(DWORD ad, int baudRate,
                        int transferType, int fifo1, int misc1);
int WaitSerialString(int portNr, const char *strLogin);
void SetSerialPortSpeed(int nCom, int bps);

// Asm.IRQ-handler
extern void serial_asmirqhandler4(void);
extern void serial_asmirqhandler3(void);
void serial_irqhandler4(void);
void serial_irqhandler3(void);
void serial_handlerirq(int);
void SerialThreadProgram(void);
void serial_init1(void);

//
extern FBUF comportR[4]; // incoming data buffer(read)
extern FBUF comportW[4]; // outcoming data buffer(write)
extern int primitiveSerial;
int serial_directget(int i);
int serial_directput(int i, int d1);

//
#endif

//
