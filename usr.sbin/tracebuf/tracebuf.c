#if     defined(DOSCCS) && !defined(lint)
static char *sccsid = "@(#)tracebuf.c 1.0(2.11BSD) 2026/5/13";
#endif

/*
 * tracebuf - dump the kernel tracebuf[] array in a human readable form
 *
*/
#include <stdio.h>
#include <nlist.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/trace.h>

extern	char	*__progname;
	void	usage();

struct	nlist nl[] =
	{
	{ "_tracebuf" },
	{ "" },
	};

	int	mf, lines = 1, iter = 1, hdrcnt = 19, secs = 5;
	void	hdr();

main(argc, argv)
	int argc;
	char **argv;
	{
	register int i;
	int	c;

	nlist("/unix", nl);
	if	(nl[0].n_type == 0)
		errx(1, "%s: tracebuf not found in /unix", __progname);

	mf = open("/dev/kmem", 0);
	if	(mf < 0)
		{
		fprintf(stderr, "cannot open /dev/kmem\n");
		exit(1);
		}
	
	while	((c = getopt(argc, argv, "i:w:h:")) != EOF)
		{
		switch	(c)
			{
			case	'w':
				secs = atoi(optarg);
				break;
			case	'i':
				iter = atoi(optarg);
				break;
			case	'h':
				hdrcnt = atoi(optarg);
				break;
			case	'?':
			default:
				usage();
			}
		}
	
	if	(iter <= 0)   /* bogus iteration count set to 1 */
		iter = 1;

	if	(secs <= 0)   /* bogus interval set to 5 seconds */
		secs = 5;

	signal(SIGCONT, hdr);
loop:
	if	(--lines == 0)
		hdr();
	nmstats();
	fflush(stdout);
	if	(--iter)
		{
		sleep(secs);
		goto loop;
		}
	}

void
hdr()
	{

	puts(" breadhit  breadmiss    bwrite  breadhitra  breadmissra    brelse  swapio  %bhit");
	lines = hdrcnt;
	}

nmstats()
	{
	float	pct;
	long	tracebuf[TR_NUM_210];

	lseek(mf, (long)nl[0].n_value, 0);
	read(mf, tracebuf, sizeof tracebuf);

	pct = tracebuf[TR_BREADHIT];
	pct /= (tracebuf[TR_BREADHIT] + tracebuf[TR_BREADMISS]);
	pct *= 100.0;

	printf("%9D %10D %9D %11D %12D %9D %7D  %.2f\n",
		tracebuf[TR_BREADHIT],
		tracebuf[TR_BREADMISS],
		tracebuf[TR_BWRITE],
		tracebuf[TR_BREADHITRA],
		tracebuf[TR_BREADMISSRA],
		tracebuf[TR_BRELSE],
		tracebuf[TR_SWAPIO],
		pct);
	}

void
usage()
	{
	errx(1, "Usage: %s [-w secs] [-h lines] [-i iterations]", __progname);
	}
