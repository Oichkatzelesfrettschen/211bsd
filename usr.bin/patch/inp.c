#if     !defined(lint) && defined(DOSCCS)
char rcsid[] = "inp.c 2.1 (2.11BSD) 2026/6/18";
#endif

/*
 * $Log:	inp.c,v $
 * Revision 2.0.1.2  96/12/7  22:15:00 sms
 * Files right up against 64kb in size caused patch to core dump because
 * a large (negative) value was passed to malloc().  Therefore force
 * plan B for files over 48kb in size if on a pdp-11.
 *
 * Revision 2.0.1.1  88/06/03  15:06:13  lwall
 * patch10: made a little smarter about sccs files
 * 
 * Revision 2.0  86/09/17  15:37:02  lwall
 * Baseline for netwide release.
 */

#include "EXTERN.h"
#include "common.h"
#include "util.h"
#include "pch.h"
#include "INTERN.h"
#include "inp.h"

/* Input-file-with-indexable-lines abstract type */

static long i_size;			/* size of the input file */
static char *i_womp;			/* plan a buffer for entire file */
static char **i_ptr;			/* pointers to lines in i_womp */

static int tifd = -1;			/* plan b virtual string array */
static char *tibuf[2];			/* plan b buffers */
static LINENUM tiline[2] = {-1, -1};	/* 1st line in each buffer */
static LINENUM lines_per_buf;		/* how many lines per buffer */
static int tireclen;			/* length of records in tmp file */

static char cwtf[] = "can't write temp file\n";
static char doesnt[] = "Warning: this file doesn't appear to be the %s version--patching anyway.\n";
static char appear[] = "This file doesn't appear to be the %s version--patch anyway? [n] ";

/* New patch--prepare to edit another file. */

void
re_input()
{
    if (using_plan_a) {
	i_size = 0;
	if (i_ptr != Null(char**))
	    free((char *)i_ptr);
	if (i_womp != Nullch)
	    free(i_womp);
	i_womp = Nullch;
	i_ptr = Null(char **);
    }
    else {
	using_plan_a = TRUE;		/* maybe the next one is smaller */
	close(tifd);
	tifd = -1;
	free(tibuf[0]);
	free(tibuf[1]);
	tibuf[0] = tibuf[1] = Nullch;
	tiline[0] = tiline[1] = -1;
	tireclen = 0;
    }
}

/* Constuct the line index, somehow or other. */

void
scan_input(filename)
char *filename;
{
    if (!plan_a(filename))
	plan_b(filename);
    if (verbose) {
	fprintf(stderr, "Patching file %s using Plan %s...\n", filename,
	  (using_plan_a ? "A" : "B") );
    }
}

/* Try keeping everything in memory. */

bool
plan_a(filename)
char *filename;
{
    int ifd;
    register char *s;
    register LINENUM iline;

    if (ok_to_create_file && stat(filename, &filestat) < 0) {
	if (verbose)
	    fprintf(stderr, "(Creating file %s...)\n",filename);
	makedirs(filename, TRUE);
	close(creat(filename, 0666));
    }

    if (stat(filename, &filestat) < 0)
	fatal("Can't find %s\n", filename);

    filemode = filestat.st_mode;
    if ((filemode & S_IFMT) & ~S_IFREG)
	fatal("%s is not a normal file--can't patch.\n", filename);
    i_size = filestat.st_size;
#ifdef	pdp11
/*
 * csh.1 was 65392 bytes long and caused patch to core dump because
 * malloc() couldn't cope.
*/
    if (i_size > 49152L)
	return FALSE;
#endif
    if (out_of_mem) {
	set_hunkmax();		/* make sure dynamic arrays are allocated */
	out_of_mem = FALSE;
	return FALSE;			/* force plan b because plan a bombed */
    }
    i_womp = malloc((MEM)(i_size+2));
    if (i_womp == Nullch)
	return FALSE;
    if ((ifd = open(filename, 0)) < 0)
	fatal("Can't open file %s\n", filename);
    if (read(ifd, i_womp, (int)i_size) != i_size) {
	close(ifd);	/* probably means i_size > 15 or 16 bits worth */
	free(i_womp);
	return FALSE;
    }
    close(ifd);
    if (i_size && i_womp[i_size-1] != '\n')
	i_womp[i_size++] = '\n';
    i_womp[i_size] = '\0';

    /* count the lines in the buffer so we know how many pointers we need */

    iline = 0;
    for (s=i_womp; *s; s++) {
	if (*s == '\n')
	    iline++;
    }
    i_ptr = (char **)malloc((MEM)((iline + 2) * sizeof(char *)));
    if (i_ptr == Null(char **)) {	/* shucks, it was a near thing */
	free((char *)i_womp);
	return FALSE;
    }
    
    /* now scan the buffer and build pointer array */

    iline = 1;
    i_ptr[iline] = i_womp;
    for (s=i_womp; *s; s++) {
	if (*s == '\n')
	    i_ptr[++iline] = s+1;	/* these are NOT null terminated */
    }
    input_lines = iline - 1;

    /* now check for revision, if any */

    if (revision != Nullch) { 
	if (!rev_in_string(i_womp)) {
	    if (force) {
		if (verbose)
		    fprintf(stderr, doesnt, revision);
	    }
	    else {
		sprintf(buf, appear, revision);
		ask(buf);
	    if (*buf != 'y')
		fatal("Aborted.\n");
	    }
	}
	else if (verbose)
	    fprintf(stderr, "Good.  This file appears to be the %s version.\n",
		revision);
    }
    return TRUE;			/* plan a will work */
}

/* Keep (virtually) nothing in memory. */

void
plan_b(filename)
char *filename;
{
    register FILE *ifp;
    register int i = 0;
    register int maxlen = 1;
    bool found_revision = (revision == Nullch);

    using_plan_a = FALSE;
    if ((ifp = fopen(filename, "r")) == Nullfp)
	fatal("Can't open file %s\n", filename);
    if ((tifd = creat(TMPINNAME, 0666)) < 0)
	fatal("Can't open file %s\n", TMPINNAME);
    while (fgets(buf, sizeof buf, ifp) != Nullch) {
	if (revision != Nullch && !found_revision && rev_in_string(buf))
	    found_revision = TRUE;
	if ((i = strlen(buf)) > maxlen)
	    maxlen = i;			/* find longest line */
    }
    if (revision != Nullch) {
	if (!found_revision) {
	    if (force) {
		if (verbose)
		    fprintf(stderr, doesnt, revision);
	    }
	    else {
		sprintf(buf, appear, revision);
		ask(buf);
		if (*buf != 'y')
		    fatal("Aborted.\n");
	    }
	}
	else if (verbose)
	    fprintf(stderr, "Good.  This file appears to be the %s version.\n",
		revision);
    }
    rewind(ifp);
    lines_per_buf = BUFFERSIZE / maxlen;
    tireclen = maxlen;
    tibuf[0] = malloc((MEM)(BUFFERSIZE + 1));
    tibuf[1] = malloc((MEM)(BUFFERSIZE + 1));
    if (tibuf[1] == Nullch)
	fatal("Can't seem to get enough memory.\n");
    for (i=1; ; i++) {
	if (! (i % lines_per_buf))	/* new block */
	    if (write(tifd, tibuf[0], BUFFERSIZE) < BUFFERSIZE)
		fatal(cwtf);
	if (fgets(tibuf[0] + maxlen * (i%lines_per_buf), maxlen + 1, ifp)
	  == Nullch) {
	    input_lines = i - 1;
	    if (i % lines_per_buf)
		if (write(tifd, tibuf[0], BUFFERSIZE) < BUFFERSIZE)
		    fatal(cwtf);
	    break;
	}
    }
    fclose(ifp);
    close(tifd);
    if ((tifd = open(TMPINNAME, 0)) < 0) {
	fatal("Can't reopen file %s\n", TMPINNAME);
    }
}

/* Fetch a line from the input file, \n terminated, not necessarily \0. */

char *
ifetch(line,whichbuf)
register LINENUM line;
int whichbuf;				/* ignored when file in memory */
{
    if (line < 1 || line > input_lines)
	return "";
    if (using_plan_a)
	return i_ptr[line];
    else {
	LINENUM offline = line % lines_per_buf;
	LINENUM baseline = line - offline;

	if (tiline[0] == baseline)
	    whichbuf = 0;
	else if (tiline[1] == baseline)
	    whichbuf = 1;
	else {
	    tiline[whichbuf] = baseline;
	    (void)lseek(tifd, (long)baseline / lines_per_buf * BUFFERSIZE, 0);
	    if (read(tifd, tibuf[whichbuf], BUFFERSIZE) < 0)
		fatal("Error reading tmp file %s.\n", TMPINNAME);
	}
	return tibuf[whichbuf] + (tireclen*offline);
    }
}

/* True if the string argument contains the revision number we want. */

bool
rev_in_string(string)
char *string;
{
    register char *s;
    register int patlen;

    if (revision == Nullch)
	return TRUE;
    patlen = strlen(revision);
    if (strnEQ(string,revision,patlen) && isspace(s[patlen]))
	return TRUE;
    for (s = string; *s; s++) {
	if (isspace(*s) && strnEQ(s+1, revision, patlen) && 
		isspace(s[patlen+1] )) {
	    return TRUE;
	}
    }
    return FALSE;
}

