#       Makefile        2.1 (2.11BSD) 2026/7/11

DATFILES=brd.dat mon.dat prop.dat
CFILES=	monop.c cards.c execute.c getinp.c houses.c jail.c misc.c morg.c \
	print.c prop.c rent.c roll.c spec.c strcmp.c trade.c
OBJS=	monop.o cards.o execute.o getinp.o houses.o jail.o misc.o morg.o \
	print.o prop.o rent.o roll.o spec.o strcmp.o trade.o strings.o
BINDIR=	/usr/games
LIBDIR=	/usr/games/lib
LDFLAGS= -i
CFLAGS=	-O -w
DAT=	brd.dat monop.dat prop.dat
CC=	cc
XSTR=	xstr
CTAGS=	ctags

#
# Be cool about compiling strings.
#
.c.o:
	${CC} -E ${CFLAGS} $*.c | ${XSTR} -c -
	${CC} -c ${CFLAGS} x.c
	mv x.o $*.o

all:	monop cards.pck

monop:	${OBJS}
	${CC} -o monop ${LDFLAGS} ${OBJS}

install: monop cards.pck
	rm -f ${BINDIR}/monop ${LIBDIR}/cards.pck
	install -c -s -m 755 monop ${BINDIR}
	mv cards.pck ${LIBDIR}

strings.o: strings
	${XSTR}
	${CC} -c -o strings.o xs.c
	rm -f xs.c

monop.o: ${DATFILES} monop.def
	${CC} -E ${CFLAGS} monop.c | ${XSTR} -c -
	${CC} -c ${CFLAGS} x.c
	mv x.o monop.o

cards.o: deck.h
	${CC} -E ${CFLAGS} cards.c | ${XSTR} -c -
	${CC} -c ${CFLAGS} x.c
	mv x.o cards.o

ctags:
	${CTAGS} ${CFILES} monop.h deck.h monop.def initdeck.c

cards.pck: initdeck cards.inp
	./initdeck

initdeck: initdeck.c deck.h
	${CC} -o initdeck ${CFLAGS} initdeck.c

clean:
	rm -f ${OBJS} monop tags x.c x.o monop.tar initdeck cards.pck errs strings
