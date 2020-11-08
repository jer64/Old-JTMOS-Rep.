// Background process
//
#include <stdio.h>

//
int main(int argc,char **argv)
{
 //
 char str[255];
 int i,i2,i3,i4;

 //
 while(1)
 {
  //
  for(i=0; i<(1024*16); i++) {
		str[i&255] = i;
		str[(i+64)&255] = 0;
		str[(i+128)&255] = 128;
	}

  //
  Sleep(100);
 }

 //
 return 0;
}

//
