#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that this notice is preserved and that due credit is given
# to the University of California at Berkeley. The name of the University
# may not be used to endorse or promote products derived from this
# software without specific written prior permission. This software
# is provided ``as is'' without express or implied warranty.
#
#	@(#)Makefile	5.3.3 (2.11BSD) 2026/5/12

CFLAGS=	-O -DUNIX -DUNIX_BSD4_2

SRCS=	curses.c	\
	hit.c		\
	init.c		\
	inventory.c	\
	level.c		\
	machdep.c	\
	main.c		\
	message.c	\
	monster.c	\
	move.c		\
	object.c	\
	pack.c		\
	play.c		\
	random.c	\
	ring.c		\
	room.c		\
	save.c		\
	score.c		\
	spec_hit.c	\
	throw.c		\
	trap.c		\
	use.c		\
	zap.c

OV1=	score.o zap.o
OV2=	init.o save.o
OV3=	message.o room.o
OV4=	level.o
OV5=	pack.o trap.o ring.o

BASE=	curses.o	\
	hit.o		\
	inventory.o	\
	machdep.o	\
	main.o		\
	monster.o	\
	move.o		\
	object.o	\
	play.o		\
	random.o	\
	spec_hit.o	\
	throw.o		\
	use.o

OBJS=	${BASE} ${OV1} ${OV2} ${OV3} ${OV4} ${OV5}

all: rogue

rogue: ${OBJS}
	${CC} -i ${BASE} -Z ${OV1} -Z ${OV2} -Z ${OV3} -Z ${OV4} -Z ${OV5} -Y -o $@ -lcurses -ltermlib

clean: FRC
	rm -f ${OBJS} core rogue

depend: FRC
	mkdep ${CFLAGS} ${SRCS}

install: rogue
	install -c -s -m 4711 -o daemon -g staff rogue /usr/games
	install -c -m 644 -o daemon /dev/null /usr/games/lib/rogue.scores

lint: FRC
	lint ${CFLAGS} ${SRCS}

tags: FRC
	ctags ${SRCS}

FRC:

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
