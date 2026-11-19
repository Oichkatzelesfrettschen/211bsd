/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)movecurs.c     5.2 (2.11BSD) 2026/5/11";
#endif

#include "2648.h"

movecurs(x, y)
{
	char mes[20];

	if (x==_curx && y==_cury)
		return;
	sprintf(mes, "%d,%do", x, y);
	escseq(ESCD);
	outstr(mes);
	escseq(NONE);
	_curx = x;
	_cury = y;
}
