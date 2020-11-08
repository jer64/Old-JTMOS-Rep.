// Network Test
#include <stdio.h>

//
#define CIP(n1,n2,n3,n4) ((n1)|(n2<<8)|(n3<<16)|(n4<<24))

//
int main(int argc, char **argv)
{
	int sh,ip1,ip2,ip3,ip4,port;
	char str[256];

	//
	printf("Network test 1.0\n");
	if(!strcmp(argv[1],""))
	{
		printf("Example: 195 74 0 47  1234\n");
		printf("Enter IP (format: n1 n2 n3 n4 port): ");
		gets(str);
	}
	else
	{
		sprintf(str, "%s %s %s %s %s",
			argv[1],argv[2],argv[3],argv[4],argv[5]);
	}
	sscanf(str, "%d %d %d %d %d", &ip1,&ip2,&ip3,&ip4,&port);
	if(ip1==0)
	{
		printf("Invalid IP address('%s').\n",
			str);
		return 1;
	}
	if(port<=0 || port>65535)
	{
		printf("Invalid port.\n");
		return 1;
	}

	// Open & close socket
	printf("Connecting to %d.%d.%d.%d:%d ... ",
		ip1,ip2,ip3,ip4, port);
	sh = OpenSocket(CIP(ip1,ip2,ip3,ip4), port);
	if(sh<0)
		printf("\nCould not connect.\n");
	else
	{
		printf("\nConnection established.\n");
		CloseSocket(sh);
	}

	//
	return 0;
}
