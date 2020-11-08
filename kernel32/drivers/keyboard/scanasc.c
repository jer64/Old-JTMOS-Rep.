// =========================================================
// keyb_scanasc.c - Scan to ASCII converting code.
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// =========================================================
#include "basdef.h"
#include "cpu.h"
#include "io.h"
#include "keyb.h"
#include "sysglvar.h"
#include "ega.h"
#include "int.h"
#include "vga.h"
#include "kernel32.h"
//
#include "driver_sys.h" // driver_register_new_device, ...
#include "driver_api.h"
//
#include "ps2mouse.h"
#include "scanasc.h"

//
int reset_attempt=0;

//
int uppercase(int c)
{
	char *upper="QWERTYUIOPASDFGHJKLZXCVBNM";
	char *lower="qwertyuiopasdfghjklzxcvbnm";
	int i,l;

	l = strlen(upper);
	for(i=0; i<l; i++)
	{
		if(c==lower[i])
			return upper[i];
	}
	return c;
}

// scan_to_asc function
//
int scan_to_asc(int mark)
{
	int ad,ad2;
	int m1,m2,m3,m4,ch,
		i,i2,i3,i4;

	//
	ch=0;
 
	//
#ifdef __KEYDEBUG__
	printk("[key 0x"); p8(mark); printk("]");
#endif

	//
	// Check out keys that does have more than one keys pressed
	//
	// Handle extra key exception
	//

repeatcheck:
	//
	if(kbstr.ExtraKey==0xfe)
	{
		kbstr.ExtraKey=0xfd;
		return -2;
	}
	if(kbstr.ExtraKey==0xfd)
	{
		if( !(ad=kbstr.macro[kbstr.i_macro]) )
		{
			kbstr.ExtraKey=0;
			goto repeatcheck;
		}
		else
		{
			kbstr.i_macro++;
			return ad;
		}
	}

	//
	if(kbstr.ExtraKey==0xff || kbstr.ExtraKeyCode)
	{
		m1=kbstr.ExtraKeyCode;
		kbstr.ExtraKey=0;
		kbstr.counter=0;
		kbstr.ExtraKeyCode=0;
		return m1;
	}

	// CTRL+C => TERMINATE PROCESS
	if( //(kbstr.ktab[SCODE_CTRL] && kbstr.ktab[1])
		//||
	     (kbstr.ktab[SCODE_CTRL] && kbstr.ktab[46]) )
	{
		// Unpress keys
		kbstr.ktab[SCODE_CTRL]=0; kbstr.ktab[46]=0;
		// Press the symbolic "terminate key"
		syskey_terminate=1;
		//
		printk("^C");
	}

	// TODO: LWIN+'r' should be immediate reset
	// CTRL+ALT+DEL or LWIN+'r' hit ? => Reboot the system.
	if( (kbstr.ktab[SCODE_CTRL] && kbstr.ktab[SCODE_ALT] && kbstr.ktab[83])
	    ||
	    (kbstr.ktab[91] && kbstr.ktab[19]) )
	{
		if(!reset_attempt && Multitasking)
		{
			// FIRST TIME: NEGOTIATE A REBOOT
			reboot_now = TRUE;
		}
		else
		{
			// SECOND TIME: REBOOT NOW
			system_reset1();
		}
		reset_attempt++;
	}

	// If not already ExtraKey specified,
	// we'll make a check if we should define one
	// from the ktab keypressings.
	// -----------------------------------------------------------
	if(!kbstr.ExtraKey)
	{
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//
		if(
			  kbstr.ktab[SCODE_ALT] && kbstr.ktab[12]
		  )
		{
			if(kbstr.place>=2)kbstr.place--;
			kbstr.ktab[12]=0;
			//
			kbstr.ExtraKeyCode='\\';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( kbstr.ktab[SCODE_ALT] && kbstr.ktab[8] )
		{
			kbstr.ktab[8]=0;
			kbstr.ExtraKeyCode='{';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( kbstr.ktab[SCODE_ALT] && kbstr.ktab[11] )
		{
			kbstr.ktab[11]=0;
			kbstr.ExtraKeyCode='}';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( kbstr.ktab[SCODE_ALT] && kbstr.ktab[9] )
		{
			kbstr.ktab[9]=0;
			kbstr.ExtraKeyCode='[';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( kbstr.ktab[SCODE_ALT] && kbstr.ktab[10] )
		{
			kbstr.ktab[10]=0;
			kbstr.ExtraKeyCode=']';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[12] )
		{
			kbstr.ktab[12]=0;
			kbstr.ExtraKeyCode='?';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[8] )
		{
			kbstr.ktab[8]=0;
			kbstr.ExtraKeyCode='/';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[3]) && kbstr.ktab[56] )
		{
			kbstr.ktab[56]=0;
			kbstr.ktab[3]=0;
			kbstr.ExtraKeyCode='@';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[4]) && kbstr.ktab[56] )
		{
			kbstr.ktab[56]=0;
			kbstr.ktab[4]=0;
			kbstr.ExtraKeyCode='£';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[43] )
		{
			kbstr.ktab[43]=0;
			kbstr.ExtraKeyCode='*';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		// L/R SHIFT + , => ;
		if(((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[51]))
		{
			//
			kbstr.ktab[0x35]=0;
			//
			kbstr.ExtraKeyCode=';';
			kbstr.ktab[0x02]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// L/R SHIFT + . => :
		if(((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[52]))
		{
			//
			kbstr.ktab[0x35]=0;
			//
			kbstr.ExtraKeyCode=':';
			kbstr.ktab[0x02]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// L/R SHIFT + '-' => '_'
		if(((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[0x35]))
		{
			//
			kbstr.ktab[0x35]=0;
			//
			kbstr.ExtraKeyCode='_';
			kbstr.ktab[0x02]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// ALT + ¨ -> '~'
		if(kbstr.ktab[0x38] && kbstr.ktab[0x1b])
		{
			//
			kbstr.ktab[0x1b]=0;
			//
			kbstr.ExtraKeyCode='~';
			kbstr.ktab[0x02]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// ^
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) && kbstr.ktab[0x1b])
		{
			//
			kbstr.ktab[0x1b]=0;
			//
			kbstr.ExtraKeyCode='^';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		// !
		if(kbstr.ktab[0x1b])
		{
			//
			kbstr.ktab[0x1b]=0;
			//
			kbstr.ExtraKeyCode='!';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+1 -> '!'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
		   kbstr.ktab[0x02])
		{
			kbstr.ExtraKeyCode='!';
			kbstr.ktab[0x02]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+2 -> '\"'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x03])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ktab[0x3]=0;
			//
			kbstr.ExtraKeyCode='\"';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+3 -> '#'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x04])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='#';
			kbstr.ktab[0x04]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+4 -> '¤'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x05])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='¤';
			kbstr.ktab[0x05]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// ALT+4 -> '$'
		if((kbstr.ktab[SCODE_ALT]) &&
			kbstr.ktab[0x05])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='$';
			kbstr.ktab[0x05]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+5 -> '%'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x06])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='%';
			kbstr.ktab[0x06]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+6 -> '&'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x02])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='&';
			kbstr.ktab[0x07]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+7 -> '/'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x02])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='/';
			kbstr.ktab[0x08]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+8 -> '('
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x09])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode='(';
			kbstr.ktab[0x09]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+9 -> ')'
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x0a])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ExtraKeyCode=')';
			kbstr.ktab[0x0a]=0; kbstr.ExtraKey=0xff;
			return 0;
		}

		// Shift+0 -> '='
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT]) &&
			kbstr.ktab[0x0b])
		{
			if(kbstr.place>=2)kbstr.place--;
			//
			kbstr.ktab[0xb]=0;
			//
			kbstr.ExtraKeyCode='=';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if(kbstr.ktab[0x33]==0xff && kbstr.ktab[SCODE_LSHIFT]==0xff)
		{
			if(kbstr.place>=2)kbstr.place--;
			kbstr.ktab[0x33]=0;
			//
			kbstr.ExtraKeyCode=';';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if(kbstr.ktab[0x34]==0xff && kbstr.ktab[SCODE_LSHIFT]==0xff)
		{
			if(kbstr.place>=2)kbstr.place--;
			kbstr.ktab[0x34]=0;
			//
			kbstr.ExtraKeyCode=':';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if(kbstr.ktab[0x08]==0xff && kbstr.ktab[SCODE_LSHIFT]==0xff)
		{
			if(kbstr.place>=2)kbstr.place--;
			kbstr.ktab[0x08]=0;
			//
			kbstr.ExtraKeyCode='/';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		//
		if( (kbstr.ktab[SCODE_LSHIFT]==0xff && kbstr.ktab[0x2b]==0xff)
		    ||
			kbstr.ktab[0x37]==0xff)
		{
			if(kbstr.place>=2)kbstr.place--;
			kbstr.ktab[0x2b]=0;
			kbstr.ktab[0x37]=0;
			//
			kbstr.ExtraKeyCode='*';
			kbstr.ExtraKey=0xff;
			return 0;
		}
	
		//
		if( kbstr.ktab[0x0c]==0xff &&
			(kbstr.ktab[SCODE_LSHIFT]==0xff ||
			kbstr.ktab[SCODE_RSHIFT]==0xff) )
		{
			kbstr.ktab[SCODE_LSHIFT]=0;
			kbstr.ktab[SCODE_RSHIFT]=0;
			//
			kbstr.ExtraKeyCode='?';
			kbstr.ExtraKey=0xff;
			return 0;
		}

		// |
		if( kbstr.ktab[0x56] &&
			kbstr.ktab[0x38] )
		{
			kbstr.ktab[0x56]=0;
			//
			kbstr.ExtraKeyCode='|';
			kbstr.ExtraKey=0xff;
			return 0;
		}
	}
	// <--- END OF EXTRA KEYS!


	// EGA SCREEN SWITCH WITH ALT+1,ALT+2,ALT+3,...
	// 3B (F1), 3C (F2), 3D (F3), 3E (F4),
	// 3F (F5), 40 (F6), 41 (F7), 42 (F8), 43 (F9), 44 (F10) 
	//
	// Fixed a major bug here:
	// If ALT+1 & ALT+2 were pressed at sametime,
	// it would cause a crash.
	// Now allowing only one terminal change at once.
	//

	// ALT+F1-F9
	if( (kbstr.ktab[SCODE_ALT] || kbstr.ktab[SCODE_CTRL])
		&& dispadmin_getcurrentmode()==3)
	{
		//
		i=0;
		if(kbstr.ktab[0x3B])	i=1;
		if(kbstr.ktab[0x3C])	i=2;
		if(kbstr.ktab[0x3D])	i=3;
		if(kbstr.ktab[0x3E])	i=4;
		if(kbstr.ktab[0x3F])	i=5;
		if(kbstr.ktab[0x40])	i=6;
		if(kbstr.ktab[0x41])	i=7;
		if(kbstr.ktab[0x42])	i=8;
		if(kbstr.ktab[0x43])	i=9;

		//
		if(i)
		{
			//
			EgaSelectWindow(i-1);
			kbstr.ktab[i]=0;
			return 0;
		}
	}

	// Check out keys that does have less than two keys pressed at once.
	if(kbstr.ExtraKey==0)
	{
		//
		switch(mark)
		{
			// Rivi 1
			case 1:
			ch = 27;
			break;
			case 0x33:
			ch = ',';
			break;
			case 0x34:
			ch = '.';
			break;
			case 0x39:
			ch = ' ';
			break;
			case 0x10:
			ch = 'q';
			break;
			case 0x11:
			ch = 'w';
			break;
			case 0x12:
			ch = 'e';
			break;
			case 0x13:
			ch = 'r';
			break;
			case 0x14:
			ch = 't';
			break;
			case 0x15:
			ch = 'y';
			break;
			case 0x16:
			ch = 'u';
			break;
			case 0x17:
			ch = 'i';
			break;
			case 0x18:
			ch = 'o';
			break;
			case 0x19:
			ch = 'p';
			break;
			// Rivi 2
			case 0x1e:
			ch = 'a';
			break;
			case 0x1f:
			ch = 's';
			break;
			case 0x20:
			ch = 'd';
			break;
			case 0x21:
			ch = 'f';
			break;
			case 0x22:
			ch = 'g';
			break;
			case 0x23:
			ch = 'h';
			break;
			case 0x24:
			ch = 'j';
			break;
			case 0x25:
			ch = 'k';
			break;
			case 0x26:
			ch = 'l';
			break;
			// Rivi 3
			case 0x2c:
			ch = 'z';
			break;
			case 0x2d:
			ch = 'x';
			break;
			case 0x2e:
			ch = 'c';
			break;
			case 0x2f:
			ch = 'v';
			break;
			case 0x30:
			ch = 'b';
			break;
			case 0x31:
			ch = 'n';
			break;
			case 0x32:
			ch = 'm';
			break;
			// Numerot+muut
			case 0x02:
			ch = '1';
			break;
			case 0x03:
			ch = '2';
			break;
			case 0x04:
			ch = '3';
			break;
			case 0x05:
			ch = '4';
			break;
			case 0x06:
			ch = '5';
			break;
			case 0x07:
			ch = '6';
			break;
			case 0x08:
			ch = '7';
			break;
			case 0x09:
			ch = '8';
			break;
			case 0x0a:
			ch = '9';
			break;
			case 0x0b:
			ch = '0';
			break;
			case 15:
			ch = '\t';
			break;
			case 0x0e:
			ch = '\b';
			break;
			case 0x1c:
			ch = '\n';
			break;
			case 0x0c:
			ch = '+';
			break;
			case 0x35:
			ch = '-';
			break;
			case 41:
			ch = '§';
			break;
			case 43:
			ch = '\'';
			break;
			case 26:
			ch = 'å';
			break;
			case 39:
			ch = 'ä';
			break;
			case 40:
			ch = 'ö';
			break;

			// Function keys:
			case 59: // F1
			ch = 0xFF00 + 59;
			break;
			case 60: // F2
			ch = 0xFF00 + 60;
			break;
			case 61: // F3
			ch = 0xFF00 + 61;
			break;
			case 62: // F4
			ch = 0xFF00 + 62;
			break;
			case 63: // F5
			ch = 0xFF00 + 63;
			break;
			case 64: // F6
			ch = 0xFF00 + 64;
			break;
			case 65: // F7
			ch = 0xFF00 + 65;
			break;
			case 66: // F8
			ch = 0xFF00 + 66;
			break;
			case 67: // F9
			ch = 0xFF00 + 67;
			break;
			case 68: // F10
			ch = 0xFF00 + 68;
			break;
			case 87: // F11
			ch = 0xFF00 + 87;
			break;
			case 88: // F12
			ch = 0xFF00 + 88;
			break;


			// Cursor keys:
			case 72:
			ch = 0xFF00 + 72;
			return ch;

			case 75:
			ch = 0xFF00 + 75;
			return ch;

			case 80:
			ch = 0xFF00 + 80;
			return ch;

			case 77:
			ch = 0xFF00 + 77;
			return ch;

			// Home,End
			case 71:
			ch = 0xFF00 + 71;
			return ch;
			case 79:
			ch = 0xFF00 + 79;
			return ch;

			// PGUP
			case 73:
			ch = 0xFF00 + 73;
			return ch;
			// PGDOWN
			case 81:
			ch = 0xFF00 + 81;
			return ch;

			// INSERT
			case 82:
			ch = 0xFF00 + 82;
			return ch;
			// DELETE:
			case 83:
			ch = 0xFF00 + 83;
			return ch;

			// SCROLL LOCK:
			case 70:
			ch = 0xFF00 + 70;
			return ch;

			// Default:
			default:
			ch = 0;
			break;
		}

		// With l/r shift
		if((kbstr.ktab[SCODE_LSHIFT] || kbstr.ktab[SCODE_RSHIFT])
			&&
			ch)
		{
			ch = uppercase(ch);
		}

		// With ctrl?
	/*	if(kbstr.ktab[SCODE_CTRL] && ch)
		{
			ch-=0x40;
		}*/
		return ch;
	}
loppu:
	return 0;
}


