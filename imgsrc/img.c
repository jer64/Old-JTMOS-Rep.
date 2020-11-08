//================================================================
// img.c - Image Construction Program
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
//================================================================
#include <stdio.h>

// Create image
void createimage(int argc, const char **argv)
{
	FILE *f,*f2;
	static char str[256],str2[256],
		fname[256];
	int track,sector,i,i2,i3,i4,siz;
	unsigned char *image;

	//
	image=NULL;

	//     Double Density/Double Sided 3.5" 1.44M Diskette)
	//
	// == 160*18*512 (160 tracks,
	// 18 sectors per a track, 512 bytes per a sector)
	//
	// Tracks 0,2,4,6 .. will be written on side 0,
	// Tracks 1,3,5,7 .. will be written on side 1.
	//
	// - each side has 80 tracks

	// Open configuration file
	f=fopen(argv[1],"rb");
	if(!f)
	{
		printf("Cannot find configuration file(%s).\n",argv[1]);
		exit(1);
	}

	// - - - - - - - - - - - - - - - - -
	// Read Configuration
	//
	while(!0)
	{
		// Get filename
		fscanf(f,"%s",&fname);
		if(!strcmp(fname,"}"))
		{
			//
			break;
		}
		if(!strcmp(fname,"{"))
		{
			//
			continue;
		}

		//
		if(!strcmp(fname,"size"))
		{
			//
			fscanf(f,"%d",&siz);

			//
			printf("Diskimage Size = %dK\n",siz);

			//
			image=malloc(1024*siz*2);

			// Erase Image Contents
			for(i=0; i<1024*siz; i++) image[i] = 0;
			continue;
		}

		// - - - - - - Only if image is allocated :
		if(image)
		{
			// Get TRACK/SECTOR
			fscanf(f,"%d %d",&track,&sector);

			//
			printf("%s -> %d,%d (Track/Sector)\n",fname,track,sector);

			//
			if(track==-1)
			{
				printf("Using K offset = %dK\n", sector);
				i2 = sector*1024;
				track=0; sector=1;
				goto skipconv;
			}

			// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			// Load file into inside the image.
			// calc. offs.
			i2=(track*18*512)+((sector-1)*512);
skipconv:
			//
			printf("     (Offs = 0x%1.8x -- %u)\n",i2,i2);

			//
			f2=fopen(fname,"rb");
			if(!f2)
			{
				printf("Error: cannot find Partial Image File '%s'.\n",fname);
				fclose(f);
				exit(2);
			}

			//
			for(i=i2; i<1024*siz; i++)
			{
				//
				if(feof(f2))break;

				//
				image[i] = fgetc(f2);
			}

			//
			fclose(f2);

			//
			printf("     End at Track #%d - OK!\n", i/(512*18) );
			continue;
		}
		else
		{
			printf("Error: you didn't specify image size in configuration!( f.e. 'size 1440' ).\n");
			fclose(f);
			exit(3);
		}
	}

	//
	fclose(f);



	// - - - - - - - - - - - - - - -
	// Write Image File !
	//
	printf("Writing Image File at %s ...\n",argv[2]);

	//
	f=fopen(argv[2],"wb");
	if(f)
	{
		for(i=0; i<(siz*1024); i++)
		{
			//
			fputc(image[i],f);
		} 
		fclose(f);
	}
	else
	{
		printf("Cannot write image!!!!!!: %s\n",argv[2]);
	}

	//
	printf("Done.\n");
}

//
int main(int argc,char **argv)
{
	//
	if(argc<3)
	{
		//
		printf("----+--------------------------------------------------------------------+\n");
		printf("img | image creator utility -- by Jari Tapio Tuominen                    |\n");
		printf("    | Usage: img [configuration file.cfg] [diskimage.img]                |\n");
		printf("    | Distributed under GPL, see COPYING for more information.           |\n");
		printf("----+--------------------------------------------------------------------+\n");
		return 0;
	}

	//
	createimage(argc,argv);

	//
	return 0;
}




