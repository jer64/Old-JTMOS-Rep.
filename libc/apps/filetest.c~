// This sophisticated application was
// brought to you by Jari Tuominen!!
//
#include <stdio.h>

//
int main(int argc,char **argv)
{
 //
 FILE *f;
 char str[255];
 long i,i2,i3,i4;

 //
 for(i=0; i<10; i++)
 {
  sprintf(str,"hellofile_%d.txt",i);
  printf("Creating '%s'\n",str);
  f=fopen(str,"wb"); 
  fclose(f);
 }

 //
 for(i=0; i<10; i++)
 {
  sprintf(str,"hellofile_%d.txt",i);
  printf("Removing '%s'\n",str);
  remove(str);
 }

 //
 return 0;
}

//

