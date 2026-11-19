#
#	Makefile for sendmail base directory
#
#		@(#)Makefile	4.9.4	(2.11BSD) 2026/5/1
#

ALL=	src/sendmail
DIRS=	/usr/spool/mqueue
#
SENDMAIL=/usr/sbin/sendmail
SYSLOG=	/usr/spool/mqueue/syslog
HELPFILE=/usr/share/misc/sendmail.hf
STATUSFILE=/var/log/sendmail.st
STRINGFILE=/usr/share/misc/sendmail.sr
FROZENFILE=/etc/sendmail.fc

LN=	ln -s

all:
	cd lib; m4 Makefile.m4 > Makefile; make ${MFLAGS}
	cd src; m4 Makefile.m4 > Makefile; make ${MFLAGS} all

install: $(ALL) $(DIRS)
	install -c -s -o root -m 4755	src/sendmail	$(SENDMAIL)
	install -c -o root -m 644	src/sendmail.sr	$(STRINGFILE)
	install -c -o root -m 666	/dev/null	$(STATUSFILE)
	install -c -o root -m 644	/dev/null	$(FROZENFILE)
	cd lib; make install
	rm -f /usr/bin/newaliases
	$(LN) ../sbin/sendmail /usr/bin/newaliases
	rm -f /usr/bin/mailq
	$(LN) ../sbin/sendmail  /usr/bin/mailq

src/sendmail:
	cd src; make ${MFLAGS} sendmail

/usr/spool/mqueue:
	-mkdir			/usr/spool/mqueue
	-chmod	777		/usr/spool/mqueue

clean:
	-cd src; make ${MFLAGS} clean
	-cd cf;  make ${MFLAGS} clean
	-cd lib; make ${MFLAGS} clean
	-cd doc; make ${MFLAGS} clean
