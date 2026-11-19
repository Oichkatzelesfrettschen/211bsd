/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)out.c     5.2 (2.11BSD) 2026/5/12";
#endif

# include	"trek.h"

/*
**  Announce Device Out
*/

out(dev)
int	dev;
{
	register struct device	*d;

	d = &Device[dev];
	printf("%s reports %s ", d->person, d->name);
	if (d->name[length(d->name) - 1] == 's')
		printf("are");
	else
		printf("is");
	printf(" damaged\n");
}
