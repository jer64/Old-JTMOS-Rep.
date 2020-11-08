//------------------------------------------------------------------------------------------------------------
//
// World Wide Web server
// (C) 2005 Jari Tuominen.
//
//------------------------------------------------------------------------------------------------------------
#include "kernel32.h"
#include "www_server.h"

//------------------------------------------------------------------------------------------------------------
int compare(char *s1, char *s2, int len)
{
	int l1,l2,i,i2,i3,i4;

	//
	l1 = strlen(s1);
	l2 = strlen(s2);

	//
	for(i=0,i2=0; i<l1 && i<l2 && i2<len; i++,i2++)
	{
		if(s1[i]!=s2[i])
			return TRUE;
	}

	// Complete match.
	return FALSE;
}

//------------------------------------------------------------------------------------------------------------
//
int GetDoc(char *path, char *request, char *content)
{
	int i,i2,i3,i4;
	static char *rootdoc[]={
		"HTTP/1.0 200 OK\r\n",
		"Server: Lenin Web Server 1.0.17112005\r\n",
		"Content-type: text/html\r\n",
		"\r\n",
		"<FONT size=5 color=#FF0000>\r\n",
		"Apparently this works!!\r\n",
		"</FONT>\r\n",

		"<BR>\r\n",
		"<IMG SRC=\"http://jtmos.vunet.org/altwelcome.jpg\"><BR>\r\n",
		"<BR>\r\n",
		"<HR><BR>\r\n",
		"JTMOS world wide web server 1.0.2005<BR><BR>\r\n",
		"*"
		};

	//
	strcpy(content, "");
	for(i=0; strcmp(rootdoc[i],"*"); i++)
	{
		strcat(content, rootdoc[i]);
	}

	//
	strcat(content, "<PRE>");
	if(strstr(request,"lenin"))
	{
		strcat(content, request);
	}
	strcat(content, "</PRE>");

	//
	return strlen(content)+1;
}

//------------------------------------------------------------------------------------------------------------
//
// World Wide Web server.
//
int www_server(SOCKET *s, char *request, char *content)
{
	int i,i2,i3,i4;

	//
	return GetDoc("/", request, content);

        //
        if( !compare(request, "GET / HTTP", 10) )
        {
        }

        //
        strcpy(content, "HTTP/1.0 440 NOT FOUND\r\nServer: Lenin Web Server 1.0.17112005\r\nContent-type: text/html\r\n\r\n440 NOT FOUND, SORRY...<BR>");
        return strlen(content)+1;
}

