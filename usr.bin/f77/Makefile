#       Makefile        2.2 (2.11BSD) 2026/6/28

# Fortran 77 compiler generating code for the PDP11 using the Ritchie pass 2

#	1996/10/25 - 2.11BSD
#	2020/2/8 - remove unused malloc.c from SRCS
#		   Add -O to CFLAGS.  "Don't use -O here" was a historical relic
#       2025/11/27 - cleanup
#	2026/6/28 - Don't use -O and then run /lib/c2 on the assembly file.
#		    That runs the optimizer twice.  Borrow from pascal/pi and
#		    use tmp/ as the prefix for mkstr.

LD=		ld
STRINGS=	f77_strings
CFLAGS=		-O -I. -w
LDFLAGS= -i

SRCS= 	data.c driver.c equiv.c error.c exec.c expr.c gram.c init.c \
	intr.c io.c lex.c main.c misc.c pdp11.c pdp11x.c \
	proc.c put.c putdmr.c

PASS1OBJECTS=	data.o equiv.o error.o exec.o expr.o gram.o init.o \
		intr.o io.o lex.o main.o misc.o pdp11.o pdp11x.o proc.o \
		put.o putdmr.o

all:	tmp f77 f77pass1

f77:	driver.o pdp11x.o
	${CC} ${LDFLAGS} driver.o pdp11x.o -o f77
	@size f77

f77pass1:	${PASS1OBJECTS}
	${LD} /lib/crt0.o ${LDFLAGS} -o f77pass1 \
		main.o misc.o gram.o lex.o \
		-Z proc.o init.o \
		-Z equiv.o data.o error.o \
		-Y io.o expr.o intr.o exec.o \
		put.o putdmr.o pdp11.o pdp11x.o -lc
	@size f77pass1

gram.o:	gram.c
	rm -f tmp/gram.c
	mkstr - f77_strings tmp/ gram.c
	${CC} ${CFLAGS} -c tmp/gram.c 
	rm -f tmp/gram.c

gram.c:	gram.head gram.dcl gram.expr gram.exec gram.io tokdefs
	( sed <tokdefs "s/#define/%token/" ;\
	cat gram.head gram.dcl gram.expr gram.exec gram.io ) >gram.in
	${YACC} ${YFLAGS} gram.in
	mv y.tab.c gram.c
	rm gram.in

tmp:
	-mkdir tmp

.c.o:
	rm -f tmp/$*.c
	mkstr - f77_strings tmp/ $*.c
	${CC} ${CFLAGS} -c tmp/$*.c
	rm -f tmp/$*.c

driver.o:
	${CC} -c ${CFLAGS} $*.c

pdp11x.o:
	${CC} -c ${CFLAGS} $*.c

defs:	ftypes defines
	touch defs

tokdefs:	tokens
	grep -n . <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs

install: all
	install -c -s -m 755 -o root f77 /usr/bin
	install -c -s -m 755 -o root f77pass1 /usr/libexec
	install -c -m 644 ${STRINGS} /usr/share/misc

clean:
	rm -f f77 f77pass1 *.o gram.c f77_strings
	rm -fr tmp

tags:
	ctags ${SRCS}

data.o:		defs string_defs data.c
driver.o:	defs drivedefs driver.c
equiv.o:	defs string_defs equiv.c
error.o:	defs string_defs error.c
exec.o:		defs string_defs exec.c
expr.o:		defs string_defs expr.c
gram.o:		defs string_defs gram.c
init.o:		defs string_defs init.c
intr.o:		defs string_defs intr.c
io.o:		defs string_defs io.c
lex.o:		defs string_defs tokdefs lex.c
main.o:		defs string_defs main.c
misc.o:		defs string_defs misc.c
pdp11.o:	defs string_defs pdp11.c
proc.o:		defs string_defs proc.c
put.o:		defs string_defs put.c
putdmr.o:	defs string_defs dmrdefs putdmr.c
