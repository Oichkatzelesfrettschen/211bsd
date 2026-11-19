#
# Public Domain.  1996/10/24 - sms
#
#	@(#)Makefile	1.1 (2.11BSD) 2025/11/25
#

makewhatis.0: makewhatis.8
	/usr/man/manroff makewhatis.8 > makewhatis.0

all: makewhatis.0

clean:
	rm -f makewhatis.0

install: makewhatis.0
	install -c -o root -g staff -m 755 makewhatis.sh /usr/sbin/makewhatis
	install -c -m 444 -o root -g staff makewhatis.0 /usr/man/cat8
