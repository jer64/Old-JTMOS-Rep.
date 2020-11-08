//
#include "console.h"
#include "error.h"
#include "file.h"
#include "error.h"

// assert - Abort the program if assertion is false.
void assert (int expression)
{
	//
	if(!expression)
	{
		LIBCERROR("(%d)Assert failed", GetTickCount());
	}
}
