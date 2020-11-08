// ren.c - Rename a file
#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc<3)
	{
		printf("%s: missing file arguments\n", argv[0]);
		printf("Try `%s --help' for more information.\n", argv[0]);
		printf("Renames a file or files.\n");
		printf("RENAME [drive:][path]filename1 filename2.\n");
		printf("REN [drive:][path]filename1 filename2.\n");
		printf("Note that you cannot specify a new drive or path for your destination file.\n");
		return 0;
	}

	printf("%s: debug: %s => %s\n", argv[0], argv[1],argv[2]);
	rename(argv[1], argv[2]);
	return 0;
}
