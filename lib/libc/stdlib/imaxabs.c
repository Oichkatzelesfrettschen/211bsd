/*
 * Public domain - 2026/1/19 - ragge
 *
 *      @(#)imaxdiv.c    1.1 (2.11BSD) 2026/1/19
 */

/*
 * Get absolute value of integer j (C99 7.8.2.1).
 */
 
#include <inttypes.h>

intmax_t
imaxabs(intmax_t j)
{
	return j < 0 ? -j : j;
}
