#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)point.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
point(xi,yi){
	putc('p',stdout);
	putsi(xi);
	putsi(yi);
}
