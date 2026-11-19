#       Makefile        2.1 (2.11BSD) 2026/6/29

################################################################################
# A horrid Makefile for EFL, a language even more dead than Latin
#
# I wrote this to make EFL compile on a PDP-11.  It attempts to stuff
# EFL into 64K, and it just barely fits.  The program is barely under 64K
# text, just under 56K bss and data, and 8K stack.
#
# Sorry for the mess, but this is the best I could do while limiting changes
# to the EFL source itself.  I admit I don't understand most of it, so
# confining changes to here is probably a good idea.
#                                       -- Frank Wortner
################################################################################
PROGRAM= efl
CFLAGS = -O -w
LDFLAGS = -i
EFLSHAREDIR = /usr/share/efl

FILES1 =  Makefile efl.1 defs tokens main.c init.c tailor.c\
		gram.head gram.dcl gram.expr gram.exec\
		lex.l fixuplex symtab.c\
		dcl.c addr.c struct.c field.c misc.c alloc.c mk.c
FILES2 =  exec.c blklab.c simple.c print.c temp.c io.c error.c\
		pass2.c icfile.c free.c dclgen.c namgen.c
FILES = $(FILES1) $(FILES2)

LIBS=

BASE=   addr.o alloc.o blklab.o dcl.o dclgen.o error.o exec.o \
        field.o free.o gram.o icfile.o init.o lex.o main.o \
        mk.o misc.o pass2.o print.o struct.o \
        symtab.o tailor.o temp.o 

OV1= namgen.o io.o
OV2= simple.o

OBJECTS = $(BASE) ${OV1} ${OV2}

# Removal of the unusable/incomplete meter() code removed enough code that
# an overlay scheme is not needed.  Keep the commented overlay layout in
# case it's needed in the future.

$(PROGRAM): ${OBJECTS}
#	/bin/ld $(LDFLAGS) -X -o $(PROGRAM) /lib/crt0.o \
#	   -Z ${OV1} -Z ${OV2} -Y ${BASE} -lc
	/bin/ld $(LDFLAGS) -X -o $(PROGRAM) /lib/crt0.o ${OV1} ${OV2} ${BASE} -lc
	size $(PROGRAM)
	./catastrophe $(PROGRAM)

# Extract strings to a file using mkstr.  Unfortunately, that program will 
# rewrite only calls to a subroutine called error().  map calls of dclerr(),
# exprerr(), execerr(), and laberr() to error via some C macros. 
ERRDEFS = -D'dclerr(a, b)=error(a,b,DCLERR)'\
	-D'exprerr(a,b)=error(a,b,EXPERR)'\
	-D'execerr(a,b)=error(a,b,EXECERR)'\
	-D'laberr(a,b)=error(a,b,LABERR)'\
	-D'DCLERR=1'\
	-D'EXPERR=2'\
	-D'EXECERR=3'\
	-D'LABERR=4'

# This creates two annoyances.
# 1) The error file probably should be rebuilt whenever any source
#    is changed.  Basically, you need to do a make clean && make.
# 2) The compiler only sees the rewritten source files, so fixing
#    syntax errors is a pain because line numbers and file names
#    in compiler messages are no longer correct.  Sigh.
#
.c.o:	; /lib/cpp -P $(ERRDEFS) < $*.c >xx$*.c
	mkstr - efl_errorstrings x xx$*.c
	$(CC) $(CFLAGS) -c xxx$*.c
	mv xxx$*.o $*.o
	rm xx$*.c xxx$*.c

$(OBJECTS) : defs

lex.o init.o : tokdefs

gram.c:	gram.head gram.dcl gram.expr gram.exec tokdefs
	( sed <tokdefs "s/#define/%token/" ;\
	cat gram.head gram.dcl gram.expr gram.exec) >gram.in
	$(YACC) $(YFLAGS) gram.in
	(echo "#define YYDEBUG"; cat y.tab.c) > gram.c
	rm -f gram.in y.tab.c

lex.c: fixuplex
	lex lex.l
	sh fixuplex
	mv lex.yy.c lex.c

tokdefs: tokens
	grep -n . <tokens | sed "s/\([^:]*\):\(.*\)/#define \2 \1/" >tokdefs

clean:
	-rm -f efl
	-rm -f efl_errorstrings
	-rm efl[a-z].[0-9][0-9][0-9][0-9]
	-rm -f xx*.c
	-rm -f a.out *.o gram.c lex.c tokdefs 

install: $(PROGRAM) efl_errorstrings
	-@size $(PROGRAM) /usr/bin/efl
	install -c -s -m 755 -o root -g staff $(PROGRAM) /usr/bin/efl
	-mkdir -p $(EFLSHAREDIR)
	-chmod 755 $(EFLSHAREDIR)
	install -c -m 444 -o root -g staff efl_errorstrings $(EFLSHAREDIR)

lint: $(PROGRAM)
	lint -p *.c -lS

# I could not find the original test programs so I substituted some
# example EFL procedures I found in the EFL documentation.
# It's better than nothing.
# The file walk.e currently does not compile on 4.3BSD.  In fact,
# it fails with exactly the same error messages, so I assume that this
# means EFL works on 2.11BSD.  ;-)
test:	$(PROGRAM)
	$(PROGRAM) "system=unix" efltest/george.e >z1 2>z2
	cmp z1 efltest/george.out
	$(PROGRAM) "system=unix" efltest/select.e >z1 2>z2
	cmp z1 efltest/select.out
	$(PROGRAM) "system=unix" efltest/matmul.e >z1 2>z2
	cmp z1 efltest/matmul.out
# This one fails on 4.3BSD as well, so we will just ignore it for now.
	-$(PROGRAM) "system=unix" efltest/walk.e >z1 2>z2
	cmp z1 efltest/walk.out
	rm -f z1 z2
	@echo TEST OK
