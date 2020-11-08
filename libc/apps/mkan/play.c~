// play.c
#include <stdio.h>

//
int AUTOPLAY=0,blinx;

//
void Trigger(void)
{
	if(!AUTOPLAY)blinx++;
}

//
int main(int argc,char *argv[])
{
	//
	FILE *f,*f2;
	long ad; long ad2,ad3,ad4,len,a1,a2,a3,a4,flen;
	long x,y; long length;
	unsigned char *b,*b2,*a;
	char m1,m2,m3,m4;
	char str[255],str2[255];
	int NEWFRAME;
	long TFRAMES,FRAME,METER=1;
	float fa,fb,fc,fd;
	unsigned char *vbuf;

	//
	if(argc<2)
	{
		//
		printf("Usage: play [file]\n");
		return 0;
	}
	
	//
	vbuf = malloc(320*256);

	//
	b=malloc(1024*64); // For the sfbmp output
	b2=malloc(1024*64); // ...

	//
	NEWFRAME=1;
	if( (f=fopen(argv[1], "rb"))==NULL )
	{
		printf("Couldn't find any matching file.\n");
	}
	else
	{
		// Load movie.
		fseek(f, 0, SEEK_END);
		flen = ftell(f);
		len=flen;
		fseek(f, 0, SEEK_SET);
		a=malloc(flen); // For the animation
		fread(a,flen,1,f);
		fclose(f);

		// Determine amount of frames.
		ad2=flen;
		for(ad=0,TFRAMES=0; ad<ad2; ad++)
		{
			if(a[ad]==0x53)TFRAMES++;
		}
		TFRAMES-=2;
		if(TFRAMES<0)
			TFRAMES=0;
		if(TFRAMES==0)
		{
			printf("No frames detected for this video file.\n");
			abort();
		}

		//-----------------------------------------------------------------
		// Setup video mode.
		setvmode(0x13);
		greyscale256();

repeat:
		//
		FRAME=0;

		//
		for(ad=0; ad<(1024*64); ad+=2)
		{
			b[ad+0]=a[2];
			b[ad+1]=0x00;
		}

		//
		ad=3;
		
		//
		while(1)
		{
			// Get atleast one frame
			//
			if(NEWFRAME)
			{
				NEWFRAME=0;
				if(ad>=len)break;
				if(a[ad]!=0x53)break; // This actually means error !
				ad++;
				ad2=0;
				while(1)
				{
					if(ad>=len)break;
					if(a[ad]==0x53)break;
					if(a[ad]==0x56) // "Monistus"
					{
						for(ad4=ad2; ad4<ad2+a[ad+1]*2; ad4+=2)b[ad4]=a[ad+2];
						ad2+=a[ad+1]*2;
						ad+=3;
					}
					else
					if(a[ad]==0x54)
					{
						// Warp --->
						ad2+=a[ad+1]*2;
						ad+=2;
					}
					else
					{
						// Absolute store
						b[ad2]=a[ad];
						ad++; ad2+=2;
					}
				}

				//
				FRAME++;
			}



			//
			// SCBMP -> VGA
			for(y=0; y<200; y++)
			{
				ad2=y*320;
				for(x=0; x<320; x+=2)
				{
//					vbuf[ad2+x+0] = multiread(b,x,y)*(256/4);
//					vbuf[ad2+x+1] = multiread(b,x,y)*(256/4);
				}
			}

			//
			if(METER)
			{
			}
			
			//
			vf(vbuf);

			//
			m1=jgetch();
			if(AUTOPLAY)
			{
				NEWFRAME=1;
			}
			else
			{
				if(m1==32)NEWFRAME=1;
			}
			if(m1=='m')
			{
				METER=1-METER;
			}
			if(m1=='a')
			{
				blinx=blinx%32;
				AUTOPLAY=1-AUTOPLAY;
			}
			if(m1==27)break;
			if(ad>=len)break;
		}

        	if(m1==27)goto loppu;
        	if(AUTOPLAY)goto repeat;
        	m1=jgetch();
        	if(m1==32)goto repeat;
        }
loppu:
	return 0;
}
