//
#ifndef __INCLUDED_COPROCESSOR_NOT_FOUND_H__
#define __INCLUDED_COPROCESSOR_NOT_FOUND_H__

//
void coprocessor_not_found(void);
// coprocessor.asm
extern void fpu_restore(void *ptr);
extern void fpu_save(void *ptr);
extern int mathHandling;

#endif




