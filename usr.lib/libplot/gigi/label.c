/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     5.2 (2.11BSD) 2026/5/11";
#endif

#include "gigi.h"

label(s)
char *s;
{
	printf("T(S0 H2 D0 I0) \"");
	for(;*s!='\0';s++) {
		putchar(*s);
		if (*s == '"') putchar('"');
	}
	putchar('"');
}
