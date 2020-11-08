// mkan.c - Make animation v1.2003
#include <stdio.h>
#include "mkan.h"

//
int main(int argc,char **argv)
{
	int XLEN=320,YLEN=200,CXPON=160,CYPON=100;
	char str[255],str2[255];
	int BGBYTE=0,DITHER=1,MAXI=0,FORCESIZE=0,NST=0,BEGIVAL=0; // 0=0 1 2, 1=0000 0001 0002
	int ad,ad2,ad3,ad4;
	char fname[255];

	//
	extparam[11]=1; // Interpolation on.
	vb=malloc(320*200+1024*4);
	extparam[8]=0xFFFFFFFF;
	extpointer[0]=malloc(320*240);
	extparam[4]=0; // No background picture as default.
	extparam[5]=0; // No blurring as default (BLUR VALUE SET TO ZERO)

	//
	extparam[7]=0; // No 2 color mode as default.
                      // (Force everything expect black as white)
	//
	extparam[13]=1;

	//
	if(argc<2)
	{
		//
		printf("Usage: %s [source file] [options]\n",
			argv[0]);
		return 0;
	}

	//
	if(argc>3)
	{
		for(ad=3; ad<argc; ad++)
		{
			if(argv[ad][0]=='#')
			{
				if(!strcmp(argv[ad],"#2cols"))
				{
					extparam[7]=1;
				}

				if(argv[ad][1]=='z')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[6]);
				}

				// TX / TY - CHANGING CENTER POINT
				if(argv[ad][1]=='t' && argv[ad][2]=='x')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					argv[ad][2]=' ';
					sscanf(argv[ad],"%u",&CXPON);
				}

				//
				if(argv[ad][1]=='t' && argv[ad][2]=='y')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					argv[ad][2]=' ';
					sscanf(argv[ad],"%u",&CYPON);
				}

				// X / Y - RESIZING ARGUMENTS
				if(argv[ad][1]=='x')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&XLEN);
				}
				if(argv[ad][1]=='y')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&YLEN);
				}

				//
				if(argv[ad][1]=='g')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%x",&BGBYTE);
				}
				if(argv[ad][1]=='b')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&BEGIVAL);
				}
				if(argv[ad][1]=='d')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&DITHER);
				}

				// KILL NOISE
				if(argv[ad][1]=='k')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[9]);
				}
				if(argv[ad][1]=='m')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&MAXI);
				}
				if(argv[ad][1]=='f')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&FORCESIZE);
				}
				// FRAME STEP
				if(argv[ad][1]=='.')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[13]);
				}
				if(argv[ad][1]=='n')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&NST);
				}
				if(argv[ad][1]=='p') // BACKGROUND PICTURE SELECT (PCX ONLY)
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%s",&fname);
					decode_pcxto(fname,extpointer[0],0);
					extparam[4]=1;
				}
				if(argv[ad][1]=='s') // BLURRING VALUE (AKA SMOOTHING VALUE)
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[5]);
				}
				if(argv[ad][1]=='l')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[8]);
				}
				// Delay
				if(argv[ad][1]=='Ï')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[10]);
				}
				// Interpolation
				if(argv[ad][1]=='&')
				{
					argv[ad][0]=' ';
					argv[ad][1]=' ';
					sscanf(argv[ad],"%u",&extparam[11]);
				}
			}
		}
	}

	//
#ifdef USERINT
	jtm_setvmode(3);
	drawansiwindow(4,1,80-4,4,    0,"",1);
	window(5,2,80-5,3);
	sprintf(str,"MKAN ** Creating animation file from PCX/BMP file(s) ** Options: MAXI = %u, DITHER = %u.",MAXI,DITHER);
	printf(str);
	//
	drawansiwindow(4,6,80-4,25-2, 1,"",1);
	window(5,5+2,80-5,(25-5)+2);
#endif

	//
	switch(NST)
	{
		//
		case 0:
		sprintf(str2,"%s%u.pcx",argv[1],BEGIVAL);
		break;
		//
		case 1:
		sprintf(str2,"%s%1.4u.pcx",argv[1],BEGIVAL);
		break;
		//
		case 2:
		sprintf(str2,"%s%1.3u.pcx",argv[1],BEGIVAL);
		break;
		//
		case 3:
		sprintf(str2,"%s%1.2u.pcx",argv[1],BEGIVAL);
		break;
		//
		case 4:
		sprintf(str2,"%s%1.1u.pcx",argv[1],BEGIVAL);
		break;
	}
	printf(str2); printf(" [check]\n");
	if(fexist(str2))
	{
		processimages(BGBYTE,NST,FORCESIZE,MAXI,DITHER,1,argv[1],argv[2],BEGIVAL,XLEN,YLEN,CXPON,CYPON);
	}
	else
	{
		switch(NST) // NST check
		{
			//
			case 0: // 0=DEFAULT
			sprintf(str2,"%s%u.bmp",argv[1],BEGIVAL);
			break;
			//
			case 1:
			sprintf(str2,"%s%1.4u.bmp",argv[1],BEGIVAL);
			break;
			//
			case 2:
			sprintf(str2,"%s%1.3u.bmp",argv[1],BEGIVAL);
			break;
			//
			case 3:
			sprintf(str2,"%s%1.2u.bmp",argv[1],BEGIVAL);
			break;
			//
			case 4:
			sprintf(str2,"%s%1.1u.bmp",argv[1],BEGIVAL);
			break;
		}
		printf(str2); printf(" [check]\n");
		if(fexist(str2))
		{
			processimages(BGBYTE,NST,FORCESIZE,MAXI,DITHER,2,argv[1],argv[2],BEGIVAL,XLEN,YLEN,CXPON,CYPON);
		}
		else
		{
			printf("Couldn't find any matching file.\n");
		}
	}

	//
#ifdef USERINT
	window(1,1,80,25);
	drawansiwindow(4,8,80-4,25-8,    0,"",1);
	window(5,9,80-5,25-9);
#endif
	removefiletypefromfilename(argv[2]);
	printf("** Result **\n");
	sprintf(str,"File: %s, size=%u bytes, %u kb, %u blocks.\n",argv[2],fsizeof(argv[2]),fsizeof(argv[2])/1024,fsizeof(argv[2])/256);
	printf(str);
//	waitkey();

#ifdef USERINT
	jtm_setvmode(3);
#endif
	free(extpointer[0]);
	return 0;
}


