/* "@(#)pdp11x.c 1.1 (2.11BSD) 2026/6/28"; */

#include <stdio.h>
#include "defines"
#include "pdp11defs"

prchars(fp, s)
FILE *fp;
register int *s;
{
register int i;

fprintf(fp, ".byte ");
for(i = 0; i<SZSHORT ; ++i)
	fprintf(fp, "%o%c", *s++, (i==SZSHORT-1 ? '\n' : ',' ) );
}

prskip(fp, k)
FILE *fp;
ftnint k;
{
/* works around bug in the pdp11 assembler that bombs on  . = .+x  for x>= 32768 */
for( ; k > 30000 ; k -= 30000)
	fprintf(fp, "\t. = .+30000.\n");
fprintf(fp, "\t. = .+%ld.\n", k);
}
