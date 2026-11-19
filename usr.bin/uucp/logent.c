#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)logent.c	5.7 (2.11BSD) 2026/5/9";
#endif

#include "uucp.h"
#include <time.h>
#include <fcntl.h>

static FILE *Lp = NULL;
static FILE *Sp = NULL;
static Ltried = 0;
static Stried = 0;

/*
 *	make log entry
 */
logent(text, status)
char *text, *status;
{
#ifdef LOGBYSITE
	char lfile[MAXFULLNAME];
	static char LogRmtname[64];
#endif LOGBYSITE
	if (Rmtname[0] == '\0')
		strcpy(Rmtname, Myname);
	/* Open the log file if necessary */
#ifdef LOGBYSITE
	if (strcmp(Rmtname, LogRmtname)) {
		if (Lp != NULL)
			fclose(Lp);
		Lp = NULL;
		Ltried = 0;
	}
#endif LOGBYSITE
	if (Lp == NULL) {
		if (!Ltried) {
			int savemask;
#ifdef F_SETFL
			int flags;
#endif
			savemask = umask(LOGMASK);
#ifdef LOGBYSITE
			(void) sprintf(lfile, "%s/%s/%s", LOGBYSITE, Progname, Rmtname);
			strcpy(LogRmtname, Rmtname);
			Lp = fopen (lfile, "a");
#else !LOGBYSITE
			Lp = fopen (LOGFILE, "a");
#endif LOGBYSITE
			umask(savemask);
#ifdef F_SETFL
			flags = fcntl(fileno(Lp), F_GETFL, 0);
			fcntl(fileno(Lp), F_SETFL, flags|O_APPEND);
#endif
		}
		Ltried = 1;
		if (Lp == NULL)
			return;
		fioclex(fileno(Lp));
	}

	/*  make entry in existing temp log file  */
	mlogent(Lp, status, text);
}

/*
 *	make a log entry
 */

mlogent(fp, status, text)
char *text, *status;
register FILE *fp;
{
	static pid = 0;
	register struct tm *tp;
	extern struct tm *localtime();

	if (text == NULL)
		text = "";
	if (status == NULL)
		status = "";
	if (!pid)
		pid = getpid();
	ftime(&Now);
	tp = localtime(&Now.time);
	fprintf(fp, "%s %s (%d/%d-%02d:%02d-%d) ",
		User, Rmtname, tp->tm_mon + 1, tp->tm_mday,
		tp->tm_hour, tp->tm_min, pid);
	fprintf(fp, "%s (%s)\n", status, text);

	/* Since it's buffered */
#ifndef F_SETFL
	lseek (fileno(fp), (long)0, 2);
#endif !F_SETFL
	fflush (fp);
	if (Debug) {
		fprintf(stderr, "%s %s ", User, Rmtname);
		fprintf(stderr, "(%d/%d-%02d:%02d-%d) ", tp->tm_mon + 1,
			tp->tm_mday, tp->tm_hour, tp->tm_min, pid);
		fprintf(stderr, "%s (%s)\n", status, text);
	}
}

/*
 *	close log file
 */
logcls()
{
	if (Lp != NULL)
		fclose(Lp);
	Lp = NULL;
	Ltried = 0;

	if (Sp != NULL)
		fclose (Sp);
	Sp = NULL;
	Stried = 0;
}


/*
 *	make system log entry
 */
syslog(text)
char *text;
{
	register struct tm *tp;
#ifdef LOGBYSITE
	char lfile[MAXFULLNAME];
	static char SLogRmtname[64];

	if (strcmp(Rmtname, SLogRmtname)) {
		if (Sp != NULL)
			fclose(Sp);
		Sp = NULL;
		Stried = 0;
	}
#endif LOGBYSITE
	if (Sp == NULL) {
		if (!Stried) {
			int savemask;
#ifdef F_SETFL
			int flags;
#endif F_SETFL
			savemask = umask(LOGMASK);
#ifdef LOGBYSITE
			(void) sprintf(lfile, "%s/xferstats/%s", LOGBYSITE, Rmtname);
			strcpy(SLogRmtname, Rmtname);
			Sp = fopen (lfile, "a");
#else !LOGBYSITE
			Sp = fopen (SYSLOG, "a");
#endif LOGBYSITE
			umask(savemask);
#ifdef F_SETFL
			flags = fcntl(fileno(Sp), F_GETFL, 0);
			fcntl(fileno(Sp), F_SETFL, flags|O_APPEND);
#endif F_SETFL

		}
		Stried = 1;
		if (Sp == NULL)
			return;
		fioclex(fileno(Sp));
	}

	ftime(&Now);
	tp = localtime(&Now.time);

	fprintf(Sp, "%s %s ", User, Rmtname);
	fprintf(Sp, "(%d/%d-%02d:%02d) ", tp->tm_mon + 1,
		tp->tm_mday, tp->tm_hour, tp->tm_min);
	fprintf(Sp, "(%ld.%02u) %s\n", Now.time, Now.millitm/10, text);

	/* Position at end and flush */
#ifndef F_SETFL
	lseek (fileno(Sp), (long)0, 2);
#endif F_SETFL
	fflush (Sp);
}

/*
 * Arrange to close fd on exec(II).
 * Otherwise unwanted file descriptors are inherited
 * by other programs.  And that may be a security hole.
 */
#include <sgtty.h>

fioclex(fd)
int fd;
{
	register int ret;

	ret = fcntl(fd, F_SETFD, 1);	/* Steve Bellovin says this does it */
	if (ret)
		DEBUG(2, "Can't F_SETFD %d\n", fd);
}
