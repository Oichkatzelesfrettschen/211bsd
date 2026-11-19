#       @(#)Makefile    1.0 (2.11BSD) 2026/1/30

SRCS=	ps.c
OBJS=	ps.o
CFLAGS=	-O -I.

all:	ps

ps: ${OBJS}
	cc -i -o ps ${OBJS}

install:
	install -c -s -o root -g kmem -m 2755 ps /bin

clean:
	rm -f *.o ps
