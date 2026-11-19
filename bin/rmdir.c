/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)rmdir.c	5.2 (2.11BSD) 2026/5/7";
#endif

/*
 * Remove directory
 */
#include <stdio.h>
#include <err.h>

main(argc,argv)
	int argc;
	char **argv;
{
	register int errors = 0;

	if (argc < 2)
		errx(1, "usage: %s directory ...", argv[0]);
	while (--argc)
		if (rmdir(*++argv) < 0) {
			warn("%s", *argv);
			errors++;
		}
	exit(errors != 0);
}
