#ifndef __INCLUDED_PS2MOUSE_H__
#define __INCLUDED_PS2MOUSE_H__

// Which color it takes
// Special note:
// This rate value does not modify the speed of moving,
// it modifies how many times per a second the place
// of the arrow will be updated, so f.e. we set up a
// rate of 10 updates per second, it'll look pretty
// 'not enough' on a monitor with 70hz refresh rate,
// because only 1/7 of the screen updates will show
// the mouse at the right location,
// but if we setup a rate of 70, it'll be just fine
// for a screenmode of 70hz, but 200 updates per a
// second should be okay for anything however.
#define PS2MOUSE_XORCOLOR               0x08
//
#define PS2MOUSE_DEFAULT_RATE           100  // For 80x25
#define PS2MOUSE_DEFAULT_RATE2          100  // For 640x480 and higher
//
#define PS2MOUSE_DEFAULT_RESOLUTION     0x01 // For 80x25
#define PS2MOUSE_DEFAULT_RESOLUTION2    0x03 // For 640x480 and higher

/*
  SAMPLE_10      10  reports/second maximum
  SAMPLE_20      20  reports/second maximum
  SAMPLE_40      40  reports/second maximum
  SAMPLE_60      60  reports/second maximum
  SAMPLE_80      80  reports/second maximum
  SAMPLE_100     100 reports/second maximum
  SAMPLE_200     200 reports/second maximum

  200 is a good value in all cases.
 */

//
#define PS2MOUSE_TIMEOUT        20000
#define PS2MOUSE_RETRIES        20
//
#include "basdef.h"

//
void ps2mouse_asmirqhandler12(void);
void ps2mouse_setframe(long x1, long y1, long x2, long y2);
void ps2mouse_setposition(long x, long y);
void ps2mouse_hidecursor(void);
void ps2mouse_showcursor(void);
void ps2mouse_init(void);
int ps2mouse_setresolution(int value);
int ps2mouse_setrate(BYTE rate);
void ps2mouse_modeChange(int vmode);

//
void ps2mouse_init(void);

//
extern int ps2mouse_counter, ps2mouse_first_time,
        PS2MOUSE_TEST_OK;

#endif
