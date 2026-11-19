#
# Public Domain.  1996/3/14 - Steven Schultz
#
#	@(#)Makefile	1.2 (2.11BSD) 2026/4/5
#
CFLAGS=	 -O
LDFLAGS= -i
SRCS=	mkerrlst.c
OBJS=	mkerrlst.o
MAN=	mkerrlst.0
MANSRC=	mkerrlst.1

all: mkerrlst mkerrlst.0

mkerrlst: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} -lerrlst

mkerrlst.0: ${MANSRC}
	/usr/man/manroff ${MANSRC} > ${MAN}

clean:
	rm -f ${OBJS} ${MAN} mkerrlst tags 

depend: ${SRCS}
	mkdep ${CFLAGS} ${SRCS}

install: mkerrlst
	install -c -o root -g staff -m 444 ${MAN} /usr/man/cat1
	install -c -s -o root -g staff -m 755 mkerrlst /usr/bin/mkerrlst
	/usr/bin/mkerrlst

lint: ${SRCS}
	lint -hax ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
