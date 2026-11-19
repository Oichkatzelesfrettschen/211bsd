/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)f77_abort.c	5.2.2 (2.11BSD)	2026/7/30
 *
 *	all f77 aborts eventually call f77_abort.
 *
 *	f77_abort prints a message and if a coredump is NOT requested cleans 
 *	up open files.
 */

#include <signal.h>
#include "fio.h"

f77_abort(nargs, err_val, act_core)
{
	int core_dump;

	signal(SIGILL, SIG_DFL);
	(void)sigsetmask(0L);		/* don't block */

	/*
	 * See if we want a core dump.  The line previously read
	 *         core_dump = (nargs != 2) || act_core;
	 * which is wrong.  Only signal related aborts use nargs of 2 with
	 * act_core present.
	*/

	core_dump = (nargs == 2) && act_core;

	if	(!core_dump)
		fprintf(units[STDERR].ufd,"*** Execution terminated\n");

	/*
	 * Don't call f_exit() here because abort() wants to print messages
	 * to stderr from the signal handler sigdie().  f_exit() reinits 
	 * the units[] table and fprintf would use a NULL file pointer causing 
	 * another signal which would reference the NULL stderr pointer and
	 * trigger another signal, ad nauseum.
	*/
	if	(nargs)		/* nargs == 1 err_val is the error code.
				 * nargs == 2 err_val is the signal number.
				*/
		errno = err_val;
	else 
		errno = -2;	/* nargs == 0 prior errno is meaningless,
				 * set errno to undefined value.
				*/
	/*
	 * Don't need to call _cleanup() because exit() will do that for us
	 * and if we're dumping core we don't care.
	*/
	if	(core_dump)
		abort();
	f_exit();
	exit(errno);
}
