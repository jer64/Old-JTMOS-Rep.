// WRITE TEST
#include <stdio.h>
int main(int argc,char **argv)
{
	//
	FILE *f;
	char *textdata="Here is some text that will be saved on a file.\0";
	int i;

	//
	printf("argc=%d\n", argc);
	for(i=0; i<argc; i++)
	{
		printf("%d: %s, ", i,argv[i]);
	}
	printf("\n");

	//
	printf("Creating file ...\n");
	f=fopen("wtest.bin","wb");
	printf("Writing to file ...\n");
	fwrite(textdata, strlen(textdata)+1, 1, f);
	printf("Closing the file ...\n");
	fclose(f);

	//
	f=fopen("wtest.bin","rb");
	if(!f)
	{
		printf("File not found on 'wtest.bin'\n");
	}
	else
	{
		fclose(f);
		printf("File found.\n");
	}

 //
 return 0;
}
