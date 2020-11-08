//
#include "console.h"
#include "error.h"
#include "file.h"

//
void panic(const char *s)
{
	//
	printf("\n[LIBC error: '");
	printf(s);
	printf("']\n");
	abort();
}

//
void LIBCERROR(const char *apfmt, va_list aparg)
{
	//
	printf("\n[LIBC error: '");
	printf(apfmt, aparg);
	printf("']\n");
	printf("Program terminated(PID=%d).\n", GetCurrentThread());
	abort();
}

//
void perror(char *prefix)
{
 //
 fprintf(stderr,"[Application error: '%s']\n", prefix);

 //
}

//

