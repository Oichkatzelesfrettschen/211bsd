#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)lastpart.c	5.5 (2.11BSD) 2026/5/9";
#endif

#include "uucp.h"

/*
 *	find last part of file name
 *
 *	return - pointer to last part
 */

char *
lastpart(file)
register char *file;
{
	register char *c;

	c = rindex(file, '/');
	if (c++)
		return c;
	else
		return file;
}
