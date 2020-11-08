// plasma.c
//
#include <stdio.h>

//
void plasma(void)
{
	char col[]={	' ','°','±','²',
			'Û','²','±','°'};
	char c0l[]={	8,7,14,15,
			15,14,7,8};
	int x,y,cyc,c,c2;

	for(cyc=0; ; cyc++)
	{
		if( getch1()==27 )break;

		gotoxy(1,1);
		for(y=0; y<20; y++)
		{
			for(x=0; x<70; x++)
			{
				c = col[ ((x&y)+cyc)&7 ];
				c2 = c0l[ ((x&y)+cyc)&7 ];
				textcolor(c2);
				printf("%c",	c);
			}

			printf("\n");
		}

		Sleep(50);
	}

	textcolor(7);
	clrscr();
	printf("Thank you for watching plasma demo.\n");
}

int main(int argc,char **argv)
{
	plasma();
	return 0;
}

