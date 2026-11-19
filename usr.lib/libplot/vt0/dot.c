#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)dot.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;
dot(xi,yi,dx,n,pat)
int pat[];
{
	struct {char pad,c; int xi,yi,dx;} p;
	p.c = 7;
	p.xi = xsc(xi);
	p.yi = ysc(yi);
	p.dx = xsc(dx);
	write(vti,&p.c,7);
	write(vti,pat,n?n&0377:256);
}
