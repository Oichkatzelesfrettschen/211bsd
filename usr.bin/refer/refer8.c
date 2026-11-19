#if	!defined(lint) && defined(DOSCCS)
static char *sccsid = "@(#)refer8.c	4.2 (2.11BSD) 2026/5/9";
#endif

#include "refer..c"

static char ahead[1024];
static int peeked = 0;
static char *noteof = (char *) 1;

char *
input(s)
char *s;
{
	if (peeked) {
		peeked = 0;
		if (noteof == 0)
			return(0);
		strcpy(s, ahead);
		return(s);
	}
	return(fgets(s, 1000, in));
}

char *
lookat()
{
	if (peeked)
		return(ahead);
	noteof = input(ahead);
	peeked = 1;
	return(noteof);
}

addch(s, c)
register char *s;
{
	while (*s)
		s++;
	*s++ = c;
	*s = 0;
}
