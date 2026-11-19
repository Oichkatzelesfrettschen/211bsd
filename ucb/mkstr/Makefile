#
# @(#)Makefile	1.0 (2.11BSD) 2026/8/26
#

CFLAGS=	-O
LDFLAGS= -i
PROG=	mkstr
SRCS=	mkstr.c
OBJS=	mkstr.o
MAN=	mkstr.0
MANSRC=	mkstr.1

all: ${PROG} ${MAN}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS}

${MAN}:	${MANSRC}
	/usr/man/manroff ${MANSRC} > ${MAN}

clean:
	rm -f ${OBJS} ${MAN} ${PROG}

install: ${PROG} ${MAN}
	install -c -s -o root -g staff -m 755 ${PROG} /usr/ucb
	install -c -o root -g staff -m 444 ${MAN} /usr/man/cat1
