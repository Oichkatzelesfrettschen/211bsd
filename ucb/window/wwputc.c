#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)wwputc.c	3.5 (2.11BSD) 2026/5/9";
#endif

/*
 * Copyright (c) 1983 Regents of the University of California,
 * All rights reserved.  Redistribution permitted subject to
 * the terms of the Berkeley Software License Agreement.
 */

#include "ww.h"

wwputc(c, w)
char c;
struct ww *w;
{
	(void) wwwrite(w, &c, sizeof c);
}
