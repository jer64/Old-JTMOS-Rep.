#ifndef __INCLUDED_MSGBOX_H__
#define __INCLUDED_MSGBOX_H__

//
#define MB_PROTOCOL_DC		12341234
#define MB_PROTOCOL_POSTMAN	46842632
#define MB_PROTOCOL_GRAOS	99436111

//
#define MSG_RESERVED		0x55555555
#define MSG_FREE		0xAAAAAAAA

//
#define ISFREEX			0x00000010

//
#define MSGBOX_MAGIC1		0x55AACCEE
#define MSGBOX_MAGIC2		0xFF0088DD

// Preferences
#define N_MAX_QUEUED_MSGS	5
#define N_MAX_BYTES_PER_MSG	2048

// Message -structure
typedef struct
{
	//
	DWORD magic1;

	// TRUE or FALSE
	DWORD isFree;

	// Message type
	int type;

	// Protocol type
	int protocol;

	// Sender's PID
	int sndPID;

	// Receiver's PID
	int rcvPID;

	// Amount of bytes message contains
	int amount;

	// Packet number
	DWORD un;

	//
	DWORD magic2;

	// Message content
	BYTE buf[N_MAX_BYTES_PER_MSG];
}MSG;

// Message Box -structure
typedef struct
{
	// TRUE or FALSE
	char isFree;

	// Name of the message box
	char name[256];

	// Message queue
	MSG q[N_MAX_QUEUED_MSGS];
	// Number of message entries registered
	int n_q;
}MSGBOX;

// Function prototypes
MSGBOX *createMessageBox(void);
void initMessageBox(MSGBOX *m);
int receiveMessage(MSG *om);
int sendMessage(MSG *m, int pid);
int ActivateThreadMessageBox(int pid);
int VerifyMessage(MSG *msg);

#endif


