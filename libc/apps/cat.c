#include <stdio.h>
#include <jtmos/filefind.h>

void cat_now(const char *src)
{
	FILE *f;
	int da,i;

	f=fopen(src, "rb");
	if(!f)
	{
		printf("%s: File not found\n", src);
		return;
	}

	for(i=0; ; i++)
	{
		da = fgetc(f);
		if( da==EOF )break;
		printf("%c", da);

		if(i>=100)
		{
			i=0;
			if(getch1())break;
		}
	}
	fclose(f);
}

int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("%s: missing file argument(s)\n", argv[0]);
		printf("Try `%s --help' for more information.\n", argv[0]);
		return 0;
	}

	cat_now(argv[1]);
	return 0;
}
