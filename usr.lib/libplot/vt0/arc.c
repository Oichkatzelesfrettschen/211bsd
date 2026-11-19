#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)arc.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;
arc(xi,yi,x0,y0,x1,y1){
	char c;
	c = 6;
	write(vti,&c,1);
	write(vti,&xi,12);
}
