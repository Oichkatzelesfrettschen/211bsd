/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)circle.c     5.2 (2.11BSD) 2026/5/11";
#endif

circle(x,y,r){
	arc(x,y,x+r,y,x+r,y);
}
