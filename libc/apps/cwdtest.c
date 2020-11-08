//
#include <stdio.h>

//
int main(int argc, char **argv)
{
	char dir[256];

	//
	getcwd(dir, 256);
	printf("Current work directory = '%s'.\n",
		dir);
	return 0;
}
