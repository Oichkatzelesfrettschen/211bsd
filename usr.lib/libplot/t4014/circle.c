#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)circle.c     4.2 (2.11BSD) 2026/5/11";
#endif

circle(x,y,r){
	arc(x,y,x+r,y,x+r,y);
}
