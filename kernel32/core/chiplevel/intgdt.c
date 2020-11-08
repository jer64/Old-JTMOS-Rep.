//===================================================================
// intgdt.c
// llint.asm multithreading system
// (C) 2001-2008 by Jari Tuominen.
//===================================================================
#include "kernel32.h"
#include "int.h"
#include "intgdt.h"

//
void randomscreen(void)
{
	char *p;
	int i;

	//
	p=0xb8000;
	for(i=0; i<(80*25*2); i++) p[i]=i;
}

//
void OnAppThread(void)
{
	int i;

	//
	for(i=0; i<100; i++)putch('a');
	// randomscreen();
	while(1);
}

//
void update_gdt_for_appthread(void)
{
	DWORD *p,*p2;
	int i,i2,i3,i4;

	//
	i = nr_curthread;
	if(i<N_MAX_THREADS && i>=0)
	{
		// Get pointers
		p=    &gdt_table->appcode32;
		p2=   thread_codedescs;
		// Copy
		p[0]= p2[0 + i*2];
		p[1]= p2[1 + i*2];

		// Get pointers
		p=     &gdt_table->appdata32;
		p2=    thread_datadescs;
		// Copy
		p[0]=  p2[0 + i*2];
		p[1]=  p2[1 + i*2];

		//
		if(p2[0 + i*2]) { OnAppThread(); }
		//  if(p2[0 + i*2]) { showappgdt(); ESCLOCK(); }
	}
	else
	{
		panic("gdt entry swapper: if(i<N_MAX_THREADS && i>=0)");
	}
}

//



