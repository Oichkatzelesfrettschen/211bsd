#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)list.c	4.3	(2.11BSD)	2026/5/8";
#endif

#include "stdio.h"
#include "lrnref.h"
#include "signal.h"

int istop;

list(r)
char *r;
{
	void stop(), intrpt();
	FILE *ft;
	char s[100];

	if (r==0)
		return;
	istop = 1;
	signal(SIGINT, stop);
	ft = fopen(r, "r");
	if (ft != NULL) {
		while (fgets(s, 100, ft) && istop)
			fputs(s, stdout);
		fclose(ft);
	}
	signal(SIGINT, intrpt);
}

stop()
{
	istop=0;
}
