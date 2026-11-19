/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)trace.h	7.2 (2.11BSD) 2026/5/13
 */

/*
 * File system buffer tracing points; all trace <pack(dev, size), bn>
 *
 * Slipped into 2.10 in about 15 minutes.  Bears only a casual
 * relationship to the 4.X code, we just used the same names so
 * someone could do it right if they felt like it.
 *
 * Version 7.2 removed non 2.11BSD defines and relocated tracebuf[] to ufs_bio.c
 */
#define	TR_BREADHIT	0	/* buffer read found in cache */
#define	TR_BREADMISS	1	/* buffer read not in cache */
#define	TR_BWRITE	2	/* buffer written */
#define	TR_BREADHITRA	3	/* buffer read-ahead found in cache */
#define	TR_BREADMISSRA	4	/* buffer read-ahead not in cache */
#define	TR_BRELSE	5	/* brelse */
#define	TR_SWAPIO	6	/* swap i/o request */
#define	TR_NUM_210	7	/* size of array for 2.10BSD */

#if defined(KERNEL) && defined(UCB_METER) && !defined(SUPERVISOR)
#define	trace(a)	tracebuf[a]++;
extern u_long tracebuf[TR_NUM_210];
#else
#define trace(a) ;
#endif
