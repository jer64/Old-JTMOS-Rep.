//--------------------------------------------------------------------------------------------
// ansiwin.c - ansi windows.
// (C) 1999-2003 by Jari Tuominen
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include "ansiwin.h"

/////////////////////////////////////////////////////////////////////////////
//DRAWANSIWINDOW
//
//Description :
//
//This function is a very nice function to draw Windows,
//it can also setup window arrays, if requested.
//
//      ***************
//      ***************
//      *************** <-- First of all, if fill is set to TRUE (!=0),
//      ***************     it'll fill window area with empty character.
//      ***************
//
//
//   x1,y1
//      ---------------
//      |             |
//      |             | <-- Type&127
//      |             |
//      ---------------
//                  x2,y2
//
//
//      Type :
//
//              1 = Inactive window
//              2 = Active window
//              3 = Inactive window + headline
//              4 = Active window + headline
//
//      If you "OR" 128 to the TYPE then drawansiwindow function will
//      call function "window" to setup window borders.
//
//      Notice: Drawansiwindow moves cursor inside the window, if not
//              requested to not to do it.
//
//
void DrawAnsiWindow(int x1,int y1,int x2,int y2,
		int type, char *headline,int fill)
{
	char stringi[100];
	register unsigned x,y;

	//
	for(x=0; x<(x2-x1); x++)stringi[x]=' '; stringi[x]=0;
	for(x=0; x<(x2-x1); x++)stringi[x]=' '; stringi[x]=0;
//	window(1,1,80,25);

	//
	if(fill!=0)
	{
		for(y=y1; y<y2; y++)
		{
			gotoxy(x1,y);
			printf("%s", stringi);
		}
	}

	//
	if((type&127)==0)
	{
		//
		gotoxy(x1,y1); printf("Ú");
		gotoxy(x2,y1); printf("¿");
		gotoxy(x1,y2); printf("À");
		gotoxy(x2,y2); printf("Ù");
		for(x=x1+1; x<x2; x++){gotoxy(x,y1); printf("Ä");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y2); printf("Ä");}
		for(y=y1+1; y<y2; y++){gotoxy(x1,y); printf("³");}
		for(y=y1+1; y<y2; y++){gotoxy(x2,y); printf("³");}
//		if((type&128)!=0) window(x1+1,y1+1,x2-1,y2-1);
	}

	//
	if((type&127)==1)
	{
		//
		gotoxy(x1,y1); printf("É");
		gotoxy(x2,y1); printf("»");
		gotoxy(x1,y2); printf("È");
		gotoxy(x2,y2); printf("¼");
		for(x=x1+1; x<x2; x++){gotoxy(x,y1); printf("Í");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y2); printf("Í");}
		for(y=y1+1; y<y2; y++){gotoxy(x1,y); printf("º");}
		for(y=y1+1; y<y2; y++){gotoxy(x2,y); printf("º");}
//		if((type&128)!=0) window(x1+1,y1+1,x2-1,y2-1);
	}

	//
	if((type&127)==2)
	{
		//
		gotoxy(x1,y1); printf("Ú");
		gotoxy(x2,y1); printf("¿");
		gotoxy(x1,y2); printf("À");
		gotoxy(x2,y2); printf("Ù");
		gotoxy(x1+1,y1+1); printf("%s", headline);
		for(x=x1+1; x<x2; x++){gotoxy(x,y1+2); printf("Ä");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y1); printf("Ä");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y2); printf("Ä");}
		for(y=y1+1; y<y2; y++){gotoxy(x1,y); printf("³");}
		for(y=y1+1; y<y2; y++){gotoxy(x2,y); printf("³");}
		gotoxy(x1,y1+2); printf("Ã");
		gotoxy(x2,y1+2); printf("´");
//		if((type&128)!=0) window(x1+1,y1+3,x2-1,y2-1);
	}

	//
	if((type&127)==3)
	{
		//
		gotoxy(x1,y1); printf("É");
		gotoxy(x2,y1); printf("»");
		gotoxy(x1,y2); printf("È");
		gotoxy(x2,y2); printf("¼");
		gotoxy(x1+1,y1+1); printf("%s",headline);
		for(x=x1+1; x<x2; x++){gotoxy(x,y1+2); printf("Í");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y1); printf("Í");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y2); printf("Í");}
		for(y=y1+1; y<y2; y++){gotoxy(x1,y); printf("º");}
		for(y=y1+1; y<y2; y++){gotoxy(x2,y); printf("º");}
		gotoxy(x1,y1+2); printf("Ì");
		gotoxy(x2,y1+2); printf("¹");
//		if((type&128)!=0) window(x1+1,y1+3,x2-1,y2-1);
	}

	//
	if((type&127)==4)
	{
		//
		gotoxy(x1,y1); printf("Ú");
		gotoxy(x2,y1); printf("¿");
		gotoxy(x1,y2); printf("À");
		gotoxy(x2,y2); printf("Ù");
		gotoxy(x1+1,y1+1); printf("                                                                           ");
		gotoxy(x1+1,y1+1); printf(headline);
		for(x=x1+1; x<x2; x++){gotoxy(x,y1+2); printf("Ä");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y1); printf("Ä");}
		for(x=x1+1; x<x2; x++){gotoxy(x,y2); printf("Ä");}
		for(y=y1+1; y<y2; y++){gotoxy(x1,y); printf("³");}
		for(y=y1+1; y<y2; y++){gotoxy(x2,y); printf("³");}
		gotoxy(x1,y1+2); printf("Ã");
		gotoxy(x2,y1+2); printf("´");
//		if((type&128)!=0) window(x1+1,y1+3,x2-1,y2-1);

		//
		if(((type&127)==2 || (type&127)==4) && (type&128)==0 )gotoxy(x1+1,y1+3);
		if(((type&127)==1 && (type&128)==0))gotoxy(x1+1,y1+1);
		//
		if(((type&127)==2 || (type&127)==4) && (type&128)!=0 )gotoxy(1,1);
		if(((type&127)==1 && (type&128)!=0))gotoxy(1,1);
	}
}
