/*
	Shar puts readable text files together in a package
	from which they are extracted with /bin/sh and friends.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#if	!defined(lint) && defined(DOSCCS)
static	char	sccsid[] = "@(#)shar.c 2.1 (2.11BSD) 2026/1/29";
#endif

/* GLOBALS */
int 	Lastchar;   /* the last character printed */
int 	Ctrlcount;  /* how many bad control characters are in file */

extern int isdir(char *);

/* COMMANDS */
#define	EXTRACT "#! /bin/sh"          /* magic exec string at shar file start */
#define	PATH    "/bin:/usr/bin:$PATH" /* search path for programs */
#define	CAT     "cat";                /* /bin/cat */
#define	SED     "sed 's/^%s//'"       /* /bin/sed removes Prefix from lines */
#define	MKDIR   "mkdir"               /* make a new dirctory */
#define	CHMOD   "chmod"               /* change file mode */
#define	CHDIR   "cd"                  /* change current directory */
#define	TEST    "test"                /* /bin/test files */
#define	WC_C    "wc -c <"             /* counts chars in file */
#define	ECHO    "echo shar:"          /* echo a message to extractor */
#define	DECODE  "uudecode"            /* used to decode uuencoded files */

/*FUNCTION main: traverse files to make archive to standard output */
main(argc, argv) char **argv;	
	{
	int 	shar();
	int	home;

	home = open(".", O_RDONLY);
	if	(home < 0)
		{
		fprintf(stderr, "open(.) error: %s\n", strerror(errno));
		exit(1);
		}

	if	((optind = initial(argc, argv)) < 0)
		exit(1);

	if	(header(argc, argv, optind))
		exit(1);

	for	(; optind < argc; optind++)
		{
		traverse(argv[optind], shar);
		fchdir(home);
/*
 * after processing a relative path (./a/b/c/d) it is necessary to go back 
 * to the "home directory" because sh will have  done "cd .." back to
 * next to last component (c).  Any future relative files will be looked for
 * in 'c' and not from the starting directory.  System patches are usually
 * generated relative to /.  Installing ./usr/src/bin/mkdir via a shar file
 * would leave 'sh' in ./usr/src/bin instead of /
 *
 * We do not check if the argument is relative or not. This may cause an
 * extra but harmless "chdir $STARTDIR" to be output.
 */
		if	(isdir(argv[optind]))
			printf("%s $STARTDIR\n", CHDIR);
		}

	footer();
	exit(0);
	}

/* OPTIONS */
int	Verbose = 0;       /* provide append/extract feedback */
int	Basename = 0;      /* extract into basenames */
int	Count = 0;         /* count characters to check transfer */
int	Overcheck = 1;      /* check overwriting */
int	Uucode = 0;        /* uuencode the file */
char	*Delim = "SHAR_EOF";   /* put after each file */
char	Filter[100] = CAT;     /* used to extract archived files */
char	*Prefix = NULL;        /* line prefix to avoid funny chars */
int	Modeset = 0;       /* set exact modes on extraction */

/*FUNCTION: initial: do option parsing and any setup */
int /* returns the index of the first operand file */
initial (argc, argv) char **argv;
	{
	int 	errflg = 0;
	extern	int 	optind;
	extern	char	*optarg;
	int 	C;
	char	*optstring = "abcmsuvp:d:";
	char	*usage = "[-abcmsuv] [-p prefix] [-d delim] files > archive";

	while ((C = getopt (argc, argv, optstring)) != EOF)
		switch (C)
			{
			case 'u': Uucode = 1; break;
			case 'b': Basename = 1; break;
			case 'c': Count = 1; break;
			case 'd': Delim = optarg; break;
			case 'm': Modeset = 1; break;
			case 's': /* silent running */
				Overcheck = 0;
				Verbose = 0;
				Count = 0;
				Prefix = NULL;
				break;
			case 'a': /* all the options */
				Verbose = 1;
				Count = 1;
				Basename = 1;
				/* fall through to set prefix */
				optarg = "	X";
				/* FALLTHROUGH */
			case 'p': (void) sprintf (Filter, SED, Prefix = optarg); break;
			case 'v': Verbose = 1; break;
			default: errflg++;
			}

	if (errflg || optind == argc)
		{
		if (optind == argc)
			fprintf (stderr, "shar: No input files\n");
		fprintf (stderr, "Usage: shar %s\n", usage);
		return (-1);
		}

	return (optind);
	}

/*FUNCTION header: print header for archive */
header (argc, argv, optind)
char	**argv;
	{
	int 	i;
	int	problems = 0;
	long	clock;
	char	*NAME = getenv ("NAME");
	char	*ORG = getenv ("ORGANIZATION");

	for (i = optind; i < argc; i++)
		if (access (argv[i], 4)) /* check read permission */
			{
			fprintf (stderr, "shar: Can't read '%s'\n", argv[i]);
			problems++;
			}

	if (problems)
		return(1);

	printf ("%s\n", EXTRACT);
	printf ("# This is a shell archive, meaning:\n");
	printf ("# 1. Remove everything above the %s line.\n", EXTRACT);
	printf ("# 2. Save the resulting text in a file.\n");
	printf ("# 3. Execute the file with /bin/sh (not csh) to create:\n");
	for (i = optind; i < argc; i++)
		printf ("#\t%s\n", argv[i]);
	(void) time (&clock);
	printf ("# This archive created: %s", ctime (&clock));
	if (NAME)
		printf ("# By:\t%s (%s)\n", NAME, ORG ? ORG : "");
	printf ("export PATH; PATH=%s\n", PATH);
	printf ("export STARTDIR; STARTDIR=`pwd`\n");
	return(0);
	}

/*FUNCTION footer: print end of shell archive */
footer ()
	{
	printf ("exit 0\n");
	printf ("#\tEnd of shell archive\n");
	}

/* uuencode options available to send cntrl and non-ascii chars */
/* really, this is getting to be too much like cpio or tar */

/* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) (((c) & 077) + ' ')

/*FUNCTION uuarchive: simulate uuencode to send files */
int
uuarchive (input, protection, output)
	char	*input;
	int 	protection;
	char	*output;
	{
	FILE	*in;
	if (in = fopen (input, "r"))
		{
		printf ("%s << \\%s\n", DECODE, Delim);
		printf ("begin %o %s\n", protection, output);
		uuencode (in, stdout);
		printf ("end\n");
		fclose (in);
		return(0);
		}
	return(1);
	}

uuencode (in, out)
FILE *in, *out;
	{
	char	buf[80];
	int 	i, n;
	for (;;)
		{
		n = fread (buf, 1, 45, in);
		putc (ENC(n), out);
		for (i = 0; i < n; i += 3)
			outdec (&buf[i], out);
		putc ('\n', out);
		if (n <= 0)
			break;
		}
	}

/* output one group of 3 bytes, pointed at by p, on file ioptr */
outdec (p, ioptr)
char	*p;
FILE	*ioptr;
	{
	int c1, c2, c3, c4;
	c1 = *p >> 2;
	c2 = (*p << 4) & 060 | (p[1] >> 4) & 017;
	c3 = (p[1] << 2) & 074 | (p[2] >> 6) & 03;
	c4 = p[2] & 077;
	putc (ENC(c1), ioptr);
	putc (ENC(c2), ioptr);
	putc (ENC(c3), ioptr);
	putc (ENC(c4), ioptr);
	}

/*FUNCTION archive: make archive of input file to be extracted to output */
archive (input, output)
char	*input, *output;
	{
	char	buf[BUFSIZ];
	FILE	*ioptr;

	if (ioptr = fopen (input, "r"))
		{
		Ctrlcount = 0;    /* no bad control characters so far */
		Lastchar = '\n';  /* simulate line start */

		printf ("%s << \\%s > '%s'\n", Filter, Delim, output);

		if (Prefix)
			{
			while (fgets (buf, BUFSIZ, ioptr))
				{
				outline (Prefix);
				outline (buf);
				}
			}
		else copyout (ioptr);

		if (Lastchar != '\n') /* incomplete last line */
			putchar ('\n');   /* Delim MUST begin new line! */

		if (Count == 1 && Lastchar != '\n')
			printf ("%s \"a missing newline was added to '%s'\"\n", ECHO, input);
		if (Count == 1 && Ctrlcount)
			printf ("%s \"%d control character%s may be missing from '%s'\"\n",
				ECHO, Ctrlcount, Ctrlcount > 1 ? "s" : "", input);

		(void) fclose (ioptr);
		return(0);
		}
	else
		{
		fprintf (stderr, "shar: Can't open '%s'\n", input);
		return(1);
		}
	}

/*FUNCTION copyout: copy ioptr to stdout */
/*
	Copyout copies its ioptr almost as fast as possible
	except that it has to keep track of the last character
	printed.  If the last character is not a newline, then
	shar has to add one so that the end of file delimiter
	is recognized by the shell.  This checking costs about
	a 10% difference in user time.  Otherwise, it is about
	as fast as cat.  It also might count control characters.
*/
#define	badctrl(c) (iscntrl (c) && !isspace (c))
copyout (ioptr)
register	FILE	*ioptr;
	{
	register	int 	C;
	register	int 	L;
	register	count;

	count = Count;

	while ((C = getc (ioptr)) != EOF)
		{
		if (count == 1 && badctrl (C))
			Ctrlcount++;
		L = putchar (C);
		}

	Lastchar = L;
	}

/*FUNCTION outline: output a line, recoring last character */
outline (s)
register	char	*s;
	{
	if (*s)
		{
		while (*s)
			{
			if (Count == 1 && badctrl (*s)) Ctrlcount++;
			putchar (*s++);
			}
		Lastchar = *(s-1);
		}
	}

/*FUNCTION shar: main archiving routine passed to directory traverser */
shar (file, type, pos)
char	*file;     /* file or directory to be processed */
int 	type;      /* either 'f' for file or 'd' for directory */
int 	pos;       /* 0 going in to a file or dir, 1 going out */
	{
	struct	stat	statbuf;
	char	*basefile = file;
	int 	protection;

	if (!strcmp (file, "."))
		return;

	if (stat (file, &statbuf))
		statbuf.st_size = 0;
	else
		protection = statbuf.st_mode & 07777;

	if (Basename == 1)
		{
		while (*basefile) basefile++; /* go to end of name */
		while (basefile > file && *(basefile-1) != '/')
			basefile--;
		}

	if (pos == 0) /* before the file starts */
		{
		beginfile (basefile, type, statbuf.st_size, protection);
		if (type == 'f')
			{
			if (Uucode)
				{
				if (uuarchive (file, protection, basefile) != 0)
					exit(1);
				}
			else
				if (archive (file, basefile) != 0)
					exit(1);
			}
		}
	else /* pos == 1, after the file is archived */
		endfile (basefile, type, statbuf.st_size, protection);
	}

/*FUNCTION beginfile: do activities before packing up a file */
beginfile (basefile, type, size, protection)
char	*basefile;  /* name of the target file */
int 	type;       /* either 'd' for directory, or 'f' for file */
long 	size;       /* size of the file */
int 	protection; /* chmod protection bits */
	{
	if (type == 'd')
		{
		printf ("if %s ! -d '%s'\n", TEST, basefile);
		printf ("then\n");
		if (Verbose == 1)
			printf ("	%s \"creating directory '%s'\"\n", ECHO, basefile);
		printf ("	%s '%s'\n", MKDIR, basefile);
		printf ("fi\n");
		if (Verbose == 1)
			printf ("%s \"entering directory '%s'\"\n", ECHO, basefile);
		printf ("%s '%s'\n", CHDIR, basefile);
		}
	else /* type == 'f' */
		{
		if (Verbose == 1)
			printf ("%s \"extracting '%s'\" '(%ld character%s)'\n",
				ECHO, basefile, size, size == 1 ? "" : "s");

		if (Overcheck == 1)
			{
			printf ("if %s -f '%s'\n", TEST, basefile);
			printf ("then\n");
			printf ("	%s \"will not over-write existing file '%s'\"\n",
				ECHO, basefile);
			printf ("else\n");
			}

		}
	}

/*FUNCTION endfile: do activities after packing up a file */
endfile (basefile, type, size, protection)
char	*basefile;  /* name of the target file */
int 	type;       /* either 'd' for directory, or 'f' for file */
long 	size;       /* size of the file */
int 	protection; /* chmod protection bits */
	{
	if (type == 'd')
		{
		if (Modeset == 1)
			printf ("%s %o .\n", CHMOD, protection);
		if (Verbose == 1)
			printf ("%s \"done with directory '%s'\"\n", ECHO, basefile);
		printf ("%s ..\n", CHDIR);
		}
	else /* type == 'f' (plain file) */
		{
		printf ("%s\n", Delim);
		if (Count == 1)
			{
			printf ("if %s %ld -ne \"`%s '%s'`\"\n", TEST, size, WC_C, basefile);
			printf ("then\n");
			printf ("	%s \"error transmitting '%s'\" ", ECHO, basefile);
			printf ("'(should have been %ld character%s)'\n",
				size, size == 1 ? "" : "s");
			printf ("fi\n");
			}
		if (Uucode == 0) /* might have to chmod by hand */
			{
			if (Modeset == 1) /* set all protection bits (W McKeeman) */
				printf ("%s %o '%s'\n",
						CHMOD, protection, basefile);
			else if (protection & 0111) /* executable -> chmod +x */
				printf ("%s +x '%s'\n", CHMOD, basefile);
			}
		if (Overcheck == 1)
			printf ("fi\n");
		}
	}
