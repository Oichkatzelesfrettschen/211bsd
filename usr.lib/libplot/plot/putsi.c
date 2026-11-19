#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)putsi.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
putsi(a){
	putc((char)a,stdout);
	putc((char)(a>>8),stdout);
}
