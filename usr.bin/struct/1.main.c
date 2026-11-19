#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)1.main.c	4.2	(2.11BSD)	2026/5/9";
#endif

#include <stdio.h>
#include "def.h"
int endbuf;

mkgraph()
	{
	if (!parse())
		return(FALSE);
	hash_check();
	hash_free();
	fingraph();
	return(TRUE);
	}
