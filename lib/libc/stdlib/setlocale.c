/*
 * Public domain - 2026/1/23 - ragge
 *
 *	@(#)setlocale.c	 1.1 (2.11BSD) 2026/1/23
 */

/*
 * Set locale. Currently only supports C locale, so trivial function.
 * (C99 7.11).
 */

#include <locale.h>

char *
setlocale(register int category, register const char *locale)
{
	if (category < 0 || category >= _LC_LAST)
		return (NULL);
	if (locale == NULL ||
	    locale[0] == 0 || (locale[0] == 'C' && locale[1] == 0))
		return "C";
	return NULL;
}

