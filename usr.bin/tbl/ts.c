#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)ts.c	4.3 (2.11BSD) 2026/5/9";
#endif

 /* ts.c: minor string processing subroutines */
match (s1, s2)
	register char *s1, *s2;
{
	while (*s1 == *s2)
		if (*s1++ == '\0')
			return(1);
		else
			s2++;
	return(0);
}
prefix(small, big)
	register char *small, *big;
{
int c;
while ((c= *small++) == *big++)
	if (c==0) return(1);
return(c==0);
}
letter (ch)
	{
	if (ch >= 'a' && ch <= 'z')
		return(1);
	if (ch >= 'A' && ch <= 'Z')
		return(1);
	return(0);
	}
numb(str)
	register char *str;
	{
	/* convert to integer */
	register int k;

	for (k=0; *str >= '0' && *str <= '9'; str++)
		k = k*10 + *str - '0';
	return(k);
	}
digit(x)
	{
	return(x>= '0' && x<= '9');
	}
max(a,b)
{
return( a>b ? a : b);
}
tcopy (s,t)
	register char *s, *t;
{
	while (*s++ = *t++);
}
