#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)circle.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;
circle(x,y,r){
	char c;
	c = 5;
	write(vti,&c,1);
	write(vti,&x,6);
}
