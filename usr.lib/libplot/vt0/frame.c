#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)frame.c     4.2 (2.11BSD) 2026/5/11";
#endif

frame(n)
{
	extern vti;
	n=n&0377 | 02000;
	write(vti,&n,2);
}
