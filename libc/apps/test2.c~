#include <stdio.h>

int main(int argc,char **argv)
{
	//
	FILE *f;
	int i;

	//
	f=fopen("testfile","wb");
	if(!f) { printf("error 1\n"); exit(1); }
	for(i=0; i<100; i++) { fputc('a',f); fputc('!',f); }
	fclose(f);

	//
	f=fopen("testfile","rb");
	if(!f) { printf("error 2\n"); exit(2); }
	for(i=0; i<100; i++) { printf("%c", fgetc(f)); }
	fclose(f);
	printf("\n");
	
	//
	return 0;
}
