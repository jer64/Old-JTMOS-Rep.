// ===========================================================================
// File handling functions.
// (C) 2002-2005 by Jari Tuominen(jari@vunet.org).
//
// TODO: buffers.
// ===========================================================================
#include "basdef.h"
#include "console.h"
#include "syscallcodes.h"
#include "cpu.h"
#include "file.h"
#include "postman.h"
#include "filebuf.h"
#include "dfile.h"

// READ FILE
int readfile(const char *fname, BYTE *buf, int len, int offset)
{
        static POSTMANCMD post;
	int ack;

        //
        post.id =               PCMD_READ_FILE;
        post.par1 =             fname;
        post.par2 =             buf;
        post.v1 =               len;
        post.v2 =               offset;

	//
	return PostmanPost(&post);
}

// WRITE FILE
int writefile(const char *fname, BYTE *buf, int len, int offset)
{
        static POSTMANCMD post;
        int r,ack;

        // 1) File has to be created first
        if(!fexist(fname))
		__fcreate(fname);
        // 2A) Write file then afterwards
        post.id =               PCMD_WRITE_FILE;
        post.par1 =             fname;
        post.par2 =             buf;
        post.v1 =               len;
        post.v2 =               offset;

        // 2B) Commence operation
	r = PostmanPost(&post);
        return r;
}

