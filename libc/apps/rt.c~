// ramdisk tool
#include <stdio.h>
#define HD_FILE		"/hda/ramdisk.img"
#define RAM_FILE	"ram"
#define HDA_FILE	"hda"

// Store ramdisk image on hard disk
void store(void)
{
	FILE *f,*f2;
	int l,i,i2;

	// Test hda
	f=fopen(HDA_FILE, "rb");
	if(f)
		fclose(f);
		else
		{
			printf("No hard disk detected.\n");
			return;
		}

	// Open ramdisk
	f=fopen(RAM_FILE, "rb");
	if(f==NULL)
	{
		printf("Unable to open ramdisk.\n");
		return;
	}

	// Open ramdisk file on hard disk
	f2=fopen(HD_FILE, "wb");
	if(f2==NULL)
	{
		printf("Unable to write file on hard disk.\n");
		fclose(f);
		return;
	}

	// Get size
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	fseek(f, 0, SEEK_SET);

	//
	printf("Now storing ...\n");
	// Copy from stream to stream
	for(i=0; i<l; i++)
	{
		//
		if( !(i&4095) )
		{
			//
			printf("%dK / %dK (%dK left)        \r",
				i/1024,l/1024, (l-i)/1024);
		}

		//
		fputc(fgetc(f), f2);
	}
	printf("\nDone.\n");

	// Close streams
	fclose(f);
	fclose(f2);
}

// Restore ramdisk from hard disk
void restore(void)
{
	FILE *f,*f2;
	int l,i,i2;
	int dnr;

	// Get ramdisk's device ID
	dnr = getdevicenrbyname(RAM_FILE);

	// Test hda
	f=fopen(HDA_FILE, "rb");
	if(f)
		fclose(f);
	else
		{
			printf("No hard disk detected.\n");
			return;
		}

	// Open ramdisk image file
	f=fopen(HD_FILE, "rb");
	if(f==NULL)
	{
		printf("Unable to open ramdisk image file.\n");
		return;
	}

	// Prepare ramdisk
	diskChange(dnr);

	// Write on ramdisk
	f2=fopen(RAM_FILE, "wb");
	if(f2==NULL)
	{
		printf("Unable to write file on ramdisk.\n");
		fclose(f);
		return;
	}

	// Get size
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	fseek(f, 0, SEEK_SET);

	//
	printf("Now restoring ... \n");
	// Copy from stream to stream
	for(i=0; i<l; i++)
	{
		//
		if( !(i&4095) )
		{
			//
			printf("%dK / %dK (%dK left)        \r",
				i/1024,l/1024, (l-i)/1024);
		}

		//
		fputc(fgetc(f), f2);
	}

	// Close streams
	fclose(f);
	fclose(f2);
	printf("\nDone.\n");

	//
	printf("Refreshing %s device ... ",
		RAM_FILE);
	diskChange(dnr);
	printf("OK.\n");
}

// Ramdisk tool main function
int main(int argc, char **argv)
{
	//
	if(argc<2)
	{
		//
		printf("Usage: %s [options]\n", argv[0]);
		printf("-s     store ramdisk     %s -> %s\n",
			RAM_FILE, HDA_FILE);
		printf("-r     restore ramdisk   %s -> %s\n",
			HDA_FILE, RAM_FILE);
		return 0;
	}

	//
	if(!strcmp(argv[1], "-s"))
	{
		//
		store();
	}

	//
	if(!strcmp(argv[1], "-r"))
	{
		//
		restore();
	}

	//
	return 0;
}



