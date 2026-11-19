/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)circle.c     5.3 (2.11BSD) 2026/5/11";
#endif


#include "aed.h"

/*---------------------------------------------------------
 *	Circle draws a circle.
 *
 *	Results:	None.
 *
 *	Side Effects:
 *	A circle of radius r is drawn at (x,y).
 *---------------------------------------------------------
 */
circle(x, y, r)
int x, y, r;
{
    char buf[3];
    setcolor("01");
    putc('Q', stdout);
    outxy20(x, y);
    putc('O', stdout);
    chex((r*scale)>>12, buf, 2);
    fputs(buf, stdout);
    (void) fflush(stdout);
}
