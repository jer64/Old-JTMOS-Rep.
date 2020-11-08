//================================================================
// logging.c - system logging service.
//================================================================
#include "kernel32.h"
#include "logging.h"

//
#define FN_SYSLOG	"/var/system.log"

//
THREAD LoggerThread;
FBUF SystemLog;
int system_logging=FALSE;

// Init system logging.
// Makes possible to log messages to the system log,
// despite the fact the log might not get written
// on the disk until the system has completely been
// started up.
// System is usually initialized at the system boot up,
// early in the kernel initialization, so that even
// the earliest messages could be recorded to the log.
void initSystemLog(void)
{
	// Only allow initialization once.
	if(system_logging)
		return;

	// 32K of space for system messages buffer.
//	CreateBuffer(&SystemLog, 1024*32);
//	system_logging = TRUE;
}

// Writes a single string into the system log.
void WriteSystemLog(const char *msg)
{
	int i,l;

	// System log must be running.
	if(!system_logging)
		return;

	//
	l = strlen(msg);
	for(i=0; i<l; i++)
		PutBuffer(&SystemLog, msg[i]);
	PutBuffer(&SystemLog, '\n');
}

// Start system logging process.
// Writes system log updates on drive.
void initSystemLogging(void)
{
	//
	ThreadCreateStack(&LoggerThread,1024*16);
	LoggerThread.pid =
		create_thread(LoggerThread.stack,
			LoggerThread.l_stack, LoggerThreadProgram);

	// max. 31 characters.
	IdentifyThread(LoggerThread.pid, "KLOGGER");
}

// Filter for determing whether if the character is proper to be
// outputted at the system log.
// Return value:
// TRUE: Character is OK.
// FALSE: Character should be ignored.
static int filter(int c)
{
	//
	if( c>0x7F )
		return FALSE;
	if( !c )
		return FALSE;

	//
	return TRUE;
}

// This process will start logging on root file system
// only after the system has started.
void LoggerThreadProgram(void)
{
	int i,i2,i3,i4,d;
	static BYTE buf[8192];
	int fd;

	//----------------------------------------------------------------------------------
	// Be sure to make system log rolling if it has not yet started.
	//
	initSystemLog();

	//----------------------------------------------------------------------------------
	// Somethings to do before starting log saving...
	//

	// Wait until whole system starts.
	while( !(systemStarted&2) )
	{
		sleep(1);
	}

	//
	WriteSystemLog("System started.");
	WriteSystemLog("System logger now starts saving system log on root file system.");

	//
	mkdir("/var", 0);

	// Remove any old log.
	remove(FN_SYSLOG);

	// Indicate that the process is running.
	fd = open("/var/klogger.pid", O_RDONLY);
	if(fd<0) {
		fd=open("/var/klogger.pid", O_CREAT);
		if(fd>=0)
		{
			close(fd);
		}
		else	printk("%s: Could not create PID file.\n", __FUNCTION__);
	} else {
		close(fd);
	}

	//----------------------------------------------------------------------------------
	// This loop writes any data available at system log to root file system.
	//
	while(1)
	{
moreplease:
		// Read upto 8192 bytes of data to our buffer.
		for(i=0; i<8192; i++)
		{
			if( (d=GetBuffer(&SystemLog))<0 )
				break;
			if( filter(d) )
				buf[i] = d;
		}

		// Got any bytes to write?
		if(i)
		{
			fd = open(FN_SYSLOG, O_CREAT | O_APPEND);
			if(fd>=0)
			{
				write(fd, buf, i);
				close(fd);
			}
			else	printk("Cannot write system log.\n");

			// Safety idle.
			SwitchToThread();

			//
			goto moreplease;
		}

		//
		sleep(2);
	}
}

//



