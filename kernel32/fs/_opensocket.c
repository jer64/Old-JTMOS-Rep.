// _opensocket.c
#include "kernel32.h"
#include "threads.h"
#include "scs.h"
#include "_opensocket.h"

// Returns TRUE or FALSE.
int isValidSocketString(const char *s)
{
	int i,l,dots1,dots2;

	// Rule 1: first character must be a number
	if( !ISNUMBER(*s) )
	{
		// Not valid
		return FALSE;
	}

	// Must contain three dots and one ":" character
	l = strlen(s);
	for(i=0,dots1=0,dots2=0; i<l; i++)
	{
		if(s[i]=='.')
			dots1++;
		if(s[i]==':')
			dots2++;
	}

	// Got valid counts?
	if( !(dots1==3 && dots2==1) )
		return FALSE;

	// Valid.
	return TRUE;
}

//
int GetIPADDRPORT(const char *s, int *ipaddr, int *storeport)
{
	char str[50];
	int i,i2,l,cn,num,n1,n2,n3,n4,port;
	int *ntab[]={&n1,&n2,&n3,&n4,&port};

	// Reset IP and port
	n1=0; n2=0; n3=0; n4=0;
	port=0;

	// Get string length
	l = strlen(s);

	// Loop until got all five numbers
	// (A.B.C.D:PORT)
	for(i=0,cn=0; cn!=5 && i<l; i++)
	{
		if( ISNUMBER(s[i]) )
		{
			// Get number
			for(i2=0; i<l && ISNUMBER(s[i]); i2++,i++)
				str[i2]=s[i];
			str[i2]=0;
			// Convert string to integer
			num = dec2int(str);

			// Store
			ntab[cn++] = num;
		}
	}

	// Gather up an IP address
	*ipaddr = n1 | (n2<<8) | (n3<<16) | (n4<<24);
	// Store port
	*storeport = port;

	//
	return 0;
}

// Returns fd
// addStr is format "a.b.c.d:port"
int _opensocket(const char *addrStr)
{
	int i,sh;
	DWORD ipaddr,port;
	FILEDES *f;

	//--------------------------------------------------------
	// Check string validity
	if( !isValidSocketString(addrStr) )
	{
		// Invalid socket string
		return -1;
	}

	// Resolve the IP address and port form the string
	GetIPADDRPORT(addrStr, &ipaddr, &port);

	// Discard illegal IP address, if any.
	if(ipaddr==0)
		// Invalid IP address
		return -1;

	// Discard illegal port, if any.
	if(port<=0 || port>65535)
		// Invalid port
		return -1;

	//-----------------------------------------------------------
	// Call TCP stack
	//
	
	// Attempt to open socket
	if( (sh=scall(SYS_opensocket, ipaddr,port,0, 0,0,0))<0 )
	{
		// Couldn't connect
		return -1;
	}

	// Create new file descriptor
	if( (f = NewFD())==NULL )
	{
		// We should close the socket here:
		scall(SYS_closesocket, sh,0,0, 0,0,0);

		// Couldn't create file descriptor
		return -1;
	}

	// Set socket handle
	f->socketHandle = sh;

	// Connection created successfully, return FD code
	return f->fd;
}


