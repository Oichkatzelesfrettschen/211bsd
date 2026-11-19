/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)what.c	5.2 (2.11BSD) 2026/5/8";
#endif

#include <stdio.h>

/*
 * what
 */

char	*infile = "Standard input";

main(argc, argv)
	int argc;
	char *argv[];
{

	argc--, argv++;
	do {
		if (argc > 0) {
			if (freopen(argv[0], "r", stdin) == NULL) {
				perror(argv[0]);
				exit(1);
			}
			infile = argv[0];
			printf("%s\n", infile);
			argc--, argv++;
		}
		fseek(stdin, (long) 0, 0);
		find();
	} while (argc > 0);
	exit(0);
}

find()
{
	static char buf[BUFSIZ];
	register char *cp;
	register int c, cc;
	register char *pat;

contin:
	while ((c = getchar()) != EOF)
		if (c == '@') {
			for (pat = "(#)"; *pat; pat++)
				if ((c = getchar()) != *pat)
					goto contin;
			putchar('\t');
			while ((c = getchar()) != EOF && c && c != '"' &&
			    c != '>' && c != '\n')
				putchar(c);
			putchar('\n');
		}
}
