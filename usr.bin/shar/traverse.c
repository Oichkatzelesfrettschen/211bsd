#if	!defined(lint) && defined(DOSCCS)
static	char	sccsid[] = "@(#)traverse.c 1.1 (2.11BSD) 2026/1/30";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>

int isdir(char *path)
	{
	struct stat buf;

	return((stat(path, &buf) ? 0 : (buf.st_mode&S_IFMT)==S_IFDIR));
	}

traverse (path, func)
char	*path;
int 	(*func) ();
	{
	DIR 	*dirp;
	struct	direct	*entry;
	int 	filetype = isdir(path) ? 'd' : 'f';

	(*func) (path, filetype, 0);
	if (filetype == 'd')
		{
		if (chdir(path) == 0)
			{
			if (dirp = opendir("."))
				{
				while (entry = readdir(dirp))
					{
					char	name[MAXNAMLEN];
					(void) strcpy(name, entry->d_name);
					if (strcmp(name, ".") && strcmp(name, ".."))
						traverse(name, func);
					}
				(void) closedir(dirp);
				}
			(void)chdir ("..");
			}
		}
	(*func) (path, filetype, 1);
	}

#ifdef STANDALONE

static	Indent = 0;

tryverse (file, type, pos)
char	*file;
	{
	register int 	in;
	if (pos == 0)
		{
		for (in = 0; in < Indent; in++) putchar ('\t');
		if (type == 'd')
			{
			printf ("%s/\n", file);
			Indent++;
			}
		else puts (file);
		}
	else if (type == 'd') Indent--;
	}

main (argc, argv) char **argv;
	{
	char	*root = argc > 1 ? argv[1] : ".";

	traverse(root, tryverse);
	}
#endif
