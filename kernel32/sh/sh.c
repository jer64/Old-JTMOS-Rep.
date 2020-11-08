//
#include <stdio.h>
#include "sh.h"
#include "lsapp.h"
#include "formatapp.h"
#include "chdirapp.h"
#include "plasma_app.h"
#include "debugapp.h"

//
#define sh_debug dprintk

//
int help_app(int argc, char **argv);

//
static char *ptr_string_table_cmds[] = {
    "debug",	debug_app,	"Change Debug Mode", NULL,
    "cd",	chdir_app,	"Change Work Directory",NULL,
    "chdir",	chdir_app,	"Change Work Directory",NULL,
    "ls",	ls_app,		"Lists A Directory",NULL,
    "dir",	ls_app,		"Lists A Directory",NULL,
    "mkfs",	format_app,	"Format A Device",NULL,
    "format",	format_app,	"Format A Device",NULL,
    "plasma",	plasma_app,	"Plasma Demo Effect",NULL,
    "help",	help_app,	"Shell Manual",NULL,
    NULL, NULL, NULL, NULL
};

// Simple Help App.
int help_app(int argc, char **argv)
{
    int i;
    
    //
    printf("List Of Commands Available Internally In The Shell.\n");
    for(i=0; ptr_string_table_cmds[i+0]!=NULL; i+=4)
    {
        printf("[%s]: \"%s\"\n",
            ptr_string_table_cmds[i+0],
            ptr_string_table_cmds[i+2]);
    }
    return 0;
}

// Returns argc.
int getargumentlist(char *ptr_string_table_cmdstr, char **argv)
{
    int i,argc;
    char *ptr,*src;
    
    ptr = strdup(ptr_string_table_cmdstr);
    src = strtok(ptr, " ");
    if(src==NULL)
    {
        argc = 0;
    }
    else
    {
        for(i=0,argc=0; i<5000; i++,argc++)
        {
            argv[i] = src;
            if(src==NULL) { break; } // Break On No More Tokens Available.

            src = strtok(NULL, " ");
        }
    }

    // Return the argument count.
    return argc;
}

//
int run_shell(void)
{
    char *ptr_long_string, *ptr_str_fn_executable,
        *ptr_str_user_input, *ptr_string_head,
        *ptr_head;
    void (*ptr_call)(int,char **);
    int i,i2;
    int argc;
    char **argv;

    // RESERVE MEMORY    
    // 64K should be enough for a command line prompt string of a maintenance shell.
    ptr_long_string = malloc(1024*64);
    if(ptr_long_string==NULL)
    {
        printf("Error: \"ptr_long_string\" memory allocation failed, malloc returned NULL.\n");
        return 1;
    }

    ptr_str_fn_executable = malloc(1024*64);
    if(ptr_str_fn_executable==NULL)
    {
        printf("Error: \"ptr_str_fn_executable\" memory allocation failed, malloc returned NULL.\n");
        return 1;
    }

    // HELLO TEXT TO THE USER.
    printf("Built-In System Maintenance Debug Shell 1.0 / 7/2020\n");

    // ACTUAL SHELL MAIN LOOP.
    while(1)
    {
        // Indicator.
        printf("#");
        // Tell about happening, inputting a string.
        sh_debug("%s: Inputting string.\n", __FUNCTION__);
        //getch();
        gets(ptr_long_string);
        ptr_str_user_input = ptr_long_string;
        //
        if(ptr_string_table_cmds!=NULL)
        {
          for(i=0; ; i+=4)
          {
            if(ptr_string_table_cmds[i+0]==NULL)
            {
                // Let's try non-internal application with system(..)?
                // It seems to be the last parameter on cmds table.
                //
                sh_debug("strtok on \"ptr_string_head\", to get the executable file name.\n");
                //getch();
                ptr_string_head = strdup(ptr_str_user_input);
                ptr_str_fn_executable = strtok(ptr_string_head, " ");
                // END OF LIST AND NO RECOGNIZED INTERNAL COMMAND.
                // Did not recognize this as an internal command.
                if(ptr_str_fn_executable!=NULL)
                {
                    sh_debug("%s: Launching external application '%s': %s\n",
                        __FUNCTION__, ptr_long_string, ptr_str_fn_executable);
                    //getch();
                    //system(ptr_long_string);
                }
                else
                {
                    sh_debug("Error: empty executable file parameter.\n");
                    //getch();
                }
                break;
            }
            else
            {
                //
                sh_debug("Doing strtok to get the executable file name.\n");
                //getch();
                ptr_string_head = strdup(ptr_str_user_input);
                ptr_str_fn_executable = strtok(ptr_string_head, " ");

                sh_debug("Checking whether command is the one in the string table array.\n");
                //getch();
                //
                if(ptr_str_fn_executable!=NULL)
                {
                    if(ptr_string_table_cmds!=NULL && ptr_string_table_cmds[i+0]!=NULL)
                    {
                        if(!strcmp(ptr_string_table_cmds[i+0], ptr_str_fn_executable))
                        {
                            // See We Got A Recognized internal command!
                            sh_debug("Calling getargumentlist\n");
                            //getch();
                            argv = malloc(1024*64);
                            memset(argv, 0, 1024*64);
                            argc = getargumentlist(ptr_str_user_input, argv);
                            //
                            ptr_call = ptr_string_table_cmds[i+1];
                            sh_debug("%s: Launching up internal application (ptr=0x%x): %s (app. has %d arguments, argv at 0x%x).\n",
                                __FUNCTION__,ptr_call,ptr_str_fn_executable,argc,argv);
                            //getch();
                            ptr_call(argc,argv);
                            sh_debug("Application finished.\n");
                            //getch();
                            break;
                        }
                    }
                }
                else
                {
                    // Continue search loop below:
                }
            }
        }
        
        
      }
      // Require non-NULL array of strings for commands, routine execution pointers, and extra options.
    }
    // FREE SHELL MEMORY.
    sh_debug("Preparing to exit the shell. Freeing memory.\n");
    free(ptr_long_string);
    free(ptr_str_fn_executable);
    //
    printf("Exiting Shell, Thank You For Visiting Us!\n");
    ExitThread(0);
}

//

