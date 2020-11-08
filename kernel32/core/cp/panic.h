#ifndef __INCLUDED_PANIC_H__
#define __INCLUDED_PANIC_H__

//
#define PANIC_DISABLES_INTERRUPTS
// Jump to terminal #1 when panicing? or just use the current window?
#define PANICHNDLR_ALWAYS_SWITCH_TO_FIRST_TERMINAL

// #define PANIC_WITH_CLEARSCR
void FATALERROR(const char *s);

#endif
