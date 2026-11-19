#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)assert.c	5.6 (2.11BSD) 2026/5/9";
#endif

#include "uucp.h"
#include <time.h>
#include <errno.h>

/*
 *	print out assetion error
 */

assert(s1, s2, i1)
char *s1, *s2;
{
	register FILE *errlog;
	register struct tm *tp;
	time_t clock;
	int pid;

	errlog = NULL;
	if (!Debug) {
		int savemask;
		savemask = umask(LOGMASK);
		errlog = fopen(ERRLOG, "a");
		umask(savemask);
	}
	if (errlog == NULL)
		errlog = stderr;

	pid = getpid();
	fprintf(errlog, "ASSERT ERROR (%.9s)  ", Progname);
	fprintf(errlog, "pid: %d  ", pid);
	(void) time(&clock);
	tp = localtime(&clock);
	fprintf(errlog, "(%d/%d-%02d:%02d) ", tp->tm_mon + 1,
		tp->tm_mday, tp->tm_hour, tp->tm_min);
	fprintf(errlog, "%s %s (%d)\n", s1 ? s1 : "", s2 ? s2 : "", i1);
	if (errlog != stderr)
		(void) fclose(errlog);
	return;
}
