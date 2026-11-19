#if	!defined(lint) && defined(DOSCCS)
static char *sccsid = "@(#)deliv1.c	4.2 (2.11BSD) 2026/1/28";
#endif

#include <stdio.h>
#include <strings.h>

main(argc, argv)	/* goes from file:begin,l to actual characters */
char *argv[];
{
	FILE *fa = NULL;
	char line[750], name[100];
	register char *p;
	long lp;
	int len;

	if (argc > 1 && argv[1] && argv[1][0])
		chdir(argv[1]);
	name[0] = NULL;
	while (fgets(line, sizeof line, stdin))
	{
		p = index(line, '\0');
		if (p) *p = '\0';

		if (line[0] == '$' && line[1] == '$') 
		{
			chdir(line+2);
			continue;
		}
		for (p = line; *p != ':'; p++)
			;
		*p++ = 0;
		sscanf(p, "%ld,%d", &lp, &len);
		if (p == line)
			fa = stdin;
		else
			if (strcmp(name, line) != 0)
			{
				if (fa != NULL)
					fclose(fa);
				fa = fopen(line, "r");
				if (fa == NULL)
					err("Can't open %s", line);
				strcpy(name, line);
			}
		if (fa != NULL)
		{
			fseek (fa, lp, 0);
			fread (line, 1, len, fa);
			line[len] = 0;
			fputs(line, stdout);
		}
	}
}
