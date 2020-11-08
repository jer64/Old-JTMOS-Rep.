//--------------------------------------------------------------------------------------------
// vgaplasma.c
// (C) 2002-2005 by Jari Tuominen.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <jtmos/directdisk.h>
#include <jtmos/video.h>
#include <math.h>

//
typedef struct
{
	BYTE *sin[10];
	int cycle;
}PLASMA;

//
PLASMA pl;

// Render plasma video frame.
void renderPlasmaFrame(BYTE *buf, int width, int height)
{
	int x,y,tick,ad,ad2,cycle;

	//
	cycle = pl.cycle;
	tick = GetTickCount();

	//
	for(y=cycle&1; y<height; y+=2)
	{
		ad = width*y;
		for(x=0; x<width; x++)
		{
			buf[ad+x] = (pl.sin[0][
				pl.sin[0][x+tick&65535] +
				pl.sin[0][y+tick&65535] +
				pl.sin[0][x-(tick>>1)&65535] +
				pl.sin[0][y-(tick>>2)&65535]
					+ cycle & 65535
					] >> 2) + 64;
		}
	}
}

// Calculate new sinus table.
void doSinus(BYTE *buf, int len)
{
	int i,i2,i3,i4;
	float fa,fb;

	//
	for(i=0,fa=0; i<len; i++,fa+=.014)
	{
		buf[i] = 128+(127*sin(fa));
	}
}

// Plasma effect main loop.
void plasmaEffect(BYTE *buf, int width, int height)
{
	int key;

	//
	pl.sin[0] = malloc(65536+1024);
	doSinus(pl.sin[0], 65536);

	//
	while(1)
	{
		//
		renderPlasmaFrame(buf, width, height);

		//
		waitretrace();
		drawframe(buf, 0, 64000);

		//
		pl.cycle++;
		key = getch1();
		if(key==27)break;
		SwitchToThread();
	}
}

// Plasma start function.
void plasmaStart(BYTE *image, BYTE *palette,
		int width, int height)
{
	int i,i2,i3,i4;

	//
	printf("Press any key to view image\n");
	getch();

	//
	setmode(0x13);

	//
	for(i=64,i2=0,i3=0; i<128; i++,i2+=3,i3++)
	{
		palette[i2+0] = i3;
		palette[i2+1] = i3;
		palette[i2+2] = i3;
	}

	// Offset 64, amount 64 palette entries, palette *
	setpalettemap(64, 64, palette);

	//
	plasmaEffect(image, width, height);

	//
	setmode(3);
}

// Program main function.
int main(int argc, char **argv)
{
	BYTE *image,*palette;

	//
	printf("Welcome to plasma demo!\n");

	//
	image =		malloc(1024*64);
	palette =	malloc(256*3);
	plasmaStart(image, palette, 320,200);
	free(image);
	free(palette);

	//
	printf("Thank you for watching plasma demo.\n");
	return 0;
}

