// UNIX open, write, close test.
#include <stdio.h>

//
#define SZ	2048

//
main()
{
	int i,i2,i3,i4,fd;
	static unsigned char str[SZ];
	char fname[256];

	//
	for(i=0; i<5; i++)
	{
		//
		sprintf(fname, "otestfile%d.tmp", i);
		if( (fd = open(fname, O_CREAT))<0 )
		{
			printf("Could not create file '%s', reason=%d.\n",
				str, fd);
			break;
		}
		else	printf("%s: fd = %d.\n", 
					fname, fd);

		// Write stuff...
		for(i2=0; i2<400; i2++)
		{
			printf(".");
			memset(str, i2, SZ);
			write(fd, str, SZ);
		}
		printf("\n");
		close(fd);

		// Verify ....
		fd = open(fname, 0);
		for(i2=0; i2<400; i2++)
		{
			printf("v");
			memset(str, 0xFF, SZ);
			read(fd, str, SZ);
			for(i4=0; i4<SZ; i4++)
			{
				if(str[i4]!=(i2&255))
				{
					printf("\nChecksum error: i2=%d (0x%x), file=%s\n",
						i2,i2*SZ,fname);
					sleep(5);
					goto past;
				}
			}
		}
		close(fd);

	}

	//
past:

	//
	printf("%d files written.\n",
		i);
}

//


