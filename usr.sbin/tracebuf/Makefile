#
#	@(#)Makefile	1.0 (2.11BSD) 2026/5/13
#
CFLAGS=	 -O
LDFLAGS= -i
SRCS=	tracebuf.c
OBJS=	tracebuf.o
MAN=	tracebuf.0
MANSRC=	tracebuf.1

all: tracebuf tracebuf.0

tracebuf: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS}

tracebuf.0: ${MANSRC}
	/usr/man/manroff ${MANSRC} > ${MAN}

clean:
	rm -f ${OBJS} ${MAN} tracebuf tags 

depend: ${SRCS}
	mkdep ${CFLAGS} ${SRCS}

install: tracebuf
	install -c -o root -g staff -m 444 ${MAN} /usr/man/cat1
	install -c -s -o root -g kmem -m 2755 tracebuf /usr/sbin

lint: ${SRCS}
	lint -hax ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
