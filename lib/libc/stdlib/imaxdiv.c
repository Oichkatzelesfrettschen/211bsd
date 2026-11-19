/*
 * Public domain - 2026/1/19 - ragge
 *
 *	@(#)imaxdiv.c	 1.1 (2.11BSD) 2026/1/19
 */

/*
 * Ansi-correct division. (C99 7.8.2.2).
 */

#include <inttypes.h>

imaxdiv_t
imaxdiv(intmax_t num, intmax_t denom)
{
	imaxdiv_t r;

	r.quot = num / denom;
	r.rem = num % denom;
	return r;
}

