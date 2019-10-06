// =============================================================
// nzip(Peanut Zip) - general file packer/unpacker 1.0.
// Also works as unz.bin (see unz.mak).
// (C) 2003-2005 by Jari Tuominen.
// =============================================================
#include <stdio.h>
#include "nzip.h"

#ifdef JTMOS
// Request 4096K of RAM from the init process.
// I tested this program with 2048K ram setting,
// but it seemed to not to work properly.
// So I decided to use 4M setting.
SYSMEMREQ(APPRAM_4096K)
#endif

//
CO co;
int ta=0;

/////////////////////////////////////////////////////////////////////////////////////////
// Program failure function
void pbug(const char *expl)
{
	printf("\nProgram failure: '%s'\n", expl);
	exit(1);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Used by compress1 -function.
// The actual compression algorithm.
// Attempt to find similarity block and produce compressed code
// non-zero reply = was able to compress
// zero reply = was not able to compress at this offset
inline int findSimBlock(CO *c, int offs, SIM *s, FILE *f)
{
	static int i,i2,i3,i4,ii,fo,staoffs,o;

	//
	s->method=0;

	// -------------------------------------------------
	// 1) Char packer
	//
	// [CODE] [WORD fill_length] [BYTE fill_byte]
	//
#ifdef ENABLE_CHRPAK
	for(i=offs,fo=0,i2=0; i<(c->l_buf-1); i++,i2++)
	{
		if(c->buf[i]==c->buf[i+1])
		{
			fo++;
			if(fo>=0xFFFF)break;
		}
		else	break;
	}

	// Found enough?
	if(fo>=8)
	{
		// Produce char packer code:
		fputc(CODE_CHARPACK & 255, f);
		fputc(CODE_CHARPACK>>8 & 255, f);
		fputc(fo&255, f);
		fputc(fo>>8, f);
		fputc(c->buf[offs], f);
		s->offs = i2;
		// Char packer
		s->method=1;

		// Some compression was made:
		return 1;
	}
#endif

	//
	ta=ta+1&COMP_STEP;
	if(ta==0)
	{
#ifdef ENABLE_ADVPAK
		// -------------------------------------------------
		// 2) Advanced compression
		i = offs-MAX_LOOK; if(i<0)i=0;
		for(staoffs=0,fo=0,i2=0; i<(offs-REACHING) && i2<MAX_LOOK; i++,i2++) // 4!
		{
			if(c->buf[i]==c->buf[fo+offs])
			{
				if(!fo)
				{
					staoffs=i; // !!
				}
				fo++;
				if(fo>=0xFFFF)break;
			}
			else
			{
				if(fo>=10 && staoffs)
					break;
				else
				{
					fo=0;
					staoffs=0;
				}
			}
		}
	
		// Found enough?
		if(fo>=10)
		{
			// Logic check
			if((staoffs+fo)>=offs)
			{
				printf("Logic Error\n");
				// Wrong logic
				goto past;
			}

			//
		//	printf("ADVPAK: add code: COPY %d bytes from offset 0x%x\n",
		//		fo, staoffs);

			// Produce char packer code:
			fputc(CODE_ADVPAK & 255, f); // code
			fputc(CODE_ADVPAK>>8 & 255, f);
			fputc(fo&255, f); // length
			fputc(fo>>8, f);
			fputc(staoffs&255, f); // read offset
			fputc(staoffs>>8&255, f);
			fputc(staoffs>>16&255, f);
			fputc(staoffs>>24&255, f);
			s->offs = fo;
			// Advanced packer
			s->method=2;
			return 1;
		}
		// ADV.COMPR.^^
#endif
	}
past:

	// -------------------------------------------------
	// 3) Method III
	// ...


	// -------------------------------------------------
	// No compression(suggest uncompressed byte):
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
void compress1(const char *fname,
	CO *c)
{
	static int i,i2,i3,i4,cyc,compl;
	static SIM sim;
	static FILE *f;

	//
	f=fopen(fname, "wb");
	if(f==NULL) { printf("Write error.\n"); return; }

	//
	for(i=0,cyc=0; i<c->l_buf; cyc++)
	{
		// Progress indicator
		if( !((cyc++)&IND_STEP) )
		{
			fprintf(stderr, "Compressing %d/%d       \r",
				i/1024, c->l_buf/1024);
		}

		// Can we look back?
		if( i>=MIN_LOOKBACK &&
			i<(c->l_buf-MIN_LOOKBACK)
			&& findSimBlock(c, i, &sim, f) )
		{
			i+=sim.offs;
		}
		else
		{
			// Modify for output
			if(c->buf[i]>=0xE3 &&	   // 0xNNE3
				c->buf[i]<=0xFE &&
				c->buf[i+1]==0xFC) // 0xFCNN
			{
				// Fix encountered code
				fputc(CODE_FIX & 255, f);
				fputc(CODE_FIX>>8 & 255, f);
				fputc(c->buf[i], f);
				fputc(c->buf[i+1], f);
				i+=2;
			}
			else
			{
				// Output uncompressed byte:
				fputc(c->buf[i], f);
				i++;
			}
		}
	}

	// Mark end
	fputc(CODE_END & 255, f);
	fputc(CODE_END>>8 & 255, f);

	// Close file
	printf("Old file size =           %dK (%d bytes)\n", c->l_buf/1024, c->l_buf);
	printf("Compressed file size =    %dK (%d bytes)\n", ftell(f)/1024, ftell(f));
	fclose(f);
}

/////////////////////////////////////////////////////////////////////////////////////////
void compress(const char *fname)
{
	FILE *f;
	int i,i2;
	char str[256];

	// ================================================
	// Load uncompressed data
	f=fopen(fname, "rb");
	if(f==NULL){ printf("Read error(%s).\n", fname); return; }

	// Determine file length
#ifdef JTMOS
	i = fsizeof(fname);
#else
	fseek(f, 0, SEEK_END); i=ftell(f);
	fseek(f, 0, SEEK_SET);
#endif

	// Allocate buffer
	co.buf = malloc(i);
	if(co.buf==NULL)
	{
		printf("(2) malloc returns NULL: out of memory ?\n");
		printf("buf = 0x%x (requested %d bytes)\n", co.buf, i);
		abort();
	}
	co.l_buf = i;

	// Read to buffer
	fread(co.buf, i,1, f);
	fclose(f);

	// -----------------------------------------------
	// Build output file name
	sprintf(str, "%s.nz", fname); // nutzip

	// Produce compressed file
	compress1(str, &co);
#ifdef REMOVE_OLD
	// Remove old
	remove(fname);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
// Uncompression procedure.
//
void uncompress1(const char *fname,
		BYTE *buf, int l_buf)
{
	static int i,i2,i3,i4,length,byte,ii,offs,known,beg;
	static char *p;
	static WORD *wp;
	static DWORD *dp;
	static FILE *f;
	static BYTE *tmp;
	static int l_tmp,o_tmp;

	// Allocate temporary read-back buffer
	l_tmp = MAX_UNSIZE; // 512K
	tmp = malloc(l_tmp); // allocate memory for buffer
	if(tmp==NULL)
	{
		printf("malloc returns NULL: out of memory ?\n");
		printf("tmp = 0x%x (requested %d bytes)\n", tmp, l_tmp);
		abort();
	}
	memset(tmp, 0, l_tmp);
	o_tmp=0; // offset to zero

	// Produce output file
	f=fopen(fname, "wb");
	if(f==NULL)	return;

	// Loop for uncompression
	for(i=0; i<l_buf; )
	{
		// Make char pointer
		p = buf+i;
		// Make word pointer
		wp = p;

		// Only one byte left, therefore can't be a code
		if(i>=(l_buf-1))
		{
			// Only one byte left
			goto uncompressIt;
		}

		// Check for codes
		if(*wp==CODE_END) // End of compressed stream
			// End
			break;
		else
		if(*wp==CODE_FIX) // Fix word
		{
			// Get parameters
			i+=2; wp = buf+i;
			i4 = *wp;
			i+=2;
			tmp[o_tmp+0] = i4&255;
			tmp[o_tmp+1] = i4>>8&255;
			fputc(i4&255, f);
			fputc(i4>>8&255, f);
			o_tmp+=2;
		}
		else
		if(*wp==CODE_CHARPACK) // Char pack
		{
			// Get parameters
			i+=2; wp = buf+i;
			length = *wp;
			i+=2; p = buf+i;
			byte = *p;
			i++;

			// Produce fill
			for(ii=0; ii<length; ii++,o_tmp++)
			{
				if(o_tmp>=MAX_UNSIZE) pbug("File is too big!");
				fputc(byte, f);
				tmp[o_tmp] = byte;
			}
		}
		else
		if(*wp==CODE_ADVPAK) // Adv. compr.
		{
			// Get parameters
			i+=2; wp = buf+i;
			length = *wp;
			i+=2; dp = buf+i;
			offs = *dp;
			i+=4;

			// Produce a copy of already known memory
			known = o_tmp;
			beg = o_tmp;
			//
		//	printf("Adv.compr.: length=%d, dst offs=%x, src offs=%x\n",
		//		length, beg, offs);
			for(ii=0; ii<length; ii++,o_tmp++,offs++)
			{
				if(offs>=known)
				{
					printf("beg=%x, known=%x, offs=%x, o_tmp=%x, length=%d\n",
						beg, known, offs, o_tmp, length);
					pbug("offset stepped past known area");
				}
				if(o_tmp>=MAX_UNSIZE) pbug("File is too big!");
				fputc(tmp[offs], f);
			//	fputc(0xFF, f);
				tmp[o_tmp] = tmp[offs];
			}
		}
		else
		{
uncompressIt:
			// Uncompressed byte, simply copy it
			if(o_tmp>=MAX_UNSIZE) pbug("File is too big!");
			fputc(*p, f);
			tmp[o_tmp]=*p;
			i++; o_tmp++;
		}
	}

	// Close output file
/*	printf("Compressed file size =    %d bytes\n", l_buf);
	printf("Uncompressed file size =  %d bytes\n", ftell(f));*/
	fclose(f);

	// Deallocate buffer
	free(tmp);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
void unNutFname(char *otp, const char *src)
{
	int i,i2,l;

	//
	l = strlen(src);
	strcpy(otp, src);
	otp[l-3] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
void uncompress(const char *fname)
{
	FILE *f;
	BYTE *buf;
	int l_buf;
	static char str[256];

	// Make sure that it is a .nz file
	if(strstr(fname, ".nz")==NULL)
	{
		printf("Unknown file: '%s', has no '.nz' ending.\n", fname);
		return;
	}

	// Produce file name for uncompressed file.
	unNutFname(str, fname);

	//
	printf("Uncompressing %s -> %s ...\n", fname, str);

	// Read compressed file
	f=fopen(fname,"rb");
	if(f==NULL)
	{
		printf("File not found ? (%s)\n", fname);
		return;
	}
#ifdef JTMOS
	l_buf = fsizeof(fname);
#else
	fseek(f,0,SEEK_END);
	l_buf = ftell(f);
	fseek(f,0,SEEK_SET);
#endif
	buf = malloc(l_buf);
	if(buf==NULL)
	{
		printf("malloc returns NULL: out of memory ?\n");
		printf("buf = 0x%x (requested %d bytes)\n", buf, l_buf);
		abort();
	}
	fread(buf,l_buf,1,f);
	fclose(f);

	// Produce uncompressed file
	uncompress1(str, buf, l_buf);
#ifdef REMOVE_OLD
	// Remove old
	remove(fname);
#endif

	// Deallocate buffer
	free(buf);
}





