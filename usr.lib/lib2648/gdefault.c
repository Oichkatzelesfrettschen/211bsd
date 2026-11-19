/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)gdefault.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * reset terminal to default graphics state
 */

#include "2648.h"

gdefault()
{
	escseq(ESCM);
	outstr("r");
}
