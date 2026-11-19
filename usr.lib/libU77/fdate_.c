/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)fdate_.c	5.2	(2.11BSD) 2026/7/30
 */

/*
 * Return date and time in an ASCII string.
 *
 * calling sequence:
 *	character*24 string
 * 	call fdate(string)
 * where:
 *	the 24 character string will be filled with the date & time in
 *	ascii form as described under ctime(3).
 *	No 'newline' or NULL will be included.
 */

#include <time.h>

fdate_(s, strlen)
char *s; long strlen;
{
	char *c;
	time_t t;

	t = time(0);
	c = ctime(&t);
	c[24] = '\0';
	b_char(c, s, strlen);
}
