#include <stdio.h>
#include <math.h>

main()
{
	char str[256];
	double f=4732.473247437;
	int tmp;

	printf("f = %s\n", ecvt(f,10,&tmp,&tmp));
}

