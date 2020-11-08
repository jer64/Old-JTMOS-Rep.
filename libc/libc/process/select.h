//
#ifndef __INCLUDED_SELECT_H__
#define __INCLUDED_SELECT_H__

//
typedef struct
{
	//
	DWORD fds_bits[200];
}fd_set;

//
//int select(int n, fd_set *readfds, fd_set *writefds,
//        fd_set *exceptfds, struct timeval *timeout);

#endif




