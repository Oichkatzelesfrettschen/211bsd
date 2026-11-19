#if	!defined(lint) && defined(DOSCCS)
static char *sccsid = "@(#)sleep.c	4.2 (2.11BSD) 2026/5/8";
#endif

main(argc, argv)
char **argv;
{
	register int c, n;
	register char *s;

	n = 0;
	if(argc < 2) {
		printf("arg count\n");
		exit(0);
	}
	s = argv[1];
	while(c = *s++) {
		if(c<'0' || c>'9') {
			printf("bad character\n");
			exit(0);
		}
		n = n*10 + c - '0';
	}
	sleep(n);
}
