/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)linemd.c     5.2 (2.11BSD) 2026/5/11";
#endif

linemd_(s, len)
register char *s;
long len;
{
	char buf[256];
	register char *cp, *cend;

	cp = buf;
	cend = cp + (len < 255 ? len : 255 );
	while ( cp < cend  && *s != ' ') 
		*cp++ = *s++;
	*cp = 0;
	linemod( buf );
}
