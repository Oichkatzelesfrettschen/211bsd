/*
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char copyright[] = "Copyright (c) 1990 Regents of the University of California.\nAll rights reserved.\n";
static char SccsId[] = "@(#)@(#)pop_lower.c	2.2 (2.11BSD) 2026/5/12";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>

/* 
 *  lower:  Convert a string to lowercase
 */

pop_lower (buf)
char        *   buf;
{
    char        *   mp;

    for (mp = buf; *mp; mp++)
        if (isupper(*mp) && isupper(*mp)) *mp = (char)tolower((int)*mp);
}
