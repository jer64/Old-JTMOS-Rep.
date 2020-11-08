//--------------------------------------------------------------------
// centralprocessMain.c
//--------------------------------------------------------------------
#include "kernel32.h"
#include "centralprocess.h"
#include "centralprocessMain.h"
#include "threads.h"
#include "centralprocessInit.h"
#include "sysglvar.h"
#include "keyb.h"
#include "driver_api.h"     // driver_getLock
#include "callapp.h"

//
int reboot_now=FALSE;

// The kernel init process(KINITPROCESS) itself.
// - creates processes
// - oversees the creation process
void centralprocess_run(void)
{
	int key,lt,amount;

	// ALLOCATE RESOURCES
	// Allocate buffer for EXE
	amount = 1024*256;
	centralprocess.l_bin = amount;
	centralprocess.bin = malloc(amount);

	// 1) Set priority to allowable
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	// 2) This must be done safely
	disable();

	// 4) Setup appropriate visual theme
	textcolor(14); textbackground(1);
	clrscr();
	printk("TCP: The Central Process\n");
	centralprocess_HelpPage();
	//
	textbackground(0);
	window(0,2,79,24); clrscr();

	// 5) Enable interrupts
	enable();

	// 6) Main loop of the KINIT -process
	while(1)
	{
		//
		if(reboot_now)
		{
			reboot_now = FALSE;
			callapp( SystemReboot );
		}

		// Got a job to do?
		if(centralprocess.isBusy)
		{
			// Report action
			printk("\rcentralprocess launching '%s' at '%s'\n",
				centralprocess.fname, centralprocess.runPath);

			// Load application executable binary
			if( centralprocess_loadEXE(centralprocess.fname) )
			{
				// Could not load:
				goto pastx;
			}

			// Launch the application
			centralprocess_job();

			// OK or NOT ?
			if(centralprocess.appPID)
			{
				// Wait until it has gone
				for(lt=GetTickCount(); centralprocess.isBusy && !launchapp_cleartogo; )
				{
		//
		if( (GetTickCount()-lt)>CENTRALPROCESS_EXEC_WAIT_TIMEOUT )
		{
						//
						KillThread(centralprocess.appPID);
						printk("\rcentralprocess internal error occured creation failed.\n");
						break;
		}

					//
					if(centralprocess.error)
						break;

					//
					SwitchToThread();
				}

				//
				if(centralprocess.appPID && !centralprocess.error)
					printk("\rProcess created, PID=%d                  \n", centralprocess.appPID);
					else
				{
					//
					printk("\rNo process created.\n");
					centralprocessExplainError();
				}
			}
			else
			{
pastx:
				//
				printk("Error: Process launch failed - Error #%d\n",
					centralprocess.error);
			}
		}

		// Save CPU time.
		SwitchToThread();

		// Input user keyboard
		key = getch1();

		// Show list of devices
		if( key=='D' )
		{
			//syssh_lsdev();
		}

		// Tonggle verbose debugging
		if( key=='v' )
		{
			if(jtmos_debug)
			{
				printk("Verbose debug messages are now disabled.\n");
				jtmos_debug=0;
			}
			else
			{
				printk("Verbose debug messages are now enabled.\n");
				jtmos_debug=1;
			}
		}

		// FILEDES info
		if( key=='f' )
		{
			filedesInformation();
		}

		//
		if( key=='h' )
		{
			centralprocess_HelpPage();
		}

		//
		if( key=='y' )
		{
			printk("Lock owner PID(%d) last function: '%s'\n",
				driver_getLock(0), driver_getDebugVar());
		}

		//
		if( key=='u' )
		{
			// Lock #0 to 0(0=Unlocked)
			driver_forceSetLock(0, 0);
		}

		//
		if( key=='l' )
		{
			printk("driver_getLock() = %d\n",
					driver_getLock(0));
		}

		//
		if( key=='s' )
		{
			//
			for(; getch1(); ) { SwitchToThread(); }

			//
			printk("Launch new shell to which terminal(1-9)? ");
			key = getch(); printk("%c - OK\n", key);

			//
/*			if(key=='1')
				syssh_callappex( ShellPrompt, 0, 0 );
			if(key=='2')
				syssh_callappex( ShellPrompt, 0, 1 );
			if(key=='3')
				syssh_callappex( ShellPrompt, 0, 2 );
			if(key=='4')
				syssh_callappex( ShellPrompt, 0, 3 );
			if(key=='5')
				syssh_callappex( ShellPrompt, 0, 4 );
			if(key=='6')
				syssh_callappex( ShellPrompt, 0, 5 );
			if(key=='7')
				syssh_callappex( ShellPrompt, 0, 6 );
			if(key=='8')
				syssh_callappex( ShellPrompt, 0, 7 );
			if(key=='9')
				syssh_callappex( ShellPrompt, 0, 8 );*/
		}

		//
		if( key==32 )
		{
			// PROCESS LIST
			callappex( ps, 0, GetCurrentTerminal() );
		}

		//
		if( key=='k' && centralprocess.appPID )
		{
			disable();
			printk("Killing PID=%d, state now = %d\n",
				centralprocess.appPID, thread_states[RPID(centralprocess.appPID)]);
			KillThread( centralprocess.appPID );
			enable();
		}
	}

	//
}

// Called at last parts of system init.
// So we don't need to create a seperate process.
//
void centralprocess_init(void)
{
	// =================================================
	memset(&centralprocess, 0, sizeof(centralprocess));
	// =================================================
	centralprocessInit();
}


