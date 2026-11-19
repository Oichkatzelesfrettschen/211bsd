/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)system_.c	5.2	(2.11BSD) 2026/9/21
 */

/*
 * execute a unix command
 *
 * calling sequence:
 *	iexit = system(command)
 * where:
 *	iexit will return the exit status of the command
 *	command is a character string containing the command to be executed
 */

#include	"../libI77/fiodefs.h"
#include	"../libI77/f_errno.h"
#include <strings.h>
#include <stdlib.h>
#include <sys/param.h>
#ifndef	NCARGS
#define NCARGS	256
#endif

 
long system_(s, n)
char *s;
long n;
{
	char buf[NCARGS - 50];
	long i;

	if (n >= sizeof buf)
		return(-(long)(errno=F_ERARG));
	for (i = 0; i < MXUNIT; i++)
		flush_(&i);
	g_char(s, n, buf);
	return((long)system(buf));
}

/*
 * this is a sane version of the libc/stdio routine.
 */

#include	<signal.h>

system(s)
char *s;
{
	sig_t istat, qstat;
	int status, pid, w;
	register char	*shname, *shell;

	if ((shell = getenv("SHELL")) == NULL)
		shell = "/bin/sh";

	if (shname = rindex(shell, '/'))
		shname++;
	else
		shname = shell;

	if ((pid = fork()) == 0) {
		execl(shell, shname, "-c", s, 0);
		_exit(127);
	}
	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	while ((w = wait(&status)) != pid && w != -1)
		;
	if (w == -1)
		status = -1;
	signal(SIGINT, istat);
	signal(SIGQUIT, qstat);
	return(status);
}
