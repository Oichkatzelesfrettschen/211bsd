/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)message.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * message: print str on the screen in the message area.
 */

#include "2648.h"

message(str)
char *str;
{
	dispmsg(str, 4, 4, 100);
}
