/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)move.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * move to (x, y).  Both the _pen and cursor are supposed to be moved.
 * We really just remember it for later, in case we move again.
 */

#include "2648.h"

move(x, y)
{
#ifdef TRACE
	if (trace)
		fprintf(trace, "\tmove(%d, %d), ", x, y);
#endif
	_supx = x;
	_supy = y;
}
