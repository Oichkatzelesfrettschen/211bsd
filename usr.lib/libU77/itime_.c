/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)itime_.c	5.2	(2.11BSD) 2026/7/21
 */

/*
 * return the current time in numerical form
 *
 * calling sequence:
 *	integer iarray(3)
 *	call itime(iarray)
 * where:
 *	iarray will receive the current time; hour, min, sec.
 */

#include <time.h>
#include <sys/time.h>

itime_(iar)
struct { long ihr; long imin; long isec; } *iar;
{
	struct tm *lclt;
	time_t t;

	t = time(0);
	lclt = localtime(&t);
	iar->ihr = lclt->tm_hour;
	iar->imin = lclt->tm_min;
	iar->isec = lclt->tm_sec;
}
