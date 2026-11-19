#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)line.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
line(x0,y0,x1,y1){
	putc('l',stdout);
	putsi(x0);
	putsi(y0);
	putsi(x1);
	putsi(y1);
}
