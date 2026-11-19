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

static char sccsid[] = "@(#)man.c	5.17.2 (2.11BSD) 2026/4/1";
#endif

#include <sys/param.h>
#include <sys/file.h>
#include <sys/dir.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#define	DEF_PAGER	"/usr/ucb/more -s"
#define	DEF_PATH	"/usr/man:/usr/new/man:/usr/local/man"

static char	*command,		/* command buffer */
		*defpath,		/* default search path */
		*manpath,		/* current search path */
		*pager,			/* requested pager */
		how;			/* how to display */

#define	ALL	0x1			/* show all man pages */
#define	CAT	0x2			/* copy file to stdout */
#define	WHERE	0x4			/* just tell me where */

main(argc, argv)
	int argc;
	register char **argv;
{
	int ch;

	while ((ch = getopt(argc, argv, "-M:P:afkw")) != EOF)
		switch((char)ch) {
		case '-':
			how |= CAT;
			break;
		case 'M':
		case 'P':		/* backward compatibility */
			defpath = optarg;
			break;
		case 'a':
			how |= ALL;
			break;
		/*
		 * "man -f" and "man -k" are backward contemptible,
		 * undocumented ways of calling whatis(1) and apropos(1).
		 */
		case 'f':
			jump(argv, "-f", "whatis");
		case 'k':
			jump(argv, "-k", "apropos");
		/*
		 * Deliberately undocumented; really only useful when
		 * you're moving man pages around.  Not worth adding.
		 */
		case 'w':
			how |= WHERE | ALL;
			break;
		case '?':
		default:
			usage();
		}
	argv += optind;

	if (!*argv)
		usage();

	if (!(how & CAT))
		if (!isatty(1))
			how |= CAT;
		else if (pager = getenv("PAGER")) {
			register char *p;

			/*
			 * if the user uses "more", we make it "more -s"
			 * watch out for PAGER = "mypager /usr/ucb/more"
			 */
			for (p = pager; *p && !isspace(*p); ++p);
			for (; p > pager && *p != '/'; --p);
			if (p != pager)
				++p;
			/* make sure it's "more", not "morex" */
			if (!strncmp(p, "more", 4) && (!p[4] || isspace(p[4]))){
				char *opager = pager;
				/*
				 * allocate space to add the "-s"
				 */
				if (!(pager = malloc((u_int)(strlen(opager) 
				    + sizeof("-s") + 1))))
				   errx(1, "man: out of space.");
				(void)sprintf(pager, "%s %s", opager, "-s");
			}
		}
		else
			pager = DEF_PAGER;
	if (!defpath && !(defpath = getenv("MANPATH")))
		defpath = DEF_PATH;
	man(argv);
	/* use system(3) in case someone's pager is "pager arg1 arg2" */
	if	(command)
		(void)system(command);
	exit(0);
}

typedef struct {
	char	*name, *msg;
} MANDIR;
static MANDIR	list1[] = 
	{		/* section one list */
	"cat1", "1st",		"cat8", "8th",		"cat6", "6th",
	NULL, NULL,
	};
static MANDIR	list2[] =
	{		/* rest of the list */
	"cat2", "2nd",		"cat3", "3rd",		"cat4", "4th",
	"cat5", "5th", 		"cat7", "7th",		"cat3f", "3rd (F)",
	NULL, NULL,
	};
static	MANDIR	list3[2];		/* single section */

static
man(argv)
	char **argv;
{
	register char *p;
	MANDIR *section, *getsect();
	int res;

	for (; *argv; ++argv) {
		manpath = defpath;
		section = NULL;
		switch(**argv) {
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8':
			if (section = getsect(*argv))
				++argv;
		}

		if (*argv) {
			if (section)
				res = manual(section, *argv);
			else {
				res = manual(list1, *argv);
				if (!res || (how & ALL))
					res += manual(list2, *argv);
			}
			if (res || how&WHERE)
				continue;
		}

		fputs("man: ", stderr);
		if (*argv)
			fprintf(stderr, "no entry for %s in the ", *argv);
		else
			fputs("what do you want from the ", stderr);
		if (section)
			fprintf(stderr, "%s section of the ", section->msg);
		if (*argv)
			fputs("manual.\n", stderr);
		else
			fputs("manual?\n", stderr);
		exit(1);
	}
}

/*
 * manual --
 *	given a directory list and a file name find a file that
 *	matches; check ${directory}/${dir}/{file name}
 */
static
manual(section, name)
	MANDIR *section;
	char *name;
{
	register char *beg, *end;
	register MANDIR *dp;
	register int res;
	char fname[MAXPATHLEN + 1], *index();

	if (strlen(name) > MAXNAMLEN-2)	/* leave room for the ".0" */
		name[MAXNAMLEN-2] = '\0';
	for (beg = manpath, res = 0;; beg = end + 1) {
		if (end = index(beg, ':'))
			*end = '\0';
		for (dp = section; dp->name; ++dp) {
			(void)sprintf(fname, "%s/%s/%s.0", beg, dp->name, name);
			if (access(fname, R_OK))
			   continue;
			if (how & WHERE)
				printf("man: found in %s.\n", fname);
			else if (how & CAT)
				cat(fname);
			else
				add(fname);
			if (!(how & ALL))
				return(1);
			res = 1;
		}
		if (!end)
			return(res);
		*end = ':';
	}
}

/*
 * cat --
 *	cat out the file
 */
static
cat(fname)
	char *fname;
{
	register int fd, n;
	char buf[BUFSIZ];

	if	(!(fd = open(fname, O_RDONLY, 0)))
		err(1, "open");
	while	((n = read(fd, buf, sizeof(buf))) > 0)
		if	(write(1, buf, n) != n)
			err(1, "write");
	if	(n == -1)
		err(1, "read");
	(void)close(fd);
}

/*
 * add --
 *	add a file name to the list for future paging
 */
static
add(fname)
	char *fname;
{
	static u_int buflen;
	static int len;
	static char *cp;
	int flen;

	if (!command) {
		if	(!(command = malloc(buflen = 1024)))
			errx(1, "out of space.", stderr);
		len = strlen(strcpy(command, pager));
		cp = command + len;
	}
	flen = strlen(fname);
	if (len + flen + 2 > buflen) {		/* +2 == space, EOS */
		if (!(command = realloc(command, buflen += 1024)))
			errx(1, "man: out of space.");
		cp = command + len;
	}
	*cp++ = ' ';
	len += flen + 1;			/* +1 = space */
	(void)strcpy(cp, fname);
	cp += flen;
}

/*
 * getsect --
 *	return a point to the section structure for a particular suffix
 */
static MANDIR *
getsect(s)
	register char *s;
{
	switch(*s++) {
	case '1':
		if (!*s)
			return(list1);
		break;
	case '2':
		if (!*s) {
			list3[0] = list2[0];
			return(list3);
		}
		break;
	/* sect. 3 requests are for either section 3, or section 3[fF]. */
	case '3':
		if (!*s) {
			list3[0] = list2[1];
			return(list3);
		}
		else if ((*s == 'f'  || *s == 'F') && !*++s) {
			list3[0] = list2[5];
			return(list3);
		}
		break;
	case '4':
		if (!*s) {
			list3[0] = list2[2];
			return(list3);
		}
		break;
	case '5':
		if (!*s) {
			list3[0] = list2[3];
			return(list3);
		}
		break;
	case '6':
		if (!*s) {
			list3[0] = list1[2];
			return(list3);
		}
		break;
	case '7':
		if (!*s) {
			list3[0] = list2[4];
			return(list3);
		}
		break;
	case '8':
		if (!*s) {
			list3[0] = list1[1];
			return(list3);
		}
	}
	return((MANDIR *)NULL);
}

/*
 * jump --
 *	strip out flag argument and jump
 */
static
jump(argv, flag, name)
	char **argv, *name;
	register char *flag;
{
	register char **arg;

	argv[0] = name;
	for (arg = argv + 1; *arg; ++arg)
		if (!strcmp(*arg, flag))
			break;
	for (; *arg; ++arg)
		arg[0] = arg[1];
	execvp(name, argv);
	errx(1, "%s: Command not found.", name);
}

/*
 * usage --
 *	print usage and die
 */
static
usage()
{
	errx(1, "usage: man [-] [-a] [-M path] [section] title ...");
}
