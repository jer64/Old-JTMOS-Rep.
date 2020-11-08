// WRITE TEST
#include <stdio.h>
int main(int argc,char **argv)
{
	//
	FILE *f;
	char str[255];
	int i,i2,i3,i4;

	//
	printf("File system performance test.\n");
	printf("Creating files ...\n");
	for(i=0; i<1000; i++)
	{
		i2=GetTickCount();
		sprintf(str, "wtest%d.bin", i);
		printf("%s (performance=%d)                \r", str, i3-i2);
		f=fopen(str,"wb");
		fclose(f);
		i3=GetTickCount();
	}
	printf("\nDone.\n");
	return 0;
}
