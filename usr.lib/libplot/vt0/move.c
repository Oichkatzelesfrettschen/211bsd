#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)move.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;
extern xnow,ynow;
move(xi,yi){
	struct {char pad,c; int x,y;} p;
	p.c = 9;
	p.x = xnow = xsc(xi);
	p.y = ynow = ysc(yi);
	write(vti,&p.c,5);
}
