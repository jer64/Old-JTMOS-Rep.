// ============================================================
// 		      Squirrel Archiver v1.0
// 		  Special archiver - JTMOS only.
// (C) 2002-2003 by Jari Tuominen(jari@vunet.org)
// ============================================================
#include <stdio.h>
#include <stdlib.h>
#include "sqa.h"

//
FE ent[400];
char jtmosdir[256];

// Calculate 32-bit checksum
unsigned long int chksum(BYTE *buf, int len)
{
        int i,val;

        //
        for(i=0,val=0; i<len; i++)
                val+=buf[i]+1; // count zeroes too

        //
        return val;
}

// This function removes the path from the file name.
// old: /home/yep/blablalbla/file.txt
// new: file.txt
void CutName(char *dst, const char *src)
{
	int i,i2;

	//
	i=strlen(src)-1;
	if(i<0)i=0;

	// search pass
	for(; i!=0; i--)
	{
		if(src[i]=='/'){i++; break;}
	}

	// copy pass
	for(i2=0; i<strlen(src); i++,i2++)
	{
		dst[i2]=src[i];
	}
	dst[i2]=0;
}

//
int archive1(void)
{
	static char str[255],str2[255],tmp[256];
	FILE *f,*f2,*f3,*src;
	int files,i,i2,i3,i4,a1,a2,x,x2,l,z;
	BYTE *t;
	FE *e;
	char *p;

	// COLLECT FILES TO A SINGLE BINARY FILE
	files=0;
	f=fopen("/tmp/sqarc.tmp", "rb");
	f2=fopen("/tmp/sqarcbin.tmp", "wb");
	if(f==NULL)	return 1;
	if(f2==NULL)	return 2;

	// skip first entry, is always dir
	fgets(str, 250, f);

	// Go through all entries
	while(1)
	{
		//
		if(feof(f))
			break;
		strcpy(str,"");
		fscanf(f, "%s", &str);

		//
		ent[files].offset = ftell(f2);

		//
		src=fopen(str, "rb");
		a1=ftell(f2);
		if(src==NULL)continue;

		//
		fprintf(stderr, "Adding %s\n", str);

		// Get source length
		fseek(src, 0, SEEK_END);
		l = ftell(src);
		fseek(src, 0, SEEK_SET);

		// Allocate temporary buffer
		t = malloc(l);

		// Read to buffer
		fread(t, l,1, src);

		// Write to output stream
		for(z=0; z<l; z++) { fputc(t[z],f2); }

		// Close source stream
		fclose(src);

		// Add file name & length
		a2=ftell(f2);
		ent[files].length = a2-a1;
		strcpy(ent[files].fname, str);
		// Add checksum
		ent[files].chksum = chksum(t,l);
		// Increas amount of files
		files++;

		// Free temporary buffer
		free(t);
	}
	fclose(f);
	fclose(f2);

	//
	strcpy(str2,"");
	// WRITE HEADER
	f2=fopen("/tmp/sqarchdr.tmp", "wb");
	if(!f2)return 3;

	// Write bootsector + safearea(total 2K)
	sprintf(tmp, "%s/boot32/nonsysdisk.biz", jtmosdir);
	f3=fopen(tmp, "rb");
	fprintf(stderr,"- Adding nonsysdisk.bin\n");
	if(!f3)
	{
		fprintf(stderr, 
			"%s: Warning: Boot sector file not found.\n",
			tmp);
	//	fclose(f2);
	//	exit(10);
	}
	for(x=0; x<(1024*2); x++)
	{
		if(f3==NULL || feof(f3)) { fputc(0, f2); }
		else fputc(fgetc(f3), f2);
	}
	if(f3!=NULL)
		fclose(f3);

	// IDENTIFIER STRING
	fputc('S', f2);
	fputc('Q', f2);
	fputc('A', f2);
	fputc('R', f2);
	fputc('C', f2);
	fputc('5', f2);
	fputc('2', f2);
	fputc('0', f2);
	// NUMBERS OF FILES
	fputc(files, f2); fputc((files>>8), f2);
	for(i=0; i<files; i++)
	{
		// Get entry PTR
		e = &ent[i];

		// Process name
		CutName(str2, e->fname);
		strcpy(e->fname, str2);
		//fprintf(stderr, "CutName = '%s'(old=%s)\n", str2, ent[i].fname);

		// Store structure
		for(z=0,p=e; z<sizeof(FE); z++,p++)
			fputc(*p, f2);
	}
	fclose(f2);

	// Flush everything to stdout
	system("cat /tmp/sqarchdr.tmp");
	system("cat /tmp/sqarcbin.tmp");

	// Report
	fprintf(stderr, "%d files archived.\n", files);

	// Remove temporary files
	remove("/tmp/sqarc.tmp");
	remove("/tmp/sqarchdr.tmp");
	remove("/tmp/sqarcbin.tmp");
}

int archive(const char *path)
{
	char str[255];

	//
	sprintf(str, "find %s > /tmp/sqarc.tmp", path);
	system(str);

	//
	archive1();
}

//
int main(int argc,char **argv)
{
	//
	if(argc<2)
	{
		//
		printf("sqarc - Squirrel Archiver\n");
		printf("(C) 2002-2003 by Jari Tuominen(jari@vunet.org)\n");
		printf("Usage: sqarc [directory]\n");
		printf("Will output to stdout, redirect the stream to a file:\n");
		printf("sqarc mydir > myarchive.sqa\n");
		return 0;
	}

	//
	if(getenv("jtmosdir"))
		strcpy(jtmosdir, getenv("JTMOSDIR"));
	else
		strcpy(jtmosdir, ".");
	archive(argv[1]);
	return 0;
}

