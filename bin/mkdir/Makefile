#       Makefile        1.0 (2.11BSD) 2026/1/27

SRCS=	mkdir.c
OBJS=	mkdir.o
CFLAGS=	-O
LDFLAGS= -i

all:	mkdir mkdir.0

mkdir.0: mkdir.1
	/usr/man/manroff mkdir.1 > mkdir.0

mkdir: ${OBJS}
	cc ${LDFLAGS}  -o mkdir ${OBJS}

install: all
	install -c -o root -g staff -m 444 mkdir.0 /usr/man/cat1/mkdir.0
	install -c -s -o root -g staff -m 755 mkdir /bin/mkdir

clean:
	rm -f ${OBJS} *.0 mkdir
