//
#ifdef JTMOSKERNEL
#include "kernel32.h"
#include "filebuf.h"
#include "filedef.h"
#else
#include <stdio.h>
#endif

//
int feof(FILE *f)
{
	if(f==NULL)return 123;

#ifndef JTMOSKERNEL
	if(f==stdin)return 0;
	if(f==stdout)return 0;
	if(f==stderr)return 0;
#endif

	return f->eof;
}


