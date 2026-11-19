#if     !defined(lint) && defined(DOSCCS)
char rcsid[] = "util.c 2.1 (2.11BSD) 2026/6/21";
#endif

#include "EXTERN.h"
#include "common.h"
#include "INTERN.h"
#include "util.h"

void	my_exit();
static	char internalerr[] = "patch: internal error, can't reopen %s\n";

/* Rename a file, copying it if necessary. */

int
move_file(from,to)
char *from, *to;
{
    char bakname[512];
    register char *s;
    register int i;
    register int fromfd;

    /* to stdout? */

    if (strEQ(to, "-")) {
#ifdef DEBUGGING
	if (debug & 4)
	    fprintf(stderr, "Moving %s to stdout\n", from);
#endif
	fromfd = open(from, 0);
	if (fromfd < 0)
	    fatal(internalerr, from);
	while ((i=read(fromfd, buf, sizeof buf)) > 0)
	    if (write(1, buf, i) != 1)
		fatal("patch: write failed\n");
	close(fromfd);
	return 0;
    }

	if (origprae) {
		strcpy (bakname, origprae);
    	strcat(bakname, to);
	} else {
   		strcpy(bakname, to);
    	strcat(bakname, origext?origext:ORIGEXT);
	}
    if (stat(to, &filestat) >= 0) {	/* output file exists */
	dev_t to_device = filestat.st_dev;
	ino_t to_inode  = filestat.st_ino;
	char *simplename = bakname;
	
	for (s=bakname; *s; s++) {
	    if (*s == '/')
		simplename = s+1;
	}
	/* find a backup name that is not the same file */
	while (stat(bakname, &filestat) >= 0 &&
		to_device == filestat.st_dev && to_inode == filestat.st_ino) {
	    for (s=simplename; *s && !islower(*s); s++) ;
	    if (*s)
		*s = toupper(*s);
	    else
		strcpy(simplename, simplename+1);
	}
	while (unlink(bakname) >= 0) ;	/* while() is for benefit of Eunice */
#ifdef DEBUGGING
	if (debug & 4)
	    fprintf(stderr, "Moving %s to %s\n", to, bakname);
#endif
	if (link(to, bakname) < 0) {
	    fprintf(stderr, "patch: can't backup %s, output is in %s\n",
		to, from);
	    return -1;
	}
	while (unlink(to) >= 0) ;
    }
#ifdef DEBUGGING
    if (debug & 4)
	fprintf(stderr, "Moving %s to %s\n", from, to);
#endif
    if (link(from, to) < 0) {		/* different file system? */
	int tofd;
	
	tofd = creat(to, 0666);
	if (tofd < 0) {
	    fprintf(stderr, "patch: can't create %s, output is in %s\n",
	      to, from);
	    return -1;
	}
	fromfd = open(from, 0);
	if (fromfd < 0)
	    fatal(internalerr, from);
	while ((i=read(fromfd, buf, sizeof buf)) > 0)
	    if (write(tofd, buf, i) != i)
		fatal("patch: write failed\n");
	close(fromfd);
	close(tofd);
    }
    unlink(from);
    return 0;
}

/* Copy a file. */

void
copy_file(from,to)
char *from, *to;
{
    register int tofd;
    register int fromfd;
    register int i;
    
    tofd = creat(to, 0666);
    if (tofd < 0)
	fatal("patch: can't create %s\n", to);
    fromfd = open(from, 0);
    if (fromfd < 0)
	fatal(internalerr, from);
    while ((i=read(fromfd, buf, sizeof buf)) > 0)
	if (write(tofd, buf, i) != i)
	    fatal("patch: write (%s) failed\n", to);
    close(fromfd);
    close(tofd);
}

/* Allocate a unique area for a string. */

char *
savestr(s)
register char *s;
{
    register char *rv;
    register char *t;

    if (!s)
	s = "Oops";
    t = s;
    while (*t++);
    rv = malloc((MEM) (t - s));
    if (rv == Nullch) {
	if (using_plan_a)
	    out_of_mem = TRUE;
	else
	    fatal("patch: out of memory (savestr)\n");
    }
    else {
	t = rv;
	while (*t++ = *s++);
    }
    return rv;
}

void
fatal(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    my_exit(1);
}

/*
 * Get a response from the user.
 *
 * stdin (0) is almost always patch input
 * stdout (1) is often new file output
 * stderr (2) can be redirected to a file
 *
 * open /dev/tty to get a terminal  to prompt and read response
*/

void
ask(prompt)
char *prompt;
{
    int ttyfd;
    int r;

    if	((ttyfd = open("/dev/tty", 2)) >= 0)
	{
	write(ttyfd, prompt, strlen(prompt));
	r = read(ttyfd, buf, sizeof buf);
	close(ttyfd);
        }
    else
	{				/* no terminal - can't happen */
	buf[0] = '\n';
	r = 1;
	}
    if	(r <= 0)
	buf[0] = 0;
    else
	buf[r] = '\0';
}

/* How to handle certain events when not in a critical region. */

void
set_signals(reset)
int reset;
{
    static sig_t hupval, intval;

    if (!reset) {
	hupval = signal(SIGHUP, SIG_IGN);
	if (hupval != SIG_IGN)
	    hupval = my_exit;
	intval = signal(SIGINT, SIG_IGN);
	if (intval != SIG_IGN)
	    intval = my_exit;
    }
    signal(SIGHUP, hupval);
    signal(SIGINT, intval);
}

/* How to handle certain events when in a critical region. */

void
ignore_signals()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
}

/* Make sure we'll have the directories to create a file. */

void
makedirs(filename,striplast)
register char *filename;
bool striplast;
{
    char tmpbuf[256];
    register char *s = tmpbuf;
    char *dirv[20];
    register int i;
    int dirvp = 0;

    while (*filename) {
	if (*filename == '/') {
	    filename++;
	    dirv[dirvp++] = s;
	    *s++ = '\0';
	}
	else {
	    *s++ = *filename++;
	}
    }
    *s = '\0';
    dirv[dirvp] = s;
    if (striplast)
	dirvp--;
    if (dirvp < 0)
	return;
    strcpy(buf, "mkdir");
    s = buf;
    for (i=0; i<=dirvp; i++) {
	while (*s) s++;
	*s++ = ' ';
	strcpy(s, tmpbuf);
	*dirv[i] = '/';
    }
    system(buf);
}

/* Make filenames more reasonable. */

char *
fetchname(at,strip_leading,assume_exists)
char *at;
int strip_leading;
int assume_exists;
{
    char *s;
    char *name;
    register char *t;
    char tmpbuf[200];

    if (!at)
	return Nullch;
    s = savestr(at);
    for (t=s; isspace(*t); t++) ;
    name = t;
#ifdef DEBUGGING
    if (debug & 128)
	fprintf(stderr, "fetchname %s %d %d\n",name,strip_leading,assume_exists);
#endif
    if (strnEQ(name, "/dev/null", 9))	/* so files can be created by diffing */
	return Nullch;			/*   against /dev/null. */
    for (; *t && !isspace(*t); t++)
	if (*t == '/')
	    if (--strip_leading >= 0)
		name = t+1;
    *t = '\0';
    if (name != s && *s != '/') {
	name[-1] = '\0';
	if (stat(s, &filestat) && filestat.st_mode & S_IFDIR) {
	    name[-1] = '/';
	    name=s;
	}
    }
    name = savestr(name);
    free(s);
    if (stat(name, &filestat) < 0 && !assume_exists) {
	free(name);
	name = Nullch;
    	}
    return name;
}
