/*
 * Display processes in order of CPU utilization
 *
 * Implemented for 2.11 BSD to evaluate mechanisems for accessing
 * system system state from user mode.
 * Process information access is modeled on mechanism used by ps(1)
 * in particular, makes use of the cached static information in
 *  /var/run/psdatabase
 * which is generated for the ps program at system boot
 *
 * Digby Tarvin 
 *  December 2019.
 *
 * History
 * =======
 * Ed Date       What                                             Who
 * -- ---------- -----------------------------------------------  ----
 * 01 05/12/2019 Written                                          DRST
 * 02 07/03/2020 Removed kmem based nswap access (sysctl updated) DRST
 * 03 09/03/2020 added BADKEY_IGNORE, delay spec now in seconds   DRST
 * 04 12/03/2020 Changed to use curses, correct memory size info  bqt
 * 05 21/03/2020 Corrected memory calculations to be unsigned     bqt
 * 06 29/08/2021 Changed and corrected uptime calculation.        bqt
 *               Changed header layout.
 *               Added %cpu time to header.
 * 07 26/8/2022  Truncate hostname by first period or 7 char.     bqt
 * 08 20/9/2024  Change /vmunix to /unix                          sms
 * 09 28/3/2025  Exit gracefully when out of memory               martin
 * 10 06/5/2025  Handle long user names gracefully                hbe
 * 11 06/14/2026 None of the tty info (gettty()) was used! Almost sms
 *               no info in psdatabase was used. udata_s had      sms
 *               unused members. psdb.c and psdb.h removed        sms
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <ctype.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <pwd.h>
#include <signal.h>
#include <utmp.h>
#include <curses.h>
#include <sys/dk.h>
#include <nlist.h>

/*
 * limit o_comm to 16 instead of MAXCOMLEN.  A udata_s struct is allocated
 * for each process.  A savings of 48 bytes * 100+ processs will allow a
 * larger window size.  Command names are much smaller than 64.
*/
#define OCOMMLEN 16

typedef struct udata_s
{
	char	*o_uname;	/* login name of process owner */
	time_t	o_utime;	/* u_utime */
	time_t	o_stime;	/* u_stime */
	time_t	o_ttime;	/* u_utime + u_stime */
	time_t	o_dtime;	/* o_ttime - o_LastTime */
	size_t	o_tsize;	/* text size */
	char	o_comm[OCOMMLEN];	/* u_comm */
	/*
	 * details from last update
	 */
	short	o_LastPid;	/* last pid in this slot */
	short	o_LastStat;	/* is previous process data valid? */
	time_t	o_LastTime;	/* time at last update */
} udata_t;

#define DRATE	(1000000)	/* default delay between updates (us) */
#define MAXUSER	10		/* user names to cache */

static struct userdb_s
{
	char	uname[UT_NAMESIZE];
	short	uid;
} untab[MAXUSER];

struct nlist nl[] = {
#define X_CPTIME 0
	{ "_cp_time" },
#define X_PROC	1
	{ "_proc" },
#define X_NPROC	2
	{ "_nproc" },
#define	X_HZ	3
	{ "_hz" },
	{ "" },
};

	int	nproc_sym, proc_sym, hz_sym;

static long		upd_us = DRATE;	/* delay between updates */
static struct proc	*proctab;	/* buffer to contain proc array */
static int		ptsz = 0;	/* size of proc array */
static int		*procidx;	/* indirect array for proc access */
static struct udata_s	*userdata;	/* process information from u */

static	char		*memf	= "/dev/mem";
static	char		*kmemf	= "/dev/kmem";

static	int		sort = 9, idle = 1, kmem, mem, swap, hz;
static	u_int		nproc;
static	float		fhz;
static	struct tchars	tc;	/* determine t_intrc and t_quitc */

static	time_t		now;
static	struct tm		*ltp;
static	struct timeval	boottime;
static	time_t		uptime;
static	FILE		*ut;
static	struct utmp	utmp;
static	int		nusers;
static	double		avenrun[3];
static	char		host[8];

/*
 * Configure/restore output terminal
 */
int config_tty(onoff)
int	onoff;
{
	if(onoff == 0) {
		resetty();
	} else {
		initscr();
		savetty();
		raw();
		noecho();
	}
	return(0);
}

/*
 * Exit gracefully when out of memory
 */
static int oom_reset_exit(int status)
{
	resetty();
	printf("the window size is too large.\n");
	exit(status);
}

/*
 * Read from specified location in file
 *  returns 1 on success
 *         -1 on failure
 */
static int readat(fd, pos, buff, count)
	int 		fd;
	off_t		pos;
	char 		*buff;
	unsigned short	count;
{
	if(lseek(fd, pos, L_SET) == (off_t) -1)
		return(-1);
	return((read(fd, buff, count) == count)? 1 : -1);
}

static	int fuser(idx, uid)
	int	idx;
	short	uid;
{
	register struct passwd *pw;

	if((pw = getpwuid(uid)) == NULL)
		return(-1);
	untab[idx].uid = uid;
	strcpy(untab[idx].uname, pw->pw_name);
	if(strlen(untab[idx].uname) > 8) {
		untab[idx].uname[7] = '+';
		untab[idx].uname[8] = 0;
	}
	return(1);
}

static	getu(pp, a)
	struct proc	*pp;
	struct udata_s	*a;
{
	off_t			addr;
	struct user		user;		/* for reading user structs */
	register struct	user	*up	= &user;
	int			pifile;
	int			i;

	if(pp->p_flag & SLOAD)	/* in memory */
	{
		addr = ctob((off_t) pp->p_addr);
		pifile = mem;
	}
	else				/* swapped */
	{
		addr = (off_t)pp->p_addr << 9;
		pifile = swap;
	}

	if(readat(pifile, addr, (char *) up, sizeof(user)) < 0)
	{
		perror("read");
		return(0);
	}

	a->o_tsize	= 0;
	a->o_utime	= 0;
	a->o_stime	= 0;
	a->o_ttime	= 0;
	a->o_dtime	= 0;
	a->o_uname	= NULL;
	a->o_comm[0]	= 0;

	if(pp->p_stat == SZOMB)
		return(1);

	a->o_tsize	= up->u_tsize;
	a->o_utime	= up->u_ru.ru_utime;
	a->o_stime	= up->u_ru.ru_stime;
	a->o_ttime	= a->o_utime + a->o_stime;
	if(a->o_LastStat && (pp->p_pid == a->o_LastPid))
		a->o_dtime = a->o_ttime - a->o_LastTime;
	else
		a->o_dtime = a->o_ttime;

	for(i = 0; i < MAXUSER; i++)
	{
		if(untab[i].uname[0] == 0)
			break;
		if(untab[i].uid == pp->p_uid)
		{
			a->o_uname = untab[i].uname;
			break;
		}
	}
	if((a->o_uname == NULL) && (i< MAXUSER))
	{
		/* name not found and space in table */
		if(fuser(i, pp->p_uid) >= 0)
			a->o_uname = untab[i].uname;	
	}

	strncpy(a->o_comm, up->u_comm, OCOMMLEN);
	a->o_comm[OCOMMLEN - 1] = 0;
	return(1);
}

/*
 * Compare function for process sorting
 */
static	int sortcmp(i1, i2)
	register int *i1, *i2;
{
	struct proc *p1, *p2;
	struct udata_s *u1, *u2;

	p1 = &proctab[*i1];
	p2 = &proctab[*i2];
	u1 = &userdata[*i1];
	u2 = &userdata[*i2];

	switch (sort) {
	case 0:
		return p1->p_pid - p2->p_pid;
	case 1:
		return strcmp(u1->o_uname, u2->o_uname);
	case 2:
		return p2->p_pri - p1->p_pri;
	case 3:
		return p1->p_nice - p2->p_nice;
	case 4:
		return u2->o_tsize - u1->o_tsize;
	case 5:
		return p2->p_dsize - p1->p_dsize;
	case 6:
		return p2->p_ssize - p1->p_ssize;
	case 7:
		return p1->p_stat - p2->p_stat;
	case 9:
		{
			time_t t;
			t = u2->o_dtime - u1->o_dtime;
			if (t) return t;
		}
	case 8:
		return u2->o_ttime - u1->o_ttime;
	case 10:
		return strcmp(u1->o_comm, u2->o_comm);
	}
	return 0;
}
/*
 * Update the process list
 */
static	int update()
{
	int	np;
	register int	i;

	/*
	 * before over-writing old proc structure,
	 * 	record old stat and if applicable, pid and ttime 
	 */
	for(i = 0; i < nproc; i++)
	{
		struct proc 		*procp	= &proctab[i];
		register struct	udata_s	*up	= &userdata[i];

		if((up->o_LastStat = procp->p_stat) != 0)
		{
			up->o_LastTime	= up->o_ttime;
			up->o_LastPid	= procp->p_pid;
		}
	}

	/* read the current proc table into buffer */
	readat(kmem, (off_t) proc_sym, proctab, ptsz);

	for(np = i = 0; i < nproc; i++)
	{
		struct proc *procp = &proctab[i];

		if(procp->p_stat == 0)
			continue;

		getu(procp, &userdata[i]);
		procidx[np] = i;
		np++;
	}
	return(np);
}

static	char states[] = { '?', 'S', 'W', 'R', 'I', 'Z', 'T' };

/*
 * Get Memory Statistics for summary lines
 * Returns:
 *   totmem  - total physical memory in bytes
 *   freemem - available memory in bytes
 */
static	int GetMemStats(totmem, freemem)
	unsigned long *totmem;
	unsigned long *freemem;
{
	int		mib[2];
	size_t		size;
	static size_t	cmsz	= 0;
	static unsigned long 	physmem	= 0;
	static char	*coremap = NULL;
	struct mapent	*map;
	int		i;
	unsigned long	tfree;

	if(physmem == 0)
	{
		/* first call - get static values */
		mib[0] = CTL_HW;
		mib[1] = HW_PHYSMEM;
		size = sizeof(physmem);
		sysctl(mib, 2, &physmem, &size, NULL, 0);

		mib[0] = CTL_VM;
		mib[1] = VM_COREMAP;
		sysctl(mib, 2, NULL, &cmsz, NULL, 0);
		if((coremap = (char *)malloc((unsigned)cmsz)) == NULL)
		{
			perror("COREMAP");
			exit(1);
		}
		memset(coremap, 0, cmsz);
	}
	mib[0] = CTL_VM;
	mib[1] = VM_COREMAP;
	size = cmsz;
	sysctl(mib, 2, coremap, &size, NULL, 0);
	
	map = (struct mapent *)coremap;
	for(tfree = i = 0; i < cmsz / sizeof(struct mapent); i++)
		tfree += map[i].m_size;
	*totmem = physmem;
	*freemem = (long) tfree*64;
	return(0);
}

/*
 * Get Swap Statistics for summary lines
 * Returns:
 *   totmem  - total physical memory in bytes
 *   freemem - available memory in bytes
 */

static	int GetSwapStats(totswap, freeswap)
	unsigned long *totswap;
	unsigned long *freeswap;
{
	register int	i;
	int		mib[2];
	size_t		size;
	static size_t	smsz	= 0;
	static unsigned int	nswap = 0;
	static char	*swapmap = NULL;
	struct mapent	*map;
	unsigned long	tfree;

	if(nswap == 0)
	{
		/* first call - get static values */
		mib[0] = CTL_VM;
		mib[1] = VM_NSWAP;
		size = sizeof (nswap);
		sysctl(mib, 2, &nswap, &size, NULL, 0);
	
		mib[0] = CTL_VM;
		mib[1] = VM_SWAPMAP;
		sysctl(mib, 2, NULL, &smsz, NULL, 0);
		
		if((swapmap = (char *)malloc((unsigned)smsz)) == NULL)
		{
			perror("SWAPMAP");
			oom_reset_exit(1);
		}
		memset(swapmap, 0, smsz);
	}
	mib[0] = CTL_VM;
	mib[1] = VM_SWAPMAP;
	size = smsz;
	sysctl(mib, 2, swapmap, &size, NULL, 0);
	
	map = (struct mapent *)swapmap;
	for	(tfree = i = 0; i < smsz / sizeof(struct mapent); i++)
		tfree += map[i].m_size;
	*totswap = (unsigned long)nswap * 512;
	*freeswap = (unsigned long)tfree * 512;
	return(0);
}

static	void
show_stats(npr)
	int npr;
{
	int	i, run, sleep, stop, zombie;
	static int calls = 0;
	int	mib[2];
	size_t	size;
	unsigned long	memsize, memfree;
	unsigned long	swapsize, swapfree;
	long s_time[CPUSTATES], t, etime;
	static long s1_time[CPUSTATES] = {0};

	/* get time */
	time(&now);
	ltp = localtime(&now);
	printw("%s - %02d:%02d:%02d", host, ltp->tm_hour, ltp->tm_min, ltp->tm_sec);

	/* get uptime */

	mib[0] = CTL_KERN;
	mib[1] = KERN_BOOTTIME;
	size = sizeof(boottime);
	if(sysctl(mib, 2, &boottime, &size, NULL, 0) != -1 &&
		boottime.tv_sec != 0)
	{
		int	days, hrs, mins;

		uptime = (now - boottime.tv_sec + 30) / 60;
		mins = uptime % 60;
		uptime /= 60;
		hrs = uptime % 24;
		days = uptime / 24;

		printw(" up %2d day%s", days, days != 1?"s":"");
		printw(", %2d:%02d", hrs, mins);
	}

	/* get user count */

	nusers = 0;
	while(fread(&utmp, sizeof(utmp), 1, ut))
		if(utmp.ut_name[0] != '\0')
			nusers++;
	rewind(ut);
	printw(", %3d user%c", nusers, nusers > 1? 's': '\0');

	/* load average */
	getloadavg(avenrun, sizeof(avenrun)/sizeof(avenrun[0]));
	printw(",  load average:");
	for	(i = 0; i < 3; i++)
		{
		if	(i > 0)
			printw(",");
		printw(" %.2f", avenrun[i]);
		}
	printw("\n");
	/* process status summary */
	run = sleep = stop = zombie = 0;
	for(i = 0; i < npr; i++)
	{
		register struct proc	*p = &proctab[procidx[i]];
		switch(p->p_stat)
		{
			case SSLEEP: sleep++;  break;
			case SWAIT:  sleep++;  break;
			case SRUN:   run++;    break;
			case SIDL:   run++;    break;
			case SZOMB:  zombie++; break;
			case SSTOP:  stop++;   break;
			default:
				zombie++; /* shouldn't happen */
		}
	}
	printw("Tasks:%4d total, %3d running,", npr, run);
	printw(" %4d sleeping, %3d stopped, %3d zombie\n",
		sleep, stop, zombie);

	/* Cpu states */
	lseek(kmem, (long)nl[X_CPTIME].n_value, L_SET);
	read(kmem, s_time, sizeof s_time),
	etime = 0;
	for(i=0; i < CPUSTATES; ++i)
	{
		t = s_time[i];
		s_time[i] -= s1_time[i];
		s1_time[i] = t;
		etime += s_time[i];
	}
	if(etime == 0) etime = 1;

	printw("Cpu  : %5.1f%% us, %5.1f%% ni, %5.1f%% sy, %5.1f%% id\n",
		((float)s_time[0] / etime) * 100.,
		((float)s_time[1] / etime) * 100.,
		((float)s_time[2] / etime) * 100.,
		((float)s_time[3] / etime) * 100.);

	if(GetMemStats(&memsize, &memfree) < 0)
		return;
	if(GetSwapStats(&swapsize, &swapfree) < 0)
		return;
	printw("Mem  : %7.1fK total, %7.1fK free, %7.1fK used (%2ld%%)\n",
		memsize/1024.0, memfree/1024.0, (memsize-memfree)/1024.0,
		(memsize-memfree)*100L/memsize);
	printw("Swap : %7.1fK total, %7.1fK free, %7.1fK used (%2ld%%)\n",
		swapsize/1024.0, swapfree/1024.0, (swapsize-swapfree)/1024.0,
		(swapsize-swapfree)*100L/swapsize);
}

/*
 * show heading information
 */
static	void
show_titles()
	{

	standout();
	printw("  PID USER      PR  NI  TEXT  DATA STACK S     TIME   DTIME COMMAND \n");
	standend();
	}

static	void
show_procs(np, max)
int	np;
int	max;
{
	int	i;

	qsort((char *)procidx, np, sizeof(int), sortcmp);
	for(i = 0; i < np; i++)
	{
		register struct proc	*p = &proctab[procidx[i]];
		register struct udata_s	*a = &userdata[procidx[i]];

		if (idle || a->o_dtime || p->p_stat == SRUN)
		{
			printw("%5d ",	p->p_pid);
			if(a->o_uname != NULL)
				printw("%-8s", a->o_uname);
			else
				printw("%8d", p->p_uid);
			printw("%4d",	p->p_pri);
			printw("%4d",	p->p_nice);
			printw("%5.1fK", (ctob(a->o_tsize))/1024.0);
			printw("%5.1fK", (ctob(p->p_dsize))/1024.0);
			printw("%5.1fK", (ctob(p->p_ssize))/1024.0);
			printw(" %c",   (p->p_stat>6)? '?' : states[p->p_stat]);
			printw("%c", 	(p->p_flag & SLOAD)? ' ':'s');
			printw("%8.2f ", a->o_ttime/fhz);
			printw("%7.2f ", a->o_dtime/fhz);
			if((a->o_comm[0] == 0)&&(p->p_pid == 0))
				printw("%-19s",    "[swapper]");
			else
				printw("%-19s",    a->o_comm);
			if(i >= (max-1))
				break;
			printw("%s", "\n");
		}
	}
}

/*
 * Wait for a key press on stdin, signal  or until tout microseconds
 * have elapsed
 * returns the key value, -1 on error, or zero if timeout occured
 */
static int keypress(usec)
	long usec;
{
	struct timeval	tv;
	fd_set		rfds;

	tv.tv_sec	= usec / 1000000;
	tv.tv_usec	= usec % 1000000; 
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	if(select(1, &rfds, NULL, NULL, &tv) > 0)
	{
		char c;
		if(read(0, &c, 1) != 1)
			return(-1);
		if((c == tc.t_intrc)||(c == tc.t_quitc))
			c = 'q';
		return(c & 0x7f);
	}
	return(0);
}

/*
 * Convert and return a ms value formatted s[.d[d[d]]]
 * returns delay on success
 *        -1 on invalid delay spec
 */
static	long GetDelay(ds)
	char *ds;
{
	register char *p;
	char *dp = NULL;
	long delay = 0;
	register int	dd;

	/*check valid syntax delay spec */
	for(p = ds; *p; p++)
	{
		if(*p == '.')
		{
			if(dp != NULL)
				return(-1);
			dp = p;
			continue;
		}
		if(!isdigit(*p))
			return(-1);
	}
	/* do conversion */
	if(dp != NULL)	/* subseconds specified */
	{
		*dp++ = 0;
		dd = p - dp;

		if((dd > 3)||(dd < 1))
			return(-1);
		delay = atoi(dp);
		if(dd == 1)
			delay *= 100;
		else if(dd == 2)
			delay *= 10;
		delay *= 1000;
	}
	delay += atol(ds)*1000000;
	return(delay);
}

static	char *ReadLine()
{
	char		c;
	static char	lbuff[50];
	char		*lp = lbuff;

	noraw();
	echo();
	while((c = getch()) >= 0)
	{
		if(c == '\n')
		{
			*lp = 0;
			noecho();
			raw();
			return(lbuff);
		}
		if(c == 0x7f)
		{
			if(lp == &lbuff[0])
				continue;
			--lp;
			printw("\b");
			clrtoeol();
			refresh();
			continue;
		}
		*lp++ = c;
		if(lp == &lbuff[sizeof(lbuff)-2])
			break;
	}
	noecho();
	raw();
	return(NULL);
}

#define STATLINES (6)

static	int
do_top()
{
	int	nread, npr;
	char	swapf[20];

	nlist("/unix", nl);
	if(nl[0].n_type == 0)
	{
		fprintf(stderr, "no /unix namelist\n");
		return(1);
	}

	/*
	 * Open files required for obtaining system information:
	 * /dev/kmem - kernel symbols and proc array
	 * /dev/mem  - incore process user struct and stack
	 * swap      - swapped process user struct and stack
	 */
	if((kmem = open(kmemf, 0)) < 0)
	{
		perror(kmemf);
		return(1);
	}
	if((mem = open(memf, 0)) < 0)
	{
		perror(memf);
		return(1);
	}

/*
 * /dev/ entries are small.  /dev/xp0b + null is 10 bytes.  if 20 bytes
 * isn't enough change the entry in /dev and re-run dev_mkdb(8)
*/
	(void)swapdevname(swapf, sizeof (swapf));
	if((swap = open(swapf, 0)) < 0)
	{
		perror(swapf);
		return(1);
	}

	proc_sym        = nl[X_PROC].n_value;
	nproc_sym       = nl[X_NPROC].n_value;
	hz_sym          = nl[X_HZ].n_value;

	readat(kmem, (off_t) nproc_sym, (char *)&nproc,sizeof(nproc));
	readat(kmem, (off_t) hz_sym, (char *)&hz,sizeof(hz));
	
	fhz = hz * 1.0;
	ptsz = nproc * sizeof(struct proc);

	if((proctab = (struct proc *)malloc(ptsz))==NULL)
	{
		fprintf(stderr, "malloc procs\n");
		oom_reset_exit(1);
	}
	
	if((userdata=(struct udata_s *)calloc(nproc, sizeof(struct udata_s)))==NULL)
	{
		fprintf(stderr, "malloc user\n");
		oom_reset_exit(1);
	}

	/*
	 * allocate an indirection table for efficient sorting
	 */
	if( (procidx = (int *) malloc(nproc*sizeof(short))) == NULL)
	{
		fprintf(stderr, "malloc index\n");
		oom_reset_exit(1);
	}
	if((npr = update()) < 0)
		return(1);

	if((ut = fopen(_PATH_UTMP, "r")) == NULL)
	{
		perror(_PATH_UTMP);
		return(1);
	}

	while(1)
	{
		char c = keypress(upd_us);

		if(c != 0)
		{
			if(c == 'q')
			{
				move(LINES-1,0);
				clrtoeol();
				refresh();
				endwin();
				return(0);
			}

			if(c == 's')
			{
				char	*s;

				move(STATLINES-1,0);
				printw("Change delay from %4.3f to ",
					(upd_us *1.0)/1000000);
				clrtoeol();
				refresh();

				s = ReadLine();

				move(STATLINES-1,0);
				clrtoeol();
				move(0,0);
				refresh();

				if (s)
				{
					long newdelay;

					if((newdelay = GetDelay(s)) > 0)
					{
						upd_us = newdelay;
						continue;
					}
					if(*s == 0)
						continue;
				}

				move(STATLINES-1,0);
				printw("Unacceptable delay: %s", s);
				move(0,0);
				refresh();
				sleep(1);
				continue;
			}

			if(c == 'o')
			{
				char	*s;

				move(STATLINES-1,0);
				printw("Order: ");
				clrtoeol();
				refresh();

				s = ReadLine();

				move(STATLINES-1,0);
				clrtoeol();
				move(0,0);
				refresh();

				if (s)
				{
					int newsort;

					newsort = atoi(s);
					if ((newsort >= 0) && (newsort <= 10))
					{
						sort = newsort;
					}
					else
					{
						move(STATLINES-1,0);
						printw("Bad order");
						move(0,0);
						refresh();
						sleep(1);
					}
				}
			}

			if (c == 'I')
			{
				idle = 1-idle;
			}
		}
		if((npr = update()) < 0)
			return(1);

		erase();
		move(0,0);
		show_stats(npr);
		printw("\n");
		show_titles();
		show_procs(npr, LINES-(STATLINES+1));
		move(0,0);
		refresh();
	}
}

static	void usage(p)
	char *p;
{
	fprintf(stderr, "Usage: %s [opts]\n", p);
	fprintf(stderr, "\t-h         show this summary\n");
	fprintf(stderr, "\t-s<delay>  delay in seconds between updates\n");
	fprintf(stderr, "\t-o<order>  select sort column\n");
	fprintf(stderr, "\t-I         do not show idle processes\n");
}

int main(int argc, char *argv[])
{
	int 	status = 0;
	char	ch;
	char	*p;

	if (argc < 2)
	{
		if (argv[1] = getenv("TOP")) argc = 2;
	}

	while((ch = getopt(argc, argv, "hs:o:I")) != EOF)
	{
		switch(ch)
		{
			case 's':
				if((upd_us = GetDelay(optarg)) <= 0)
				{
					fprintf(stderr, "Invalid delay\n");
					exit(1);
				}
				break;		
			case 'o':
				sort = atoi(optarg);
				break;
			case 'I':
				idle = 0;
				break;
			default:
			case 'h':
				usage(argv[0]);
				exit(1);
		}
		argc -= optind;
		argv += optind;
	}

	gethostname(host, sizeof(host)-1);
	host[sizeof(host)-1] = 0;
	if (p = strchr(host, '.')) *p = 0;

	if(config_tty(1) < 0)
		return(1);

	if(do_top() != 0)
		status = 1;

	config_tty(0);
	return(status);
}
