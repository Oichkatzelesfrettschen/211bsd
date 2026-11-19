#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)move.c     4.2 (2.11BSD) 2026/5/11";
#endif

move(xi,yi){
		movep(xconv(xsc(xi)),yconv(ysc(yi)));
		return;
}
