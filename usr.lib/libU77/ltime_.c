/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)ltime_.c	5.2 (2.11BSD) 2026/7/30
 */

#include <time.h>

/*
 * return broken down time
 *
 * calling sequence:
 *	integer time, t[9]
 *	call ltime(time, t)
 * where:
 *	time is a  system time. (see time(3F))
 *	t will receive the broken down time corrected for local timezone.
 *	(see ctime(3))
 */

ltime_(clock, t)
	time_t *clock, *t;
{
	register int i;
	register int *l;

	l = (int *)localtime(clock);
	for (i=0; i<9; i++)
		*t++ = *l++;
}
