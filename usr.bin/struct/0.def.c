#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)0.def.c	4.2	(2.11BSD)	2026/5/9";
#endif

#include <stdio.h>
#include "def.h"

int routnum;
FILE *debfd;
LOGICAL routerr;
int nodenum, accessnum;
int **graph;
int progtype;
VERT stopvert, retvert;
VERT START;
