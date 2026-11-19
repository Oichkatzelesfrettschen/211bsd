/*
 * Program: usleep.c
 * Copyright: 1997, sms
 * Author: Steven M. Schultz
 *
 * Version   Date	Modification
 *     1.0  1997/9/26	1. Initial release.
 *     1.1  2026/9/17   2. Fix Program: name
 *			   Document limitation of select().
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

/*
 * This implements the usleep(3) function using only 1 system call (select)
 * instead of the 9 that the old implementation required.  Also this version 
 * avoids using signals (with the attendant system overhead).
 *
 * Nothing is returned and if less than ~20000 microseconds is specified the
 * select will return without any delay at all.
 *
 * select() can sleep for a maximum of 32767 clock ticks.  This is about
 * 9 minutes with a 60Hz clock and 13 minutes with a 50Hz clock.  sleep(3)
 * uses a loop for longer sleeps.
*/

void
usleep(micros)
	long micros;
	{
	struct timeval s;

	if	(micros > 0)
		{
		s.tv_sec = micros / 1000000L;
		s.tv_usec = micros % 1000000L;
		select(0, NULL, NULL, NULL, &s);
		}
	}
