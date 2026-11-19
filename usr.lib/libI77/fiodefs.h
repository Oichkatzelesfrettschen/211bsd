/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)fiodefs.h	5.3 (2.11BSD) 2026/8/2
 */

/*
 * fortran file i/o type definitions
 */

#include <stdio.h>
#include <sys/types.h>
#include "f_errno.h"

/*
 * If 'tylogical' in f77/init.c is set to TYSHORT and the memory size of a
 * logical is SZLONG then this define must be set.
*/
#define SHORT_LOGICAL

/* Logical Unit Table Size */
#define MXUNIT 100

#define GLITCH '\2'	/* special quote for Stu, generated in f77pass1 */

#define NAMELIST      -2
#define LISTDIRECTED  -1
#define FORMATTED      1

#define ERROR	1
#define OK	0
#define YES	1
#define NO	0

#define STDERR	0
#define STDIN	5
#define STDOUT	6

#define WRITE	1
#define READ	2
#define SEQ	3
#define DIR	4
#define FMT	5
#define UNF	6
#define EXT	7
#define INT	8

typedef char ioflag;
typedef long ftnint;
typedef ftnint flag;
typedef long ftnlen;

typedef struct		/*external read, write*/
{	flag cierr;
	ftnint ciunit;
	flag ciend;
	char *cifmt;
	ftnint cirec;
} cilist;

typedef struct		/*internal read, write*/
{	flag icierr;
	char *iciunit;
	flag iciend;
	char *icifmt;
	ftnint icirlen;
	ftnint icirnum;
	ftnint icirec;
} icilist;

typedef struct		/*open*/
{	flag oerr;
	ftnint ounit;
	char *ofnm;
	ftnlen ofnmlen;
	char *osta;
	char *oacc;
	char *ofm;
	ftnint orl;
	char *oblnk;
} olist;

typedef struct		/*close*/
{	flag cerr;
	ftnint cunit;
	char *csta;
} cllist;

typedef struct		/*rewind, backspace, endfile*/
{	flag aerr;
	ftnint aunit;
} alist;

typedef struct		/*units*/
{	FILE *ufd;	/*0=unconnected*/
	char *ufnm;
	ino_t uinode;
	int url;	/*0=sequential*/
	char useek;	/*true=can backspace, use dir, ...*/
	char ufmt;
	char uprnt;
	char ublnk;
	char uend;
	char uwrt;	/*last io was write*/
	char uscrtch;
	char uapnd;	/* 1 if access=sequential and opneof set */
} unit;

typedef struct		/* inquire */
{	flag inerr;
	ftnint inunit;
	char *infile;
	ftnlen infilen;
#ifdef	SHORT_LOGICAL
	short	*inex;	/* parameters in standard's (and compiler's) order */
	short	*inopen;
#else
	ftnint	*inex;
	ftnint	*inopen;
#endif
	ftnint	*innum;
#ifdef	SHORT_LOGICAL
	short	*innamed;
#else
	ftnint	*innamed;
#endif
	char	*inname;
	ftnlen	innamlen;
	char	*inacc;
	ftnlen	inacclen;
	char	*inseq;
	ftnlen	inseqlen;
	char 	*indir;
	ftnlen	indirlen;
	char	*inform;
	ftnlen	informlen;
	char	*infmt;
	ftnint	infmtlen;
	char	*inunf;
	ftnlen	inunflen;
	ftnint	*inrecl;
	ftnint	*innrec;
	char	*inblank;
	ftnlen	inblanklen;
} inlist;

typedef union
{	float pf;
	double pd;
} ufloat;

typedef union
{	short is;
	char ic;
	long il;
} uint;

struct ioiflg {
	short if_oeof;
	short if_ctrl;
	short if_bzro;
};
#define	opneof	ioiflg_.if_oeof
#define	ccntrl	ioiflg_.if_ctrl
#define	blzero	ioiflg_.if_bzro
