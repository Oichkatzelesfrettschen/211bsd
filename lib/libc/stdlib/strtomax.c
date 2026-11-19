/*
 * Public domain - 2026/1/19 - ragge
 *
 *      @(#)strtomax.c    1.1 (2.11BSD) 2026/1/19
 */

/*
 * Convert string to intmax/uintmax (C99 7.8.2.3).
 * The same as strtol/strtoul on pdp11.
 * XXX - change when pdp11 gets long long.
 */
 
#include <inttypes.h>

intmax_t
strtoimax(const char *restrict nptr, char **restrict endptr, int base)
{
	return strtol(nptr, endptr, base);
}

uintmax_t
strtoumax(const char * restrict nptr, char ** restrict endptr, int base)
{
	return strtoul(nptr, endptr, base);
}
