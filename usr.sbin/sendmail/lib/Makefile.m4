#
#  Sendmail
#  Copyright (c) 1983  Eric P. Allman
#  Berkeley, California
#
#  Copyright (c) 1983 Regents of the University of California.
#  All rights reserved.  The Berkeley software License Agreement
#  specifies the terms and conditions for redistribution.
#
#	@(#)Makefile.m4	5.2 (2.11BSD) 2026/5/1
#

ALL=	sendmail.hf
SRCS=	sendmail.hf

all: FRC
clean: FRC
depend: FRC

install: sendmail.hf
	install -c -o root -m 444 sendmail.hf /usr/share/misc/sendmail.hf

FRC:
