#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)chksum.c	4.3 (2.11BSD) 2026/5/9";
#endif

chksum(s,n)
register char *s;
register n;
{
	register unsigned sum, t;
	register x;

	sum = -1;
	x = 0;

	do {
		if (sum&0x8000) {
			sum <<= 1;
			sum++;
		} else
			sum <<= 1;
		t = sum;
		sum += (unsigned)*s++ & 0377;
		x += sum^n;
		if ((sum&0xffff) <= (t&0xffff)) {
			sum ^= x;
		}
	} while (--n > 0);

	return sum & 0xffff;
}
