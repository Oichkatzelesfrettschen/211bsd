/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)query.c     5.2 (2.11BSD) 2026/5/12";
#endif

# include	"robots.h"

/*
 * query:
 *	Ask a question and get a yes or no answer.  Default is "no".
 */
query(prompt)
char	*prompt;
{
	register int	c, retval;
	register int	y, x;

	getyx(stdscr, y, x);
	move(Y_PROMPT, X_PROMPT);
	addstr(prompt);
	clrtoeol();
	refresh();
	retval = ((c = getchar()) == 'y' || c == 'Y');
	move(Y_PROMPT, X_PROMPT);
	clrtoeol();
	move(y, x);
	return retval;
}
