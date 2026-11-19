/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)label.c     5.2 (2.11BSD) 2026/5/11";
#endif

#include "dumb.h"

label(string)
	char *string;
{
	while(*string != '\0' && currentx < COLS){
		screenmat[currentx][currenty] = *string++;
		currentx++;
	}
	if(currentx == COLS)
		currentx = currentx-1;

}
