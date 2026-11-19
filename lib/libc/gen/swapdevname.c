#if     defined(DOSCCS) && !defined(lint)
static char *sccsid = "@(#)swapdevname.c 1.0(2.11BSD) 2026/6/5";
#endif

/*
 * swapdevname(3)
 *
 * fetch 'swapdev' from kernel, lookup 'swapdev' in device database 
 * (/var/run/dev) produced by dev_mkdb(8) and prepend /dev/ (e.g. /dev/xp0b).
 *
 * If sysctl fails (kernel doesn't support VM_SWAPDEV) or if device not
 * found in dev_mkdb(8) database the traditional string "/dev/swap" is returned.
*/

#include <stdio.h>
#include <stdlib.h>
#include <paths.h>
#include <sys/stat.h>
#include <sys/sysctl.h>

static	char swapstr[] = "swap";

int
swapdevname(char *buf, int len)
	{
	int	mib[2];
	int	miblen, swapdev;
	register char	*cp;
	register int	i;

	mib[0] = CTL_VM;
	mib[1] = VM_SWAPDEV;
	miblen = sizeof (int);
	if	(sysctl(mib, 2, &swapdev, &miblen, NULL, 0) == -1)
		cp = swapstr;
	else
		{
		cp = devname(swapdev, S_IFBLK);
		if	(cp[0] == '?' && cp[1] == '?' && cp[2] == '\0')
			cp = swapstr; 
		}
/*
 * snprintf doesn't include the terminating null even though it occupies
 * a byte in the buffer so add one in the calculation of bytes needed.
*/
	i = snprintf(buf, len, "%s%s", _PATH_DEV, cp) - len + 1;
	devname_close();
/*
 * i < 0 there were unused bytes remaining in the buffer
 * i = 0 the string + null exactly fit in the buffer
 * i > 0 the buffer was 'i' bytes too small.
*/
	return(i <= 0 ? 0: i);
	}
