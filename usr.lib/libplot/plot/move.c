#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)move.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
move(xi,yi){
	putc('m',stdout);
	putsi(xi);
	putsi(yi);
}
