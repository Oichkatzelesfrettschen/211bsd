# Public domain - 1997/1/10 - sms
#
#	@(#)Makefile	1.2 (2.11BSD) 2026/1/26
#
# DESTDIR is defined here to be something which does *NOT* exist and should be
# the same as specified in ../Makefile (See the comments in ../Makefile)
#
# $HOME of 'root' is now /root BUT a minimal /.profile must be present to set
# PATH in single user mode.  Without /.profile sh can't find fsck (for example)

DESTDIR=/foobar
OWN=root
GRP=wheel

# all the usual targets do nothing
all install depend lint clean tags:

distribution:
	install -c -o ${OWN} -g ${GRP} -m 644 dot.cshrc ${DESTDIR}/root/.cshrc
	install -c -o ${OWN} -g ${GRP} -m 644 dot.login ${DESTDIR}/root/.login
	install -c -o ${OWN} -g ${GRP} -m 644 dot.profile ${DESTDIR}/root/.profile
	install -c -o ${OWN} -g ${GRP} -m 644 dot.minimal ${DESTDIR}/.profile
