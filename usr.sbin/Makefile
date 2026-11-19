#
# Public domain - 1996/10/24 - sms
#
#	@(#)Makefile	1.6 (2.11BSD) 2026/5/13

SUBDIR=	ac accton arp bad144 chown chroot cron dev_mkdb \
	edquota inetd \
	lpr makewhatis mkhosts named nc ntp pstat quot \
	quotaon repquota rmt rwhod rxformat sa sendmail sysgen syslogd timed \
	tracebuf traceroute trpt trsp update vipw

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; make ${MFLAGS} all

FRC:

install: FRC
	-for i in ${SUBDIR}; do \
		(cd $$i; make ${MFLAGS} install); done

clean:
	for i in ${SUBDIR}; do (cd $$i; make ${MFLAGS} clean); done
