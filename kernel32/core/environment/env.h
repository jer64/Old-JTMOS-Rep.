//
#ifndef __INCLUDED_ENV_H__
#define __INCLUDED_ENV_H__

// Max. number of variables in the variable database.
#define __N_MAX_VARS_IN_ENVDB		100

//
#define __ENVDB_VARIABLE_LENGTH_BYTES	100

//
#define __ENV_MAGIC	0x26FD9CCC

//
typedef struct
{
	// Validation magic number.
	DWORD magic;

	// Storage of all variables.
	BYTE *big;

	// Number of variable entries registered.
	int n_vars;
	// Array of PTRs to strings of variables.
	char *var[__N_MAX_VARS_IN_ENVDB];
}ENVDB;

//
int envdb_DeleteVariable(ENVDB *e,
        const char *name);
int envdb_SetVariable(ENVDB *e,
        const char *name, const char *value);
static void *envdb_FindVariable(ENVDB *e, const char *name);
ENVDB *envdb_CreateVariableDatabase(void);
int envdb_GetVariable(ENVDB *e,
        const char *name, char *content);

//
#include "envint.h"

#endif



