#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;

label(s)
char *s;
{
	int i, o;

	o = 01401;
	write(vti, &o, 2);
	for(i=0; s[i++]; )
		;
	write(vti, s, i);
}
