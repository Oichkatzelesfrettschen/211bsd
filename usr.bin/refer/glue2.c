#if	!defined(lint) && defined(DOSCCS)
static char *sccsid = "@(#)glue2.c	4.2 (2.11BSD) 2026/5/9";
#endif

char refdir[50];

savedir()
{
	if (refdir[0]==0)
		corout ("", refdir, "/bin/pwd", "", 50);
	trimnl(refdir);
}

restodir()
{
	chdir(refdir);
}
