#	@(#)Makefile	4.5.3	(2.11BSD)	2026/1/28
#
CFLAGS = -O
LDFLAGS= -i

all:	deliv mkey inv hunt refer addbib lookbib sortbib

deliv: deliv1.o deliv2.o
	cc ${LDFLAGS} deliv?.o  -o deliv
mkey: mkey1.o mkey2.o mkey3.o deliv2.o
	cc ${LDFLAGS} mkey?.o deliv2.o -o mkey
inv: inv1.o inv2.o inv3.o inv5.o inv6.o deliv2.o
	cc ${LDFLAGS} inv?.o deliv2.o -o inv
hunt: hunt1.o hunt2.o hunt3.o hunt5.o hunt6.o hunt7.o glue5.o \
          refer3.o hunt9.o shell.o deliv2.o hunt8.o glue4.o tick.o
	cc ${LDFLAGS} hunt?.o refer3.o glue5.o glue4.o shell.o deliv2.o tick.o -o hunt

glue3.o: refer..c
hunt2.o: refer..c
hunt3.o: refer..c
refer0.o: refer..c
refer1.o: refer..c
refer2.o: refer..c
refer3.o: refer..c
refer4.o: refer..c
refer6.o: refer..c
refer5.o: refer..c
refer: glue1.o refer1.o refer2.o refer4.o refer5.o refer6.o mkey3.o \
	  refer7.o refer8.o hunt2.o hunt3.o deliv2.o hunt5.o hunt6.o hunt8.o \
	  glue3.o hunt7.o hunt9.o glue2.o glue4.o glue5.o refer0.o shell.o
	cc ${LDFLAGS} glue?.o refer[01245678].o hunt[2356789].o mkey3.o \
		shell.o deliv2.o -o refer

addbib: addbib.o
	cc ${LDFLAGS} addbib.o -o addbib
lookbib: lookbib.o
	cc ${LDFLAGS} lookbib.o -o lookbib
sortbib: sortbib.o
	cc ${LDFLAGS} sortbib.o -o sortbib

install: all /usr/dict/papers
	-mkdir -p /usr/libexec/refer
	chmod 755 /usr/libexec/refer
	install -c -s -m 755 deliv /usr/libexec/refer
	install -c -s -m 755 mkey /usr/libexec/refer
	install -c -s -m 755 inv  /usr/libexec/refer
	install -c -s -m 755 hunt /usr/libexec/refer
	install -c -s -m 755 refer /usr/bin
	install -c -s -m 755 addbib /usr/bin
	install -c -s -m 755 sortbib /usr/bin
	install -c -m 755 roffbib.sh /usr/bin/roffbib
	install -c -m 755 indxbib.sh /usr/bin/indxbib
	install -c -s -m 755 lookbib /usr/bin
	install -c -m 444 tmac.bib /usr/share/tmac
	(cd /usr/dict/papers; /bin/sh runinv)

clean:
	rm -f refer inv hunt deliv mkey addbib lookbib sortbib *.o
