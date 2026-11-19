#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that this notice is preserved and that due credit is given
# to the University of California at Berkeley. The name of the University
# may not be used to endorse or promote products derived from this
# software without specific prior written permission. This software
# is provided ``as is'' without express or implied warranty.
#
#       @(#)Makefile    2.1 (2.11BSD) 2026/5/1
#
# Uncomment these two lines to enable string extractions
CC=	./mkstrcc
XSTR=	/usr/ucb/xstr
EXTRACT=extract.o

SRCS=	alias.c arpadate.c clock.c collect.c conf.c convtime.c daemon.c \
	deliver.c domain.c envelope.c err.c headers.c macro.c main.c \
	parseaddr.c queue.c recipient.c savemail.c srvrsmtp.c \
	stab.c stats.c sysexits.c trace.c usersmtp.c util.c Version.c
BASE=	clock.o conf.o daemon.o envelope.o macro.o main.o queue.o \
	parseaddr.o recipient.o savemail.o stab.o srvrsmtp.o \
	trace.o util.o $(EXTRACT) str.o
OVLY1=	sysexits.o arpadate.o convtime.o readcf.o headers.o
OVLY2=	collect.o deliver.o
OVLY3=	alias.o err.o domain.o stats.o usersmtp.o
OBJS=	${BASE} ${OVLY1} ${OVLY2} ${OVLY3}

CFLAGS=	-O -I. -I../include -DVMUNIX -DMXDOMAIN -DNOSCCS
LDFLAGS=-i

.c.o:
	$(CC) ${CFLAGS} -c $*.c

sendmail: ${OBJS} Version.o
	cc ${LDFLAGS} ${COPTS} -o sendmail Version.o ${BASE} \
		-Z ${OVLY1} -Z ${OVLY2} -Z ${OVLY3} -Y -ldbm -lstubs; \
	size sendmail; ls -l sendmail; ${WHAT} < Version.o

dsendmail: ${OBJS} Version.o
	${CC} ${COPTS} -o sendmail Version.o ${OBJS} -ldbm -lresolv
	size sendmail; ls -l sendmail; ${WHAT} < Version.o

install: sendmail 
	install -c -s -o root -g kmem -m 6755 sendmail /usr/sbin
	install -c -o root -m 644 /dev/null /etc/sendmail.fc
	install -c -o root -m 666 /dev/null /var/log/sendmail.st
	rm -f /usr/bin/newaliases
	ln -s ../sbin/sendmail /usr/bin/newaliases
	rm -f /usr/bin/mailq
	ln -s ../sbin/sendmail /usr/bin/mailq
	-if [ -s sendmail.sr ]; then \
		install -c -o root -m 644 sendmail.sr \
			/usr/share/misc/sendmail.sr; \
	fi

version: ${OBJS} Version.c

Version.c: version.c
	@echo generating Version.c from version.c
	@cp version.c Version.c
	@chmod 644 Version.c
	@echo "" >> Version.c
	@echo "# ifdef COMMENT" >> Version.c
	@echo "" >> Version.c
	@echo "code versions:" >> Version.c
	@echo "" >> Version.c
	@${WHAT} ${OBJS} >> Version.c
	@echo "" >> Version.c
	@echo "# endif COMMENT" >> Version.c

clean: FRC
	rm -f ${OBJS} core sendmail a.out Version.o sendmail.sr mk.c xxmk.c strings

depend: FRC
	mkdep ${CFLAGS} ${SRCS}

str.o:	strings
	${XSTR}
	cc -c xs.c
	mv xs.o str.o
	rm -f xs.c

lint: FRC
	lint -chapbx ${CCONFIG} ${SRCS}

tags: FRC
	ctags ${SRCS}

FRC:
