#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)erase.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include "con.h"
erase(){
	int i;
		for(i=0; i<11*(VERTRESP/VERTRES); i++)
			spew(DOWN);
		return;
}
