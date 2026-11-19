#
# 	#(@)Makefile	2.2 (2.11BSD) 2026/5/13
#
CFLAGS=	-O
LDFLAGS= -i

# Programs that live in subdirectories, and have makefiles of their own.
SUBDIR= afio cxref ddd less mkovmake mtools welcome qterm zmodem

# C programs that live in the current directory and do not need
# explicit make lines.

# Manpage directory
MANDIR= /usr/local/man

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make

FRC:

install: FRC
	-mkdir -p ${MANDIR}/cat1
	-chmod 755 ${MANDIR} ${MANDIR}/cat1
	-for i in ${SUBDIR}; do \
		(cd $$i; make install); done
	-sh /usr/sbin/makewhatis ${MANDIR}

clean:
	for i in ${SUBDIR}; do (cd $$i; make clean); done

depend:
