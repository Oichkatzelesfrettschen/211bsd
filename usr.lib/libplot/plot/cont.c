#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)cont.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
cont(xi,yi){
	putc('n',stdout);
	putsi(xi);
	putsi(yi);
}
