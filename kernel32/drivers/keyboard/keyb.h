//
#ifndef __INCLUDED_SYSKEYB_H__
#define __INCLUDED_SYSKEYB_H__

//
#define LED_SCROLL_LOCK         1
#define LED_NUM_LOCK            2
#define LED_CAPS_LOCK           4

//
#define KEYBOARD_BUFFER_LENGTH 64

//
#define KBD_MODE_KBD_INT        0x01
#define KBD_MODE_MOUSE_INT      0x02
#define KBD_MODE_SYS            0x04
#define KBD_MODE_NO_KEYLOCK     0x08
#define KBD_MODE_DISABLE_KBD    0x10
#define KBD_MODE_DISABLE_MOUSE  0x20
#define KBD_MODE_KCC            0x40
#define KBD_MODE_RFU            0x80

//
#define KBD_STAT_OBF		0x01
#define KBD_STAT_IBF		0x02
#define KBD_STAT_SELFTEST	0x04
#define KBD_STAT_CMD		0x08
#define KBD_STAT_UNLOCKED	0x10
#define KBD_STAT_MOUSE_OBF	0x20
#define KBD_STAT_GTO		0x40
#define KBD_STAT_PERR		0x80

//
typedef struct
{
  int kb_test_ok;
  BYTE led_status;
  BYTE *ktab;
  WORD *buf, *abuf;
  int i, ai, key_pressed;
  WORD rkey;
  WORD ExtraKey, ExtraKeyCode;
  char *macro;
  int i_macro;
  int counter, place;
  int specialkeyconvert2asc;
  char diagnosticsOK;
}KBSTR;
//
extern KBSTR kbstr;


//
char getmark (int offs);
void storemark (int which, int offs);
void incmark (void);
void kbwait (void);
void kbwaitdata (void);
extern int translate_keyboard_situation (void);
extern int kb_output_asc_buffer (int);
extern void ulong_strcnv ();
extern void keyb_ISR (void);
extern int kbwaitstatus (DWORD value, DWORD mask);
extern int kbread (void);
extern int kbwrite (int value);
extern int kbcmd (int cmd);

//
int InitKeyboard();
void keyb_handler();
void setleds();
int getch();
int getch1();
int kbhit();
extern unsigned char keyboard_buffer_size;
extern int syskey_terminate;
extern void keyb_ISR (void);
void kb_outbuf (int c);
int GetALT(void);
int GetCTRL(void);
int OutputKeyboard(int ch);
int WriteKeyboard(const char *s);

//
#include "scanasc.h"

//
typedef struct
{
        int alt,ctrl;
}KEYS;
KEYS keys;

#endif
