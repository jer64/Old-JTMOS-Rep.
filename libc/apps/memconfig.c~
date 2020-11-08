// This program performs some basic checks about the present memory configuration
// of the application, and tells about it to the user via stdout.
//
#include <stdio.h>

//
extern DWORD appregionstart,appregionend,end;

//
int main(int argc,char **argv)
{
 //
 char *p1,*p2;

 //
 printf("Application memory size is %dK.\n", GetMemorySize()/1024);

 //
 printf("Testing malloc:\n");
 p1=malloc(1024*8);
 p2=malloc(1024*8);
 printf("allocated memory, 8K at p1=0x%x, 8K at p2=0x%x\n",p1,p2);
 free(p2); free(p1); // backwards order just to test the optimization

 //
 return 0;
}

//

