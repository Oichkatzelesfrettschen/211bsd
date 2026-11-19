#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)dot.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
dot(xi,yi,dx,n,pat)
int  pat[];
{
	int i;
	putc('d',stdout);
	putsi(xi);
	putsi(yi);
	putsi(dx);
	putsi(n);
	for(i=0; i<n; i++)putsi(pat[i]);
}
