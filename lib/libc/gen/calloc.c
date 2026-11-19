#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)calloc.c	5.3 (2.11BSD) 2026/8/19";
#endif LIBC_SCCS and not lint

#include <unistd.h>
#include <stdlib.h>

/*
 * Calloc - allocate and clear memory block
 */
void *
calloc(num, size)
	register unsigned num, size;
{
	register char *p;

	size *= num;
	if (p = malloc(size))
		bzero(p, size);
	return (p);
}
