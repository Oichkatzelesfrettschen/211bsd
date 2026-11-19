/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)libzer.c	5.2 (2.11BSD) 2026/5/10";
#endif

#include <stdio.h>

yyerror(s)
	char *s;
{

	fprintf(stderr, "%s\n", s);
}
