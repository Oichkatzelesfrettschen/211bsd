/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)time_.c	5.2	(2.11BSD) 2026/7/21
 */

/*
 * return the current time as an integer
 *
 * calling sequence:
 *	integer time
 *	i = time()
 * where:
 *	i will receive the current GMT in seconds.
 */

#include <time.h>

time_t time_()
{
	return(time((time_t *)0));
}
