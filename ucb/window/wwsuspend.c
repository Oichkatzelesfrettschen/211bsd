#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)wwsuspend.c	3.8 (2.11BSD) 2026/5/9";
#endif

/*
 * Copyright (c) 1983 Regents of the University of California,
 * All rights reserved.  Redistribution permitted subject to
 * the terms of the Berkeley Software License Agreement.
 */

#include "ww.h"
#include "tt.h"
#include <signal.h>

wwsuspend()
{
	sig_t	oldsig;

	oldsig = signal(SIGTSTP, SIG_IGN);
	wwend();
	(void) signal(SIGTSTP, SIG_DFL);
	(void) kill(0, SIGTSTP);
	(void) signal(SIGTSTP, SIG_IGN);
	(void) wwsettty(0, &wwnewtty, &wwoldtty);
	(*tt.tt_init)();
	wwredraw();
	(void) signal(SIGTSTP, oldsig);
}
