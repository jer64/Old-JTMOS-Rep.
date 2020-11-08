// mkanProcess.c
#include <stdio.h>
#include "mkan.h"
#include "mkanProcess.h"

// This function removes MSDOS stylish filetype from end of
// filename, it actually also does remove all the other marks after
// first '.' mark and sets end of string mark(chr:0) at the place.
// filename.typ -> filename
void removefiletypefromfilename(char *fname)
{
	int i;

	//
	for(i=0; i<strlen(fname); i++)
	{
		//
		if(fname[i]=='.')
		{
			//
			fname[i]=0;
			break;
		}
	}
}

// Measure noise level
int getRegionNoiseLevel(BYTE *src,	
	int x,int y, int w,int h, int reg)
{
	int xx,yy,nl,c1,c2,ct,ad,ad2;

	//
	for(yy=y; yy<(y+reg); yy++)
	{
		//
		ad = w*yy;

		//
		for(xx=x; xx<(x+reg-1); xx++)
		{
			//
			c1 = src[ad+x+0];
			c2 = src[ad+x+1];
			ct=c1-c2; if(ct<0)ct=-ct;
			nl+=ct;
		}
	}

	//
	return nl;
}

//
int killnoise(BYTE *src,BYTE *dst,
		int w,int h,int reg)
{
	int x,y,ad,ad2,i,i2,xx,yy;

	//
	for(y=0; y<h; y+=reg)
	{
		//
		for(x=0; x<w; x+=reg)
		{
			//
			if( getRegionNoiseLevel(src, x,y, w,h, reg)>reg*10 )
			{
				// Do filtering here: (TODO)
			}
		}
	}
	return 0;
}

//
#ifndef JTMOS
// Check whether if a file exists
int fexist(const char *fname)
{
	FILE *f;

	//
	f=fopen(fname, "rb");
	if(f==NULL)
		return 0;
	fclose(f);
	return 1;
}

// Get file size, -1 if none found
int fsizeof(const char *fname)
{
	FILE *f;
	int l;

	//
	f=fopen(fname, "rb");
	if(f==NULL)
		return -1;
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	fclose(f);
	return l;
}
#endif

//
//TYPE 1:PCX 2:BMP
void processimages(int BGBYTE,int NST,int FORCESIZE,int MAXI,int DITHER,int type,char *fname,char *ofname,int begival,
	unsigned int XLEN,unsigned int YLEN,unsigned int CXPON,unsigned int CYPON)
{
	int CHARPACKING=1; // Recudes some unneccesary memory eaters.
	int frame,dela;
	char str[255],str2[255],str3[255],str4[255];
	char m1,m2,m3,m4;
	unsigned char *b,*b2,*b3,*b4;
	int ad,ad2,ad3,ad4,x,y,attempt=0,color,finalcolor,diffe;
	FILE *f,*f2,*f3,*f4;
	char dither[]={
                0x00,0x00, //
                0x00,0x00,

                0x01,0x00, //
                0x00,0x00,

                0x01,0x00, //
                0x00,0x01,

                0x01,0x00, //
                0x01,0x01               // finalcolor=dither[((color&3)<<2)+(x&1)+((y&1)<<1)]+(color>>2);
	};

	//
	extparam[0]=CXPON-(XLEN>>1);
	extparam[1]=CYPON-(YLEN>>1);
	extparam[2]=CXPON+(XLEN>>1);
	extparam[3]=CYPON+(YLEN>>1);
	extparam[8]=99999999;

	//
	b=malloc(1024*1024);  // VŽLIKUVA
	b2=malloc(1024*256); // ALUSTA(CSBMP)
	b3=malloc(1024*256); // VŽLIKUVA(CSBMP)
	b4=malloc(1024*256); //

	//
	for(ad=0; ad<(1024*64); ad++){b2[ad]=BGBYTE; b[ad]=BGBYTE; b3[ad]=BGBYTE; b4[ad]=BGBYTE;}

	//
	dela=0;

	//
	f2=fopen(ofname,"wb");
	fputc(0x00,f2);
	fputc(0x40,f2);
	fputc(BGBYTE,f2);
	for(frame=begival; frame<1000000; )
	{
		//
		extparam[8]--;
		if(extparam[8]==-1)
		{
			//
			printf("End of frames(1).\n");
			break;
		}

		//
		if(type==1)
		{
			switch(NST)
			{
				//
				case 0:
				sprintf(str,"%s%u.pcx",fname,frame);
				break;
				//
				case 1:
				sprintf(str,"%s%1.4u.pcx",fname,frame);
				break;
				//
				case 2:
				sprintf(str,"%s%1.3u.pcx",fname,frame);
				break;
				//
				case 3:
				sprintf(str,"%s%1.2u.pcx",fname,frame);
				break;
				//
				case 4:
				sprintf(str,"%s%1.1u.pcx",fname,frame);
				break;
				//
				default:
				sprintf(str,"%s%u.pcx",fname,frame);
				break;
			}

			//
			if(!fexist(str))
			{
				printf("End of frames(2, '%s' not found).\n",
					str);
				break;
			}
			printf(str); printf(" [add] (%dK)\n", ftell(f2)/1024);
			mkan_decode_pcxto(str,b,0);
		}

		//
		if(type==2)
		{	
			switch(NST)
			{
				//
				case 0:
				sprintf(str,"%s%u.bmp",fname,frame);
				break;
				//
				case 1:
				sprintf(str,"%s%1.4u.bmp",fname,frame);
				break;
				//
				case 2:
				sprintf(str,"%s%1.3u.bmp",fname,frame);
				break;
				//
				case 3:
				sprintf(str,"%s%1.2u.bmp",fname,frame);
				break;
				//
				case 4:
				sprintf(str,"%s%1.1u.bmp",fname,frame);
				break;
			}

			//
			if(!fexist(str))break;
			printf(str); printf(" [add]\n");
			attempt=0;
again:
			attempt++;
			ad=mkan_jtm_loadbmp(str,b);
			if(ad==1)
			{
				//
				printf("Unknown image format.\n");
				return 1;
			}

			//
			if(ad==2)
			{
				//
				if(attempt>5)return 2;
				printf("File not available.\n");
				switch(NST)
				{
					//
					case 0:
					sprintf(str,"%s%u.bmp",fname,frame);
					break;
					//
					case 1:
					sprintf(str,"%s%1.4u.bmp",fname,frame);
					break;
					//
					case 2:
					sprintf(str,"%s%1.3u.bmp",fname,frame);
					break;
					//
					case 3:
					sprintf(str,"%s%1.2u.bmp",fname,frame);
					break;
					//
					case 4:
					sprintf(str,"%s%1.1u.bmp",fname,frame);
					break;
				}

				//
				if(fexist(str))
				{
					printf("FEXIST founds $STR.\n");
				}
				else
				{
					//
					printf("Self test passed: File is really not available.\n");
					printf("Forcing disk write test : ");
					f3=fopen("testfile.tmp","wb");
					if(f3!=NULL)
					{
						fprintf(f3,"yeyeye");
						fclose(f3);
					}
					else
					{
						printf("Couldn't create file!! ");
					}

					//
					if(fexist(f3))
					{
						printf("Successfully created a testfile\n");
					}
					else
					{
						printf("Creation of a new file didn't success, strange!\n");
					}
					remove("testfile.tmp");
				}
				return 2;
			}
		}

		// Force 2 color mode
		if(extparam[7])
		{
			// TODO !!
			//jtm_force2color(b,320,200);
		}

		// Zooming
		if(extparam[6])
		{
			if(!(extparam[0]+4>=extparam[2] ||
			      extparam[1]+4>=extparam[3]))
			{
				extparam[0]+=extparam[6];
				extparam[1]+=extparam[6];
				extparam[2]-=extparam[6];
				extparam[3]-=extparam[6];
			}
		}

		// Maximize image colors (Stretch colors, normalize ..)
		if(MAXI)
		{
			for(ad=0; ad<(320*200); ad++)vb[ad]=b[ad];
			//jtmscr_maximize(); // TODO??
			for(ad=0; ad<(320*200); ad++)b[ad]=vb[ad];
		}

		//
		if(extparam[9])
		{
			killnoise(b,vb,320,200,32);
			for(ad=0; ad<(320*200); ad++)b[ad]=vb[ad];
		}

		//
		for(ad=0; ad<(1024*64); ad++){b3[ad]=0; b4[ad]=0;}

		// Convert
		for(y=0; y<200; y+=2)
		{
			ad=y*320;
			for(x=0; x<320; x+=2)
			{
				if(extparam[11])
				{
					color=((b[ad+x]+b[ad+x+1]+b[ad+x+320]+b[ad+x+321])>>2)>>4;
				}
				else
				{
					color=b[ad+x]>>4;
				}
				if(DITHER)
				{
					//finalcolor=dither[((color&3)<<2)+((x>>1)&1)+(((y>>1)&1)<<1)]+(color>>2);

					finalcolor=dither[((color&3)<<2)+((x>>1)&1)]+(color>>2);
					if(finalcolor>3)finalcolor=3;
				}
				else
				{
					finalcolor=color/4;
				}
				multiplot(b4,x,y,finalcolor);
			}
		}

		//
		for(ad=0,ad2=0; ad<(320*200/8); ad+=2,ad2++)b3[ad2]=b4[ad];
		// Fix bitmap
		for(ad=0; ad<((320*200)/8); ad++)
		{
			if(b3[ad]==0x53)b3[ad]=0x52; // Beginning of frame sign
			if(b3[ad]==0x54)b3[ad]=0x50; // Write position warp sign
			if(b3[ad]==0x56)b3[ad]=0x59; // Charpacking sign
		}

		//
		fputc(0x53,f2);
		// ********* COMPRESS *********
		//
		ad=0;
		do
		{
			for(ad2=ad,ad3=0; ad2<ad+250; ad2++, ad3++)
			{
				if(ad2>=((((320*200)/8)/2)-1))
				{
					break;
				}
				if( b3[ad2]!=b2[ad2] ) { ad3--; break; }
			}
			if(ad3>=4)
			{
				// Write warp instruction
				fputc(0x54,f2);
				fputc(ad3,f2);
				ad+=ad3;
			}
			else
			{
				if(CHARPACKING)
				{
					ad2=ad; ad3=0; diffe=0;
					do
					{
						if(ad2>=((((320*200)/8)/2)-1))
						{
							ad3--;
							break;
						}
						if(ad3>0xfe)break;
						if(b3[ad2]!=b3[ad2+1])diffe++;
						if(!FORCESIZE)
						{
							if(diffe)break;
						}
						else
						{
							if(diffe>=FORCESIZE)break;
						}
						ad2++; ad3++;
					}while(0==0);

					//
					if(ad3<5)
					{
						fputc(b3[ad],f2);
						ad++;
					}
					else
					{
						fputc(0x56,f2);
						fputc(ad3,f2);
						fputc(b3[ad2],f2);
						ad+=ad3;
					}
				}
				else
				{ // Uncompressed data flush, memory runs like toilet water! :---D
					fputc(b3[ad],f2);
					ad++;
				}
			}

			//
			if(ad>=(((320*200)/8)/2))break;

			//
		//	m1=getch(); if(m1==27)break;
		}while(0==0);

		//
		// lonjstcopy
		for(ad=0; ad<(((320*200)/8)/2); ad++)
		{
			b2[ad]=b3[ad];
		}

		//
		if(extparam[10])
		{
			dela++;
			if(dela==extparam[10]){dela=0; frame=frame+extparam[13]; }else{}
		}
		else
		{
			frame=frame+extparam[13];
		}
	}

	//
	fputc(0x53,f2);
	fputc(0x53,f2);
	fclose(f2);
}


