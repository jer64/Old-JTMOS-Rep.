//
#ifndef __INCLUDED_BLOCKCACHE_H__
#define __INCLUDED_BLOCKCACHE_H__

//
#define N_MAX_BCACHE_ENTRIES	100

//
typedef struct
{
	//
	BYTE *buf;
	int l_buf;
	int dnr,bnr;
	char isFree;
}CAC;

//
typedef struct
{
	//
	CAC **e; 
	int n_e;
}BCACHE;

//
extern BCACHE bc;

//
int cacheRB(int dnr, int bnr, BYTE *buf);
int cacheWB(int dnr, int bnr, BYTE *buf);

#endif


