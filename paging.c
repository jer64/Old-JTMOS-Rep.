/////////////////////////////////////////////////////////////////////////////////////////////////
//
// paging.c
// (C) 2004-2006 by Jari Tuominen.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "kernel32.h"
#include "mm.h"
#include "paging.h"

// Set the page directory near 2 megabyte area.
static DWORD *page_directory;
static DWORD *physical_page_directory;
// Page directory starts exactly at 2 megabyte area.
static DWORD *page_table;
static DWORD *user_page_tables;
//
char paging_enabled = FALSE;

//
DWORD lin_walk;
//
DWORD search_walk;
static DWORD search_walk_start;
static DWORD search_walk_end;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
DWORD pg_mapmemory(int realpage, int pages)
{
	int i,i2,i3,i4;
	DWORD dpa,rad;

	/****dpa = lin_walk / PAGESIZE;***/

	//------------------------------------------------
	//
	dpa = FindEmptyArea(pages) / PAGESIZE;
	rad = realpage * PAGESIZE;
	for(i=0; i<pages; i++,rad+=PAGESIZE)
	{
		SetPage(dpa+i, rad | 3);
	}

	//
	return dpa*PAGESIZE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
void pg_unmapmemory(DWORD addy, int pages)
{
	int i,i2,i3,i4;
	DWORD dpa;

	//------------------------------------------------
	//
	dpa = addy / PAGESIZE;
	for(i=0; i<pages; i++)
	{
		SetPage(dpa+i, 2);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
DWORD FindEmptyArea(int amount)
{
	DWORD i,i2,i3,i4,l,ad,ad2,cnt;

	//
	for(cnt=0,ad=0; cnt<amount; search_walk++)
	{
		//
		if(search_walk >= search_walk_end)
		{
			search_walk = search_walk_start;
			cnt = 0;  ad = 0;
		}

		// 3=su,rw,p (011)
		if( !(GetPage(search_walk)&(~3)) )
		{
			if(!cnt)
				ad = search_walk*PAGESIZE;
			cnt++;
		}
		else
		{
			ad = 0;  cnt = 0;
		}
	}

	// Skip by 16 KB.
	search_walk += 4;

	//
	if(!ad || cnt<amount)
		panic("FindEmptyArea error...");

	//
	return ad;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
void *_pg_alloc(int amount)
{
	DWORD plus,addy;

	// Search for empty area to map the memory.
	addy = FindEmptyArea(amount);

	//
	return _pg_allocEx(amount, addy);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Allocate & map memory at the requested location in memory.
// NOTE: All allocations made by this function will currently be permanent (TODO).
//
void *_pg_allocEx(int amount, DWORD mapad)
{	
	int i,i2,i3,i4;
	int pages,spa,dpa; // amount of pages to copy, source page address, destination page address...
	void *tmp;
	DWORD ad;

	//--------------------------------------------------
	//
	pages = (amount/PAGESIZE)+1;
	dpa = (mapad/PAGESIZE);

	//
	tmp = procmalloc(amount,0, __FUNCTION__);
	if(tmp==NULL)
		return NULL;
	ad = tmp;
	spa = (ad/PAGESIZE);

	//--------------------------------------------------
	// Copy pages to the mapad location.
	//
	for(i=0; i<pages; i++)
	{
		SetPage(dpa+i, GetPage(spa+i));
	}

	// Zero the pages at the temp. alloc region.
	for(i=0; i<pages; i++)
	{
		SetPage(spa+i, 0);
	}

	//
	return mapad;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Returns pointer in linear memory map (not paged).
//
void *GetKernelPageDirPTR(void)
{
	return page_directory;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Returns pointer in linear memory map (not paged).
//
void *GetKernelPageTablePTR(void)
{
	return page_table;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
void Demonstrate(void)
{
	int i,i2,i3,i4,l,x;
	BYTE *p;
	
	//
	for(x=0,p = 0xB8000; ; x++)
	{
		//
		l = 80*25*2;
		for(i=0; i<l; i++)
			p[i] += i+x;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Generate a linearly mapped page table, size of 4 megabytes.
//
DWORD GenLinTable(DWORD *p, DWORD ad)
{
	int i,i2;

	// Map the first 4MB of memory. Chunk by chunk.
	for(i=0; i<1024; i++,ad+=PAGESIZE)
	{
		p[i] = ad | 3;
	}
	return ad;
}

//
DWORD GetPage(DWORD which)
{
	//
	return GetPage1(page_table, which);
}

//
DWORD GetPage1(DWORD *p, DWORD which)
{
	//
	return p[which];
}

//
void SetPage(DWORD which, DWORD where)
{
	//
	SetPage1(page_table, which,where);
}

// Set page to point to the specified location.
// page table PTR, which page entry (0 - 1024*1024), point to where | flags
void SetPage1(DWORD *p, DWORD which, DWORD where)
{
	//
	if( which >= (1024*1024) )
		panic("SetPage1 illegal access");
	p[which] = where;
}

//
void pg_enable(void)
{
	// Enable paging.
	SETCR0(GETCR0() | 0x80000000);
}

//-------------------------------------------------------------------------
// Map memory from physical memory map to logical memory map.
//
// Physical start, length, where to map at
//
void *pg_QuickMap(DWORD phst,
	int length,
	DWORD mapad)
{
	DWORD ad,ad2,i,i2,l;

        // Map. the mm allocation
        l = (length/PAGESIZE)+1;

	//
	for(i=mapad/PAGESIZE,i2=0,ad=phst; i2<l; i2++,ad+=PAGESIZE)
	{
		// 3=su,rw,p (011)
		SetPage(i+i2, ad|3);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize paging ...
//
void pg_init(void)
{
	DWORD i,i2,i3,i4,l;
	DWORD *p,*p2,ad,ad2;

	//------------------------------------------------------------------------------
	printk("Paging initializing:\n");
	printk("- Page tables ...\n");

	// Give initial values.
	paging_enabled = FALSE;
	//
	search_walk_start = 	0x40000000/PAGESIZE;
	search_walk_end =   	0x70000000/PAGESIZE;
	search_walk =		search_walk_start;
	//
	page_directory =	0x1FF000; // 4 KB
	page_table =		0x200000; // 4 MB
	user_page_tables =	0x600000; // 2 MB
	memset(page_table, 0, 1024*1024*4);
	memset(user_page_tables, 0, 1024*1024*2);
	physical_page_directory = NULL;

	//------------------------------------------------------------------------------
	// 1) wipe the whole table
	l = 1024*1024;
	printk("(1) ");
	for(i=0; i<l; i++)
	{
		// non-present
		SetPage(i, 0);
	}

	// 2) map the first 8 megabytes straight address to address.
	l = 1024*1024*8/PAGESIZE;
	printk("(2) ");
	for(i=0,ad=0; i<l; i++,ad+=PAGESIZE)
	{
		// 3=su,rw,p (011)
		SetPage(i, ad|3);
	}

	//
	if(vesa_frame_buf!=NULL)
	{
		// MAP vesa_frame_buf. Map megabyte of video RAM at 0xF0000000.
		l = 1024*1024*1/PAGESIZE;
		ad2 = vesa_frame_buf;
		for(i=ad2/PAGESIZE,ad=ad2,i2=0; i2<l; i++,i2++,ad+=PAGESIZE)
		{
			// 3=su,rw,p (011)
			SetPage(i, ad|3);
		}
	}

	// Set zero page as non-present.
	SetPage(0, 2); // 0x0000

	//------------------------------------------------------------------------------
	//
	printk("- Page directory ...\n");
	printk("(3) ");
	p = page_directory;
	for(i=0,ad=page_table; i<1024; i++,ad+=PAGESIZE)
	{
		// 3=supervisor,rw,present (011)
		p[i] = ad | 3;
	}

	// Map page directory at 0x10000000.
	l = 1024*1024*8/PAGESIZE;
	i2 = 0x10000000/PAGESIZE; // to be mapped
	//i3 = 0x1F0000/PAGESIZE; // to be unmapped
	for(i=0,ad=0; i<l; i++,ad+=PAGESIZE)
	{
		// 3=su,rw,p (011)
		// Map new location for page directory.
		SetPage(i+i2, ad|3);

		// Unmap old location.
		//if(i>=i3)
		//SetPage(i, ad|1); // read-only
	}

	// 1F0000-1FFFFF           PAGE DIRECTORY, ETC.
	// 200000-5FFFFF           KERNEL PAGE TABLES
	// 600000-7FFFFF           USER PAGE TABLES
	//
	physical_page_directory = page_directory;
	
	//------------------------------------------------------------------------------
	printk("- Enabling paging ... ");

	// Set page directory location.
	SETCR3(physical_page_directory);
	//
	pg_enable();
	printk("OK.\n");

	//
//	page_directory += (DWORD)0x10000000;
//	page_table += (DWORD)0x10000000;

	//
	printk("\nDone (CR0 = 0x%x).\n",
		GETCR0());
	printk("page_directory = 0x%x\n",	page_directory);
	printk("page_table =     0x%x\n",	page_table);

	//
	ad = p_mm;
	printk("p_mm =      0x%x\n",	ad);
	printk("p_mm page = 0x%x\n",	GetPage(ad/4096));

	//
	paging_enabled = TRUE;

	//
}

//
void PageDump(DWORD spad, DWORD count)
{
	DWORD i,i2,pad;

	// Page Dump At [Address]
	printk("PDD@%x: ", spad);
	for(i=0,pad=spad; i<count; i++,pad++)
	{
		printk("%x, ", GetPage(pad));
	}
	printk("\n");
}

//
void PageDumpNear(DWORD ad)
{
	DWORD pad;
	int amount;

	//
	pad = ad/PAGESIZE;

	//
	PageDump(pad,20);
}

//




