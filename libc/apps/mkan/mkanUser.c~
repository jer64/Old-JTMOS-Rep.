//
#include <stdio.h>
#include "mkanUser.h"

//
int extparam[100]={0};
int ixl,iyl;
unsigned char *extpointer[100]={NULL};
unsigned char *vb=NULL;

//
#ifdef DJGPP
//
int jgetch(void)
{
	if(bioskey(1))
		return getch();
	else
		return 0;
}
#endif

//
void waitkey(void)
{
	char str[256];

	//
	printf("Press enter ...\n");
	fgets(str, 250, stdin);
}
