// ctypefamily.c - ctype -family functions
#include <stdio.h>

int tolower(int c)
{
	char *AL="QWERTYUIOPASDFGHJKLÖÄÅZXCVBNM";
	char *al="qwertyuiopasdfghjklöäåzxcvbnm";
	int i,l;

	l = strlen(al);
	for(i=0; i<l; i++)
	{
		if(AL[i]==c)return al[i];
	}
	return c;
}

int toupper(int c)
{
	char *AL="QWERTYUIOPASDFGHJKLÖÄÅZXCVBNM";
	char *al="qwertyuiopasdfghjklöäåzxcvbnm";
	int i,l;

	l = strlen(al);
	for(i=0; i<l; i++)
	{
		if(al[i]==c)return AL[i];
	}
	return c;
}

int isalnum(int c)
{
	return isalpha(c);
}

int isalpha(int c)
{
	char *AL="QWERTYUIOPASDFGHJKLÖÄÅZXCVBNM";
	char *al="qwertyuiopasdfghjklöäåzxcvbnm";
	int i,l;

	l = strlen(al);
	for(i=0; i<l; i++)
	{
		if(al[i]==c)return 1;
		if(AL[i]==c)return 1;
	}
	return 0;
}

int isdigit(int c)
{
	char *nu="0123456789";
	int i,l;

	l = strlen(nu);
	for(i=0; i<l; i++)
	{
		if(nu[i]==c)return 1;
	}
	return 0;
}

int iscntrl(int c)
{
	return 0;
}

int ispunct(int c)
{
	return 0;
}

int isxdigit(int c)
{
	char *al="0123456789abcdefABCDEF";
	int i,l;

	l = strlen(al);
	for(i=0; i<l; i++)
	{
		if(al[i]==c)return 1;
	}
	return 0;
}

int isspace(int c)
{
	if(c==' ')return 1;
	return 0;
}

int islower(int c)
{
	return 1-isupper(c);
}

int isupper(int c)
{
	char *AL="QWERTYUIOPASDFGHJKLÖÄÅZXCVBNM";
	int i,l;

	l = strlen(AL);
	for(i=0; i<l; i++)
	{
		if(AL[i]==c)return 1;
	}
	return 0;
}

int isprint(int c)
{
	return 0;
}

int isgraph(int c)
{
	return 0;
}


