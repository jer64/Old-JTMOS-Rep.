//
#ifdef JTMOSKERNEL
#include "kernel32.h"
#include "filebuf.h"
#include "filedef.h"
#else
#include <stdio.h>
#endif

//
int ftell(FILE *_stream)
{
	//
	return _stream->baseOffs + _stream->offs;
}


