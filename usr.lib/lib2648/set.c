/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)set.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * Routines to set line type.
 */

#include "2648.h"

setxor()
{
	_supsmode = MX;
}

setclear()
{
	_supsmode = _video==INVERSE ? MS : MC;
}

setset()
{
	_supsmode = _video==INVERSE ? MC : MS;
}
