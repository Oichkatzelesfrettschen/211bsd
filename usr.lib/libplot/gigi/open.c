/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)open.c     5.2 (2.11BSD) 2026/5/11";
#endif

/*
 * Displays plot files on a gigi "graphics" terminal.
 */

#include <signal.h>
#include "gigi.h"

int currentx = 0;
int currenty = 0;
double lowx = 0.0;
double lowy = 0.0;
double scalex = 1.0;
double scaley = 1.0;

openpl()
{
	sig_t closepl;

	/* catch interupts */
	signal(SIGINT, closepl);
	currentx = 0;
	currenty = 0;
	/* enter grapics mode */
	putchar(ESC); putchar('P'); putchar('p');

	/* set some parameters */
	printf("S(I0 T0 [0,0])");

	space(0, 0, XMAX, YMAX);
}
