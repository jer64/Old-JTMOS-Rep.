// kchecksum.c
#include <stdio.h>

int main(int argc,char **argv)
{
	//
	FILE *f;
	unsigned short int sum,u;

	//
	if(argc<2)
	{
		fprintf(stderr,"kchecksum.c by Jari Tuominen\n");
		fprintf(stderr,"Usage: kchecksum [source.bin]\n");
		fprintf(stderr,"Program outputs data at stdout, errors at stderr.\n");
		return 0;
	}

	//
	f=fopen(argv[1],"rb");
	if(!f)return 0;
	for(sum=0; !feof(f); )
	{
		u = ((unsigned char)fgetc(f));
		sum += u;
	}
	fclose(f);

	//
	fprintf(stdout,"; KERNEL CHECKSUM\n");
	fprintf(stdout,"kchecksum equ 0x%1.4x\n",sum&0xffff);

	//
	return 0;
}
