#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)setbrk.c	4.3 (2.11BSD) 2026/5/11";
#endif

/*
 *	UNIX shell
 *
 *	S. R. Bourne
 *	Bell Telephone Laboratories
 */

#include	"defs.h"

setbrk(incr)
{
	REG BYTPTR	a=sbrk(incr);
	brkend=a+incr;
	return(a);
}
