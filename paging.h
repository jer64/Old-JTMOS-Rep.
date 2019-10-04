//
#ifndef __INCLUDED_PAGING_H__
#define __INCLUDED_PAGING_H__

//
void pg_init(void);
void SetPage1(DWORD *p, DWORD which, DWORD where);
void SetPage(DWORD which, DWORD where);
DWORD GenLinTable(DWORD *p, DWORD ad);
void Demonstrate(void);
void *GetKernelPageDirPTR(void);
void *GetKernelPageTablePTR(void);
void pg_enable(void);
DWORD GetPage1(DWORD *p, DWORD which);
DWORD GetPage(DWORD which);
void *_pg_alloc(int amount);
void *_pg_allocEx(int amount, DWORD mapad);
char paging_enabled;
void pg_unmapmemory(DWORD addy, int pages);
DWORD pg_mapmemory(int realpage, int pages);
void PageDump(DWORD spad, DWORD count);
void PageDumpNear(DWORD ad);
//
extern DWORD lin_walk;
extern DWORD search_walk;
//
void *pg_QuickMap(DWORD phst,
        int length,
        DWORD mapad);
DWORD FindEmptyArea(int amount);

#endif


