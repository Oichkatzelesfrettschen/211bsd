#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)uucpname.c	5.6 (2.11BSD) 2026/5/9";
#endif

#include "uucp.h"
#include <sys/stat.h>

char Myfullname[64];

/*
 *	uucpname(name)		get the uucp name
 *
 *	return code - none
 */
uucpname(name)
register char *name;
{
	register char *s;

	/*
	 * Since some UNIX systems do not honor the set-user-id bit
	 * when the invoking user is root, we must change the uid here.
	 * So uucp files are created with the correct owner.
	 */
	if (geteuid() == 0 && getuid() == 0) {
		struct stat stbuf;
		stbuf.st_uid = 0;	/* In case the stat fails */
		stbuf.st_gid = 0;
		stat(UUCICO, &stbuf);	/* Assume uucico is correctly owned */
		setgid(stbuf.st_gid);
		setuid(stbuf.st_uid);
	}

	s = NULL;	/* system name unknown, so far */

	if (s == NULL || *s == '\0') {
		s = Myfullname;
		if(gethostname(Myfullname, sizeof(Myfullname)) == -1) {
			DEBUG(1, "gethostname", _FAILED);
			s = NULL;
		}
	}

	if (s == NULL || *s == '\0') {
		s = MYNAME;
		strncpy(Myfullname, s, sizeof Myfullname);
	}

	if (s == NULL || *s == '\0') {
		/*
		 * As a last ditch thing, we *could* search Spool
		 * for D.<uucpname> and use that,
		 * but that is too much trouble, isn't it?
		 */
		logent("SYSTEM NAME", "CANNOT DETERMINE");
		strcpy(Myfullname, "unknown");
	}

	/*
	 * copy uucpname back to caller-supplied buffer,
	 * truncating to MAXBASENAME characters.
	 * Also set up subdirectory names
	 * Truncate names at '.' if found to handle cases like
	 * seismo.css.gov being returned by gethostname().
	 * uucp sites should not have '.' in their name anyway
	 */
	s = index(Myfullname, '.');
	if (s != NULL)
		*s = '\0';
	strncpy(name, Myfullname, MAXBASENAME);
	name[MAXBASENAME] = '\0';
	DEBUG(1, "My uucpname = %s\n", name);

	sprintf(DLocal, "D.%.*s", SYSNSIZE, name);
	sprintf(DLocalX, "D.%.*sX", SYSNSIZE, name);
}
