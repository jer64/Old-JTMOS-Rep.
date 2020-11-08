//
#ifndef __INCLUDED_WWW_SERVER_H__
#define __INCLUDED_WWW_SERVER_H__

//
#include "tcp.h"
#include "socket.h"

//
int www_server(SOCKET *s, char *request, char *content);

//
#endif



