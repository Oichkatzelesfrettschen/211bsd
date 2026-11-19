#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)bdef.c	4.2	(2.11BSD)	2026/5/9";
#endif

#define xxtop	100		/* max size of xxstack */
int xxindent, xxval, newflag, xxmaxchars, xxbpertab;
int xxlineno;		/* # of lines already output */
int xxstind, xxstack[xxtop], xxlablast, xxt;
