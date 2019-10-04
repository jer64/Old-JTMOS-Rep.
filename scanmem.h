#ifndef __SCANMEM_H__
#define __SCANMEM_H__

typedef struct {
    // 
    int            amount,
                    unusable,
                    usable;
} MEMORY_INFORMATION;
extern MEMORY_INFORMATION ms;

extern int     scanmem1(void);
extern int     scanmem(void);
extern int     GetRamAmount(void);

#endif
