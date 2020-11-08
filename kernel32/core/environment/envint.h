//
#ifndef __INCLUDED_ENVINT_H__
#define __INCLUDED_ENVINT_H__

//
int ksetenv(ENVDB *e, char *name, char *value);
int kgetenv(ENVDB *e, char *name, char *value);
void initEnvDatabase(void);

//
extern ENVDB *kenv;

#endif
