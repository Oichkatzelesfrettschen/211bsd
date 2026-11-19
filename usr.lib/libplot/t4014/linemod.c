#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)linemod.c     4.2 (2.11BSD) 2026/5/11";
#endif

linemod(s)
char *s;
{
	char c;
	putch(033);
	switch(s[0]){
	case 'l':	
		c = 'd';
		break;
	case 'd':	
		if(s[3] != 'd')c='a';
		else c='b';
		break;
	case 's':
		if(s[5] != '\0')c='c';
		else c='`';
	}
	putch(c);
}
