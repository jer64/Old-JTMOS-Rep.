#ifndef __INCLUDED_MMALLOC_H__
#define __INCLUDED_MMALLOC_H__

//
extern char free_caller[];
extern char malloc_caller[];

//
#define malloc(x) malloc1(x, __FUNCTION__)
#define free(PTR) free1(PTR, __FUNCTION__)

//
void *malloc1(int count, const char *_property_name);
void free1(void *ptr, const char *_property_name);

// Function prototypes
void *kmalloc(int l, const *_property_name);
void kfree(void *ptr);
void *procmalloc(DWORD l, int PID, const char *_property_name);
void *_kmalloc(int bytes, int PID, const char *_property_name);
void _kfree(void *ptr);
int ChangeChunkOwner(void *PTR, int PID);
int malloc_getsize(void *PTR);
void AffectProcessML(int pid, int affection);

//
//#include "fixal.h"


#endif
