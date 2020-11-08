/** VGA **
 *
 * See vgareg.txt for list of EGA(/VGA) registers
 *
 */
#include "vga.h"
#include "sysvga.h"
#include "basdef.h"
#include "io.h"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=- Created with FEDIT written by Kinfira (Jeffrey Morgan)                  -=
//=-                                                                         -=
//=-                   Tauron VGA Utilities Version 1.0                      -=
//=-                        Released June 14, 1998                           -=
//=-                                                                         -=
//=- Font Width:  8                                                          -=
//=- Font Height: 16                                                         -=
//=- Bytes per character:  16                                                -=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static BYTE system_font[] = {
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x07E,0x081,0x0A5,0x081,0x081,0x0BD,0x099,0x081,0x081,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x07E,0x0FF,0x0DB,0x0FF,0x0FF,0x0C3,0x0E7,0x0FF,0x0FF,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x06C,0x0FE,0x0FE,0x0FE,0x0FE,0x07C,0x038,0x010,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x010,0x038,0x07C,0x0FE,0x07C,0x038,0x010,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x018,0x03C,0x03C,0x0E7,0x0E7,0x0E7,0x099,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x018,0x03C,0x07E,0x0FF,0x0FF,0x07E,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x018,0x03C,0x03C,0x018,0x000,0x000,0x000,0x000,0x000,0x000,
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0E7,0x0C3,0x0C3,0x0E7,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
0x000,0x000,0x000,0x000,0x000,0x03C,0x066,0x042,0x042,0x066,0x03C,0x000,0x000,0x000,0x000,0x000,
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0C3,0x099,0x0BD,0x0BD,0x099,0x0C3,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
0x000,0x000,0x01E,0x00E,0x01A,0x032,0x078,0x0CC,0x0CC,0x0CC,0x0CC,0x078,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x066,0x066,0x066,0x066,0x03C,0x018,0x07E,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x03F,0x033,0x03F,0x030,0x030,0x030,0x030,0x070,0x0F0,0x0E0,0x000,0x000,0x000,0x000,
0x000,0x000,0x07F,0x063,0x07F,0x063,0x063,0x063,0x063,0x067,0x0E7,0x0E6,0x0C0,0x000,0x000,0x000,
0x000,0x000,0x000,0x018,0x018,0x0DB,0x03C,0x0E7,0x03C,0x0DB,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x080,0x0C0,0x0E0,0x0F0,0x0F8,0x0FE,0x0F8,0x0F0,0x0E0,0x0C0,0x080,0x000,0x000,0x000,0x000,
0x000,0x002,0x006,0x00E,0x01E,0x03E,0x0FE,0x03E,0x01E,0x00E,0x006,0x002,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x03C,0x07E,0x018,0x018,0x018,0x018,0x07E,0x03C,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x066,0x066,0x066,0x066,0x066,0x066,0x066,0x000,0x066,0x066,0x000,0x000,0x000,0x000,
0x000,0x000,0x07F,0x0DB,0x0DB,0x0DB,0x07B,0x01B,0x01B,0x01B,0x01B,0x01B,0x000,0x000,0x000,0x000,
0x000,0x07C,0x0C6,0x060,0x038,0x06C,0x0C6,0x0C6,0x06C,0x038,0x00C,0x0C6,0x07C,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x0FE,0x0FE,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x03C,0x07E,0x018,0x018,0x018,0x018,0x07E,0x03C,0x018,0x07E,0x000,0x000,0x000,
0x000,0x000,0x018,0x03C,0x07E,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x07E,0x03C,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x018,0x00C,0x0FE,0x00C,0x018,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x030,0x060,0x0FE,0x060,0x030,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0C0,0x0C0,0x0C0,0x0C0,0x0FE,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x028,0x06C,0x0FE,0x06C,0x028,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x010,0x038,0x038,0x07C,0x07C,0x0FE,0x0FE,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x0FE,0x0FE,0x07C,0x07C,0x038,0x038,0x010,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x03C,0x03C,0x03C,0x018,0x018,0x018,0x000,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x066,0x066,0x066,0x024,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x06C,0x06C,0x0FE,0x06C,0x06C,0x06C,0x0FE,0x06C,0x06C,0x000,0x000,0x000,0x000,
0x018,0x018,0x07C,0x0C6,0x0C2,0x0C0,0x07C,0x006,0x086,0x0C6,0x07C,0x018,0x018,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x0C2,0x0C6,0x00C,0x018,0x030,0x060,0x0C6,0x086,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x06C,0x06C,0x038,0x076,0x0DC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x030,0x030,0x030,0x060,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x00C,0x018,0x030,0x030,0x030,0x030,0x030,0x030,0x018,0x00C,0x000,0x000,0x000,0x000,
0x000,0x000,0x030,0x018,0x00C,0x00C,0x00C,0x00C,0x00C,0x00C,0x018,0x030,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x066,0x03C,0x0FF,0x03C,0x066,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x018,0x018,0x07E,0x018,0x018,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x018,0x018,0x018,0x030,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x002,0x006,0x00C,0x018,0x030,0x060,0x0C0,0x080,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x0CE,0x0D6,0x0D6,0x0E6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x038,0x078,0x018,0x018,0x018,0x018,0x018,0x018,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x006,0x00C,0x018,0x030,0x060,0x0C0,0x0C6,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x006,0x006,0x03C,0x006,0x006,0x006,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x00C,0x01C,0x03C,0x06C,0x0CC,0x0FE,0x00C,0x00C,0x00C,0x01E,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FE,0x0C0,0x0C0,0x0C0,0x0FC,0x00E,0x006,0x006,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x060,0x0C0,0x0C0,0x0FC,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FE,0x0C6,0x006,0x006,0x00C,0x018,0x030,0x030,0x030,0x030,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x07E,0x006,0x006,0x006,0x00C,0x078,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x018,0x018,0x000,0x000,0x000,0x018,0x018,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x018,0x018,0x000,0x000,0x000,0x018,0x018,0x030,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x006,0x00C,0x018,0x030,0x060,0x030,0x018,0x00C,0x006,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x000,0x000,0x0FE,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x060,0x030,0x018,0x00C,0x006,0x00C,0x018,0x030,0x060,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x00C,0x018,0x018,0x018,0x000,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x07C,0x0C6,0x0C6,0x0DE,0x0DE,0x0DE,0x0DC,0x0C0,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x010,0x038,0x06C,0x0C6,0x0C6,0x0FE,0x0C6,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FC,0x066,0x066,0x066,0x07C,0x066,0x066,0x066,0x066,0x0FC,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x066,0x0C2,0x0C0,0x0C0,0x0C0,0x0C0,0x0C2,0x066,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0F8,0x06C,0x066,0x066,0x066,0x066,0x066,0x066,0x06C,0x0F8,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FE,0x066,0x062,0x068,0x078,0x068,0x060,0x062,0x066,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FE,0x066,0x062,0x068,0x078,0x068,0x060,0x060,0x060,0x0F0,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x066,0x0C2,0x0C0,0x0C0,0x0DE,0x0C6,0x0C6,0x066,0x03A,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0FE,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x01E,0x00C,0x00C,0x00C,0x00C,0x00C,0x0CC,0x0CC,0x0CC,0x078,0x000,0x000,0x000,0x000,
0x000,0x000,0x0E6,0x066,0x06C,0x06C,0x078,0x078,0x06C,0x066,0x066,0x0E6,0x000,0x000,0x000,0x000,
0x000,0x000,0x0F0,0x060,0x060,0x060,0x060,0x060,0x060,0x062,0x066,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0EE,0x0FE,0x0FE,0x0D6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0E6,0x0F6,0x0FE,0x0DE,0x0CE,0x0C6,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x06C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x06C,0x038,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FC,0x066,0x066,0x066,0x07C,0x060,0x060,0x060,0x060,0x0F0,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0D6,0x0DE,0x07C,0x00C,0x00E,0x000,0x000,
0x000,0x000,0x0FC,0x066,0x066,0x066,0x07C,0x06C,0x066,0x066,0x066,0x0E6,0x000,0x000,0x000,0x000,
0x000,0x000,0x07C,0x0C6,0x0C6,0x060,0x038,0x00C,0x006,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x07E,0x07E,0x05A,0x018,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x06C,0x038,0x010,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0D6,0x0D6,0x0FE,0x06C,0x06C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x06C,0x06C,0x038,0x038,0x06C,0x06C,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x066,0x066,0x066,0x066,0x03C,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0FE,0x0C6,0x086,0x00C,0x018,0x030,0x060,0x0C2,0x0C6,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x030,0x030,0x030,0x030,0x030,0x030,0x030,0x030,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x080,0x0C0,0x0E0,0x070,0x038,0x01C,0x00E,0x006,0x002,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x00C,0x00C,0x00C,0x00C,0x00C,0x00C,0x00C,0x00C,0x03C,0x000,0x000,0x000,0x000,
0x010,0x038,0x06C,0x0C6,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x000,0x000,
0x030,0x030,0x018,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x0E0,0x060,0x060,0x078,0x06C,0x066,0x066,0x066,0x066,0x0DC,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07C,0x0C6,0x0C0,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x01C,0x00C,0x00C,0x03C,0x06C,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07C,0x0C6,0x0FE,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x06C,0x064,0x060,0x0F0,0x060,0x060,0x060,0x060,0x0F0,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x076,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x07C,0x00C,0x0CC,0x078,0x000,
0x000,0x000,0x0E0,0x060,0x060,0x06C,0x076,0x066,0x066,0x066,0x066,0x0E6,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x018,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x006,0x006,0x000,0x00E,0x006,0x006,0x006,0x006,0x006,0x006,0x066,0x066,0x03C,0x000,
0x000,0x000,0x0E0,0x060,0x060,0x066,0x06C,0x078,0x078,0x06C,0x066,0x0E6,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0EC,0x0FE,0x0D6,0x0D6,0x0D6,0x0D6,0x0D6,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0DC,0x066,0x066,0x066,0x066,0x066,0x066,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0DC,0x066,0x066,0x066,0x066,0x066,0x07C,0x060,0x060,0x0F0,0x000,
0x000,0x000,0x000,0x000,0x000,0x076,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x07C,0x00C,0x00C,0x01E,0x000,
0x000,0x000,0x000,0x000,0x000,0x0DC,0x076,0x062,0x060,0x060,0x060,0x0F0,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07C,0x0C6,0x060,0x038,0x00C,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x010,0x030,0x030,0x0FC,0x030,0x030,0x030,0x030,0x036,0x01C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x066,0x066,0x066,0x066,0x066,0x03C,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0C6,0x0C6,0x0C6,0x0D6,0x0D6,0x0FE,0x06C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0C6,0x06C,0x038,0x038,0x038,0x06C,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07E,0x006,0x00C,0x0F8,0x000,
0x000,0x000,0x000,0x000,0x000,0x0FE,0x0CC,0x018,0x030,0x060,0x0C6,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x00E,0x018,0x018,0x018,0x070,0x018,0x018,0x018,0x018,0x00E,0x000,0x000,0x000,0x000,
0x000,0x000,0x018,0x018,0x018,0x018,0x000,0x018,0x018,0x018,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x070,0x018,0x018,0x018,0x00E,0x018,0x018,0x018,0x018,0x070,0x000,0x000,0x000,0x000,
0x000,0x000,0x076,0x0DC,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x010,0x038,0x06C,0x0C6,0x0C6,0x0C6,0x0FE,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x03C,0x066,0x0C2,0x0C0,0x0C0,0x0C0,0x0C2,0x066,0x03C,0x00C,0x006,0x07C,0x000,0x000,
0x000,0x000,0x0CC,0x0CC,0x000,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x00C,0x018,0x030,0x000,0x07C,0x0C6,0x0FE,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x010,0x038,0x06C,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x0CC,0x0CC,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x060,0x030,0x018,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x038,0x06C,0x038,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x03C,0x066,0x060,0x060,0x066,0x03C,0x00C,0x006,0x03C,0x000,0x000,0x000,
0x000,0x010,0x038,0x06C,0x000,0x07C,0x0C6,0x0FE,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x000,0x07C,0x0C6,0x0FE,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x060,0x030,0x018,0x000,0x07C,0x0C6,0x0FE,0x0C0,0x0C0,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x066,0x066,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x018,0x03C,0x066,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x060,0x030,0x018,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x0C6,0x0C6,0x010,0x038,0x06C,0x0C6,0x0C6,0x0FE,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x038,0x06C,0x038,0x000,0x038,0x06C,0x0C6,0x0C6,0x0FE,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x018,0x030,0x060,0x000,0x0FE,0x066,0x060,0x07C,0x060,0x060,0x066,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0CC,0x076,0x036,0x07E,0x0D8,0x0D8,0x06E,0x000,0x000,0x000,0x000,
0x000,0x000,0x03E,0x06C,0x0CC,0x0CC,0x0FE,0x0CC,0x0CC,0x0CC,0x0CC,0x0CE,0x000,0x000,0x000,0x000,
0x000,0x010,0x038,0x06C,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x060,0x030,0x018,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x030,0x078,0x0CC,0x000,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x060,0x030,0x018,0x000,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x0C6,0x0C6,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07E,0x006,0x00C,0x078,0x000,
0x000,0x0C6,0x0C6,0x000,0x038,0x06C,0x0C6,0x0C6,0x0C6,0x0C6,0x06C,0x038,0x000,0x000,0x000,0x000,
0x000,0x0C6,0x0C6,0x000,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x018,0x018,0x03C,0x066,0x060,0x060,0x060,0x066,0x03C,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x038,0x06C,0x064,0x060,0x0F0,0x060,0x060,0x060,0x060,0x0E6,0x0FC,0x000,0x000,0x000,0x000,
0x000,0x000,0x066,0x066,0x03C,0x018,0x07E,0x018,0x07E,0x018,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x0F8,0x0CC,0x0CC,0x0F8,0x0C4,0x0CC,0x0DE,0x0CC,0x0CC,0x0CC,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x00E,0x01B,0x018,0x018,0x018,0x07E,0x018,0x018,0x018,0x018,0x018,0x0D8,0x070,0x000,0x000,
0x000,0x018,0x030,0x060,0x000,0x078,0x00C,0x07C,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x00C,0x018,0x030,0x000,0x038,0x018,0x018,0x018,0x018,0x018,0x03C,0x000,0x000,0x000,0x000,
0x000,0x018,0x030,0x060,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x018,0x030,0x060,0x000,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x0CC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x076,0x0DC,0x000,0x0DC,0x066,0x066,0x066,0x066,0x066,0x066,0x000,0x000,0x000,0x000,
0x076,0x0DC,0x000,0x0C6,0x0E6,0x0F6,0x0FE,0x0DE,0x0CE,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x03C,0x06C,0x06C,0x03E,0x000,0x07E,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x038,0x06C,0x06C,0x038,0x000,0x07C,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x030,0x030,0x000,0x030,0x030,0x060,0x0C0,0x0C6,0x0C6,0x07C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x0C0,0x0C0,0x0C0,0x0C0,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x006,0x006,0x006,0x006,0x000,0x000,0x000,0x000,0x000,
0x000,0x0C0,0x0C0,0x0C2,0x0C6,0x0CC,0x018,0x030,0x060,0x0CE,0x093,0x006,0x00C,0x01F,0x000,0x000,
0x000,0x0C0,0x0C0,0x0C2,0x0C6,0x0CC,0x018,0x030,0x066,0x0CE,0x09A,0x03F,0x006,0x00F,0x000,0x000,
0x000,0x000,0x018,0x018,0x000,0x018,0x018,0x018,0x03C,0x03C,0x03C,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x033,0x066,0x0CC,0x066,0x033,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0CC,0x066,0x033,0x066,0x0CC,0x000,0x000,0x000,0x000,0x000,0x000,
0x011,0x044,0x011,0x044,0x011,0x044,0x011,0x044,0x011,0x044,0x011,0x044,0x011,0x044,0x011,0x044,
0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,0x055,0x0AA,
0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,0x0DD,0x077,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x0F8,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x0F8,0x018,0x0F8,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x0F6,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FE,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x000,0x000,0x000,0x000,0x000,0x0F8,0x018,0x0F8,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x036,0x036,0x036,0x036,0x036,0x0F6,0x006,0x0F6,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x000,0x000,0x000,0x000,0x000,0x0FE,0x006,0x0F6,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x0F6,0x006,0x0FE,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x0FE,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x018,0x018,0x018,0x018,0x018,0x0F8,0x018,0x0F8,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0F8,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x01F,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x01F,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x0FF,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x01F,0x018,0x01F,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x037,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x037,0x030,0x03F,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x03F,0x030,0x037,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x0F7,0x000,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0FF,0x000,0x0F7,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x037,0x030,0x037,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x000,0x000,0x000,0x000,0x000,0x0FF,0x000,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x036,0x036,0x036,0x036,0x036,0x0F7,0x000,0x0F7,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x018,0x018,0x018,0x018,0x018,0x0FF,0x000,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0FF,0x000,0x0FF,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x03F,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x018,0x018,0x018,0x018,0x018,0x01F,0x018,0x01F,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x01F,0x018,0x01F,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x03F,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x0FF,0x036,0x036,0x036,0x036,0x036,0x036,0x036,0x036,
0x018,0x018,0x018,0x018,0x018,0x0FF,0x018,0x0FF,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x0F8,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x01F,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,0x0F0,
0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,0x00F,
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x076,0x0DC,0x0D8,0x0D8,0x0D8,0x0DC,0x076,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x0FC,0x0C6,0x0FC,0x0C6,0x0C6,0x0FC,0x0C0,0x0C0,0x0C0,0x000,0x000,
0x000,0x000,0x0FE,0x0C6,0x0C6,0x0C0,0x0C0,0x0C0,0x0C0,0x0C0,0x0C0,0x0C0,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x080,0x0FE,0x06C,0x06C,0x06C,0x06C,0x06C,0x06C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x0FE,0x0C6,0x060,0x030,0x018,0x030,0x060,0x0C6,0x0FE,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07E,0x0D8,0x0D8,0x0D8,0x0D8,0x0D8,0x070,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x066,0x066,0x066,0x066,0x066,0x07C,0x060,0x060,0x0C0,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x076,0x0DC,0x018,0x018,0x018,0x018,0x018,0x018,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x07E,0x018,0x03C,0x066,0x066,0x066,0x03C,0x018,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x038,0x06C,0x0C6,0x0C6,0x0FE,0x0C6,0x0C6,0x06C,0x038,0x000,0x000,0x000,0x000,
0x000,0x000,0x038,0x06C,0x0C6,0x0C6,0x0C6,0x06C,0x06C,0x06C,0x06C,0x0EE,0x000,0x000,0x000,0x000,
0x000,0x000,0x01E,0x030,0x018,0x00C,0x03E,0x066,0x066,0x066,0x066,0x03C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x07E,0x0DB,0x0DB,0x0DB,0x07E,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x003,0x006,0x07E,0x0CF,0x0DB,0x0F3,0x07E,0x060,0x0C0,0x000,0x000,0x000,0x000,
0x000,0x000,0x01C,0x030,0x060,0x060,0x07C,0x060,0x060,0x060,0x030,0x01C,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x07C,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x0C6,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x0FE,0x000,0x000,0x0FE,0x000,0x000,0x0FE,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x018,0x018,0x07E,0x018,0x018,0x000,0x000,0x0FF,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x030,0x018,0x00C,0x006,0x00C,0x018,0x030,0x000,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x00C,0x018,0x030,0x060,0x030,0x018,0x00C,0x000,0x07E,0x000,0x000,0x000,0x000,
0x000,0x000,0x00E,0x01B,0x01B,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,
0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x018,0x0D8,0x0D8,0x0D8,0x070,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x018,0x018,0x000,0x07E,0x000,0x018,0x018,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x076,0x0DC,0x000,0x076,0x0DC,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x038,0x06C,0x06C,0x038,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x018,0x018,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x018,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x00F,0x00C,0x00C,0x00C,0x00C,0x00C,0x0EC,0x06C,0x06C,0x03C,0x01C,0x000,0x000,0x000,0x000,
0x000,0x0D8,0x06C,0x06C,0x06C,0x06C,0x06C,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x070,0x098,0x030,0x060,0x0C8,0x0F8,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x07C,0x07C,0x07C,0x07C,0x07C,0x07C,0x07C,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000
};                                                                                              


void outvgareg(long reg,char daatta)
{
 //
 outportb(0x3d4,reg);
 outportb(0x3d5,daatta);
}

void vga_640x480(void)
{
/*
         MOV DX,03CCH
         IN AL,DX
         AND AL,03FH
         OR AL,40H
 
         MOV DX,03C2H
         OUT DX,AL
 
         MOV DX,CRTC_ADDR
         MOV AX,9C10H
         OUT DX,AX
 
         MOV AX,8311H
         OUT DX,AX
 
         MOV AX,8F12H
         OUT DX,AX
 
         MOV AX,9615H
         OUT DX,AX
 
         MOV AX,0B916H
         OUT DX,AX
*/
 //
 outportb( 0x3c2, (inportb(0x03cc) & 0x3f) | 0x40 );
 //
 outportw( CRTCADDR, 0x9c10 );
 outportw( CRTCADDR, 0x8311 );
 outportw( CRTCADDR, 0x8f12 );
 outportw( CRTCADDR, 0x9615 );
 outportw( CRTCADDR, 0xb916 );
}

void setup_systemfont(void)
{
	 char *biosfont,*vidmem;
	 unsigned char oldmode,oldmisc,oldmem,oldmask;
	 unsigned char newmode,newmisc,newmem;
	 long fontnum,bytesperchar,i,j,k;
   
	//
	fontnum=256;
	bytesperchar=16;

	// Make a pointer to the font
	biosfont = system_font;
	vidmem = 0xA0000;

	// Store the OLD 'Mode Register' value
	outportb(GRACON_ADDR,5);
	oldmode = inportb(GRACON_ADDR+1);
	// Store the OLD 'Miscellaneous Register' value
	outportb(GRACON_ADDR,6);
	oldmisc = inportb(GRACON_ADDR+1);
	// Store the OLD 'Mask Map' value
	outportb(SEQ_ADDR,2);
	oldmask = inportb(SEQ_ADDR+1);
	// Store the OLD 'Memory Mode' value
	outportb(SEQ_ADDR,4);
	oldmem = inportb(SEQ_ADDR+1);

	// Write the NEW 'Mode Register' value
	newmode = (oldmode & 0xFC);
	outportw(GRACON_ADDR, (newmode << 8) | 0x05);
	// Write the NEW 'Miscellaneous Register' value
	newmisc = ((oldmisc & 0xF1)|4);
	outportw(GRACON_ADDR, (newmisc << 8) | 0x06);
	// Write the NEW 'Mask Map' value
	outportw(SEQ_ADDR, 0x0402);
	// Write the NEW 'Memory Mode' value
	newmem = (oldmem | 4);
	outportw(SEQ_ADDR, (newmem << 8) | 0x04);

	// Copy the font from BIOS
	for(i=0; i<256; i++)
	{
		for(j=0; j<bytesperchar; j++)
		{
			*vidmem++ = *biosfont++;
		}
		for(k=0; k<32-bytesperchar; k++)
		{
			*vidmem++ = 0x00;
		}
	}

	// Write the OLD 'Mode Register' value
	outportw(GRACON_ADDR, (oldmode << 8) | 0x05);
	// Write the OLD 'Miscellaneous Register' value
	outportw(GRACON_ADDR, (oldmisc << 8) | 0x06);
	// Write the OLD 'Mask Map' value
	outportw(SEQ_ADDR,(oldmask << 8) | 0x02);
	// Write the OLD 'Memory Mode' value
	outportw(SEQ_ADDR, (oldmem << 8) | 0x04);
}

// Works in f.e. 640x480 16c mode
//
// selects which plane to write at
void setwplane(long plane)
{
	//
	outportb(0x3c4, 0x02); outportb(0x3c5, 0x01 << (plane & 3));

	//
	return;
}

// selects which plane will be read
void setrplane(long plane)
{
 //
 outportb(GRACON_ADDR, 0x04); outportb(GRACON_ADDR+1, plane & 3);

 //
 return;
}

// made for 640x480 16c videomode(0x12)
//
void sysvga_drawpalette(void)
{
 //
 long x,y,ad,ad2,i,i2,i3,i4,c,b[4];
 char bits[]={128,64,32,16,8,4,2,1};
 BYTE *buf;

 //
 buf=(DWORD)0xa0000;

 // colors 0-15
 for(i=0; i<16; i++)
 {
  //
  b[0]=i&1;
  b[1]=i&2;
  b[2]=i&4;
  b[3]=i&8;

  // scroll through each four planes
  for(i2=0; i2<4; i2++)
  {
   //
   if(!b[i2])continue;

   //
   setwplane(i2);
   setrplane(i2);

   //
   for(y=0; y<32; y++)
   {
    //
    ad=y*640>>3;
    for(x=i*32; x<(i*32+32); x+=8)
    {
     ad2=ad+(x>>3);
     buf[ad2]=0xff;
    }
   }

   //
  }

  //
 }

 //
}

