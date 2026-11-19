#if     defined(DOSCCS) && !defined(lint)
static char *sccsid = "@(#)expand.c 2.1 (2.11BSD) 2026/3/21";
#endif

/*
 * Do filename expansion with the shell.  Returns a pointer to a
 * static area.
 */

#define EXPAND_BUF	2048

#include <stdio.h>
#include <string.h>
#include <strings.h>

char *
expand(input)
char *input;
{
	FILE *fp, *popen();
	int last;
	char buf[256];
	static char ans[EXPAND_BUF];

	if (input == NULL)
		return(NULL);
	if (*input == '\0')
		return("");
					/* any thing to expand? */
	if (!strpbrk(input, "$*{}[]\\?~")) {
		strcpy(ans, input);
		return(ans);
	}
					/* popen an echo */
	sprintf(buf, "echo %s", input);

	fp = popen(buf, "r");
	fgets(ans, EXPAND_BUF, fp);
	pclose(fp);

	if (!strlen(ans)) {
		strcpy(ans, input);
		return(ans);
	}

	/*
	 * A horrible kludge...  if the last character is not a line feed,
	 * then the csh has returned an error message.  Otherwise zap the
	 * line feed.
	 */
	last = strlen(ans) - 1;
	if (ans[last] != '\n') {
		strcpy(ans, input);
		return(ans);
	}
	else
		ans[last] = '\0';

	return(ans);
}
