//
#include <stdio.h>
#include <string.h>

//
int main(int argc, char **argv)
{
	FILE *f;
	int i,i2,ch,ent;

	//
	if(argc<2)
	{
		printf("JTMOS TERMINAL FILE TRANSFER (HEX)\n");
		printf("Usage: jtmtr [binary file]\n");
		return 0;
	}

	//
	f = fopen(argv[1], "rb");
	fprintf(stdout, "^^BEGINNING_OF_FILE_TRANSMISSION %s\n", argv[1]);
	for(i=0,ent=0; !feof(f); i++)
	{
		if( (ch = fgetc(f))==EOF ) { break; }

		fprintf(stdout, "%.2x ", ch);
		ent = 0;
		if( (i&15)==15 )
		{
			fprintf(stdout, "\n");
			ent = 1;
		}
	}
	if(!ent)
		printf("\n");
	fprintf(stdout, "^^END_OF_FILE_TRANSMISSION %s\n", argv[1]);
	fclose(f);

	//
	return 0;
}
