// Parse a binary file and search for readable texts.
#include <stdio.h>

//
int main(int argc, char **argv)
{
	FILE *f,*f2;
	int i,i2,i3,i4,d;

	//
	if(argc<3)
	{
		printf("Usage: %s [source] [output]\n",	
			argv[0]);
		return 0;
	}

	//
	f = fopen(argv[1], "rb");
	if(f==NULL)
		return 1;
	f2 = fopen(argv[2], "wb");
	if(f2==NULL)
		return 2;

	//
	for(i=0; ; )
	{
		//
		if( (d = fgetc(f))==EOF )
			break;

		//
		if(isalpha(d) || isdigit(d) || d==32)
		{
			fputc(d, f2);
			if( (i++)==69 )
			{
				i=0;
				fputs("\n\r", f2);
			}
		}
	}

	//
	fclose(f);
	fclose(f2);

	//
	return 0;
}

//


