/*
 * Public domain - 2026/1/23 - ragge
 *
 *	@(#)localeconv.c	 1.1 (2.11BSD) 2026/1/23
 */

/*
 * Return current locale. (C99 7.11).
 */

#include <locale.h>

extern struct lconv *__lconv;

struct lconv *
localeconv(void)
{
	return __lconv;
}

