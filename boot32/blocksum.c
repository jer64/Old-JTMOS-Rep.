// Calculate checksums for each of the 512 bytes
// blocks in the 32-bit kernel image.
#include <stdio.h>

//
int main(int argc, char **argv)
{
	FILE *f,*f2;
	unsigned short int sum;
	int i,i2,i3,i4,d1,bytes;

	//
	f = fopen("../kernel32/kernel32.bin", "rb");
	if(f==NULL)
		return 0;

	//
	f2 = fopen("bloche.inc", "wb");
	if(f2==NULL)
		return 0;

	//
	fprintf(f2, "; Checksums for each of 512 bytes blocks in the 32-bit kernel.\n");
	fprintf(f2, "; Helps to localize errors in kernel.\n");
	fprintf(f2, "bloche:\n");

	//
	for(bytes=0; ;)
	{
		//
		for(i=0,sum=0; i<512; i++,sum+=d1,bytes++)
		{
			//
			d1 = fgetc(f);
			if(d1==EOF)
				goto outti;
		}
		fprintf(f2, "\tdw 0x%x\n", sum);
	}
outti:
	//
	for(; bytes<(1024*512); bytes+=512)
	{
		//
		fprintf(f2, "\tdw 0x0000\n");
	}

	//
	fprintf(f2, "\tdw 0x0000\n");

	//
	fclose(f);
	fclose(f2);

	//
	return 0;
}



