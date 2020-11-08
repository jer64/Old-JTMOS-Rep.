// Env. interface.
#include "kernel32.h"
#include "env.h"
#include "envint.h"

// Kernel environmental variables database;
ENVDB *kenv=NULL;

// Initialize environmental database system.
void initEnvDatabase(void)
{
	//
	if(kenv!=NULL)
		panic("initEnvDatabase fault 1");

	//
	if( (kenv = envdb_CreateVariableDatabase())==NULL )
	{
		// Failure
		panic("Could not initialize kernel environmental variable system.");
	}

	//
	ksetenv(kenv, "PATH",		"./:/bin:/usr/bin");
	ksetenv(kenv, "SHELL",		"/bin/sqsh.bin");
	ksetenv(kenv, "HOSTNAME",	"squirrel");
	ksetenv(kenv, "HOSTTYPE",	"i386");
	ksetenv(kenv, "OSTYPE",		"JTMOS");
	ksetenv(kenv, "OS",		"JTMOS");
}

//
int ksetenv(ENVDB *e, char *name, char *value)
{
	//
	return envdb_SetVariable(e, name, value);
}

//
int kgetenv(ENVDB *e, char *name, char *value)
{
	//
	return envdb_GetVariable(e, name, value);
}

//


