/* 
 * version.c 2.1  (2.11BSD) 2026/6/18
 */

/*
 *  Print out the version number and die.  Skip the global rcsid, etc and
 * just print a pretty version string
*/

#include <stdio.h>

extern char	*__progname;

void
version()
{

    fprintf(stderr, "%s 2.1 (2.11BSD) 2026/6/18 pl: 14\n", __progname);
    exit(0);
}
