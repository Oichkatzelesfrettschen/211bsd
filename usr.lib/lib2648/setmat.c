/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)setmat.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * setmat: set the value in m[r, c] to nval.
 */

#include "bit.h"

setmat(m, rows, cols, r, c, nval)
bitmat m;
int rows, cols, r, c, nval;
{
	register int offset, thisbit;

	if (r<0 || c<0 || r>=rows || c>=cols) {
#ifdef TRACE
		if (trace)
			fprintf(trace, "setmat range error: (%d, %d) <- %d in a (%d, %d) matrix %x\n", r, c, nval, rows, cols, m);
#endif

		return;
	}
	offset = r*((cols+7)>>3) + (c>>3);
	thisbit = 0x80 >> (c&7);
	if (nval)
		m[offset] |= thisbit;
	else
		m[offset] &= ~thisbit;
}
