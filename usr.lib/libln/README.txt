2026/5/12
---------
This library is used in only two places in the entire system:

./lib/ccom/TEST/Makefile:  ${CC} ${LDFLAGS} ${CFLAGS} -o old-assn lex.yy.c -lln
./usr.bin/struct/Makefile: cc ${LDFLAGS} -O beauty.o tree.o lextab.o bdef.o -o beautify -lln

