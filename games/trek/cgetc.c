/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)cgetc.c     5.2 (2.11BSD) 2026/5/12";
#endif

# include	<stdio.h>

char	cgetc(i)
int	i;
{
	return ( getchar() );
}
