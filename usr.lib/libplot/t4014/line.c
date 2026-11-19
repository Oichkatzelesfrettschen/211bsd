#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)line.c     4.2 (2.11BSD) 2026/5/11";
#endif

line(x0,y0,x1,y1){
	move(x0,y0);
	cont(x1,y1);
}
