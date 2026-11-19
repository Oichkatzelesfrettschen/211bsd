#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)lrntee.c	4.4	(2.11BSD)	2026/5/8";
#endif

main()
{
	int f;
	char c;

	f = creat(".ocopy", 0666);
	while (read(0, &c, 1) == 1) {
		write (1, &c, 1);
		put(c, f);
	}
	fl(f);
	close(f);
}

static char ln[512];
char *p = ln;

put(c, f)
{
	*p++ = c;
	if (c == '\n') {
		fl(f);
		p=ln;
	}
}
fl(f)
{
	register char *s;

	s = ln;
	while (*s == '%' && *(s+1) == ' ')
		s += 2;
	write(f, s, p-s);
}
