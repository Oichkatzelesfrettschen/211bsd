#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     4.2 (2.11BSD) 2026/5/11";
#endif

#include <stdio.h>
label(s)
char *s;
{
	int i;
	putc('t',stdout);
	for(i=0;s[i];)putc(s[i++],stdout);
	putc('\n',stdout);
}
