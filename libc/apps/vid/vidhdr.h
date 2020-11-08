#ifndef __INCLUDED_VIDHDR_H__
#define __INCLUDED_VIDHDR_H__

//
typedef struct
{
	// "JTMVIDEO-1"
	char ident[10];

	// Width and height
	int width,height;

	// Only 8-bpp is supported at the moment
	int bpp;

	// Compression type (unused)
	int type;
}VIDHDR;

#endif



