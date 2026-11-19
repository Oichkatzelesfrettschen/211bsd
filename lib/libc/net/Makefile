#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that this notice is preserved and that due credit is given
# to the University of California at Berkeley. The name of the University
# may not be used to endorse or promote products derived from this
# software without specific prior written permission. This software
# is provided ``as is'' without express or implied warranty.
#
#	@(#)Makefile	5.17 (2.11BSD) 2026/9/21
#
SRCS=	getnbyaddr.c getnbyname.c getnent.c getpent.c getpname.c	\
	getproto.c getsbyname.c getsbyport.c getsent.c herror.c 	\
	rcmd.c res_comp.c res_debug.c res_init.c res_mkquery.c		\
	res_query.c res_send.c	\
	rexec.c ruserpass.c

OBJS=	getnbyaddr.o getnbyname.o getnent.o getpent.o getpname.o	\
	getproto.o getsbyname.o getsbyport.o getsent.o rcmd.o herror.o	\
	rcmd.o res_comp.o res_debug.o res_init.o res_mkquery.o		\
	res_query.o res_send.o rexec.o ruserpass.o

CFLAGS=	-O ${DEFS}

.c.o:
	${CC} -p -c ${CFLAGS} $*.c
	-ld -X -r $*.o
	mv a.out profiled/$*.o
	${CC} ${CFLAGS} -c $*.c
	-ld -x -r $*.o
	mv a.out $*.o

netlib netlib_p: ${OBJS}
	@echo "building profiled netlib"
	@cd profiled; ar cru ../netlib_p ${OBJS}
	@echo "building normal netlib"
	@ar cru netlib ${OBJS}

clean:
	rm -f *.o profiled/*.o errs a.out core netlib netlib_p tags

depend:
	mkdep ${CFLAGS} ${SRCS}
