/*
 *   @(#)id_query.c   2.0 (2.11BSD) 2026/9/21
 *
 * id_query.c           Transmit a query to an IDENT server
 *
 * Author: Peter Eriksson <pen@lysator.liu.se>
*/

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#ifdef HAVE_ANSIHEADERS
#  include <stdlib.h>
#  include <string.h>
#  include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define IN_LIBIDENT_SRC
#include "ident.h"


int id_query __P4(ident_t *, id,
		  int, lport,
		  int, fport,
		  struct timeval *, timeout)
{
    sig_t old_sig;
    int res;
    char buf[80];
    fd_set ws;
    
    sprintf(buf, "%d , %d\r\n", lport, fport);
    
    if (timeout)
    {
	FD_ZERO(&ws);
	FD_SET(id->fd, &ws);

	if ((res = select(FD_SETSIZE, (fd_set *)0, &ws, (fd_set *)0, timeout)) < 0)
	    return -1;
	
	if (res == 0)
	{
	    errno = ETIMEDOUT;
	    return -1;
	}
    }

    old_sig = signal(SIGPIPE, SIG_IGN);
    
    res = write(id->fd, buf, strlen(buf));
    
    signal(SIGPIPE, old_sig);
    
    return res;
}
