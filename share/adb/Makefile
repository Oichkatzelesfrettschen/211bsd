#
# Public domain - 1996/10/21 - sms
#
#	@(#)Makefile	1.2 (2.11BSD) 2026/8/12
#

SRCS= buf callout callout.next file ifaddr ifnet inode ip label mbstat \
	dkdevice dkpartition \
	mbuf mbuf.nxt mbufs mbufs.nxt proc protosw rusage socket tcpcb u xip

all:	${SRCS}

install:${SRCS}
	-mkdir -p /usr/share/adb
	-chmod a+r,a+x /usr/share/adb
	install -c -m 444 -o root -g wheel ${SRCS} /usr/share/adb

clean:
