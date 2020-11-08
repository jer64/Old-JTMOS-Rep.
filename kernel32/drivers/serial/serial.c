// ===========================================================
// RS232 - SERIAL PORT DRIVER FOR COM1, COM2, COM3, COM4, ETC.
//
// (C) 2002-2008 by Jari Tuominen(jari@vunet.org)
//        SEE HELPPC FOR EXCELLENT UART DOCUMENTATION!
// Added interrupt functionallity and FIFO buffer.
//
// SERIAL MOUSE DRIVER MUST BE DISABLED BEFORE USING
// THE SERIAL PORT FOR OTHER PURPOSES.
// ===========================================================
/*
	Valid I/O addresses for serial ports are(in std. order):
	0x3F8, 0x3E8, 0x2F8, 0x2E8

	(Add to base I/O, base can be e.g. 0x3F8)
	+0	W	Transmitter Holding Buffer
		R	Receiver Buffer
		RW	Divisor latch low byte
	+1	RW	Interrupt Enable Register
		RW	Divisor Latch High Byte
	+2	R	Interrupt Identification Register
		W	FIFO Control Register
	+3	RW	Line Control Register
	+4	RW	Modem Control register
	+5	R	Line Status Register
	+6	R	Modem Status Register
	+7	RW	Scratch Register
 */
//
#include "basdef.h"
#include "sysmem.h"
#include "kernel32.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h" // readblock,writeblock,getsize,getblocksize, ...
#include "zero.h"
#include "buffer.h" // FBUF
//
#include "serial.h"
#include "serial_dev.h"

// Serial driver is initialized ?
int serialInitialized=FALSE;
int primitiveSerial=FALSE;

//----------------------------------------------------------------
// Buffers for COM1 and COM2
FBUF comportR[4]; // incoming data buffer(read)
FBUF comportW[4]; // outcoming data buffer(write)
// Serial flusher thread(serial thread)
THREAD SerialThread;
// Serial port I/O configuration
SERIALPORT serialport[8];
/*
	COM1	0x3F8
	COM2	0x2F8
	COM3	0x3E8
	COM4	0x2E8

// Interrupt Enable Register            =====> I don't understand sense of
RCV,XMT,LS,MS, these terms do not appear in the documentation of my chips
specification in the "Interrupt Enable Register" Options
#define IER_RCV 0x01
#define IER_XMT 0x02
#define IER_LS  0x04
#define IER_MS  0x08
*/

//
int bios_serial_ports=0;
int sline_speed[10]={0};

// Creates serial thread.
void init_SerialThread(void)
{
        //
        ThreadCreateStack(&SerialThread,1024*16);
        SerialThread.pid =
                create_thread(SerialThread.stack,
                        SerialThread.l_stack, SerialThreadProgram);

        // max. 31 characters
        IdentifyThread(SerialThread.pid, "KSERIALIO");
}

// Serial port write flusher(serial thread).
void SerialThreadProgram(void)
{
	int i,d1;

	// Loop & flush & schedule
	while(1)
	{
		// COM1, COM2
		for(i=0; i<2; i++)
		{
			// Read write buffer and write it to I/O
			while( (d1 = GetBuffer(&comportW[i]))!=-1 )
			{
				// Flush to port
				serial_put1(serialport[i].ad, d1);
			}
		}

		//
		SwitchToThread();
	}
}

//----------------------------------------------------------------------------------------------
//
// IRQ4 handler.
//
void serial_irqhandler4(void)
{
	// COM1
	serial_handlerirq(0);
}

//----------------------------------------------------------------------------------------------
//
// IRQ3 handler.
//
void serial_irqhandler3(void)
{
	// COM2
	serial_handlerirq(1);
}

//----------------------------------------------------------------------------------------------
//
void serial_handlerirq(int whichport)
{
	static char str[8192];
	static int d1;

/*	// Get data from COM1
	while(1)
	{
		//
		if( (d1=serial_get1(serialport[whichport].ad))!=-1 )
		{
			//
		//	printk("%c", d1);
			writetext(&screens[0], "-SerialInt-");
			PutBuffer(&comportR[whichport], d1);
			continue;
		}
		break;
	}*/

	//
	while(  (d1 = serial_get1(serialport[whichport].ad)) != -1 )
	{
		/*sprintf(str, "[COM%d %c]", whichport+1, d1);
		writetext(&screens[0], str);*/
		PutBuffer(&comportR[whichport], d1);
	}

	//
	outportb(0x20, 0x20);
}

// TODO
void serial_biosdetect(void)
{
	int *p;
	int i,i2,i3,i4;

	//
	p=0x400;

	//
	for(i=0; i<4; i++)
	{
		//
	}
}

// Get specified serial line current speed
// (in bps)
void GetSerialLineSpeed(int nCom)
{
	return sline_speed[nCom];
}

// Set specified serial line speed
// (in bps)
void SetSerialPortSpeed(int nCom, int bps)
{
	//
	sline_speed[nCom] = bps;

	//
	//|7|6|5|4|3|2|1|0|  2FC, 3FC  Modem Control Register
	// | | | | | | | `---- 1 = activate DTR
	// | | | | | | `----- 1 = activate RTS
	// | | | | | `------ OUT1
	// | | | | `------- OUT2
	// | | | `-------- 0 = normal, 1 = loop back test
	// `------------- reserved (zero)
	init_serial_portEx(serialport[nCom].ad,
		bps, 0x03, DEF_SERFIFO, 1);
}

// init_serial_portEx(ad, 0x01, 0x03, DEF_SERFIFO, 0x0b);
void init_serial_portEx(DWORD ad, int baudRate, 
			int transferType, int fifoSettings, int misc1)
{
	//------------------------------------------------------
	// SET DLAB ON (change higher registers)
	outportb(ad+3, 0x80);
	// Set Baud rate - Divisor Latch Low Byte
	//outportb(ad+0, SERIAL_9600BPS);
	outportb(ad+0, 1843200 / (baudRate * 16));
	// Set Baud rate - Divisor Latch High Byte
 	outportb(ad+1, 0x00);
 	// We get DLAB back here, and set transfer type:
	outportb(ad+3, transferType); 
 	//   Port 3FA - 16550 FIFO Control Register - FCR  (write only)
	// 
	// .7.6.5.4.3.2.1.0.  2FA, 3FA  FIFO Control Register
	//  . . . . . . . ..... 1 = enable clear XMIT and RCVR FIFO queues
	//  . . . . . . ...... 1 = clear RCVR FIFO
	//  . . . . . ....... 1 = clear XMIT FIFO
	//  . . . . ........ 1 = change RXRDY & TXRDY pins from mode 0 to mode 1
	//  . . ........... reserved (zero)
	//  .............. trigger level for RCVR FIFO interrupt
	// 
	// Bits      RCVR FIFO
	//  76     Trigger Level
	//  00        1 byte
	//  01        4 bytes
	//  10        8 bytes
	//  11       14 bytes
	// FIFO Control Register
	outportb(ad+2, fifoSettings);
	//  Turn on DTR, RTS, and OUT2
	// |7|6|5|4|3|2|1|0|  2FC, 3FC  Modem Control Register
	//  | | | | | | | `---- 1 = activate DTR
	//  | | | | | | `----- 1 = activate RTS
	//  | | | | | `------ OUT1
	//  | | | | `------- OUT2
	//  | | | `-------- 0 = normal, 1 = loop back test
	//  `------------- reserved (zero)
	outportb(ad+4, misc1);
	// Turn ON/OFF interrupts
	// .7.6.5.4.3.2.1.0.  2F9, 3F9: Interrupt Enable Register
	//  . . . . . . . ..... 1 = enable data available int (and 16550 Timeout)
	//  . . . . . . ...... 1 = enable THRE interrupt
	//  . . . . . ....... 1 = enable lines status interrupt
	//  . . . . ........ 1 = enable modem-status-change interrupt
	//  ............... reserved (zero)
	// 
	// - 16550 will interrupt if data exists in the FIFO and isn't read
	//   within the time it takes to receive four bytes or if no data is
	//   received within the time it takes to receive four bytes.
	outportb(ad+1, 1); // 1 = Enable data available int
}

// [PORT ADDRESS] [WHICH COM PORT]
void init_serial_port(DWORD ad, int which)
{
	// Port address, Baud rate.
	// [8 Bits, No Parity, 1 Stop Bit], FIFO, [DTR, RTS, OUT2]
	init_serial_portEx(ad, SERIAL_LINESPEED_BPS, 0x03, DEF_SERFIFO, 0x0B);

	// Init buffer, 16K space.
	CreateBuffer(&comportR[which], 1024*16); // incoming
	CreateBuffer(&comportW[which], 1024*16); // outcoming
}

// Setup IRQ handler
void serialSetupIRQ(void)
{
	// Setup handler for IRQ4 (COM1/COM3)
	setint(M_VEC+4,
		serial_asmirqhandler4,
		GIMMIE_CS(),
		D_PRESENT + D_INT + D_DPL3);
	enable_irq(4);

	// Setup handler for IRQ3 (COM2/COM4)
	setint(M_VEC+3,
		serial_asmirqhandler3,
		GIMMIE_CS(),
		D_PRESENT + D_INT + D_DPL3);
	enable_irq(3);
}

// Define serial port addresses
// (using defaults, may not be these always!!)
void serial_init1(void)
{
	// Define default serial port addresses
	if(bios_data.com1_addr != 0)
	{
		serialport[0].ad =	bios_data.com1_addr;
		serialport[0].irq =	4;
		serialport[1].ad =	bios_data.com2_addr;
		serialport[1].irq =	3;
		serialport[2].ad =	bios_data.com3_addr;
		serialport[2].irq =	4;
		serialport[3].ad =	bios_data.com4_addr;
		serialport[3].irq =	3;
	}
	else
	{
		// defaults
		// 0x3F8, 0x3E8, 0x2F8, 0x2E8
		serialport[0].ad =	0x03F8;
		serialport[0].irq =	4;
		serialport[1].ad =	0x03E8;
		serialport[1].irq =	3;
		serialport[2].ad =	0x02F8;
		serialport[2].irq =	4;
		serialport[3].ad =	0x02E8;
		serialport[3].irq =	3;
	}

	// Declare primitive serial functions functional
	primitiveSerial = TRUE;
}

// Init port settings, buffers, install IRQ handler, etc.
void serial_init(void)
{
	//--------------------------------------------------------------------------------------
	//
	// OK to re-entry due to persistant initialization reasons.
/*	if(serialInitialized)
	{
		//
		//printk("Serial port UART driver already installed.\n");

		// Oops! Can't do this again.
		return;
	}*/

	//--------------------------------------------------------------------------------------
	//
	printk("serial.c - Serial Port UART Management Driver\n");
	serial_init1();
	if(Multitasking)
		serial_register_device();
	serial_biosdetect();

	//
	serialInitialized = TRUE;

	//
	init_serial_port( serialport[0].ad, 0 );
	init_serial_port( serialport[1].ad, 1 );
	init_serial_port( serialport[2].ad, 2 );
	init_serial_port( serialport[3].ad, 3 );

	// Setup IRQ-handler
	if(Multitasking)
		serialSetupIRQ();

	//	
	printk("Serial I/O configuration: COM1=%x, COM2=%x, COM3=%x, COM4=%x\n",
		serialport[0].ad, serialport[1].ad,
		serialport[2].ad, serialport[3].ad);

	// Start serial port flusher thread
	// (recommended to be done as last thing on the init)
	if(Multitasking)
		init_SerialThread();
}

// Zero:	Success
// Non-zero:	Failure(TIMEOUT)
int serial_wait_ctsend(int base)
{
	//
	int i,i2;

	//
	for(i2=0; ; i2++)
	{
		// Check "Clear to Send" from modem status
		if( (inportb(base+SERIAL_MODEM_STATUS)&16) )return 0;

		// CPU usage moderator
		if(i2>SERIAL_CPUMODER){ SwitchToThread(); i2=0; }
	}
	return 1;
}

//
void serial_print_out_status_on_error(int base)
{
/*
Overrun Error

An "overrun error" occurs when the UART cannot process the character that just came in before the next one arrives. Various UART devices have 
differing amounts of buffer space to hold received characters. The CPU must service the UART in order to remove characters from the buffer. If the CPU 
does not service the UART quickly enough and the buffer becomes full, an Overrun Error will occur.

Framing Error

A "framing error" occurs when the designated "start" and "stop" bits are not valid. As the "start" bit is used to identify the beginning of an 
incoming character, it acts as a reference for the remaining bits. If the data line is not in the expected idle state when the "stop" bit is expected, 
a 'a'Framing Error will occur.

Parity Error

A "parity error" occurs when the number of "active" bits does not agree with the specified parity configuration of the UART, producing a Parity Error. 
Because the "parity" bit is optional, this error will not occur if parity has been disabled. Parity error is set when the parity of an incoming data 
character does not match the expected value.

Break Condition

A "break condition" occurs when the receiver input is in at the "space" level for longer than some duration of time, typically, for more than a 
character time. This is not necessarily an error, but appears to the receiver as a character of all zero bits with a framing error.

Some equipment will deliberately transmit the "break" level for longer than a character as an out-of-band signal. When signalling rates are 
mismatched, no meaningful characters can be sent, but a long "break" signal can be a useful way to get the attention of a mismatched receiver to do 
something (such as resetting itself. UNIX systems and UNIX-like systems such as Linux can use the long "break" level as a request to change the 
signalling rate.

	|7|6|5|4|3|2|1|0|  2FD, 3FD Line Status Register
1	 | | | | | | | `---- 1 = data ready
2	 | | | | | | `----- 1 = overrun error (OE)
4	 | | | | | `------ 1 = parity error (PE)
8	 | | | | `------- 1 = framing error (FE)
16	 | | | `-------- 1 = break interrupt  (BI)
32	 | | `--------- 1 = transmitter holding register empty (THRE)
64	 | `---------- 1 = transmitter shift register empty (TSRE)
128	 `----------- 1 = 16550 PE/FE/Break in FIFO queue, 0 for 8250 & 16450
*/

	//
	if( (inportb(base+SERIAL_LINE_STATUS)&8) )
	{
		printk("%s: framing error\n", __FILE__);
	}

	//
	if( (inportb(base+SERIAL_LINE_STATUS)&4) )
	{
		printk("%s: parity error\n", __FILE__);
	}

	//
	if( (inportb(base+SERIAL_LINE_STATUS)&2) )
	{
		printk("%s: overrun error\n", __FILE__);
	}

	//
}

//
int serial_wait_transmitter(int base)
{
	//
	int i,i2;

	//
	for(i=GetTickCount(),i2=0; GetTickCount()<(i+1000); i2++)
	{
		// Wait until "Empty Transmitter Holding Register" is non-zero.
		if( (inportb(base+SERIAL_LINE_STATUS)&32) )return 0;

		//
		serial_print_out_status_on_error(base);

		// CPU usage moderator
		if(i2>SERIAL_CPUMODER){SwitchToThread(); i2=0;}
	}
	return 1;
}

//
int serial_directget(int i)
{
	//
	return serial_get1(serialport[i].ad);
}

//
int serial_directput(int i, int d1)
{
	//
	return serial_put1(serialport[i].ad, d1);
}

// TEMP FUNC
int serial_put1(int base, int c)
{
	int x;
	static int write_counter = 0;

	//
	if(!primitiveSerial)
		return 0;

	// Loop until we're ready to send(high intensivity)
	for(x=0; (inportb(base+5)&0x20)==0; x++)
	{
		//
		if(x>100000)
		{
			//
			x=0;
			
			//SwitchToThread();
		}
	}
	outportb(base, c);

	//
	return 0;
}

// Zero		OK
// Non-zero	Failure
int serial_put2(int base, int c)
{
	int tries,rv;

	for(tries=0,rv=0; tries<2; tries++)
	{
		// Wait until ready, if ok => Send.
		if( !serial_wait_ctsend(base) )
		{
			if( !serial_wait_transmitter(base) )
			{
				outportb(base, c);
				return 0;
			}
			else rv=1;
		}
		else
			rv=1;
	}
	return rv;
}

// Returns -1 if no data available.
int serial_get1(int base)
{
	DWORD x;

	// Do we have data available?
	if( (inportb(base+5)&1) )
		// Yes data
		return inportb(base);
	else
		// No data
		return -1;
}

//
int serial_put(int portnr, int c)
{
	// Write to outgoing buffer.
	return PutBuffer(&comportW[portnr&3], c);
//	return serial_put1(serialport[portnr&3].ad, c);
}

//
int serial_get(int portnr)
{
	// Read from incoming buffer.
	return GetBuffer(&comportR[portnr&3]);
	///////////////////////////return serial_get1(serialport[portnr&3].ad);
}

//
void serial_puts(int portnr, const char *s)
{
	//
	int i,l;

	//
	l = strlen(s);
	for(i=0; i<l; i++)
	{
		serial_put(portnr, s[i]);
	}
}

// Returns zero on success, non-zero no error.
int WaitSerialString(int portNr, const char *strLogin)
{
	//
	int i,i2,i3,i4,t,ch;

	// Wait for "login:"
	for(t=rtc_getseconds(),i=0; (rtc_getseconds()-t)<4; )
	{
TryAgain:
		//
		ch = serial_get(portNr);
		SwitchToThread();
		if(ch==-1)goto TryAgain;
		if(ch==strLogin[i]) i++; else i=0;
		if( i>=strlen(strLogin) ) goto notim1;
	}
	return 1;
notim1:
	return 0;
}

//

