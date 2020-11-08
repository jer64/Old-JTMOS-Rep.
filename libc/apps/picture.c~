// picture.c - Shows a VGA 320x200 256C picture
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>

void read_picture(BYTE *image)
{
	WORD chk;
	int i,i2,i3,i4,devnr;
	FILE *f;

	printf("Now reading picture ...\n");
	f=fopen("sqfam.raw", "rb");
	if(!f)
	{
		printf("File not found: 'sqfam.raw'\n");
		exit(1);
	}
	memset(image, 0, 64000);
	for(i=0; i<64000; i++) { image[i] = fgetc(f); }
	//fread(image, 64000,1, f);
	for(i=0,chk=0; i<64000; i++) { chk+=image[i]; }
	fclose(f);
	printf("Image checksum = 0x%x\n", chk);
}

void view_picture(BYTE *image, BYTE *palette)
{
	int i,i2,i3,i4;
	BYTE *buf;

	buf = malloc(64000);
	for(i=0; i<64000; i++) { buf[i]=(image[i]>>2)+64; }

	printf("Press any key to view image\n");
	getch();

	setmode(0x13);
	drawframe(buf, 0, 64000);
	for(i=64,i2=0,i3=0; i<128; i++,i2+=3,i3++)
	{
		palette[i2+0] = i3;
		palette[i2+1] = i3;
		palette[i2+2] = i3;
	}
	// offs 64, len 64, buffer ptr to palette
	setpalettemap(64, 64, palette);
	getch();
	setmode(3);
	free(buf);
}

int main(int argc, char **argv)
{
	BYTE *image,*palette;

	printf("Welcome to picture demo!\n");

	image =		malloc(1024*64);
	palette =	malloc(256*3);
	read_picture(image);
	view_picture(image, palette);
	free(image);
	free(palette);

	printf("Thank you for watching picture demo.\n");
	return 0;
}

