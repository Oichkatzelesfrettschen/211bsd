#if     defined(DOSCCS) && !defined(lint)
static char *sccsid = "@(#)quot.c 1.1 (2.11BSD) 2026/5/26";
#endif

/*
 * Disk usage by user
 */

#include <stdio.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/param.h>
#include <sys/inode.h>
#include <sys/fs.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*dargv[] = {
	"/dev/rxp0a",
	"/dev/rxp0h",
	0
};

#define	ITABSZ	256
#define	ISIZ	(DEV_BSIZE/sizeof(struct dinode))
#define	NUID	300
struct	fs	sblock;
struct	dinode	itab[ITABSZ];
struct du
{
	long	blocks;
	u_int 	nfiles;
	uid_t	uid;
	char	*name;
} du[NUID];

#define	TSIZE	500
int	sizes[TSIZE];
long	overflow;

int	nflg;
int	fflg;
int	cflg;

int	fi;
unsigned	ino;
unsigned	nfiles;

void	usage(), xacct();
extern	char *__progname;

/*
 * after processing a filesystem du[] is sorted by username not indexed by
 * userid.  the memory allocated for names needs to be released and the
 * array reinitialized before the next filesystem is processed.  easiest
 * way to initialize is read the passwd file again.
*/
init_du()
	{
	register int n, i;
	register struct passwd *lp;

	for	(i = 0; i < NUID; i++)
		{
		if	(du[i].name)
			free(du[i].name);
		}
	bzero(du, sizeof du);
	bzero(sizes, sizeof (sizes));

	setpassent(1);
	while	(lp=getpwent())
		{
		n = lp->pw_uid;
		if	(n > NUID)
			continue;
		du[n].name = strdup(lp->pw_name);
		}
	}

main(argc, argv)
char **argv;
{
	register int n;
	register char **p;

	while	((n = getopt(argc, argv, "nfc")) != EOF)
		{
		switch	(n)
			{
			case	'n':
				nflg++;
				break;
			case	'f':
				fflg++;
				break;
			case	'c':
				cflg++;
				break;
			case	'?':
			default:
				usage();
			}
		}
	argc -= optind;
	argv += optind;

	if	(argc == 0)
		{
		for	(p = dargv; *p; p++)
			{
			init_du();
			check(*p);
			report();
			}
		exit(0);
		}

	for	(; --argc >= 0; argv++)
		{
		init_du();
		check(*argv);
		report();
		}
	exit(0);
}

check(file)
char *file;
{
	register unsigned i, j;
	register c;

	fi = open(file, 0);
	if (fi < 0) {
		printf("cannot open %s\n", file);
		return;
	}
	printf("%s:\n", file);
	sync();
	bread(1, (char *)&sblock, sizeof sblock);
	nfiles = (sblock.fs_isize-2)*(DEV_BSIZE/sizeof(struct dinode));
	ino = 0;
	if (nflg) {
		if (isdigit(c = getchar()))
			ungetc(c, stdin);
		else while (c!='\n' && c != EOF)
			c = getchar();
	}
	for(i=2; ino<nfiles; i += ITABSZ/ISIZ) {
		bread(i, (char *)itab, sizeof itab);
		for (j=0; j<ITABSZ && ino<nfiles; j++) {
			ino++;
			xacct(&itab[j]);
		}
	}
	close(fi);
}

void
xacct(ip)
register struct dinode *ip;
{
	register n;
	register char *np;
	static fino;

	if ((ip->di_mode&IFMT) == 0)
		return;
	if (cflg) {
		if ((ip->di_mode&IFMT)!=IFDIR && (ip->di_mode&IFMT)!=IFREG)
			return;
		n = (ip->di_size+DEV_BSIZE-1)/DEV_BSIZE;
		if (n >= TSIZE) {
			overflow += n;
			n = TSIZE-1;
		}
		sizes[n]++;
		return;
	}
	if (ip->di_uid >= NUID)
		return;
	du[ip->di_uid].blocks += (ip->di_size+DEV_BSIZE-1)/DEV_BSIZE;
	du[ip->di_uid].nfiles++;
	if (nflg) {
	tryagain:
		if (fino==0)
			if (scanf("%d", &fino)<=0)
				return;
		if (fino > ino)
			return;
		if (fino<ino) {
			while ((n=getchar())!='\n' && n!=EOF)
				;
			fino = 0;
			goto tryagain;
		}
		if (np = du[ip->di_uid].name)
			printf("%.7s\t", np);
		else
			printf("%d\t", ip->di_uid);
		while ((n = getchar())==' ' || n=='\t')
			;
		putchar(n);
		while (n!=EOF && n!='\n') {
			n = getchar();
			putchar(n);
		}
		fino = 0;
	}
}

bread(bno, buf, cnt)
unsigned bno;
char *buf;
{

	lseek(fi, (long)bno*DEV_BSIZE, 0);
	if (read(fi, buf, cnt) != cnt) {
		printf("read error %u\n", bno);
		exit(1);
	}
}

qcmp(p1, p2)
register struct du *p1, *p2;
{
	if (p1->blocks > p2->blocks)
		return(-1);
	if (p1->blocks < p2->blocks)
		return(1);
	return(strcmp(p1->name, p2->name));
}

report()
{
	register i;

	if (nflg)
		return;
	if (cflg) {
		long t = 0;
		for (i=0; i<TSIZE-1; i++)
			if (sizes[i]) {
				t += i*sizes[i];
				printf("%d\t%d\t%ld\n", i, sizes[i], t);
			}
		printf("%d\t%d\t%ld\n", TSIZE-1, sizes[TSIZE-1], overflow+t);
		return;
	}
	qsort(du, NUID, sizeof(du[0]), qcmp);
	for (i=0; i<NUID; i++) {
		if (du[i].blocks==0)
			return;
		printf("%D\t", du[i].blocks);
		if (fflg)
			printf("%u\t", du[i].nfiles);
		if (du[i].name)
			printf("%s\n", du[i].name);
		else
			printf("#%d\n", du[i].uid);
	}
}

void
usage()
	{
	fprintf(stderr, "%s: usage: [-c][-f][-n] [rdiskdev]\n", __progname);
	exit(1);
	}
