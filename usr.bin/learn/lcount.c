#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)lcount.c	4.3	(2.11BSD)	2026/5/8";
#endif

#include "stdio.h"

main()	/* count lines in something */
{
	register n, c;

	n = 0;
	while ((c = getchar()) != EOF)
		if (c == '\n')
			n++;
	printf("%d\n", n);
}
