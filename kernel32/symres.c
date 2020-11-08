///////////////////////////////////////////////////////////////////////////////////
// Address to symbol resolver.
// (C) 2004 by Jari Tuominen.
///////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>

//
#define DWORD	unsigned long int
#define WORD	unsigned short int
#define BYTE	unsigned char

//
#define FNAME		"kernel32.sym"
//
#define N_MAX_SYM	100000

//
typedef struct
{
	//
	char name[256];
	DWORD ad;
}SYMBOL;

//
typedef struct
{
	// Symbol array.
	SYMBOL s[N_MAX_SYM];
	// Number of symbols registered.
	int n_s;
}SYM;

///////////////////////////////////////////////////////////////////////////////////
//
static SYM sym;

///////////////////////////////////////////////////////////////////////////////////
//
void LoadSymTable(const char *fname)
{
	FILE *f;
	int i,i2,i3,i4;

	//
	f = fopen(fname, "rb");
	if(f==NULL)
	{
		printf("File not found '%s'.\n",
			fname);
		abort();
	}

	//
	for(i=0; !feof(f); i++)
	{
		//
		fscanf(f, "%x %s",
			&sym.s[i].ad, 
			&sym.s[i].name);
	}

	//
	sym.n_s = i-1;

	//
	fclose(f);

	//
	printf("%d symbols loaded from %s.\n",
		sym.n_s, fname);
}

///////////////////////////////////////////////////////////////////////////////////
//
void *ResolveAd(DWORD ad)
{
	int i,i2,i3,i4,last;
	SYMBOL *s;

	//
	for(i=0; i<sym.n_s; i++)
	{
		//
		s = &sym.s[i];

		//
		if(s->ad == ad)
			return s->name;

		//
		if(s->ad > ad && i)
		{
			return &sym.s[i-1].name;
		}
	}

	//
	return "unknown";
}

///////////////////////////////////////////////////////////////////////////////////
//
int main(int argc, char **argv)
{
	DWORD ad;

	//
	if(argc<2)
	{
		//
		printf("Usage: %s [hexademical address]\n",
			argv[0]);
		return 0;
	}

	//
	sscanf(argv[1], "%x", &ad);

	//
	LoadSymTable(FNAME);

	//
	printf("Address 0x%x resolves to '%s'.\n",
		ad, ResolveAd(ad));

	//
	return 0;
}

//

