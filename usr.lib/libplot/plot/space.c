#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)space.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
space(x0,y0,x1,y1){
	putc('s',stdout);
	putsi(x0);
	putsi(y0);
	putsi(x1);
	putsi(y1);
}
