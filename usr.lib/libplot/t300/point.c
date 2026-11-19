#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)point.c     4.2 (2.11BSD) 2026/5/11";
#endif

point(xi,yi){
		move(xi,yi);
		label(".");
		return;
}
