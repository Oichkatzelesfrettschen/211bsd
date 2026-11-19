#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation,
# advertising materials, and other materials related to such
# distribution and use acknowledge that the software was developed
# by the University of California, Berkeley.  The name of the
# University may not be used to endorse or promote products derived
# from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
#	@(#)Makefile	5.20.4 (2.11BSD) 2026/9/17
#
CFLAGS=	-O
LIBS=	-ll
CSRCS=	main.c getinfo.c debug.c send.c skip.c list.c subr.c res_debug.c
SRCS=	${CSRCS} commands.c
OBJS=	main.o getinfo.o debug.o send.o skip.o list.o subr.o commands.o \
	res_debug.o

all: nslookup

nslookup: ${OBJS}
	${CC} -i -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${OBJS} core nslookup nslookup.0 commands.c lex.yy.c lex.yy.o

depend: ${SRCS}
	mkdep ${CFLAGS} ${SRCS}

install: nslookup
	install -c -s -o root -g staff -m 755 nslookup /usr/bin/nslookup
	install -c -o root -g staff -m 444 nslookup.help /usr/share/misc/nslookup.help
