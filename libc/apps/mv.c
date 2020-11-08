#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc<3)
	{
		printf("%s: missing file arguments\n", argv[0]);
		printf("Try `%s --help' for more information.\n", argv[0]);
		return 0;
	}

	printf("%s: debug: %s => %s\n", argv[0], argv[1],argv[2]);
	return 0;
}
