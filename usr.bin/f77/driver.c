static char xxxvers[] = "f77 driver version 1.14 2026/6/29\n";

#include <stdio.h>
#include <ctype.h>
#include "defines"
#include "pdp11defs"
#include "drivedefs"
#include "ftypes"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

static FILE *diagfile	= {stderr} ;
static int pid;
static int sigivalue	= 0;
static int sigqvalue	= 0;
static int sighvalue	= 0;
static int sigtvalue	= 0;

static char *pass1name	= PASS1NAME ;
static char *pass2name	= PASS2NAME ;
static char *asmname	= ASMNAME ;
static char *ldname	= LDNAME ;
static char *footname	= FOOTNAME;
static char *proffoot	= PROFFOOT;
static char *macroname	= "m4";
static char *shellname	= "/bin/sh";
static char *aoutname	= "a.out" ;
static char *tmpfiles   = "/tmp/fort" ;

static char *infname;
static char textfname[20];
static char asmfname[20];
static char asmpass2[20];
static char initfname[20];
static char sortfname[20];
static char prepfname[20];
static char objfdefault[20];
static char optzfname[20];
static char setfname[20];

static char fflags[30]	= "-";
static char cflags[20]	= "-c -V";
static char aflags[20]	= "-u -V";
static char eflags[30]	= "";
static char rflags[30]	= "";
static char lflag[3]	= "-x";
static char *fflagp	= fflags+1;
static char *cflagp	= cflags+5;
static char *aflagp	= aflags+5;
static char *eflagp	= eflags;
static char *rflagp	= rflags;
static char **loadargs;
static char **loadp;

static flag erred	= NO;
static flag loadflag	= YES;
static flag saveasmflag	= NO;
static flag profileflag	= NO;
static flag optimflag	= NO;
static flag debugflag	= NO;
static flag verbose	= NO;
static flag nofloating	= NO;
static flag fortonly	= NO;
static flag macroflag	= NO;

main(argc, argv)
int argc;
char **argv;
{
int i, c, status;
char *setdoto(), *lastchar(), *lastfield();
register char *s;
char fortfile[20], *t;
char buff[100];
int intrupt();

sigivalue = (int) signal(SIGINT, 1) & 01;
sigqvalue = (int) signal(SIGQUIT,1) & 01;
sighvalue = (int) signal(SIGHUP, 1) & 01;
sigtvalue = (int) signal(SIGTERM,1) & 01;
enbint(intrupt);

pid = getpid();
crfnames();

loadargs = (char **) calloc(argc+20, sizeof(*loadargs));
loadargs[1] = "-X";
loadargs[2] = "-u";
loadargs[3] = "_MAIN_";
loadp = loadargs + 4;

--argc;
++argv;

while(argc>0 && argv[0][0]=='-' && argv[0][1]!='\0')
	{
	for(s = argv[0]+1 ; *s ; ++s) switch(*s)
		{
		case 'T':  /* use special passes */
			switch(*++s)
				{
				case '1':
					pass1name = s+1; goto endfor;
				case '2':
					pass2name = s+1; goto endfor;
				case 'a':
					asmname = s+1; goto endfor;
				case 'l':
					ldname = s+1; goto endfor;
				case 'F':
					footname = s+1; goto endfor;
				case 'm':
					macroname = s+1; goto endfor;
				default:
					fatal1("bad option -T%c", *s);
				}
			break;

		case 'w':
			if(s[1]=='6' && s[2]=='6')
				{
				*fflagp++ = *s++;
				*fflagp++ = *s++;
				}

		copyfflag:
		case 'u':
		case 'U':
		case 'M':
		case '1':
		case 'C':
			*fflagp++ = *s;
			break;

		case 'O':
			optimflag = YES;
			*fflagp++ = 'O';
			if( isdigit(s[1]) )
				*fflagp++ = *++s;
			t = " -O";
			while (*cflagp++ = *t++)
				;
			break;

		case 'm':
			if(s[1] == '4')
				++s;
			macroflag = YES;
			break;

		case 'S':
			saveasmflag = YES;
		case 'c':
			loadflag = NO;
			break;

		case 'v':
			verbose = YES;
			break;

		case 'd':
			debugflag = YES;
			goto copyfflag;

		case 'p':
			profileflag = YES;
			t = " -p";
			while (*cflagp++ = *t++)
				;
			goto copyfflag;

		case 'o':
			if( ! strcmp(s, "onetrip") )
				{
				*fflagp++ = '1';
				goto endfor;
				}
			aoutname = *++argv;
			--argc;
			break;

		case 'f':
			nofloating = YES;
			pass2name = NOFLPASS2;
			break;

		case 'F':
			fortonly = YES;
			loadflag = NO;
			break;

		case 'I':
			if(s[1]=='2' || s[1]=='4')
				{
				*fflagp++ = *s++;
				goto copyfflag;
				}
			fprintf(diagfile, "invalid flag -I%c\n", s[1]);
			done(1);

		case 'l':	/* letter ell--library */
			s[-1] = '-';
			*loadp++ = s-1;
			goto endfor;

		case 'E':	/* EFL flag argument */
			while( *eflagp++ = *++s)
				;
			*eflagp++ = ' ';
			goto endfor;
		case 'R':
			while( *rflagp++ = *++s )
				;
			*rflagp++ = ' ';
			goto endfor;
		case 'V':
			break;
		default:
			lflag[1] = *s;
			*loadp++ = strdup(lflag);
			break;
		}
endfor:
	--argc;
	++argv;
	}

	loadargs[0] = ldname;
	if(nofloating)
		*loadp++ = (profileflag ? NOFLPROF : NOFLFOOT);
	else
		*loadp++ = (profileflag ? proffoot : footname);

for(i = 0 ; i<argc ; ++i)
	switch(c =  dotchar(infname = argv[i]) )
		{
		case 'r':	/* Ratfor file */
		case 'e':	/* EFL file */
			if( unreadable(argv[i]) )
				{
				erred = YES;
				break;
				}
			s = fortfile;
			t = lastfield(argv[i]);
			while( *s++ = *t++)
				;
			s[-2] = 'f';

			if(macroflag)
				{
				sprintf(buff, "%s %s >%s", macroname, infname, prepfname);
				if (sys(buff))
					{
					rmf(prepfname);
					erred = YES;
					break;
					}
				infname = prepfname;
				}

			if(c == 'e')
				sprintf(buff, "efl %s %s >%s", eflags, infname, fortfile);
			else
				sprintf(buff, "ratfor %s %s >%s", rflags, infname, fortfile);
			status = sys(buff);
			if(macroflag)
				rmf(infname);
			if(status)
				{
				erred = YES;
				rmf(fortfile);
				break;
				}

			if( ! fortonly )
				{
				infname = argv[i] = lastfield(argv[i]);
				*lastchar(infname) = 'f';
	
				if( dofort(argv[i]) )
					erred = YES;
				else	{
					if( nodup(t = setdoto(argv[i])) )
						*loadp++ = t;
					rmf(fortfile);
					}
				}
			break;

		case 'f':	/* Fortran file */
		case 'F':
			if( unreadable(argv[i]) )
				erred = YES;
			else if( dofort(argv[i]) )
				erred = YES;
			else if( nodup(t=setdoto(argv[i])) )
				*loadp++ = t;
			break;

		case 'c':	/* C file */
		case 's':	/* Assembler file */
			if( unreadable(argv[i]) )
				{
				erred = YES;
				break;
				}
			fprintf(diagfile, "%s:\n", argv[i]);
			sprintf(buff, "%s %s %s", CC, cflags, argv[i] );
			if( sys(buff) )
				erred = YES;
			else
				if( nodup(t = setdoto(argv[i])) )
					*loadp++ = t;
			break;

		case 'o':
			if( nodup(argv[i]) )
				*loadp++ = argv[i];
			break;

		default:
			if( ! strcmp(argv[i], "-o") )
				aoutname = argv[++i];
			else
				*loadp++ = argv[i];
			break;
		}

if(loadflag && !erred)
	doload(loadargs, loadp);
done(erred);
}

dofort(s)
char *s;
{
int retcode;
char buff[200];

infname = s;
if(verbose)
	fprintf(diagfile, "PASS1.");
sprintf(buff, "%s %s %s %s %s %s",
	pass1name, fflags, s, asmfname, initfname, textfname);
switch( sys(buff) )
	{
	case 1:
		goto error;
	case 0:
		break;
	default:
		goto comperror;
	}

if(content(initfname) > 0)
	if( dodata() )
		goto error;
if( dopass2() )
	goto comperror;
doasm(s);
retcode = 0;

ret:
	rmf(asmfname);
	rmf(initfname);
	rmf(textfname);
	return(retcode);

error:
	fprintf(diagfile, "\nError.  No assembly.\n");
	retcode = 1;
	goto ret;

comperror:
	fprintf(diagfile, "\ncompiler error.\n");
	retcode = 2;
	goto ret;
}

dopass2()
{
char buff[100];

if(verbose)
	fprintf(diagfile, "PASS2.");

	sprintf(buff, "%s %s - %s %s", pass2name, textfname, asmpass2, "-V");
	return( sys(buff) );
}

doasm(s)
char *s;
{
register char *lastc;
char *obj;
char buff[200];

if(*s == '\0')
	s = objfdefault;
lastc = lastchar(s);
obj = setdoto(s);

#ifdef PASS2OPT
if(optimflag)
	{
	sprintf(buff, "%s %s %s", PASS2OPT, asmpass2, optzfname);
	if (sys(buff))
		rmf(optzfname);
	else
		{
		sprintf(buff,"mv %s %s", optzfname, asmpass2);
		sys(buff);
		}
	}
#endif

if(saveasmflag)
	{
	*lastc = 's';
	sprintf(buff, "cat %s %s >%s", asmfname, asmpass2, obj);
	sys(buff);
	*lastc = 'o';
	}
else
	{
	if(verbose)
		fprintf(diagfile, "  ASM.");

	sprintf(buff, "%s %s -o %s %s %s", asmname, aflags,
		obj, asmfname, asmpass2);

	if( sys(buff) )
		fatal("assembler error");
	if(verbose)
		fprintf(diagfile, "\n");
	}

rmf(asmpass2);
}

doload(v0, v)
register char *v0[], *v[];
{
char **p;
int waitpid;

for(p = liblist  ; *p ; *v++ = *p++)
	;

*v++ = "-o";
*v++ = aoutname;
*v = NULL;

if(verbose)
	fprintf(diagfile, "LOAD\n");
if(debugflag)
	{
	for(p = v0 ; p<v ; ++p)
		fprintf(diagfile, "%s ", *p);
	fprintf(diagfile, "\n");
	}

	if( (waitpid = fork()) == 0)
		{
		enbint(SIG_DFL);
		execv(ldname, v0);
		fatal1("couldn't load %s", ldname);
		}
	await(waitpid,ldname);
}

/* Process control and Shell-simulating routines */

sys(str)
char *str;
{
register char *s, *t;
char *argv[100], path[100];
char *inname, *outname;
int append;
int waitpid;
int argc;

if(debugflag)
	fprintf(diagfile, "%s\n", str);
inname  = NULL;
outname = NULL;
argv[0] = shellname;
argc = 1;

t = str;
while( isspace(*t) )
	++t;
while(*t)
	{
	if(*t == '<')
		inname = t+1;
	else if(*t == '>')
		{
		if(t[1] == '>')
			{
			append = YES;
			outname = t+2;
			}
		else	{
			append = NO;
			outname = t+1;
			}
		}
	else
		argv[argc++] = t;
	while( !isspace(*t) && *t!='\0' )
		++t;
	if(*t)
		{
		*t++ = '\0';
		while( isspace(*t) )
			++t;
		}
	}

if(argc == 1)   /* no command */
	return(-1);
argv[argc] = 0;

s = path;
t = "/usr/bin/";
while(*t)
	*s++ = *t++;
for(t = argv[1] ; *s++ = *t++ ; )
	;
if((waitpid = fork()) == 0)
	{
	if(inname)
		freopen(inname, "r", stdin);
	if(outname)
		freopen(outname, (append ? "a" : "w"), stdout);
	enbint(SIG_DFL);

	texec(path+9, argv);  /* command */
	texec(path+4, argv);  /*  /bin/command */
	texec(path  , argv);  /* /usr/bin/command */

	fatal1("Cannot load %s",path+9);
	}

return( await(waitpid,path+9) );
}

#include "errno.h"

/* modified version from the Shell */
texec(f, av)
char *f;
char **av;
{

execv(f, av+1);

if (errno==ENOEXEC)
	{
	av[1] = f;
	execv(shellname, av);
	fatal("No shell!");
	}
if (errno==ENOMEM)
	fatal1("%s: too large", f);
}

done(k)
int k;
{
static int recurs	= NO;

if(recurs == NO)
	{
	recurs = YES;
	rmfiles();
	}
exit(k);
}

enbint(k)
int (*k)();
{
if(sigivalue == 0)
	signal(SIGINT,k);
if(sigqvalue == 0)
	signal(SIGQUIT,k);
if(sighvalue == 0)
	signal(SIGHUP,k);
if(sigtvalue == 0)
	signal(SIGTERM,k);
}

intrupt()
{
done(2);
}

await(waitpid,where)
int waitpid;
char where[];
{
int w, status;

enbint(SIG_IGN);
while ( (w = wait(&status)) != waitpid)
	if(w == -1)
		fatal("bad wait code");
enbint(intrupt);
if(status & 0377)
	{
	if(status != SIGINT)
/*! Error messages beefed up here PLWard 10/80 */
		fprintf(diagfile, "f77 terminated. Core dumped.\n");
		fprintf(diagfile, "Executing %s.\nSignal returned was %d.\n",where,status & 0177);
	done(3);
	}
return(status>>8);
}

/* File Name and File Manipulation Routines */

unreadable(s)
register char *s;
{
register FILE *fp;

if(fp = fopen(s, "r"))
	{
	fclose(fp);
	return(NO);
	}

else
	{
	fprintf(diagfile, "Error: Cannot read file %s\n", s);
	return(YES);
	}
}

clf(p)
FILE **p;
{
if(p!=NULL && *p!=NULL && *p!=stdout)
	{
	if(ferror(*p))
		fatal("writing error");
	fclose(*p);
	}
*p = NULL;
}

rmfiles()
{
rmf(textfname);
rmf(asmfname);
rmf(initfname);
rmf(asmpass2);
}

/* return -1 if file does not exist, 0 if it is of zero length
   and 1 if of positive length
*/
content(filename)
char *filename;
{
	struct stat buf;

	if	(stat(filename,&buf) < 0) 
		return(-1);
	return( buf.st_size > 0 );
}

crfnames()
{
fname(textfname, "x");
fname(asmfname, "s");
fname(asmpass2, "a");
fname(initfname, "d");
fname(sortfname, "S");
fname(objfdefault, "o");
fname(prepfname, "p");
fname(optzfname, "z");
fname(setfname, "A");
}

rmf(fn)
register char *fn;
{
if(!debugflag && fn!=NULL && *fn!='\0')
	unlink(fn);
}

LOCAL fname(name, suff)
char *name, *suff;
{
sprintf(name, "%s%d.%s", tmpfiles, pid, suff);
/*! added tmpfiles variable to make it easy to move  PLWard 10/80 USGS
    location of temporary files.   
 */
}

dotchar(s)
register char *s;
{
for( ; *s ; ++s)
	if(s[0]=='.' && s[1]!='\0' && s[2]=='\0')
		return( s[1] );
return(NO);
}

char *lastfield(s)
register char *s;
{
register char *t;
for(t = s; *s ; ++s)
	if(*s == '/')
		t = s+1;
return(t);
}

char *lastchar(s)
register char *s;
{
while(*s)
	++s;
return(s-1);
}

char *setdoto(s)
register char *s;
{
*lastchar(s) = 'o';
return( lastfield(s) );
}

badfile(s)
char *s;
{
fatal1("cannot open intermediate file %s", s);
}

nodup(s)
char *s;
{
register char **p;

for(p = loadargs ; p < loadp ; ++p)
	if( !strcmp(*p, s) )
		return(NO);
return(YES);
}

static fatal(t)
char *t;
{
fprintf(diagfile, "Compiler error in file %s: %s\n", infname, t);
if(debugflag)
	abort();
done(1);
exit(1);
}

static fatal1(t,d)
char *t, *d;
{
char buff[100];
sprintf(buff, t, d);
fatal(buff);
}

err(s)
char *s;
{
fprintf(diagfile, "Error in file %s: %s\n", infname, s);
}

LOCAL int nch	= 0;
LOCAL FILE *asmfile;
LOCAL FILE *sortfile;

static ftnint typesize[NTYPES]
	= { 1, SZADDR, SZSHORT, SZLONG, SZLONG, 2*SZLONG,
	    2*SZLONG, 4*SZLONG, SZLONG, 1, 1, 1};
static int typealign[NTYPES]
	= { 1, ALIADDR, ALISHORT, ALILONG, ALILONG, ALIDOUBLE,
	    ALILONG, ALIDOUBLE, ALILONG, 1, 1, 1};

dodata()
{
char buff[50];
char varname[XL+1], ovarname[XL+1];
int status;
flag erred;
ftnint offset, vlen, type;
register ftnint ooffset, ovlen;
ftnint vchar;
int size, align;
int vargroup;
ftnint totlen, doeven();

erred = NO;
ovarname[0] = '\0';
ooffset = 0;
ovlen = 0;
totlen = 0;
nch = 0;

sprintf(buff, "sort %s >%s", initfname, sortfname);
if (status = sys(buff))
	fatal1("call sort status = %d", status);
if( (sortfile = fopen(sortfname, "r")) == NULL)
	badfile(sortfname);
if( (asmfile = fopen(asmfname, "a")) == NULL)
	badfile(asmfname);
fprintf(asmfile, "%s\n", USEINIT);

while( rdname(&vargroup, varname) && rdlong(&offset) && rdlong(&vlen) && rdlong(&type) )
	{
	size = typesize[type];
	if( strcmp(varname, ovarname) )
		{
		prspace(ovlen-ooffset);
		strcpy(ovarname, varname);
		ooffset = 0;
		totlen += ovlen;
		ovlen = vlen;
		if(vargroup == 0)
			align = (type==TYCHAR ? SZLONG : typealign[type]);
		else	align = ALIDOUBLE;
		totlen = doeven(totlen, align);
		fprintf(asmfile, LABELFMT, varname);
		}
	if(offset < ooffset)
		{
		erred = YES;
		err("overlapping initializations");
		}
	if(offset > ooffset)
		{
		prspace(offset-ooffset);
		ooffset = offset;
		}
	if(type == TYCHAR)
		{
		if( ! rdlong(&vchar) )
			fatal("bad intermediate file format");
		prch( (int) vchar );
		}
	else
		{
		putc('\t', asmfile);
		while	( putc( getc(sortfile), asmfile)  != '\n')
			;
		}
	if( (ooffset += size) > ovlen)
		{
		erred = YES;
		err("initialization out of bounds");
		}
	}

prspace(ovlen-ooffset);
totlen = doeven(totlen+ovlen, (ALIDOUBLE>SZLONG ? ALIDOUBLE : SZLONG) );
clf(&sortfile);
clf(&asmfile);
clf(&sortfile);
rmf(sortfname);
return(erred);
}

prspace(n)
register ftnint n;
{
register ftnint m;

while(nch>0 && n>0)
	{
	--n;
	prch(0);
	}
m = SZSHORT * (n/SZSHORT);
if(m > 0)
	prskip(asmfile, m);
for(n -= m ; n>0 ; --n)
	prch(0);
}

ftnint doeven(tot, align)
register ftnint tot;
int align;
{
ftnint new;
new = roundup(tot, align);
prspace(new - tot);
return(new);
}

rdname(vargroupp, name)
int *vargroupp;
register char *name;
{
register int i, c;

if( (c = getc(sortfile)) == EOF)
	return(NO);
*vargroupp = c - '0';

for(i = 0 ; i<XL ; ++i)
	{
	if( (c = getc(sortfile)) == EOF)
		return(NO);
	if(c != ' ')
		*name++ = c;
	}
*name = '\0';
return(YES);
}

rdlong(n)
register ftnint *n;
{
register int c;

for(c = getc(sortfile) ; c!=EOF && isspace(c) ; c = getc(sortfile) );
	;
if(c == EOF)
	return(NO);

for(*n = 0 ; isdigit(c) ; c = getc(sortfile) )
	*n = 10* (*n) + c - '0';
return(YES);
}

prch(c)
register int c;
{
static int buff[SZSHORT];

buff[nch++] = c;
if(nch == SZSHORT)
	{
	prchars(asmfile, buff);
	nch = 0;
	}
}
