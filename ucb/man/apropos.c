/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1987 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)apropos.c	5.8 (2.11BSD) 2026/9/15";
#endif

#include <sys/param.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <err.h>

#define	DEF_PATH	"/usr/man:/usr/new/man:/usr/local/man"
#define	MAXLINELEN	1000			/* max line handled */
#define	WHATIS		"whatis"		/* database name */

extern char *__progname;

main(argc, argv)
	int argc;
	char **argv;
{
	register char *beg, *end, **C;
	int ch, foundman = 0, *found, isapropos;
	int a_match(), w_match(), (*match)();
	char *manpath = NULL, buf[MAXLINELEN + 1], fname[MAXPATHLEN + 1];
	char wbuf[MAXLINELEN + 1];
	char *cp;

	if (!strcmp(__progname, "apropos")) {
		isapropos = 1;
		match = a_match;
	}
	else {
		isapropos = 0;
		match = w_match;
	}
	while ((ch = getopt(argc, argv, "M:P:")) != EOF)
		switch((char)ch) {
			case 'M':
			case 'P':		/* backward contemptible */
				manpath = optarg;
				break;
			case '?':
			default:
				usage();
		}
	argv += optind;
	argc -= optind;
	if (argc < 1)
		usage();

	if (!(manpath = getenv("MANPATH")))
		manpath = DEF_PATH;

	if (!(found = (int *)malloc((u_int)(argc * sizeof(int)))))
		errx(1, "out of space");

	bzero((char *)found, argc * sizeof(int));

	if (isapropos)
		for (C = argv; *C; ++C)		/* convert to lower-case */
			lowstr(*C, *C);
	else for (C = argv; *C; ++C)		/* trim full paths */
		if (beg = rindex(*C, '/'))
			*C = beg + 1;

	for (beg = manpath; beg; beg = end) {	/* through path list */
		end = index(beg, ':');
		if (!end)
			(void)sprintf(fname, "%s/%s", beg, WHATIS);
		else {
			(void)sprintf(fname, "%.*s/%s", end - beg, beg, WHATIS);
			++end;
		}
		if (!freopen(fname, "r", stdin))
			continue;

						/* for each file found */
		for (foundman = 1; fgets(buf, sizeof buf, stdin);) {
			if (cp = index(buf, '\n')) *cp = '\0';
			if (isapropos)
				lowstr(buf, wbuf);
			else
				dashtrunc(buf, wbuf);
			for (C = argv; *C; ++C)
				if ((*match)(wbuf, *C)) {
					puts(buf);
					found[C - argv] = 1;

					/* only print line once */
					while (*++C)
						if ((*match)(wbuf, *C))
							found[C - argv] = 1;
					break;
				}
		}
	}
	if (!foundman)
		errx(1,"no %s file found in %s", WHATIS, manpath);

	for (C = argv; *C; ++C)
		if (!found[C - argv])
			printf("%s: %s\n", *C, isapropos ? "nothing appropriate" : "not found");
}

/*
 * a_match -- match for apropos; anywhere the string appears
 */
static
a_match(bp, str)
	register char *bp, *str;
{
	register int len;
	register char test;

	if (!*bp)
		return(0);
	/* backward compatible: everything matches empty string */
	if (!*str)
		return(1);
	for (test = *str++, len = strlen(str); *bp;)
		if (test == *bp++ && !strncmp(bp, str, len))
			return(1);
	return(0);
}

/*
 * w_match --
 *	match for whatis; looks for full word match
 */
static
w_match(bp, str)
	register char *bp, *str;
{
	register int len;
	register char *start;

	if (!*str || !*bp)
		return(0);
	for (len = strlen(str);;) {
		for (; *bp && !isdigit(*bp) && !isalpha(*bp); ++bp);
		if (!*bp)
			break;
		for (start = bp++; *bp && (isdigit(*bp) || isalpha(*bp)); ++bp);
		if (bp - start == len && !strncasecmp(start, str, len))
			return(1);
	}
	return(0);
}

/*
 * dashtrunc --
 *	truncate a string at " - "
 */
static
dashtrunc(from, to)
	register char *from, *to;
{
	do {
		if (from[0] == ' ' && from[1] == '-' && from[2] == ' ')
			break;
	} while (*to++ = *from++);
	*to = '\0';
}

/*
 * lowstr -- convert a string to lower case
 */
static
lowstr(from, to)
	register char *from, *to;
{
	do {
		*to++ = isupper(*from) ? tolower(*from) : *from;
	} while (*from++);
}

/*
 * usage -- print usage message and die
 */
static
usage()
{
	errx(1, "usage: [-M path] string ...");
}
