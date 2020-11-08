// Process to Process talk demo
#include <stdio.h>
#include <jtmos/msgbox.h>
#define OTHER "listen"

//
int main(int argc, char **argv)
{
	int pid;
	MSG m,m2;
	char *say="Hello you process outhere!";
	char *reply="Hello to you as well!";

	// Enable our message box
	startMessageBox();

#ifndef LISTEN
	// Look for listenning process
	if( (pid=GetThreadPID(OTHER))==-1 )
	{
		//
		printf("Process '%s' is not running.\n",
			OTHER);
		return 0;
	}

	//
	strcpy(m.buf, say);
	m.amount = strlen(say)+1;
	sendMessage(&m, GetThreadPID(OTHER));
	printf("Message sent: '%s'\n", say);
#endif

#ifdef LISTEN
	// Get proper name for us
	IdentifyThread(GetCurrentThread(), OTHER);
#endif

	//
	printf("Waiting for message ... ");
	while( receiveMessage(&m)==0 )
		SwitchToThread();
	printf("\nI received message: '%s'\n",
		m.buf);

#ifdef LISTEN
	// Send a reply
	strcpy(m2.buf, reply);
	m2.amount = strlen(reply)+1;
	sendMessage(&m2, m.sndPID);
#endif

	//
	return 0;
}

