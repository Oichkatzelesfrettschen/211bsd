/*
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char copyright[] = "Copyright (c) 1990 Regents of the University of California.\nAll rights reserved.\n";
static char SccsId[] = "@(#)@(#)pop_last.c	2.2 (2.11BSD) 2026/5/12";
#endif

#include <stdio.h>
#include <sys/types.h>
#include "popper.h"

/* 
 *  last:   Display the last message touched in a POP session
 */

int pop_last (p)
POP     *   p;
{
    return (pop_msg(p,POP_SUCCESS,"%u is the last message seen.",p->last_msg));
}
