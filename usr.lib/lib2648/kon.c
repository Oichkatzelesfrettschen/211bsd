/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)kon.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * Turn on keypad, so it sends codes instead of doing them in local.
 */

#include "2648.h"

kon()
{
	escseq(NONE);
	outstr("\33&s1A");
}

koff()
{
	escseq(NONE);
	outstr("\33&s0A");
}
