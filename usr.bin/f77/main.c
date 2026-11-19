/*
 * static char xxxvers[] = "f77 pass1 version 1.22 2026/6/29\n";
*/

#include "defs"
#include "string_defs"

main(argc, argv)
int argc;
char **argv;
{
char *s;
int k, retcode;
FILE *opf();

#define DONE(c)	{ retcode = c; goto finis; }

--argc;
++argv;

while(argc>0 && argv[0][0]=='-')
	{
	for(s = argv[0]+1 ; *s ; ++s) switch(*s)
		{
		case 'w':
			if(s[1]=='6' && s[2]=='6')
				{
				ftn66flag = YES;
				s += 2;
				}
			else
				nowarnflag = YES;
			break;

		case 'U':
			shiftcase = NO;
			break;

		case 'u':
			undeftype = YES;
			break;

		case 'O':
			optimflag = YES;
			if( isdigit(s[1]) )
				{
				k = *++s - '0';
				if(k > MAXREGVAR)
					{
					error("-O%d: too many register variables", k,0,WARN1);
					maxregvar = MAXREGVAR;
					}
				else
					maxregvar = k;
				}
			break;

		case 'd':
			debugflag = YES;
			break;

		case 'p':
			profileflag = YES;
			break;

		case 'C':
			checksubs = YES;
			break;

		case '1':
			onetripflag = YES;
			break;

		case 'I':
			if	(*++s == '2')
				tyint = TYSHORT;
			else if	(*s == '4')
				tyint = TYLONG;
			else
				error("invalid flag -I%c", *s,0,FATAL1);
/*
 * DON'T do this!  Uncommenting the next line brings back, with use of -I4,
 * the breakage described in src/usr.bin/f77/BUG? and fixed in patch #500.
 * A caveat regarding the use of -I2 has been added to the f77 manpage.
*/
//			tylogical = tyint;
			break;

		default:
			error("invalid flag %c", *s,0,FATAL1);
		}
	--argc;
	++argv;
	}

if(argc != 4)
	error("arg count %d", argc,0,FATAL1);
asmfile  = opf(argv[1]);
initfile = opf(argv[2]);
textfile = opf(argv[3]);

initkey();
if(inilex(strdup(argv[0])))
	DONE(1);
fprintf(diagfile, "%s:\n", argv[0]);
fileinit();
procinit();
if(k = yyparse())
	{
	fprintf(diagfile, "Bad parse, return code %d\n", k);
	DONE(1);
	}
if(nerr > 0)
	DONE(1);
if(parstate != OUTSIDE)
	{
	error("missing END statement",0,0,WARN);
	endproc();
	}
doext();
preven(ALIDOUBLE);
prtail();
DONE(0);


finis:
	done(retcode);
	return(retcode);
}

done(k)
int k;
{
static int recurs	= NO;

if(recurs == NO)
	{
	recurs = YES;
	clfiles();
	}
exit(k);
}

LOCAL FILE *opf(fn)
char *fn;
{
FILE *fp;
if( fp = fopen(fn, "w") )
	return(fp);

error("cannot open intermediate file %s", fn,0,FATAL1);
/* NOTREACHED */
}

LOCAL clfiles()
{
clf(&textfile);
clf(&asmfile);
clf(&initfile);
}

clf(p)
register FILE **p;
{
if(p!=NULL && *p!=NULL && *p!=stdout)
	{
	if(ferror(*p))
		error("writing error",0,0,FATAL);
	fclose(*p);
	}
*p = NULL;
}
