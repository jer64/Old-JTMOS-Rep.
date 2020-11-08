#include <stdio.h>
main()
{
	int heads,sectors,cylinders,blocknr,
		cylindernr,headnr,sectornr,i;

	//
	cylinders =	895;
	heads =		5;
	sectors =	55;

	//
//	blocknr = 56;
//	blocknr = 291;
	blocknr = 1024+16;

	// CONVERT LINEAR TO CHS
	sectornr = (blocknr % sectors)+1;
	headnr = (blocknr / sectors) % heads;
	cylindernr = (blocknr / sectors / heads);

	//
	printf("CHS %d,%d,%d\n",
		cylindernr,headnr,sectornr);
	printf("CHS 0x%x,0x%x,0x%x\n",
		cylindernr,headnr,sectornr);
}
