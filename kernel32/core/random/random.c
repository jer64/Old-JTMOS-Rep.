// JTMOS LIBC Random Number Generator System 1.0
// (C) 2002 by Jari Tuominen(jari@vunet.org)
//
#include "kernel32.h"

// For generating rather "randomous values",
// but not at all near random,
// but satisfactory for the present situation.
int _libc_random[10]={
		0,23468,782137,12882352,34589042,
		68723457,12783578,723578,1020575,9925123
			};
int _libc_trickery=0;

int random(void)
{
	return rand();
}

void srandom(int seed)
{
	srand(seed);
}

int _rand(int which)
{
	_libc_random[which] +=  which+_libc_trickery;
	_libc_trickery = (_libc_trickery+1)&3;
	return _libc_random[which];
}

int rand(void)
{
	int total;

	total += _rand(0);
	total += _rand(1);
	total += _rand(2);
	total += _rand(3);
	total += _rand(4);
	total += _rand(5);
	total += _rand(6);
	total += _rand(7);
	total += _rand(8);
	total += _rand(9);
	return total;
}

void srand(DWORD seed)
{
	// Do something here * TODO
}

