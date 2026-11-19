/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	defined(DOSCCS) && !defined(lint)
static char sccsid[] = "@(#)dmesg.c	5.6 (2.11BSD) 2026/4/2";
#endif

/*
 *	Suck up system messages
 */

#include <stdio.h>
#include <sys/param.h>
#include <nlist.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/vm.h>
#include <sys/msgbuf.h>

struct	msgbuf msgbuf;
char	*msgbufp;

struct	msgbuf omesg;
struct	nlist nl[2] = {
	{ "_msgbuf" },
	{ "" }
};

main(argc, argv)
char **argv;
{
	int mem;
	register char *mp, *omp, *mstart;
	int samef, sawnl, ignore;
	char msgb[MSG_BSIZE];

	nlist("/unix", nl);
	if (nl[0].n_type==0)
		done("Can't get /unix namelist\n");
	if ((mem = open("/dev/mem", O_RDONLY)) < 0)
		done("Can't open /dev/mem\n");
	lseek(mem, (long)nl[0].n_value, SEEK_SET);
	read(mem, &msgbuf, sizeof (msgbuf));
	if (msgbuf.msg_magic != MSG_MAGIC)
		done("Magic number wrong (namelist mismatch?)\n");
	if (msgbuf.msg_bufx >= MSG_BSIZE)
		msgbuf.msg_bufx = 0;
	if (omesg.msg_bufx >= MSG_BSIZE)
		omesg.msg_bufx = 0;
	msgbuf.msg_bufc = msgb;
	lseek(mem, (long)ctob((long)msgbuf.msg_click), SEEK_SET);
	read(mem, msgbuf.msg_bufc, MSG_BSIZE);
	mstart = &msgbuf.msg_bufc[omesg.msg_bufx];
	omp = &omesg.msg_bufc[msgbuf.msg_bufx];
	mp = msgbufp = &msgbuf.msg_bufc[msgbuf.msg_bufx];
	samef = 1;
	do {
		if (*mp++ != *omp++) {
			mstart = msgbufp;
			samef = 0;
			pdate();
			printf("...\n");
			break;
		}
		if (mp >= &msgbuf.msg_bufc[MSG_BSIZE])
			mp = msgbuf.msg_bufc;
		if (omp >= &omesg.msg_bufc[MSG_BSIZE])
			omp = omesg.msg_bufc;
	} while (mp != mstart);
	if (samef && omesg.msg_bufx == msgbuf.msg_bufx)
		exit(0);
	mp = mstart;
	pdate();
	sawnl = 1;
	do {
		if (sawnl && *mp == '<')
			ignore = 1;
		if (*mp && (*mp & 0200) == 0 && !ignore)
			putchar(*mp);
		if (ignore && *mp == '>')
			ignore = 0;
		sawnl = (*mp == '\n');
		mp++;
		if (mp >= &msgbuf.msg_bufc[MSG_BSIZE])
			mp = msgbuf.msg_bufc;
	} while (mp != msgbufp);
	done((char *)NULL);
}

done(s)
char *s;
{
	register char *p, *q;

	if (s) {
		pdate();
		printf(s);
	}
	exit(s ? 1 : 0);
}

pdate()
{
	extern char *ctime();
	static firstime;
	time_t tbuf;

	if (firstime==0) {
		firstime++;
		time(&tbuf);
		printf("\n%.12s\n", ctime(&tbuf)+4);
	}
}
