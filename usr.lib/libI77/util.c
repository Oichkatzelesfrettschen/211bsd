/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)util.c	5.2 (2.11BSD) 2026/8/4
 */

/*
 * utility routines
 */

#include "fio.h"

nowreading(x) unit *x;
	{
	return(now_acc(x,"r",0));
	}

nowwriting(x) unit *x;
	{
	return(now_acc(x, x->uapnd ? "a" : "r+", 1));
	}

LOCAL now_acc(x,mode, uwrt)
	unit *x; char *mode;
	int uwrt;
	{
	long loc;

	if	(!x->ufnm)
		{
		errno = EBADF;
		return(NO);
		}
	if	(x->useek)
		loc=ftell(x->ufd);
	if	(freopen(x->ufnm, mode, x->ufd))
		{
		if	(x->useek)
			fseek(x->ufd, loc, SEEK_SET);
		x->uwrt = uwrt;
		return(YES);
		}
/*
 * All the callers of nowreading() and nowwriting() check the return value
 * and if NO call err() to announce the failure.  Trying to reopen the
 * file in its previous mode after freopen failed (and closed the file)
 * seems pointless!  Code will need to be updated if enabled.
*/
#ifdef	whybother
        if (x->ufd = fopen(x->ufnm, (*mode=='a')? "r":"a"))
                if (x->useek)
                        fseek(x->ufd,loc, SEEK_SET);
#endif
	return(NO);
	}

g_char(a,alen,b) char *a,*b; ftnlen alen;
{	char *x=a+alen-1, *y=b+alen-1;
	while (x >= a  &&  *x == ' ') {x--; y--;}
	*(y+1) = '\0';
	while (x >= a) *y-- = *x--;
}

b_char(from, to, tolen) char *from, *to; ftnlen tolen;
{	int i=0;
	while (*from && i < tolen) {
		*to++ = *from++;
		i++;
	}
	while (i++ < tolen)
		*to++ = ' ';
}

char
last_char(f) FILE *f;
{
	fseek(f,-2L, SEEK_CUR);
	if(ftell(f)) return(getc(f));
	else return('\n');
}
