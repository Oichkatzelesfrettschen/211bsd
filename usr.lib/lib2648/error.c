/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)error.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * error: default handling of errors.
 */

error(msg)
char *msg;
{
	message(msg);
	/* Maybe it would be nice to longjmp somewhere here */
}
