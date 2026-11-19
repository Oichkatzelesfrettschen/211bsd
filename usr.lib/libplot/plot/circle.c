#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)circle.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
circle(x,y,r){
	putc('c',stdout);
	putsi(x);
	putsi(y);
	putsi(r);
}
