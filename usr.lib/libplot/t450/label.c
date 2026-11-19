#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include "con.h"
label(s)
char *s;
{
	int i,c;
		while((c = *s++) != '\0'){
			xnow += 6;
			spew(c);
		}
		return;
}
