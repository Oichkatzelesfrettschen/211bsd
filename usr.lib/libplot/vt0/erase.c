#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)erase.c     4.2 (2.11BSD) 2026/5/11";
#endif

extern vti;
erase(){
	int i;
	i=0401;
	write(vti,&i,2);
}
