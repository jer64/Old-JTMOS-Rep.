// mkdir.c
#include <stdio.h>

//
int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("%s: too few arguments\n", argv[0]);
		printf("Try `%s --help' for more information.\n", argv[0]);
		return 0;
	}

	mkdir(argv[1]);
	return 0;
}

//

