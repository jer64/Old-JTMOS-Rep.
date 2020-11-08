// Requires ps and cp
#include <stdio.h>

int main(int argc, char **argv)
{
	char str[256];
	int key,i;

	//
	for(i=1; ; i++)
	{
		printf("== Turn %d ==\n", i);
		system("cp");
		system("ps");
		if(getch1()==27)break;
	}
	return 0;
}


