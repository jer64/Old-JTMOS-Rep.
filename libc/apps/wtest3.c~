#include <stdio.h>

int main(int argc,char **argv)
{
	FILE *f;
	int i;
	if(argc<2)
	{
		printf("Usage: %s [file name]\n", argv[0]);
		return 0;
	}

	f=fopen(argv[1],"rb");
	if(!f) 
	{
		printf("'%s': File not found.\n", argv[1]);
	}
	else
	{
		fclose(f);
		printf("File '%s' was found.\n", argv[1]);
	}

	f=fopen(argv[1],"wb");
	if(!f)return 1;
	for(i=0; i<(1024*64); i++) { fputc(i&255,f); }
	fclose(f);
	return 0;
}
