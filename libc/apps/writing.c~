#include <stdio.h>

char *pattern="0123456789ABCDEFSquirrelDuckBirdDogCatExcitingWorld";
#define length	1024*256

void WriteFile(const char *name)
{
	FILE *f;
	int i;

	f = fopen(name, "wb");
	if(f==NULL) { printf("%s: error\n", __FUNCTION__); exit(1); }
	for(i=0; i<length; i++) { fputc(pattern[i&31], f); }
	fclose(f);
}

void VerifyFile(const char *name)
{
	FILE *f;
	int i;

	f = fopen(name, "rb");
	if(f==NULL) { printf("%s: error\n", __FUNCTION__); exit(2); }
	for(i=0; i<length; i++)
	{
		if( fgetc(f)!=pattern[i&31] )
		{
			printf("Verification failure on file %s at offset 0x%x\n",
				name, i);
			exit(10);
		}
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	char str[256];
	int i,i2;

	for(i=0; i<4; i++)
	{
		sprintf(str, "%s%d.dat", argv[0], i);
		printf("Writing %s ...\n", str);
		WriteFile(str);
	}

	for(i=0; i<4; i++)
	{
		sprintf(str, "%s%d.dat", argv[0], i);
		printf("Verifying %s ...\n", str);
		VerifyFile(str);
	}
}

