#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)subr.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern float obotx;
extern float oboty;
extern float boty;
extern float botx;
extern float scalex;
extern float scaley;
xsc(xi){
	int xa;
	xa = (xi-obotx)*scalex+botx;
	return(xa);
}
ysc(yi){
	int ya;
	ya = (yi-oboty)*scaley+boty;
	return(ya);
}
