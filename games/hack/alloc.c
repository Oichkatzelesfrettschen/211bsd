/* alloc.c - version 1.0.3 (2.11BSD) 2026/8/26 */

#include <stdlib.h>

long *
alloc(lth)
register unsigned lth;
{
	register char *ptr;

	if(!(ptr = malloc(lth)))
		panic("Cannot get %d bytes", lth);
	return((long *) ptr);
}

long *
enlarge(ptr,lth)
register char *ptr;
register unsigned lth;
{
	register char *nptr;

	if(!(nptr = realloc(ptr,lth)))
		panic("Cannot reallocate %d bytes", lth);
	return((long *) nptr);
}
