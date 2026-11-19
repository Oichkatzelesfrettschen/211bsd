#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)erase.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
erase(){
	putc('e',stdout);
}
