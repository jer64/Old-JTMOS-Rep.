#ifndef __INCLUDED_JTMLIBC_RANDOM_H__
#define __INCLUDED_JTMLIBC_RANDOM_H__

int random(void);
void srandom(int seed);
int _rand(int which);
int rand(void);
void srand(DWORD seed);

#endif

