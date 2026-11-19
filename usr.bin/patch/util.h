#if     !defined(lint) && defined(DOSCCS)
char rcsid[] = "util.h 2.1 (2.11BSD) 2026/6/21";
#endif
/*
 * $Log:	util.h,v $
 * Revision 2.0  86/09/17  15:40:06  lwall
 * Baseline for netwide release.
 */

/*
 * say?() was inlined as "fprintf(stderr)" and ask?() not being as frequently
 * used was done as "sprintf() + ask(prompt)".  fatal?() was converted to
 * use stdargs to avoid extra { ... } blocks.
 */
#include <stdarg.h>

void fatal(char *fmt, ...);

char *fetchname();
int move_file();
void copy_file();
char *savestr();
void set_signals();
void ignore_signals();
void makedirs();
