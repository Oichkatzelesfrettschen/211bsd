#       @(#)Makefile    1.1 (2.11BSD) 2026/2/2
#
# Arnold Robbins, Information and Computer Science, Georgia Tech
#	gatech!arnold
# Copyright (c) 1985 by Arnold Robbins.
# All rights reserved.
# This program may not be sold, but may be distributed
# provided this header is included.

# some files are system dependant, e.g. where sort is.
# change the appropriate macro definitions and recompile.

SCANOBJS= docxref.o cscan.o
SCANSRCS= docxref.c cscan.l

CXREF = cxref
INCLS= constdefs.h basename.c
PROGS= docxref fmtxref cxrfilt $(CXREF)
SRCS=  $(SCANSRCS) fmtxref.c cxrfilt.c $(CXREF).c
DOCS=  README makefile cxref.1

CFLAGS= -O
LDFLAGS=-i

### system dependant definitions, change when you install cxref

BIN=${DESTDIR}/usr/local
LIB=/usr/local/lib/cxref

# lex library, may be -lln on some systems
LEXLIB= -ll

# may be /bin/sort on some systems
SORT=/usr/bin/sort

all: $(PROGS) cxref.0

docxref: $(SCANOBJS)
	$(CC) $(LDFLAGS) $(SCANOBJS) $(LEXLIB) -o $@

cscan.o docxref.o cxrfilt.o: $(INCLS)

fmtxref: fmtxref.c
	$(CC) $(LDFLAGS) $(CFLAGS) $@.c $(LDFLAGS) -o $@

cxrfilt: cxrfilt.c
	$(CC) $(LDFLAGS) $(CFLAGS) $@.c $(LDFLAGS) -o $@

$(CXREF): $(CXREF).c
	$(CC) $(LDFLAGS) $(CFLAGS) -DSRCDIR='"$(LIB)"' -DSORT='"$(SORT)"' $@.c $(LDFLAGS) -o $@

install: $(PROGS) cxref.0
# don't remove my bin!
	rm -f ${BIN}/cxref
	-mkdir $(LIB)
	install -s $(CXREF) ${BIN}/$(CXREF)
	install -s docxref  $(LIB)/docxref
	install -s fmtxref  $(LIB)/fmtxref
	install -s cxrfilt $(LIB)/cxrfilt
	install -m 444 -c cxref.0 /usr/local/man/cat1/cxref.0

cxref.0:	cxref.1
	/usr/man/manroff cxref.1 > cxref.0

clean:
	rm -f $(PROGS) $(SCANOBJS) cxref.0
