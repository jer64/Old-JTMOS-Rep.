#ifndef __INCLUDED_SCANASC_H__
#define __INCLUDED_SCANASC_H__

#define SCODE_F1               	59
#define SCODE_F2               	60
#define SCODE_F3               	61
#define SCODE_F4               	62
#define SCODE_F5               	63
#define SCODE_F6               	64
#define SCODE_F7               	65
#define SCODE_F8               	66
#define SCODE_F9               	67
#define SCODE_F10              	68
#define SCODE_F11		87
#define SCODE_F12		88

#define SCODE_CURSORBLOCKUP 103		/* Cursor key block. */
#define SCODE_CURSORBLOCKLEFT 105
#define SCODE_CURSORBLOCKRIGHT 106
#define SCODE_CURSORBLOCKDOWN 108

#define SCODE_INSERT 110
#define SCODE_HOME 102
#define SCODE_PGUP 104
#define SCODE_DELETE 111
#define SCODE_END 107
#define SCODE_PGDN 109

#define SCODE_NUMLOCK 69

#define SCODE_KEYPAD0  82
#define SCODE_KEYPAD1  79
#define SCODE_KEYPAD2  80
#define SCODE_KEYPAD3  81
#define SCODE_KEYPAD4  75
#define SCODE_KEYPAD5  76
#define SCODE_KEYPAD6  77
#define SCODE_KEYPAD7  71
#define SCODE_KEYPAD8  72
#define SCODE_KEYPAD9  73
#define SCODE_KEYPADENTER      96
#define SCODE_KEYPADPLUS       78
#define SCODE_KEYPADMINUS      74
#define SCODE_KEYPADMULTIPLY   55
#define SCODE_KEYPADDIVIDE     98

#define SCODE_Q                16
#define SCODE_W                17
#define SCODE_E                18
#define SCODE_R                19
#define SCODE_T                20
#define SCODE_Y                21
#define SCODE_U                22
#define SCODE_I                23
#define SCODE_O                24
#define SCODE_P                25

#define SCODE_A                30
#define SCODE_S                31
#define SCODE_D                32
#define SCODE_F                33
#define SCODE_G                34
#define SCODE_H                35
#define SCODE_J                36
#define SCODE_K                37
#define SCODE_L                38

#define SCODE_Z                44
#define SCODE_X                45
#define SCODE_C                46
#define SCODE_V                47
#define SCODE_B                48
#define SCODE_N                49
#define SCODE_M                50

#define SCODE_ESCAPE           1
#define SCODE_ENTER            28
#define SCODE_RIGHTCONTROL     97
#define SCODE_CONTROL  97
#define SCODE_RIGHTALT 100
#define SCODE_LEFTCONTROL      29
#define SCODE_LEFTALT  56
#define SCODE_SPACE            57

#define SCODE_ALT	56
#define SCODE_CTRL	29
#define SCODE_LSHIFT	0x2A
#define SCODE_RSHIFT	0x36

//
int scan_to_asc(int);

#endif

