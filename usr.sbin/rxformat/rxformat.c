/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)rxformat.c	5.4 (2.11BSD) 2026/5/10";
#endif

#include <stdio.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pdpuba/rxreg.h>

char *devname;

/*
 * Format RX02 floppy disks.
 */

main(argc, argv)
	int argc;
	char *argv[];
{
	int fd, filarg = 1;
	int i, c;

	if (argc != 2)
		usage();
	devname = argv[1];
	if ((fd = open(devname, O_RDWR)) < 0) {
		perror(devname);
		exit(1);
	}
	if (isatty(fileno(stdin))) {
		printf("Format %s (y/n)? ", devname);
		i = c = getchar();
		while (c != '\n' && c != EOF)
			c = getchar();
		if (i != 'y')
			exit(0);
	} else
		printf("Formatting %s\n", devname);
	if (ioctl(fd, RXIOC_FORMAT) == 0)
		exit(0);
	else {
		perror(devname);
		exit(1);
	}
}

usage()
{
	fprintf(stderr, "usage: rxformat /dev/rrx??\n");
	exit(1);
}
