// 8to1.c
#include <stdio.h>

//
int main(int argc,char **argv)
{
 //
 unsigned char bits[]={128,64,32,16,8,4,2,1};
 unsigned long i,i2,i3,i4,i5,i6,i7,i8,d[8];
 FILE *f;

 //
 if(argc<2)
 {
  printf("8to1 by Jari Tuominen(jari@vunet.org)\n");
  printf("Usage: 8to1 [source]\n");
  printf("Description: performs a 8-Bit to 1-Bit conversion by shifting bits\n");
  printf("outputs to stdout\n");
  return 0;
 }

 //
 fprintf(stderr,"converting %s\n",argv[1]);

 //
 f=fopen(argv[1],"rb");
 if(!f){fprintf(stderr,"cannot open source(%s)",argv[1]); return 1;}
 fseek(f,0,SEEK_END); i2=ftell(f);
 fseek(f,0,SEEK_SET);
 for(i=0; i<(i2>>3); i++)
 {
  if(feof(f))break;
  for(i3=0; i3<8; i3++){ if(feof(f))break; d[i3]=fgetc(f); }
  for(i3=0,i4=0; i3<8; i3++)
  {
   if(d[i3])
	i4|=bits[i3&7];
  }
  fputc(i4,stdout);
 }
 fclose(f);

 //
 return 0;
}
