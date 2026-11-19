/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)yes.c	5.2 (2.11BSD) 2026/5/8";
#endif

main(argc, argv)
char **argv;
{
	for (;;)
		printf("%s\n", argc>1? argv[1]: "y");
}
