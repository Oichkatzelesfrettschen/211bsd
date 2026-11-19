/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     5.2 (2.11BSD) 2026/5/11";
#endif

#include "hp2648.h"

label(s)
char *s;
{
	handshake();
	putchar(ESC);
	putchar(GRAPHIC);
	putchar('l');
	for(;*s!='\0';s++)
		putchar(*s);
	putchar(ESC);
	putchar(GRAPHIC);
	putchar('d');
	putchar('T');
	handshake();
	putchar(ESC);
	putchar(GRAPHIC);
	putchar(PLOT);
	putchar(BINARY);
	buffcount = 4;
}
