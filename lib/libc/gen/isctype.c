/*
 * C implementation if the is* and toascii functions.
 * 	ragge 2026-01-16.  Public domain.
 *
 * 	@(#) isctype.c 1.0 (2.11BSD) 2026/1/16
 */

#include <ctype.h>

#undef isalnum
#undef isalpha
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef isascii
#undef toascii

/*
 * Use macros to create the is* functions.
 */
#define	ISMAC(n, m)	int n(int c) { return ((_ctype_+1)[c]&(m)); }

ISMAC(isalnum, _U|_L|_N)
ISMAC(isalpha, _U|_L)
ISMAC(iscntrl, _C)
ISMAC(isdigit, _N)
ISMAC(isgraph, _P|_U|_L|_N)
ISMAC(islower, _L)
ISMAC(isprint, _P|_U|_L|_N|_B)
ISMAC(ispunct, _P)
ISMAC(isspace, _S)
ISMAC(isupper, _U)
ISMAC(isxdigit, _N|_X)

int
isascii(int c)
{
	return ((unsigned)(c)<=0177);
}

int
toascii(int c)
{
	return (c & 0177);
}

