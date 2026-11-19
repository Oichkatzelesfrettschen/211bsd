/*
 * mkstr.c 6.0 (2.11BSD) 2026/7/13
 *
 * Inspired by the mkstr program.  Little remains of the original except the
 * name and function.
 * 
 * Changes from original mkstr.c are:
 *
 * 1) start search from the less frequent '"' instead of the much more common
 *    'e'.  This speeds up processing by attempting a string match only when
 *    the start of a string has been found.
 *
 * 2) remove the restriction of 'error(' being the wired in string.  Use '-e'
 *    to allow the user to specify what string to search for.  The '(' is not
 *    specified as it is added by this program.  IF -e is NOT given the string
 *    used is 'error(' as before.
 *
 * 3) allow more than one string to be searched for.  Multiple '-e str' options
 *    may be given.  If -e is used '-e error' must be specified explicitly. Up
 *    to 16 -e options may be used.
 *
 * 4) large string file support is requested with the -l option.  the  offset
 *    substituted for the string has 'L' appended, thus "error(9);"
 *    becomes "error(9L);".  See longoff comment below.
 *
 *  5) use fgetln() to process input a line at a time.
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

extern	char	*__progname;

	void    process(), inithash();
	int     fgetNUL(), match(), octdigit();
	char	*copystr(char *);
	off_t	hashit();

	FILE	*mesgread, *mesgwrite;

#define	MAX_DASHE 16

struct	estring
	{
	char	*str;
	int	len;
	};

	struct	estring dashe[MAX_DASHE + 1]; /* +1 for NULL */
	int	nume = 0;	/* number of strings in dashe[] */

	char	name[100], *strfile, *prefix;
	int	addon = 0;	/* '-' add to existing stringfile */
	int	debug = 0;	/* debug info but no file processing */

	/* 
	 * generate long offset/index number.  this allows for a large amount
	 * of general strings as well as error messages to be placed in 
	 * the file.  See battlestar(6)'s use of strstr() in the 
	 * creation of battle_strings
	*/
	int	longoff = 0;

void
usage()
	{
	errx(1, "usage: %s [-d] [-l] [-e pattern] ... [-] stringfile prefix file ...", __progname);
	}

int
compar(a, b)
	struct estring *a, *b;
	{

	return(b->len - a->len);
	}

int
main(argc, argv)
	int	argc;
	char	**argv;
	{
	int	c;
	char	*cp;

/*
 * If no -e given the program uses the default string "error("
 * 
 * use of -e overrides the default.  If "error(" is desired a '-e error'
 * option must be present.
*/
	while	((c = getopt(argc, argv, "dle:")) != EOF)
		{
		switch	(c)
			{
			case	'd':
				debug = 1;
				break;
			case	'l':
				longoff = 1;
				break;
			case	'e':
				if	(nume < MAX_DASHE)
					{
					cp = (char *)malloc(strlen(optarg) + 2);
					sprintf(cp, "%s(", optarg);
					dashe[nume].str = cp;
					dashe[nume].len = strlen(cp);
					nume++;
					}
				break;
			case	'?':
			default:
				errx(1, "%s: invalid option '%c'", __progname, c);
			}
		}
	if	(nume == 0)
		{
		dashe[0].str = "error(";	/* default if no -e given */
		dashe[0].len = 6;
		nume = 1;
		};

	argc -= optind;
	argv += optind;

/*
 * argc = 1 or 2 obviously not enough arguments
*/
	if	(argc < 3)
		usage();
/*
 * '-' causes getopt to return EOF leaving optind/optarg pointing at '-'
 * set addon and skip to the next argument
*/
	if	(argv[0][0] == '-')
		{
		addon++;
		argc--;
		argv++;
		}
/*
 * Need at least 3 more arguments for 'stringfile', 'prefix' and at least one
 * input file.
*/
	if	(argc < 3)
		usage();

/*
 * strings file
*/
	strfile = *argv++;
	argc--;
/*
 * prefix
*/
	prefix = *argv++;
	argc--;

/*
 * sort by length of strings.  look for matches on longer strings first
*/
	qsort(dashe, nume, sizeof(struct estring), compar);

	if	(debug)
		{
		for	(c = 0; dashe[c].str; c++)
			printf("-e %s\n", dashe[c].str);

		printf("addon: %d longoff: %d stringfile: %s prefix: %s\n", addon, longoff, strfile, prefix);
		}
	else
		{
		mesgwrite = fopen(strfile, addon ? "a" : "w");
		if	(mesgwrite == NULL)
			err(1, "%s", strfile);

		mesgread = fopen(strfile, "r");
		if	(mesgread == NULL)
			err(1, "%s", strfile);

		inithash();
		}

/*
 * input files
*/
	for	(; argc-- ; argv++)
		{
		if	(debug)
			printf("%s\n", *argv);
		else
			{
			snprintf(name, sizeof(name), "%s%s", prefix, *argv);
/*
 * Attempt to open the input file before creating the prefix'd file.  If
 * the input doesn't exist the prefix'd file will not be created.
*/
			if	(freopen(*argv, "r", stdin) == NULL)
				err(1, "%s", *argv);
			if	(freopen(name, "w", stdout) == NULL)
				err(1, "%s", name);
			process();
			}
		}
	exit(0);
	}

/*
 * These are global and shared between ln2c(), process() and copystr()
*/
	char	*gfp, *glp = NULL;

/*
 * read a newline terminated string with fgetln() and convert it to a C string
 * (nul terminated). If a newline is present (the normal case) overwrite
 * the newline with '\0'.  If the last line of the file does not end with a
 * newline then calloc a buffer one byte larger than the returned length and
 * copy the data to the new buffer (this should happen only at the last 
 * line of the file but lets be paranoid).
*/

char *ln2c()
	{
	size_t	len;

	gfp = fgetln(stdin, &len);
	if	(!gfp) return(NULL);

	if	(glp)
		{
		free(glp);
		glp = NULL;
		}
	if	(gfp[len - 1] != '\n')
		{
		glp = calloc(1, len + 1);
		bcopy(gfp, glp, len);
		gfp = glp;
		}
	else
		gfp[len - 1] = '\0';
	return(gfp);
	}

void process()
	{
	register char *cp;
	int	inset, n;
	register struct estring *ep;

	while	(ln2c() != NULL)
		{
		for	(cp = index(gfp, '"'); cp ; cp = index(cp, '"'))
			{
			inset = cp - gfp; /* distance from last match or beginning of line */
			for	(n = 0, ep = dashe; n < nume; n++, ep++)
				{
				if	(inset >= ep->len)
					{
					if	(bcmp(ep->str, cp - ep->len, ep->len))
						continue;
					/*
					 * have a match.  output line up to
					 * and including '(' character
					*/
					 fwrite(gfp, inset, 1, stdout);
					 cp = copystr(cp+1);  /* skip '"' */
					 gfp = cp; /* update start point of scan */
					 break;   /* out of for(nume)), to for(cp) */
					 }
				} /* for (dashe) */
/*
 * if we came out of the for(dashe) loop with no match skip the current '"' 
 * so index() will find the next '"'
 */
			if	(n >= nume)
				cp++;
			} /* for (cp) */
/*
 * At this point all matched strings ('"') have been processed or there were
 * no strings.  Either gfp points to the beginning of the line (no '"') OR
 * there are no '"' from the current (gfp) position till the end of line.  The 
 * line was converted to a C string so fputs can be used.  The nul'd \n is 
 * then output.
*/
		fputs(gfp, stdout);
		putchar('\n');
		} /* while(ln2c()) */
	}

char *copystr(register char *cp)
	{
	register int c;
	int	ch;
	char buf[512], *ep;
	register char *bp = buf;

	ep = bp + sizeof(buf) - 1;
	while	(1)
		{
		c = *cp++;
		if	(c == '"')
			break;
/*
 * the old mkstr treated a newline before the closing " as a continuation line
 * and would output the first part of the string followed by a newline and
 * then the remainder of the string.  The C compiler would complain about 
 * an unterminated literal so it's a broken file.  The old behaviour is 
 * emulated for compatibility.
 *
 * The behavior changes if a \ is given before the line break because that
 * is a valid C continuation line.
*/
		if	(c == 0)
			{
			if	((cp = ln2c()) == NULL)
				{
				cp = "";
				goto oops;
				}
			*bp++ = '\n';	/* emulate old behaviour */
			continue;
			}
		if	(c == '\\')
			{
			c = *cp++;
			switch	(c)
				{
				case	'a':
					c = '\a';
					break;
				case	'b':
					c = '\b';
					break;
				case	't':
					c = '\t';
					break;
				case	'r':
					c = '\r';
					break;
				case	'n':
					c = '\n';
					break;
				case	'\0':	/* was '\n' */
/*
 * When the input line was converted to a C string the \n became \0.  Thus
 * \ followed by \0 is the same as \ followed by \n.  Fetch the next line 
 * using ln2c().  If no next line the string is not terminated - return a
 * pointer to an empty string.
*/
					if	((cp = ln2c()) == NULL)
						{
						cp = "";
						goto oops;
						}
					continue;
				case	'f':
					c = '\f';
					break;
				case	'v':
					c = '\v';
					break;
				case	'\\':
					break;
				default:
					if	(!octdigit(c))
						break;
					c -= '0';
					ch = *cp;
					if	(!octdigit(ch))
						break;
					c <<= 3, c += ch - '0';
					cp++;
					ch = *cp;
					if	(!octdigit(ch))
						break;
					cp++;
					c <<= 3, c+= ch - '0';
					break;
				}
			}	
		if	(bp < ep)
			*bp++ = c;
		}
oops:
	*bp = 0;
	if	(longoff == 0)
		printf("%u", (unsigned int) hashit(buf, 1, (off_t)0));
	else
		printf("%ldL", hashit(buf, 1, (off_t)0));
	return(cp);
	}

int
octdigit(c)
	register char c;
	{
	return(c >= '0' && c <= '7');
	}

void inithash()
	{
	char buf[512];
	off_t mesgpt = 0;

	rewind(mesgread);
	while	(fgetNUL(buf, sizeof buf, mesgread) != 0)
		{
		(void)hashit(buf, 0, mesgpt);
		mesgpt += strlen(buf) + 2;
		}
	}

#define	NBUCKETS	511

struct	hash {
	long	hval;
	off_t	hpt;
	struct	hash *hnext;
} *bucket[NBUCKETS];

off_t hashit(str, really, fakept)
	char *str;
	int really;
	off_t fakept;
	{
	int i;
	register struct hash *hp;
	char buf[512];
	long hashval = 0;
	register char *cp;

	if	(really)
		fflush(mesgwrite);
	for	(cp = str; *cp;)
		hashval = (hashval << 1) + *cp++;
	i = hashval % NBUCKETS;
	if	(i < 0)
		i += NBUCKETS;
	if	(really != 0)
		{
		for (hp = bucket[i]; hp != 0; hp = hp->hnext)
		if	(hp->hval == hashval)
			{
			fseek(mesgread, hp->hpt, 0);
			(void)fgetNUL(buf, sizeof buf, mesgread);
/*
			fprintf(stderr, "Got (from %ld) %s\n", hp->hpt, buf);
*/
			if (strcmp(buf, str) == 0)
				break;
			}
		}
	if	(!really || hp == 0)
		{
		hp = (struct hash *) calloc(1, sizeof *hp);
		hp->hnext = bucket[i];
		hp->hval = hashval;
		hp->hpt = really ? ftell(mesgwrite) : fakept;
		if	(really)
			{
			fwrite(str, sizeof (char), strlen(str) + 1, mesgwrite);
			fwrite("\n", sizeof (char), 1, mesgwrite);
			}
		bucket[i] = hp;
		}
/*
	fprintf(stderr, "%s hashed to %ld at %ld\n", str, hp->hval, hp->hpt);
*/
	return(hp->hpt);
	}

int fgetNUL(obuf, rmdr, file)
	char *obuf;
	register int rmdr;
	FILE *file;
{
	register int c;
	register char *buf = obuf;

	while (--rmdr > 0 && (c = getc(file)) != 0 && c != EOF)
		*buf++ = c;
	*buf++ = 0;
	getc(file);
	return ((feof(file) || ferror(file)) ? 0 : 1);
}
