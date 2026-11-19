/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)nice.c	5.3 (2.11BSD) 2026/5/7";
#endif

#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>

main(argc, argv)
	int argc;
	char *argv[];
{
	register int nicarg = 10;

	if (argc > 1 && argv[1][0] == '-') {
		nicarg = atoi(&argv[1][1]);
		argc--, argv++;
	}
	if (argc < 2) {
		fputs("usage: nice [ -n ] command\n", stderr);
		exit(1);
	}
	if (setpriority(PRIO_PROCESS, 0, 
	    getpriority(PRIO_PROCESS, 0) + nicarg) < 0) {
		perror("setpriority");
		exit(1);
	}
	execvp(argv[1], &argv[1]);
	perror(argv[1]);
	exit(1);
}
