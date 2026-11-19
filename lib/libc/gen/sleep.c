/*
 * Program: sleep.c
 * Copyright: 1997, sms
 * Author: Steven M. Schultz
 *
 * Version   Date	Modification
 *     1.0  1997/9/25	1. Initial release.
 *     2.0  2026/9/19   2. select() can only sleep for 32767 ticks (~9min with
 *			   60Hz clock, 13min with 50Hz clock).  Loop unless
 *			   the select() was interrupted by a signal.
 *			   Change the return value and argument to be int 
 *			   and NOT unsigned so that "if (seconds < 0)" works.
*/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>

/*
 * This implements the sleep(3) function using only 3 system calls instead of 
 * the 9 that the old implementation required.  Also this version avoids using
 * signals (with the attendant system overhead) and returns the amount of 
 * time left unslept if an interrupt occurs.
 *
 * The error status of gettimeofday is not checked because if that fails the
 * program has scrambled the stack so badly that a sleep() failure is the 
 * smallest problem the program has.
*/

u_int
sleep(seconds)
	int seconds;
	{
	int	intr = 0;
	struct	timeval f, s;

	while	(seconds && intr == 0)
		{
		gettimeofday(&f, NULL);
		s.tv_sec = seconds;
		s.tv_usec = 0;
/*
 * with no file descriptors specified select() can only return an error
 * if the 'seconds' is bad (EINVAL) or if a signal occurred (EINTR).  If the
 * latter set 'intr' to break out of the loop after computing the number of
 * seconds remaining.
*/
		if	(select(0, NULL, NULL, NULL, &s) < 0 && (errno == EINTR))
			intr = 1;
		gettimeofday(&s, NULL);
		seconds -= (s.tv_sec - f.tv_sec);
/*
 * This can happen if the system time gets set back while we're in the 
 * select() call OR if we slept a little longer than expected due to system
 * activity.  Return 0 instead of a bogus number OR because we're done.
 * Otherwise there are seconds remaing so continue the loop.
*/
		if	(seconds < 0)
			seconds = 0;
		}
	return(seconds);
	}
