// JTMOS LIBC DYNAMICAL MEMORY ALLOCATION TEST
#include <stdio.h>
int main(int argc,char **argv)
{
 //
 int i;
 char *data1[10];

 //
 printf("JTMOS LIBC dynamic memory test\n");
 //
 printf("%dK dynamic memory free.\n", GetAmountFree()/1024);
 //
 printf("Doing some mallocs.\n");
 for(i=0; i<10; i++) { data1[i]=malloc(1024); }
 //
 printf("%dK dynamic memory free.\n", GetAmountFree()/1024);
 //
 printf("Freeing memory.\n");
 for(i=0; i<10; i++) { free(data1[i]); }
 printf("%dK dynamic memory free.\n", GetAmountFree()/1024);

 //
 return 0;
}
