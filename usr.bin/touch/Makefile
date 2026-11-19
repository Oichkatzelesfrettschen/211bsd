#       Makefile        1.0 (2.11BSD) 2026/1/21

SRCS=	touch.c
OBJS=	touch.o
CFLAGS=	-O
LDFLAGS= -i

all:	touch touch.0

touch.0: touch.1
	/usr/man/manroff touch.1 > touch.0

touch: ${OBJS}
	cc ${LDFLAGS}  -o touch ${OBJS}

install: all
	install -c -o root -g staff -m 444 touch.0 /usr/man/cat1/touch.0
	install -c -s -o root -g staff -m 755 touch /usr/bin/touch

clean:
	rm -f ${OBJS} *.0 touch
