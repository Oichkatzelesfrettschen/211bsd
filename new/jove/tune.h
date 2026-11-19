#if     defined(DOSCCS) && !defined(lint)
static char *sccsid = "@(#)tune.h 2.0 (2.11BSD) 2026/7/27";
#endif

/*
 * This program is Copyright (C) 1986, 1987, 1988 by Jonathan Payne.  JOVE
 * is provided to you without charge, and with no warranty.  You may give
 * away copies of JOVE, including sources, provided that this notice is
 * included in all the files.
 */

#define TUNED		/* don't touch this */

#define	UNIX

#define BSD4_2	/* Berkeley 4.2 BSD */
#define BSD4_3	/* Berkeley 4.3 BSD and 2.10 BSD */

#define SMALL
#define BUFSIZ	512
#define NBUF	3

/*
 * The getpwent code in get_hdir in io.c brings in the DBM routines which
 * have substantial resource (both text and data). It's better to use 
 * the less general version of get_hdir.
 */
#define NOGETPWENT

#define LOAD_AV	/* Use the load average for various commands.
#define JOB_CONTROL	/* if you have job stopping */
#define IPROCS	/* Interactive processes only work with JOB_CONTROL. */

typedef	unsigned short	disk_line;

#define BACKUPFILES	/* enable the backup files code */
#define F_COMPLETION	/* filename completion */
#define ABBREV		/* word abbreviation mode */
#define ID_CHAR      /* include code to IDchar */
#define WIRED_TERMS  /* include code for wired terminals */
#define CHDIR		/* cd command and absolute pathnames */
#define LISP		/* include the code for Lisp Mode */
#define CMT_FMT		/* include the comment formatting routines */

#define BIFF		/* if you have biff (or the equivalent) */
#define KILL0	/* kill(pid, 0) returns 0 if proc exists */
#define SPELL	/* spell words and buffer commands */

#define DFLT_MODE	0666	/* file will be created with this mode */

#define RESHAPING	/* enable windows to handle reshaping */

#define	byte_copy bcopy	/* use fast assembler version */

#define INPUT_SIG	SIGIO

/* These are here since they define things in tune.c.  If you add things to
   tune.c, add them here too, if necessary. */

#ifndef NOEXTERNS
extern char
	*d_tempfile,
	*p_tempfile,
	*Recover,
	*Joverc,
	*CmdDb,
	TmpFilePath[],
	Shell[],
	ShFlags[];
#endif /* NOEXTERNS */

extern long	SigMask;

#define	sighold(s)	sigblock(SigMask |= 1L << ((s)-1))
#define	sigrelse(s)	sigsetmask(SigMask &= ~(1L << ((s)-1)))
