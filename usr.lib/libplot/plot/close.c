#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)close.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
closevt(){
	fflush(stdout);
}
closepl(){
	fflush(stdout);
}
