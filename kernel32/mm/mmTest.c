// mmTest.c
#include <stdio.h>
#include "mm.h"
#include "mmTest.h"
// Enable or disable debug print messages
//#define TPRINT printf
#define TPRINT //

//
void mmTest(void)
{
	BYTE *EGA;
	int i,i2,ii,times;
	BYTE *tmp,*tmp2,*tmp3, *big[100];

	//
	printf("Testing MM 3.0\n");

	//
	for(times=1; times<5; times++)
	{
		//
		printf(".");	
		//
		TPRINT("---------------------------------------------\n");

		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
		TPRINT("Allocating big thing:\n");
		for(i=0; i<1000; i++) { big[i] = malloc(583); }
	
		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
	
		//
		tmp = malloc(1024*64);
		tmp3 = malloc(10);
		tmp2 = malloc(1024*64);
		TPRINT("(1) tmp=0x%x, tmp2=0x%x, tmp2-tmp=%d\n",
			tmp, tmp2, tmp2-tmp);
		free(tmp); free(tmp2);
		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
		//
		tmp = malloc(1024*128);
		tmp2 = malloc(1024*128);
		TPRINT("(2) tmp=0x%x, tmp2=0x%x, tmp2-tmp=%d\n",
			tmp, tmp2, tmp2-tmp);
		free(tmp); free(tmp2);
		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
		//
		tmp = malloc(1024*256);
		tmp2 = malloc(1024*256);
		TPRINT("(3) tmp=0x%x, tmp2=0x%x, tmp2-tmp=%d\n",
			tmp, tmp2, tmp2-tmp);
		free(tmp); free(tmp2);
		//
		free(tmp3);
		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
		//
		TPRINT("Freeing big thing:\n");
		for(i=0; i<1000; i++) { free(big[i]); }
		//
		TPRINT("mm.n_allocated = %d\n", p_mm->n_allocated);
	
		//mmPause();
	}
	TPRINT("\n");

//	while(1);
}




