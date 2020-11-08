/*
 * Functions for testing character types and converting characters.
 *
 * Parts borrowed from the creation originally 
 * made by Colin Peters <colin@bird.fu.is.saga-u.ac.jp>
 *
 * (C) 2002 by Jari Tuominen(jari@vunet.org)
 */

//
#ifndef __INCLUDED_CTYPE_H__
#define __INCLUDED_CTYPE_H__
// for compatibility
#define _CTYPE_H_

/*
 * The following flags are used to tell iswctype and _isctype what character
 * types you are looking for.
 */
#define	_UPPER		0x0001
#define	_LOWER		0x0002
#define	_DIGIT		0x0004
#define	_SPACE		0x0008
#define	_PUNCT		0x0010
#define	_CONTROL	0x0020
#define	_BLANK		0x0040
#define	_HEX		0x0080
#define	_LEADBYTE	0x8000
#define	_ALPHA		0x0103

//
#ifndef toupper
#define toupper(c)   ((c)>='a'&&(c)<='z'?(c)-('a'-'A'):(c))
#define tolower(c)   ((c)>='A'&&(c)<='Z'?(c)+('a'-'A'):(c))
#endif

// TODO functions:
// (Borrowed from ming)
int	isalnum(int);
int	isalpha(int);
int	iscntrl(int);
int	isdigit(int);
int	isgraph(int);
int	islower(int);
int	isprint(int);
int	ispunct(int);
int	isspace(int);
int	isupper(int);
int	isxdigit(int);
// Already exists as macroes(see above).
//int	tolower(int);
//int	toupper(int);

//
int	isleadbyte (int);

/*
// Wide character equivalents
// (not supported/planned yet)
int	iswalnum(wint_t);
int	iswalpha(wint_t);
int	iswascii(wint_t);
int	iswcntrl(wint_t);
int	iswctype(wint_t, wctype_t);
int	is_wctype(wint_t, wctype_t);	// Obsolete!
int	iswdigit(wint_t);
int	iswgraph(wint_t);
int	iswlower(wint_t);
int	iswprint(wint_t);
int	iswpunct(wint_t);
int	iswspace(wint_t);
int	iswupper(wint_t);
int	iswxdigit(wint_t);
//
wchar_t	towlower(wchar_t);
wchar_t	towupper(wchar_t);
*/

// End of TODO functions.

//
#ifndef WEOF
#define	WEOF	(wchar_t)(0xFFFF)
#endif

//
#endif

//

