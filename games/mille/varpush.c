# include	"mille.h"
#include <strings.h>

/*
 * @(#)varpush.c	1.2 (2.11BSD) 2026/1/27
 */

int	read(), write();

/*
 *	push variables around via the routine func() on the file
 * channel file.  func() is either read or write.
 */
varpush(file, func)
register int	file;
register int	(*func)(); {

	int	temp;

	(*func)(file, (char *) &Debug, sizeof Debug);
	(*func)(file, (char *) &Finished, sizeof Finished);
	(*func)(file, (char *) &Order, sizeof Order);
	(*func)(file, (char *) &End, sizeof End);
	(*func)(file, (char *) &On_exit, sizeof On_exit);
	(*func)(file, (char *) &Handstart, sizeof Handstart);
	(*func)(file, (char *) &Numgos, sizeof Numgos);
	(*func)(file, (char *)  Numseen, sizeof Numseen);
	(*func)(file, (char *) &Play, sizeof Play);
	(*func)(file, (char *) &Window, sizeof Window);
	(*func)(file, (char *)  Deck, sizeof Deck);
	(*func)(file, (char *) &Discard, sizeof Discard);
	(*func)(file, (char *)  Player, sizeof Player);
	if (func == read) {
		read(file, (char *) &temp, sizeof temp);
		Topcard = &Deck[temp];
		if (Debug) {
			char	buf[80];
			register char *cp;
over:
			printf("Debug file:");
			buf[0] = '\0';
			fgets(buf, sizeof buf, stdin);
			cp = index(buf, '\n');
			if (cp) *cp = '\0';
			if ((outf = fopen(buf, "w")) == NULL) {
				perror(buf);
				goto over;
			}
			if (strcmp(buf, "/dev/null") != 0)
				setbuf(outf, NULL);
		}
	}
	else {
		temp = Topcard - Deck;
		write(file, (char *) &temp, sizeof temp);
	}
}

