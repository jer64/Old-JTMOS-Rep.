#ifndef __SCANMEM_H__
#define __SCANMEM_H__

typedef struct
{
 //
 long amount,unusable,usable;
}MEMORY_INFORMATION;
extern MEMORY_INFORMATION ms;

extern long scanmem1(void);
extern long scanmem(void);
extern long GetRamAmount(void);

#endif