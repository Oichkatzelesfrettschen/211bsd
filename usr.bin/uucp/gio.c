#if	!defined(lint) && defined(DOSCCS)
static char sccsid[] = "@(#)gio.c	5.7 (Berkeley) 2026/5/9";
#endif

#include "uucp.h"
#include "pk.h"
#include <setjmp.h>

jmp_buf Failbuf;

int Retries = 0;
struct pack *Pk;

pkfail()
{
	longjmp(Failbuf, 1);
}

gturnon()
{
	struct pack *pkopen();

	if (setjmp(Failbuf))
		return FAIL;
	Pk = pkopen(Ifn, Ofn);
	if (Pk == NULL)
		return FAIL;
	return SUCCESS;
}

gturnoff()
{
	if(setjmp(Failbuf))
		return(FAIL);
	pkclose(Pk);
	return SUCCESS;
}


gwrmsg(type, str, fn)
char type;
register char *str;
{
	char bufr[BUFSIZ];
	register char *s;
	int len, i;

	if(setjmp(Failbuf))
		return(FAIL);
	bufr[0] = type;
	s = &bufr[1];
	while (*str)
		*s++ = *str++;
	*s = '\0';
	if (*(--s) == '\n')
		*s = '\0';
	len = strlen(bufr) + 1;
	if ((i = len % PACKSIZE)) {
		len = len + PACKSIZE - i;
		bufr[len - 1] = '\0';
	}
	gwrblk(bufr, len, fn);
	return SUCCESS;
}

/*ARGSUSED*/
grdmsg(str, fn)
register char *str;
{
	unsigned len;

	if(setjmp(Failbuf))
		return FAIL;
	for (;;) {
		len = pkread(Pk, str, PACKSIZE);
		if (len == 0)
			continue;
		str += len;
		if (*(str - 1) == '\0')
			break;
	}
	return SUCCESS;
}


gwrdata(fp1, fn)
FILE *fp1;
{
	char bufr[BUFSIZ];
	register int len;
	int ret, mil;
	struct timeb t1, t2;
	long bytes;
	char text[BUFSIZ];

	if(setjmp(Failbuf))
		return FAIL;
	bytes = 0L;
	Retries = 0;

	ftime(&t1);

	while ((len = read(fileno(fp1), bufr, BUFSIZ)) > 0) {
		bytes += len;
		ret = gwrblk(bufr, len, fn);
		if (ret != len) {
			return FAIL;
		}
		if (len != BUFSIZ)
			break;
	}
	ret = gwrblk(bufr, 0, fn);

	ftime(&t2);

	Now = t2;
	t2.time -= t1.time;
	mil = t2.millitm - t1.millitm;
	if (mil < 0) {
		--t2.time;
		mil += 1000;
	}
	sprintf(text, "sent data %ld bytes %ld.%02d secs",
				bytes, (long)t2.time, mil/10);
	if (Retries > 0) 
		sprintf((char *)text+strlen(text)," %d retries", Retries);
	DEBUG(1, "%s\n", text);
	syslog(text);
	return SUCCESS;
}

grddata(fn, fp2)
FILE *fp2;
{
	register int len;
	char bufr[BUFSIZ];
	struct timeb t1, t2;
	int mil;
	long bytes;
	char text[BUFSIZ];

	if(setjmp(Failbuf))
		return FAIL;
	bytes = 0L;
	Retries = 0;

	ftime(&t1);

	for (;;) {
		len = grdblk(bufr, BUFSIZ, fn);
		if (len < 0) {
			return FAIL;
		}
		bytes += len;
		if (write(fileno(fp2), bufr, len) != len)
			return FAIL;
		if (len < BUFSIZ)
			break;
	}

	ftime(&t2);

	Now = t2;
	t2.time -= t1.time;
	mil = t2.millitm - t1.millitm;
	if (mil < 0) {
		--t2.time;
		mil += 1000;
	}
	sprintf(text, "received data %ld bytes %ld.%02d secs",
				bytes, (long)t2.time, mil/10);
	if (Retries > 0) 
		sprintf((char *)text+strlen(text)," %d retries", Retries);
	DEBUG(1, "%s\n", text);
	syslog(text);
	return SUCCESS;
}

/*ARGSUSED*/
grdblk(blk, len,  fn)
register int len;
char *blk;
{
	register int i, ret;

	for (i = 0; i < len; i += ret) {
		ret = pkread(Pk, blk, len - i);
		if (ret < 0)
			return FAIL;
		blk += ret;
		if (ret == 0)
			return i;
	}
	return i;
}

/*ARGSUSED*/
gwrblk(blk, len, fn)
register char *blk;
{
	return  pkwrite(Pk, blk, len);
}
