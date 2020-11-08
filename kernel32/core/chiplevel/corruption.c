// ======================================================
// COUNTER CORRUPTION FUNCTIONS
// (C) 2002 by Jari Tuominen
// ======================================================
#include "kernel32.h"
#include "corruption.h"

//
void SetCorruptionCheck(BYTE *buf)
{
        buf[0] = 0x55;
        buf[1] = 0xAA;
        buf[2] = 0xFC;
        buf[3] = 0xE2;
}

// Zero =       OK
// Non-zero =   Error
int GetCorruptionCheck(BYTE *buf)
{
        if(buf[0] != 0x55)return 1;
        if(buf[1] != 0xAA)return 1;
        if(buf[2] != 0xFC)return 1;
        if(buf[3] != 0xE2)return 1;
        return 0;
}

