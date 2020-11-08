// env.c - environmental variables system.
//-------------------------------------------------------------------------------------
#include "kernel32.h"
#include "env.h"

// Creates a new variable database.
ENVDB *envdb_CreateVariableDatabase(void)
{
	ENVDB *e;
	int i,i2;
	void *p;

	// Allocate & init structure.
	e = malloc(sizeof(ENVDB));
	if(e==NULL)
		return NULL;
	memset(e, 0, sizeof(ENVDB));

	// Set valid magic value.
	e->magic = __ENV_MAGIC;

	// Define amount of variables we'll have on this database.
	e->n_vars = __N_MAX_VARS_IN_ENVDB;

	// Allocate space for all variables.
	e->big = malloc(e->n_vars*__ENVDB_VARIABLE_LENGTH_BYTES);

	// Define PTRs to variables.
	for(i=0,p=e->big; i<e->n_vars; i++,p+=__ENVDB_VARIABLE_LENGTH_BYTES)
	{
		e->var[i] = p;
		memset(e->var[i], 0, __ENVDB_VARIABLE_LENGTH_BYTES);
	}

	//
	return e;
}

// Creates a new variable entry with empty value.
// Internal function.
static void *envdb_CreateVariable(ENVDB *e, const char *name)
{
	int i,i2;

	//
	for(i=0; i<e->n_vars; i++)
	{
		if(!strlen(e->var[i]))
		{
			// Found empty entry, let's use it.
			sprintf(e->var[i], "%s=", name);
			return e->var[i];
		}
	}

	// Oops, couldn't create.
	return NULL;
}

// Finds a variable on database and returns PTR to it.
// Internal function.
static void *envdb_FindVariable(ENVDB *e, const char *name)
{
	char tmp[256];
	int i,i2,l;

	//
	sprintf(tmp, "%s=", name);
	l = strlen(tmp);

	//
	for(i=0; i<e->n_vars; i++)
	{
		if( !strncmp(e->var[i],tmp, l) )
		{
			return e->var[i];
		}
	}

	// Oops, couldn't create.
	return NULL;
}

// Set variable to specified value.
int envdb_SetVariable(ENVDB *e,
	const char *name, const char *value)
{
	int i;
	char *var;

	//
	if(e==NULL || value==NULL || e->magic!=__ENV_MAGIC)
		// E pointer doesn't pointer to a valid database.
		return 1;

	//
	if(strlen(value)>=(__ENVDB_VARIABLE_LENGTH_BYTES-2) || !strlen(value))
		// Value string is too long or too short.
		return 2;

	//
	var = envdb_FindVariable(e, name);
	if(var==NULL)
	{
		// Create new variable.
		var = envdb_CreateVariable(e, name);
		if(var==NULL)
			// Couldn't create variable.
			return 3;
	}

	// Change contents.
	sprintf(var, "%s=%s", name,value);

	//
	return 0;
}

// Get variable to a string buffer.
int envdb_GetVariable(ENVDB *e,
	const char *name, char *content)
{
	int i,i2;
	char *var;

	//
	if(e==NULL || content==NULL || e->magic!=__ENV_MAGIC)
		// E pointer doesn't point to a valid database.
		return 3;

	//
	var = envdb_FindVariable(e, name);
	if(var==NULL)
	{
		// Couldn't find variable.
		return 1;
	}

	//
	for(i=0,i2=0; var[i]!='=' && var[i]!=0 && i2<256; i++,i2++);
	if(var[i]!='=')
		// Oops, invalid variable.
		return 2;

	// Skip '=' char.
	i++;

	// Store content.
	strcpy(content, "");
	if( (strlen(var)-i)>0 )
		strncpy(content,var+i, strlen(var)-i);

	// OK.
	return 0;
}

// Set variable to specified value.
int envdb_DeleteVariable(ENVDB *e,
	const char *name)
{
	int i;
	char *var;

	//
	if(e==NULL || e->magic!=__ENV_MAGIC)
		// E pointer doesn't pointer to a valid database.
		return 4;

	//
	var = envdb_FindVariable(e, name);
	if(var==NULL)
	{
		// Couldn't create variable.
		return 1;
	}

	// Delete variable.
	strcpy(var, "");

	//
	return 0;
}

//

