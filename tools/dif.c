//-----------------------------------------------------------------------------------------------
// File difference finder.
// (C) 2003 by Jari Tuominen
// Updated on 27.09.2003.
//-----------------------------------------------------------------------------------------------
#include <stdio.h>
#define BYTE unsigned char

//
int fsizeof(const char *fname)
{
	FILE *f;
	int l;

	//
	f=fopen(fname, "rb");
	if(f==NULL)	return 0;
	fseek(f, 0, SEEK_END); l=ftell(f);
	fclose(f);

	//
	return l;
}

//
void loadbin(const char *fname,
	unsigned char *buf,
	int l_buf)
{
	FILE *f;
	int i,l;

	//
	f=fopen(fname, "rb");
	if(f==NULL)	return;
	fseek(f, 0, SEEK_END); l=ftell(f); rewind(f);
	fread(buf, l % (l_buf+1), 1, f);
	fclose(f);
}

// Compare two specified buffers.
int compare1(unsigned char *buf, int l_buf,
	unsigned char *buf2, int l_buf2)
{
	int i,i2,l,d,dif;

	//
	if(l_buf>l_buf2)	l = l_buf2;
		else
				l = l_buf;
	printf("Comparing %d bytes (%d <-> %d)\n",
		l, l_buf, l_buf2);
	for(dif=0,i=0,d=0; i<l; i++)
	{
		//
		if(buf[i] != buf2[i])
			d++;
			else
			{
				if(d)	printf("0x%x\n", i);
				d=0;
			}

		//
		if(d==1)
		{
			dif++;
			printf("difference at 0x%x-", i);
		}
	}
	if(d)	printf("0x%x\n", i);
	return dif;
}

// Compare two specified files.
void compare(const char *file1,
	const char *file2)
{
	FILE *f;
	BYTE *buf,*buf2;
	int l_buf,l_buf2,res;

	//
	l_buf = fsizeof(file1);
	l_buf2 = fsizeof(file2);
	buf = malloc(l_buf);
	buf2 = malloc(l_buf2);

	//
	loadbin(file1, buf, l_buf);
	loadbin(file2, buf2, l_buf2);

	//
	res = compare1(buf, l_buf, buf2, l_buf2);

	//
	if(!res)
		printf("Files compare OK.\n");
}

//
int main(int argc, char **argv)
{
	//
	if(argc<3)
	{
		printf("Usage: dif [file I] [file II]\n");
		return 0;
	}

	//
	compare(argv[1], argv[2]);

	//
	return 0;
}




