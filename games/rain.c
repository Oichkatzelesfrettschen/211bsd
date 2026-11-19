/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#if	!defined(lint) && defined(DOSCCS)
char copyright[] =
"@(#) Copyright (c) 1980 Regents of the University of California.\n\
 All rights reserved.\n";

static char sccsid[] = "@(#)rain.c	5.3 (2.11BSD) 2026/5/10";
#endif

#include <stdio.h>
#include <sgtty.h>
#include <signal.h>
#include <stdlib.h>

#define cursor(col,row) tputs(tgoto(CM,col,row),1,fputchar)

extern char *UP;
extern short ospeed;
struct sgttyb old_tty;
char *LL, *TE, *TI;

main(argc,argv)
int argc;
char *argv[];
{
    extern fputchar();
    char *tgetstr(), *tgoto();
    float ranf();
    void onsig();
    register int x, y, j;
    static int xpos[5], ypos[5];
    register char *CM, *BC, *DN, *ND;
    int CO, LI;
    char *tcp;
    register char *term;
    char tcb[100];
    struct sgttyb sg;
    float cols, lines;

    setbuf(stdout,malloc(BUFSIZ));
    if (!(term=getenv("TERM"))) {
	fprintf(stderr,"%s: TERM: parameter not set\n",*argv);
	exit(1);
    }
    if (tgetent(malloc(1024),term)<=0) {
	fprintf(stderr,"%s: %s: unknown terminal type\n",*argv,term);
	exit(1);
    }
    tcp=tcb;
    if (!(CM=tgetstr("cm",&tcp))) {
	fprintf(stderr,"%s: terminal not capable of cursor motion\n",*argv);
	exit(1);
    }
    if (!(BC=tgetstr("bc",&tcp))) BC="\b";
    if (!(DN=tgetstr("dn",&tcp))) DN="\n";
    if (!(ND=tgetstr("nd",&tcp))) ND=" ";
    if ((CO = tgetnum("co")) == -1)
	CO = 80; 
    if ((LI = tgetnum("li")) == -1)
	LI = 24; 
    cols = CO - 4;
    lines = LI - 4;
    TE=tgetstr("te",&tcp);
    TI=tgetstr("ti",&tcp);
    UP=tgetstr("up",&tcp);
    if (!(LL=tgetstr("ll",&tcp))) strcpy(LL=malloc(10),tgoto(CM,0,23));
    gtty(1, &sg);
    ospeed=sg.sg_ospeed;
    for (j=SIGHUP;j<=SIGTERM;j++)
	if (signal(j,SIG_IGN)!=SIG_IGN) signal(j,onsig);
    gtty(1, &old_tty);	/* save tty bits for exit */
    gtty(1, &sg);
    sg.sg_flags&=~(CRMOD|ECHO);
    stty(1, &sg);
    if (TI) tputs(TI,1,fputchar);
    tputs(tgetstr("cl",&tcp),1,fputchar);
    fflush(stdout);
    for (j=5;--j>=0;) {
	xpos[j]=(int)(cols*ranf())+2;
	ypos[j]=(int)(lines*ranf())+2;
    }
    for (j=0;;) {
	x=(int)(cols*ranf())+2;
	y=(int)(lines*ranf())+2;
	cursor(x,y); fputchar('.');
	cursor(xpos[j],ypos[j]); fputchar('o');
	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]); fputchar('O');
	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-1);
	fputchar('-');
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputs("|.|",stdout);
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputchar('-');
	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-2); fputchar('-');
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputs("/ \\",stdout);
	cursor(xpos[j]-2,ypos[j]);
	fputs("| O |",stdout);
	cursor(xpos[j]-1,ypos[j]+1);
	fputs("\\ /",stdout);
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputchar('-');
	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-2); fputchar(' ');
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputchar(' '); tputs(ND,1,fputchar); fputchar(' ');
	cursor(xpos[j]-2,ypos[j]);
	fputchar(' '); tputs(ND,1,fputchar); fputchar(' ');
	tputs(ND,1,fputchar); fputchar(' ');
	cursor(xpos[j]-1,ypos[j]+1);
	fputchar(' '); tputs(ND,1,fputchar); fputchar(' ');
	tputs(DN,1,fputchar); tputs(BC,1,fputchar); tputs(BC,1,fputchar);
	fputchar(' ');
	xpos[j]=x; ypos[j]=y;
	fflush(stdout);
    }
}

void
onsig(n)
int n;
{
    struct sgttyb sg;
    int fputchar();

    tputs(LL, 1, fputchar);
    if (TE) tputs(TE, 1, fputchar);
    fflush(stdout);
    stty(1, &old_tty);
    kill(getpid(),n);
    _exit(0);
}
fputchar(c)
char c;
{
    putchar(c);
}
float ranf() {
#if !defined(pdp11)
    return((float)rand()/2147483647.);
#else
    return((float)rand()/32767.);
#endif
}
