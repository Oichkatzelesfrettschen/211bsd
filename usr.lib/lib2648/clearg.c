/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)clearg.c     5.2 (2.11BSD) 2026/5/11";
#endif

#include "2648.h"

clearg()
{
	sync();
	escseq(ESCD);
	outchar((_video==INVERSE) ? 'b' : 'a');
}
