#if	!defined(lint) && defined(DOSCCS)
static char *sccsid = "@(#)tty.c	4.2 (2.11BSD) 2026/5/8";
#endif

#include <unistd.h>

/*
 * Type tty name
 */

main(argc, argv)
char **argv;
{
	register char *p;

	p = ttyname(0);
	if(argc==2 && !strcmp(argv[1], "-s"))
		;
	else
		printf("%s\n", (p? p: "not a tty"));
	exit(p? 0: 1);
}
