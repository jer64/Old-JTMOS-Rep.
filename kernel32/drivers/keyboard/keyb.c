/**************************************************
 * keyb.c - Keyboard driver version 1.3           *
 * (C) 2002-2005 by Jari Tuominen.                *
 **************************************************/
//
////#define __KEYDEBUG__
//#define __KBIRQDEBUG__
//
#include <kernel32.h>
#include <cpu.h>
#include <io.h>
#include <keyb.h>
#include <sysglvar.h>
#include <ega.h>
#include <int.h>
#include <vga.h>
#include <scanasc.h>
//
#include <driver_sys.h> // driver_register_new_device, ...
#include <driver_api.h>
//
#include <ps2mouse.h>

//
int syskey_terminate=0;

// Key status.
KEYS keys;

//
KBSTR kbstr;

// Get CTRL key status.
int GetCTRL(void)
{
	return keys.ctrl;
}

// Get ALT key status.
int GetALT(void)
{
	return keys.alt;
}

// Get CTRL/ALT.
void HandleStickKeys(void)
{
	// Get status
	keys.ctrl =	kbstr.ktab[29];
	keys.alt =	kbstr.ktab[56];
}

//
// void set_vector(void *handler,
//            unsigned char interrupt, unsigned short control_major)
//
int InitKeyboard()
{
	//
	int retval,i;

	//
	printk("Keyboard driver 0.2.0 by Jari Tuominen, Gareth Owen.\n");

	// Read all remainin keys
	printk("kbinit 1");
	for(i=0; i<1000; i++)	inportb(0x60);

	// Reset whole structure, fill with zeroes
	printk("2");
	memset(&kbstr,0,sizeof(KBSTR));

	// Some space for the macro
	printk("3");
	kbstr.macro = malloc(256);
	// 64K for keyboard defs
	kbstr.ktab = malloc(1024*64);
	// allocate data for the keyboard buffer
	printk("4");
	kbstr.buf = malloc(KEYBOARD_BUFFER_LENGTH*2+100);
	// allocate data for the ascii keyboard buffer
	kbstr.abuf = malloc(KEYBOARD_BUFFER_LENGTH*2+100); 
	printk("5");
	// set position to 0
	kbstr.i=0;
	// set ascii buffer's position to 0
	kbstr.ai=0;
	kbstr.i_macro=0;
	kbstr.counter=0;
	kbstr.place=0;
	//
	strcpy(kbstr.macro, "");

	//
	printk("6");
	setint(M_VEC+1, // M_VEC+1 orig.
		keyb_ISR,
		GIMMIE_CS(), 
		D_PRESENT + D_INT + D_DPL3);

	//
	printk("7");
	enable_irq(1);

	//
	printk("8");
	kb_hwinit();

	//
//	kbstr.led_status = 0;
//	setleds();
	
	// Finally register keyboard as a valid device.
	printk("9");
	keyb_register_device();

	//
	printk("\n");
	return retval;
}

//
void kb_outbuf(int akey)
{
	// Zero code?
	if(!akey)return 0;

	// Store if space left at the buffer.
	if(kbstr.ai<KEYBOARD_BUFFER_LENGTH)
	{
		kbstr.abuf[kbstr.ai]=akey;
		kbstr.ai++;
		kbstr.abuf[kbstr.ai]=0;
		//if(kbstr.ExtraKey)goto morekeys;
	}
}

// Converts scancodes to more practically useful ASCII codes.
// F.e. getch returns only ASCII codes.
//
int kb_output_asc_buffer(int key)
{
	int akey;

	//
	if(key==-1 || key==0)return 0;

	//
	if(kbstr.abuf[0]==0){kbstr.ai=0;}

	//
	if(kbstr.ai>=KEYBOARD_BUFFER_LENGTH)
	{
		kbstr.ai=0;
	}

	// CONVERT SCANCODE TO ASCII
	// Also gives a character from possible macro string, etc.
morekeys:
	akey=scan_to_asc(key);

	if(kbstr.ExtraKey==0xFF || kbstr.ExtraKeyCode)
	{
		akey=kbstr.ExtraKeyCode;
		kbstr.ExtraKey=0;
		kbstr.counter=0;
		kbstr.ExtraKeyCode=0;
	}

	if(akey)		kb_outbuf(akey);
	if(kbstr.ExtraKey)	goto morekeys;

	//
	return 0;
}

//
void incmark(void)
{
	char *buf;

	//
	buf=0xb8000;

	//
	buf[0]++;
}

//
char getmark(int offs)
{
	char *buf;
	buf=0xb8000;
	return buf[offs];
}

//
void storemark(int which,int offs)
{
	char *buf;
	buf=0xb8000+offs;
	buf[0]=which;
}

// Terminal getch1
int getch1(void)
{
	int key;
	SCREEN *s;

	// Get terminal
	s = &screens[GetThreadTerminal(GetCurrentThread())];

	// Input keyboard
	key = PopBufferW(&s->key);

	// Filter input data
	if(key==-1)
		return 0;
	else
		return key;
}

// Redirection of keyboard stream
// to appropriate terminal.
void RedirectKeyboardStream(void)
{
	int t,key;
	SCREEN *s;

	// Get terminal
	t = visible_terminal;

	// Get terminal
	s = &screens[t];

	// Output all keys to the terminal's
	// keyboard buffer.
	while( (key=directgetch())>=1 )
	{
		//
		PushBufferW(&s->key, key);
	}
}

//
// Keyboard Interrupt Handler
//
void keyb_handler(void)
{
	BYTE key,status;

	//--------------------------------------------------------
	// Check for errors:
	status = inportb(0x64);

	// Key detection error or *buffer **full* ?
	if( !status )
	{
		// Commence keyboard reset:
		printk("\nkeyb.c: KEYBOARD BUFFER FULL!\n");
		kbcmd(0xff);
		goto skip;
	}

	//
	if( status==0xfc
		||
	    status==0xfd )
	{
		printk("\nkeyb.c: keyboard error 0xfc/0xfd\n");
		goto skip;
	}

	// Power-on/reset diagnostics successful ?
	if( status==0xaa )
	{
		printk("\nkeyb.c: Keyboard detected.\n");
		kbstr.diagnosticsOK = 1;
		goto skip;
	}

	//
#ifdef __KBIRQDEBUG__
	incmark();
#endif

	//--------------------------------------------------------
	// Get key
	key=inportb(0x60);

	// Key released?
	if( (key&0x80)==0x80 )
	{
		// Release the key
		kbstr.ktab[key&127]=0;
		kbstr.rkey=key&127;
	}
	else
	{
		// mark key on keyboard table
		kbstr.ktab[key&127]=1;
  
		//
		translate_keyboard_situation();
  
		// output to raw keyboard buffer
		if(kbstr.i<KEYBOARD_BUFFER_LENGTH)
		{
			kbstr.key_pressed=1;
			//kbstr.buf[kbstr.i]=key;
			//kbstr.i++;
			kb_output_asc_buffer(key);
		}
	}

skip:
	//--------------------------------------------------------
	// Handle ALT/CTRL
	HandleStickKeys();

	//--------------------------------------------------------
	// Redirect keyboard stream to the currently
	// visible terminal.
	RedirectKeyboardStream();

	//--------------------------------------------------------
	outportb(0x20,0x20);

	//
	return;
}

// Output string to current terminal's keyboard buffer
int WriteKeyboard(const char *s)
{
	int l,i=0;

	//
	l = strlen(s);
	for(i=0; i<l; i++)
	{
		//
		OutputKeyboard(s[i]);
	}
	return 0;
}

// Output terminal's keyboard buffer
int OutputKeyboard(int ch)
{
	SCREEN *s;

	// Get terminal
	s = &screens[GetThreadTerminal(GetCurrentThread())];

	// Output keyboard
	PushBufferW(&s->key, ch);
	return 0;
}

//
int translate_keyboard_situation(void)
{
	// See removed.txt.
	return 0;
}

//
int keyb_device_call(DEVICE_CALL_PARS)
{
	switch(n_call)
	{
		//
		case DEVICE_CMD_INIT:
		break;
		//
		case DEVICE_CMD_SHUTDOWN:
		break;
		//
		case DEVICE_CMD_READBLOCK:
		break;
		//
		case DEVICE_CMD_WRITEBLOCK:
		break;
		//
		case DEVICE_CMD_GETSIZE:
		break;
		//
		case DEVICE_CMD_GETBLOCKSIZE:
		break;
		//
		case DEVICE_CMD_SEEK:
		break;
		//
		case DEVICE_CMD_GETCH:
		break;
		//
		case DEVICE_CMD_PUTCH:
		break;
		//
		case DEVICE_CMD_TELL:
		break;
		//
		default:
		break;
	}

	//
	return 0;
}

//
int keyb_register_device(void)
{
	//
	if( driver_getdevicebyname("keyb") )	return 1;

	//
	driver_register_new_device( "keyb", keyb_device_call, DEVICE_TYPE_CHAR );

	//
	return 0;
}

//
int kbwaitstatus(DWORD value,DWORD mask)
{
	//
	DWORD i,i2;

	//
	for(i=0; i<PS2MOUSE_TIMEOUT; i++)
	{
		if( (inportb(0x64)&mask)==value )return 0;
		inportb(0x80);
		for(i2=0; i2<10; i2++)nop();
	}

	//
	return 1;
}

//
int kbread(void)
{
	//
	static int i,i2,i3,i4,d,d2;
 
	//
	kbwaitdata();

	// key_read_ready
	for(i=0; i<64; i++)nop();

	//
	d2=inportb(0x60);
	for(i=0; i<16; i++)nop();

	//
	return d2;
}

// non-zero return value is an error
int kbwrite(int value)
{
	//
	static long i,i2,i3,i4,d;
 
	// When using PS/2 mouse we should wait here
	// for kbwaitmousebufferfull, so
	// it should be manually called before using
	// this function when commanding PS/2 mouse.

	//
	kbwait();

	// write the data
	outportb(0x60,value); for(i4=0; i4<10; i4++)inportb(0x80);
 
	//
	kbwait();

	//
	return 0;
}

// non-zero will be an error
int kbcmd(int cmd)
{
	//
	long i;
 
	//
	kbwait();
 
	// cmd_send
	outportb(0x64,cmd);
	for(i=0; i<20; i++)nop();
 
	//
	kbwait();

	//
	return 0;
}

//
void kbwait(void)
{
	long i;

	for( i=glvar.timer[0]; (inportb(0x64) & 2); )
	{
		if( (glvar.timer[0]-i)>200 )
		{
			print("syskeyb.c/kbwait: error, timeout occured\n");
			break;
		}
	}
}

// Waits until readable data is available
void kbwaitdata(void)
{
	//
	long i,i2;

	//
	for( i=glvar.timer[0],i2=0; !(inportb(0x64) & 1); i2++)
	{
		//
		inportb(0x80);
		//
		if(i2>10000)break;
		//
		if( (glvar.timer[0]-i)>200 )
		{
			printk("keyb.c/kbwaitdata: error, timeout occured\n");
			break;
		}
	}
 // 
}

// Attempt to set keyboard leds.
// new: now with timeout(doesn't hang system)
void setleds(void)
{
	//
	setpal(0, 63,63,63);
	outportb(0x60, 0xED);
	kbwait();
	outportb(0x60, kbstr.led_status);
	kbwait();
	setpal(0, 0,0,0);

	//
	return;
}

// returns non-zero on error
int kb_hwinit(void)
{
	//
	int d;

	// clear keyboard test variable
	kbstr.kb_test_ok=0;

	// Reset *keyboard* CPU, do power-on self-test,
	// return self-test result byte.
	if(kbwrite(0xFF))return 1;
	// Clears output *buffer*, enables *keyboard*.
	if(kbwrite(0xF4))return 2;

	//
	return 0;
}

// getch
//
// I don't know if keyb.c is the best place to store
// this getch() function, but let's think it's a
// inside kernel thing, applications will be not
// accessing it directly, instead via the interface.
//
int getch()
{
	int key;

	//
	while( !(key=getch1()) )
	{
		// Save some CPU time
		SwitchToThread();
	}
	return key;
}

// Direct input a char from keyboard
int directgetch()
{
	//
	int key;
	int i;
	
	// Buffer already empty?
	if( !kbstr.ai )return 0;

/*	// Check that the data is for us
	if( thread_terminal[GetCurrentThread()]!=visible_terminal )
	{
		return 0;
	}*/

	//
	key=kbstr.abuf[0];
	kbstr.ai--;
	kbstr.i--;
	
	//
	for(i=0; i<KEYBOARD_BUFFER_LENGTH; i++)
	{
		kbstr.buf[i]=kbstr.buf[i+1];
		kbstr.abuf[i]=kbstr.abuf[i+1];
	}

	//
	return key;
}

//
int kbhit()
{
	//
	if( !kbstr.ai ) return 0;

	//
	return 1;
}











