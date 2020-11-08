#define DEV_NAME "floppy"
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>

void read_picture(BYTE *image)
{
	int i,i2,i3,i4,devnr;

	printf("Reading 64K picture ...\n");

	devnr=getdevnrbyname(DEV_NAME);
	printf("Assigning device %s = %d\n",
			DEV_NAME, devnr);

	for(i=0; i<((1024*64)/512); i++)
	{
		printf("block %d        \r", i);
		readblock(devnr, i, image+i*512);
	}
	printf("\nRead done.\n");
}

void view_picture(BYTE *image, BYTE *palette)
{
	int i,i2,i3,i4;

	printf("Press any key to view image\n");
	getch();

	setmode(0x13);
	drawframe(image);
	for(i=0,i2=0; i<256; i++,i2+=3)
	{
		palette[i2+0] = i>>2;
		palette[i2+1] = i>>2;
		palette[i2+2] = i>>2;
	}
	setpalettemap(0, 256, palette);
	getch();
	setmode(3); clrscr();
}

int main(int argc, char **argv)
{
	BYTE *image,*palette;

	printf("320x200 picture demo\n");

	image=malloc(1024*64);
	palette=malloc(256*3);
	read_picture(image);
	view_picture(image, palette);
	free(image);
	free(palette);

	printf("picture.c: exit\n");

	return 0;
}

