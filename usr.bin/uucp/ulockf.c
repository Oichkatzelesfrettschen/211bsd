#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)ulockf.c	5.5.2 (2.11BSD) 2026/5/9";
#endif

#include "uucp.h"
#include <sys/stat.h>
#include <errno.h>

#define	LCKMODE	0444	/* File mode for lock files */
#define MAXLOCKS 16	/* Maximum number of lock files */

char *Lockfile[MAXLOCKS];
char *LockDirectory = LOCKDIR;
int Nlocks = 0;

/*LINTLIBRARY*/

/*
 *	This routine will attempt to create a lock file (file).
 *	It makes sure that the lock file is valid if it already exists.
 *
 *	return codes:  SUCCESS  |  FAIL
 */
ulockf(hfile, atime)
char *hfile;
time_t atime;
{
	register char *p;
	register int i;
	static char tempfile[NAMESIZE];
	char file[NAMESIZE];
	static int pid = -1;
	extern int errno;

	if (pid < 0) {
		pid = getpid();
		sprintf(tempfile, "%s/LTMP.%d", LockDirectory, pid);
	}
	sprintf(file, "%s/LCK..%s", LockDirectory, hfile);
	i = 0;
	while (onelock(pid, tempfile, file) == -1) { /* lock file exists */
		register int fd;
		fd = open(file, 0);
		if (fd >= 0) {
			int upid, ret;
			ret = read(fd, &upid, sizeof upid);
			close(fd);
			if (ret == sizeof upid && (kill(upid, 0) == 0
				|| errno != ESRCH))
				return FAIL; /* process is still running */
		}
		assert("DEAD LOCK", file, errno);
		logent(file, "DEAD LOCK");
		(void) unlink(file);
		sleep(5);	/* avoid a possible race */
		ASSERT(i++ < 5, "CAN'T GET LOCKFILE", tempfile, errno);
	}

	for (i = 0; i < Nlocks; i++) {
		if (Lockfile[i] == NULL)
			break;
	}
	ASSERT(i < MAXLOCKS, "TOO MANY LOCKS", CNULL, i);
	if (i >= Nlocks)
		i = Nlocks++;
	p = (char *)malloc((unsigned)(strlen(file)+1));
	ASSERT(p != NULL, "CAN NOT ALLOCATE FOR", file, 0);
	strcpy(p, file);
	Lockfile[i] = p;

	return SUCCESS;
}

/*
 *	remove all lock files in list or name
 */
rmlock(name)
register char *name;
{
	register int i;
	char file[MAXFULLNAME];

	if (name != NULL) {
		sprintf(file, "%s/LCK..%s", LockDirectory, name);
		name = file;
	}
	for (i = 0; i < Nlocks; i++) {
		if (Lockfile[i] == NULL)
			continue;
		if (name == NULL || strcmp(name, Lockfile[i]) == SAME) {
			unlink(Lockfile[i]);
			free(Lockfile[i]);
			Lockfile[i] = NULL;
		}
	}
}

/*
 *	makes lock a name on behalf of pid. Tempfile must be in the same
 *	file system as name.
 */
onelock(pid, tempfile, name)
int pid;
char *tempfile, *name;
{
	register int fd, ret;
	extern int errno;
	fd = creat(tempfile, LCKMODE);
	if (fd < 0) {
		DEBUG(1,"Can't creat temp file %s ", tempfile);
		DEBUG(1,"-- errno %d", errno);
		return FAIL;
	}
	ret = write(fd, (char *)&pid, sizeof(int));
	(void) close(fd);

	if (ret != sizeof(int)) {
		DEBUG(1,"Temp file write failed -- errno %d\n", errno);
		(void) unlink(tempfile);
		return FAIL;
	}
	if (link(tempfile, name) < 0) {
		(void) unlink(tempfile);
		return FAIL;
	}
	unlink(tempfile);
	return SUCCESS;
}
