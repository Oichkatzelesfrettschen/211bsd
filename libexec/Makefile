#
#	@(#)Makefile	1.8 (2.11BSD) 2026/6/25
#
CFLAGS=	-O
TAGSFILE=tags

SUBDIR=	acctd comsat ctimed fingerd ftpd getty identd makekey \
	popper rexecd rlogind rshd talkd tcpd telnetd tftpd

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS}

FRC:
	
install:
	-for i in ${SUBDIR}; do \
		(cd $$i; make ${MFLAGS} \
			install); done

tags:
	cd libc; make ${MFLAGS} TAGSFILE=../${TAGSFILE} tags

clean:
	rm -f a.out core *.s *.o
	-for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
