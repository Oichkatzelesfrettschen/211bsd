/* "@(#)intr.c 1.1 (2.11BSD) 2026/7/4"; */

#include "defs"
#include "string_defs"

/*
 * Note: an addition has been made to the intrinsic
 * function tables to support the fortran bit
 * functions:
 *  	igetbt, iand, ieor, inot, ior, ishft, iputbt
 *
 *  mod is selected by '#define BITFUNC'
 * wfj and jlh 8 august 1979 USGS Menlo Park.
 */
/* INTRINSIC FUNCTIONS: The compiler needs to recognize the name of an
intrinsic function, decide on which C routine to call depending on the 
type of the arguement, then call that routine and tell the compiler
the type of the result.
     The structure intrblock is the intrinsic function table. Each line
of the table contains the intrinsic function name (intrtab[i].intrfname)
and three numbers that are actually packed into one long word with the
structure intrbits. The first number (intrtab.intrval.f1) tells how the
type is to be checked and converted if necessary. The second number
(intrtab.intrval.f2) tells either the type of the argument required
or in the case of INTRGEN tells how many lines of the spectable to 
search. The third number (intrtab.intrval.f3) tells which line of the
spectable to go to or to begin at.
     The spectable is the structure specblock and contains four columns:
the type of argument(s) required, the type of result returned, the number
of arguments required, and the name of the C routine to be called.
     INTRCONV, INTRBOOL, INTRMAX, and INTRMIN are all done internally
with no subroutine call necessary. INTRSPEC requires that the argument
be of the type specified. INTRGEN requires that the argument be of one
of the types specified in the spectable beginning at line intrtab.intrval.
f3 and going for intrtab.intrval.f2 lines.
PLWard, Menlo  4/8/80                    */

#define BITFUNC

union
	{
	int ijunk;
	struct intrpacked bits;
	} packed;

struct intrbits
	{
	int intrgroup /* :3 */;
	int intrstuff /* result type or number of generics */;
	int intrno /* :7 */;
	};

LOCAL struct intrblock
	{
	char intrfname[VL];
	struct intrbits intrval;
	} intrtab[ ] =
{
"int", 		{ INTRCONV, TYLONG },
"real", 	{ INTRCONV, TYREAL },
"dble", 	{ INTRCONV, TYDREAL },
"dreal", 	{ INTRCONV, TYDREAL },
"cmplx", 	{ INTRCONV, TYCOMPLEX },
"dcmplx", 	{ INTRCONV, TYDCOMPLEX },
"ifix", 	{ INTRCONV, TYLONG },
"idint", 	{ INTRCONV, TYLONG },
"float", 	{ INTRCONV, TYREAL },
"dfloat",	{ INTRCONV, TYDREAL },
"sngl", 	{ INTRCONV, TYREAL },
"ichar", 	{ INTRCONV, TYLONG },
"char", 	{ INTRCONV, TYCHAR },

"max", 		{ INTRMAX, TYUNKNOWN },
"max0", 	{ INTRMAX, TYLONG },
"amax0", 	{ INTRMAX, TYREAL },
"max1", 	{ INTRMAX, TYLONG },
"amax1", 	{ INTRMAX, TYREAL },
"dmax1", 	{ INTRMAX, TYDREAL },

"and",		{ INTRBOOL, TYUNKNOWN, OPBITAND },
"or",		{ INTRBOOL, TYUNKNOWN, OPBITOR },
"xor",		{ INTRBOOL, TYUNKNOWN, OPBITXOR },
"not",		{ INTRBOOL, TYUNKNOWN, OPBITNOT },
"lshift",	{ INTRBOOL, TYUNKNOWN, OPLSHIFT },
"rshift",	{ INTRBOOL, TYUNKNOWN, OPRSHIFT },

"min", 		{ INTRMIN, TYUNKNOWN },
"min0", 	{ INTRMIN, TYLONG },
"amin0", 	{ INTRMIN, TYREAL },
"min1", 	{ INTRMIN, TYLONG },
"amin1", 	{ INTRMIN, TYREAL },
"dmin1", 	{ INTRMIN, TYDREAL },

"aint", 	{ INTRGEN, 2, 0 },
"dint", 	{ INTRSPEC, TYDREAL, 1 },

"anint", 	{ INTRGEN, 2, 2 },
"dnint", 	{ INTRSPEC, TYDREAL, 3 },

"nint", 	{ INTRGEN, 4, 4 },
"idnint", 	{ INTRGEN, 2, 6 },

"abs", 		{ INTRGEN, 6, 8 },
"iabs", 	{ INTRGEN, 2, 9 },
"dabs", 	{ INTRSPEC, TYDREAL, 11 },
"cabs", 	{ INTRSPEC, TYREAL, 12 },
"zabs", 	{ INTRSPEC, TYDREAL, 13 },

"mod", 		{ INTRGEN, 4, 14 },
"amod", 	{ INTRSPEC, TYREAL, 16 },
"dmod", 	{ INTRSPEC, TYDREAL, 17 },

"sign", 	{ INTRGEN, 4, 18 },
"isign", 	{ INTRGEN, 2, 19 },
"dsign", 	{ INTRSPEC, TYDREAL, 21 },

"dim", 		{ INTRGEN, 4, 22 },
"idim", 	{ INTRGEN, 2, 23 },
"ddim", 	{ INTRSPEC, TYDREAL, 25 },

"dprod", 	{ INTRSPEC, TYDREAL, 26 },

"len", 		{ INTRSPEC, TYLONG, 27 },
"index", 	{ INTRSPEC, TYLONG, 29 },

"imag", 	{ INTRGEN, 2, 31 },
"aimag", 	{ INTRSPEC, TYREAL, 31 },
"dimag", 	{ INTRSPEC, TYDREAL, 32 },

"conjg", 	{ INTRGEN, 2, 33 },
"dconjg", 	{ INTRSPEC, TYDCOMPLEX, 34 },

"sqrt", 	{ INTRGEN, 4, 35 },
"dsqrt", 	{ INTRSPEC, TYDREAL, 36 },
"csqrt", 	{ INTRSPEC, TYCOMPLEX, 37 },
"zsqrt", 	{ INTRSPEC, TYDCOMPLEX, 38 },

"exp", 		{ INTRGEN, 4, 39 },
"dexp", 	{ INTRSPEC, TYDREAL, 40 },
"cexp", 	{ INTRSPEC, TYCOMPLEX, 41 },
"zexp", 	{ INTRSPEC, TYDCOMPLEX, 42 },
"cdexp", 	{ INTRSPEC, TYDCOMPLEX, 42 },

"log", 		{ INTRGEN, 4, 43 },
"alog", 	{ INTRSPEC, TYREAL, 43 },
"dlog", 	{ INTRSPEC, TYDREAL, 44 },
"clog", 	{ INTRSPEC, TYCOMPLEX, 45 },
"zlog", 	{ INTRSPEC, TYDCOMPLEX, 46 },

"log10", 	{ INTRGEN, 2, 47 },
"alog10", 	{ INTRSPEC, TYREAL, 47 },
"dlog10", 	{ INTRSPEC, TYDREAL, 48 },

"sin", 		{ INTRGEN, 4, 49 },
"dsin", 	{ INTRSPEC, TYDREAL, 50 },
"csin", 	{ INTRSPEC, TYCOMPLEX, 51 },
"zsin", 	{ INTRSPEC, TYDCOMPLEX, 52 },

"cos", 		{ INTRGEN, 4, 53 },
"dcos", 	{ INTRSPEC, TYDREAL, 54 },
"ccos", 	{ INTRSPEC, TYCOMPLEX, 55 },
"zcos", 	{ INTRSPEC, TYDCOMPLEX, 56 },

"tan", 		{ INTRGEN, 2, 57 },
"dtan", 	{ INTRSPEC, TYDREAL, 58 },

"asin", 	{ INTRGEN, 2, 59 },
"dasin", 	{ INTRSPEC, TYDREAL, 60 },

"acos", 	{ INTRGEN, 2, 61 },
"dacos", 	{ INTRSPEC, TYDREAL, 62 },

"atan", 	{ INTRGEN, 2, 63 },
"datan", 	{ INTRSPEC, TYDREAL, 64 },

"atan2", 	{ INTRGEN, 2, 65 },
"datan2", 	{ INTRSPEC, TYDREAL, 66 },

"sinh", 	{ INTRGEN, 2, 67 },
"dsinh", 	{ INTRSPEC, TYDREAL, 68 },

"cosh", 	{ INTRGEN, 2, 69 },
"dcosh", 	{ INTRSPEC, TYDREAL, 70 },

"tanh", 	{ INTRGEN, 2, 71 },
"dtanh", 	{ INTRSPEC, TYDREAL, 72 },

"lge",		{ INTRSPEC, TYLOGICAL, 73},
"lgt",		{ INTRSPEC, TYLOGICAL, 75},
"lle",		{ INTRSPEC, TYLOGICAL, 77},
"llt",		{ INTRSPEC, TYLOGICAL, 79},

#ifdef BITFUNC
"igetbt",	{ INTRGEN, 2, 81},
"iand",		{ INTRGEN, 2, 83},
"ieor",		{ INTRGEN, 2, 85},
"inot",		{ INTRGEN, 2, 87},
"ior",		{ INTRGEN, 2, 89},
"ishft",	{ INTRGEN, 2, 91},
"iputbt",	{ INTRGEN, 2, 93},
#endif

"" };

/*
 * Read carefully the  block of comments at the beginning of the file!
 *
 * DO NOT remove/reorder entries in spectab[] without updating intrtab[]
 *
 * Entries without an entry in the intrno comment column have no matching
 * entry in intrtab[].  The comments say the scan starts at f2 for f3 entries.
 *
 * NOTE: rtype='TYSHORT' entry must preceed rtype='TYLONG' entry in order for

 *                 if	(tyint==TYLONG && sp->rtype==TYSHORT)
 *			++sp;
 *
 *       in intrcall() to work.
*/
LOCAL struct specblock
	{
	char atype;
	char rtype;
	char nargs;
	char spxname[XL];
	char othername;	/* index into callbyvalue table */
	} spectab[ ] =
{						/* intrno */
	{ TYREAL,TYREAL,1,"r_int" },		/* 0 */

	{ TYDREAL,TYDREAL,1,"d_int" },		/* 1 */

	{ TYREAL,TYREAL,1,"r_nint" },		/* 2 */

	{ TYDREAL,TYDREAL,1,"d_nint" },		/* 3 */

	{ TYREAL,TYSHORT,1,"h_nint" },		/* 4 */
	{ TYREAL,TYLONG,1,"i_nint" },

	{ TYDREAL,TYSHORT,1,"h_dnnt" },		/* 6 */
	{ TYDREAL,TYLONG,1,"i_dnnt" },

	{ TYREAL,TYREAL,1,"r_abs" },		/* 8 */
	{ TYSHORT,TYSHORT,1,"h_abs" },		/* 9 */
	{ TYLONG,TYLONG,1,"i_abs" },
	{ TYDREAL,TYDREAL,1,"d_abs" },		/* 11 */
	{ TYCOMPLEX,TYREAL,1,"c_abs" },		/* 12 */
	{ TYDCOMPLEX,TYDREAL,1,"z_abs" },	/* 13 */

	{ TYSHORT,TYSHORT,2,"h_mod" },		/* 14 */
	{ TYLONG,TYLONG,2,"i_mod" },
	{ TYREAL,TYREAL,2,"r_mod" },		/* 16 */
	{ TYDREAL,TYDREAL,2,"d_mod" },		/* 17 */

	{ TYREAL,TYREAL,2,"r_sign" },		/* 18 */
	{ TYSHORT,TYSHORT,2,"h_sign" },		/* 19 */
	{ TYLONG,TYLONG,2,"i_sign" },
	{ TYDREAL,TYDREAL,2,"d_sign" },		/* 21 */

	{ TYREAL,TYREAL,2,"r_dim" },		/* 22 */
	{ TYSHORT,TYSHORT,2,"h_dim" },		/* 23 */
	{ TYLONG,TYLONG,2,"i_dim" },
	{ TYDREAL,TYDREAL,2,"d_dim" },		/* 25 */

	{ TYREAL,TYDREAL,2,"d_prod" },		/* 26 */

	{ TYCHAR,TYSHORT,1,"h_len" },		/* 27 */
	{ TYCHAR,TYLONG,1,"i_len" },

	{ TYCHAR,TYSHORT,2,"h_indx" },		/* 29 */
	{ TYCHAR,TYLONG,2,"i_indx" },

	{ TYCOMPLEX,TYREAL,1,"r_imag" },	/* 31 */
	{ TYDCOMPLEX,TYDREAL,1,"d_imag" },	/* 32 */

	{ TYCOMPLEX,TYCOMPLEX,1,"r_cnjg" },	/* 33 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"d_cnjg" },	/* 34 */

	{ TYREAL,TYREAL,1,"r_sqrt", 1 },	/* 35 */
	{ TYDREAL,TYDREAL,1,"d_sqrt", 1 },	/* 36 */
	{ TYCOMPLEX,TYCOMPLEX,1,"c_sqrt" },	/* 37 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"z_sqrt" },	/* 38 */

	{ TYREAL,TYREAL,1,"r_exp", 2 },		/* 39 */
	{ TYDREAL,TYDREAL,1,"d_exp", 2 },	/* 40 */
	{ TYCOMPLEX,TYCOMPLEX,1,"c_exp" },	/* 41 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"z_exp" },	/* 42 */

	{ TYREAL,TYREAL,1,"r_log", 3 },		/* 43 */
	{ TYDREAL,TYDREAL,1,"d_log", 3 },	/* 44 */
	{ TYCOMPLEX,TYCOMPLEX,1,"c_log" },	/* 45 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"z_log" },	/* 46 */

	{ TYREAL,TYREAL,1,"r_lg10" },		/* 47 */
	{ TYDREAL,TYDREAL,1,"d_lg10" },		/* 48 */

	{ TYREAL,TYREAL,1,"r_sin", 4 },		/* 49 */
	{ TYDREAL,TYDREAL,1,"d_sin", 4 },	/* 50 */
	{ TYCOMPLEX,TYCOMPLEX,1,"c_sin" },	/* 51 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"z_sin" },	/* 52 */

	{ TYREAL,TYREAL,1,"r_cos", 5 },		/* 53 */
	{ TYDREAL,TYDREAL,1,"d_cos", 5 },	/* 54 */
	{ TYCOMPLEX,TYCOMPLEX,1,"c_cos" },	/* 55 */
	{ TYDCOMPLEX,TYDCOMPLEX,1,"z_cos" },	/* 56 */

	{ TYREAL,TYREAL,1,"r_tan", 6 },		/* 57 */
	{ TYDREAL,TYDREAL,1,"d_tan", 6 },	/* 58 */

	{ TYREAL,TYREAL,1,"r_asin", 7 },	/* 59 */
	{ TYDREAL,TYDREAL,1,"d_asin", 7 },	/* 60 */

	{ TYREAL,TYREAL,1,"r_acos", 8 },	/* 61 */
	{ TYDREAL,TYDREAL,1,"d_acos", 8 },	/* 62 */

	{ TYREAL,TYREAL,1,"r_atan", 9 },	/* 63 */
	{ TYDREAL,TYDREAL,1,"d_atan", 9 },	/* 64 */

	{ TYREAL,TYREAL,2,"r_atn2", 10 },	/* 65 */
	{ TYDREAL,TYDREAL,2,"d_atn2", 10 },	/* 66 */

	{ TYREAL,TYREAL,1,"r_sinh", 11 },	/* 67 */
	{ TYDREAL,TYDREAL,1,"d_sinh", 11 },	/* 68 */

	{ TYREAL,TYREAL,1,"r_cosh", 12 },	/* 69 */
	{ TYDREAL,TYDREAL,1,"d_cosh", 12 },	/* 70 */

	{ TYREAL,TYREAL,1,"r_tanh", 13 },	/* 71 */
	{ TYDREAL,TYDREAL,1,"d_tanh", 13 },	/* 72 */

	{ TYCHAR,TYLOGICAL,2,"hl_ge" },		/* 73 */
	{ TYCHAR,TYLOGICAL,2,"l_ge" },

	{ TYCHAR,TYLOGICAL,2,"hl_gt" },		/* 75 */
	{ TYCHAR,TYLOGICAL,2,"l_gt" },

	{ TYCHAR,TYLOGICAL,2,"hl_le" },		/* 77 */
	{ TYCHAR,TYLOGICAL,2,"l_le" },

	{ TYCHAR,TYLOGICAL,2,"hl_lt" },		/* 79 */
	{ TYCHAR,TYLOGICAL,2,"l_lt" }
#ifdef BITFUNC
		,
	{ TYSHORT,TYSHORT,3,"h_getbit" },	/* 81 */
	{ TYLONG,TYLONG,3,"i_getbit" },

	{ TYSHORT,TYSHORT,2,"h_iand" },		/* 83 */
	{ TYLONG,TYLONG,2,"i_iand" },

	{ TYSHORT,TYSHORT,2,"h_ieor" },		/* 85 */
	{ TYLONG,TYLONG,2,"i_ieor" },

	{ TYSHORT,TYSHORT,1,"h_not" },		/* 87 */
	{ TYLONG,TYLONG,1,"i_not" },

	{ TYSHORT,TYSHORT,2,"h_ior" },		/* 89 */
	{ TYLONG,TYLONG,2,"i_ior" },

	{ TYSHORT,TYSHORT,2,"h_ishft" },	/* 91 */
	{ TYLONG,TYLONG,2,"i_ishft" },

	{ TYSHORT,TYSHORT,4,"h_putbit" },	/* 93 */
	{ TYLONG,TYLONG,4,"i_putbit" }
#endif
} ;

char callbyvalue[ ][XL] =
	{
	"sqrt",
	"exp",
	"log",
	"sin",
	"cos",
	"tan",
	"asin",
	"acos",
	"atan",
	"atan2",
	"sinh",
	"cosh",
	"tanh"
	};

struct exprblock *intrcall(np, argsp, nargs)
struct nameblock *np;
struct listblock *argsp;
int nargs;
{
int i, rettype;
struct addrblock *ap;
register struct specblock *sp;
struct exprblock *q, *inline();
register chainp cp;
struct constblock *mkcxcon();
expptr ep;
int mtype;
int op;

packed.ijunk = np->vardesc.varno;
if(nargs == 0)
	goto badnargs;

mtype = 0;
for(cp = argsp->listp ; cp ; cp = cp->nextp)
	{
/* TEMPORARY */ ep = cp->datap;
/* TEMPORARY */	if( ISCONST(ep) && ep->vtype==TYSHORT )
/* TEMPORARY */		cp->datap = mkconv(tyint, ep);
	mtype = maxtype(mtype, ep->vtype);
	}

switch(packed.bits.f1)
	{
	case INTRBOOL:
		op = packed.bits.f3;
		if( ! ONEOF(mtype, MSKINT|MSKLOGICAL) )
			goto badtype;
		if(op == OPBITNOT)
			{
			if(nargs != 1)
				goto badnargs;
			q = mkexpr(OPBITNOT, argsp->listp->datap, NULL);
			}
		else
			{
			if(nargs != 2)
				goto badnargs;
			q = mkexpr(op, argsp->listp->datap,
				argsp->listp->nextp->datap);
			}
		frchain( &(argsp->listp) );
		free(argsp);
		return(q);

	case INTRCONV:
		rettype = packed.bits.f2;
		if(rettype == TYLONG)
			rettype = tyint;
		if( ISCOMPLEX(rettype) && nargs==2)
			{
			expptr qr, qi;
			qr = argsp->listp->datap;
			qi = argsp->listp->nextp->datap;
			if(ISCONST(qr) && ISCONST(qi))
				q = mkcxcon(qr,qi);
			else	q = mkexpr(OPCONV,mkconv(rettype-2,qr),
					mkconv(rettype-2,qi));
			}
		else if(nargs == 1)
			q = mkconv(rettype, argsp->listp->datap);
		else goto badnargs;

		q->vtype = rettype;
		frchain(&(argsp->listp));
		free(argsp);
		return(q);


	case INTRGEN:
		sp = spectab + packed.bits.f3;
		for(i=0; i<packed.bits.f2 ; ++i)
			if(sp->atype == mtype)
				{
				if	(tyint==TYLONG && sp->rtype==TYSHORT)
					sp++;
				goto specfunct;
				}
			else
				++sp;
		goto badtype;

	case INTRSPEC:
		sp = spectab + packed.bits.f3;
		if(tyint==TYLONG && sp->rtype==TYSHORT)
			++sp;

	specfunct:
		if(nargs != sp->nargs)
			goto badnargs;
		if(mtype != sp->atype)
			goto badtype;
		fixargs(YES, argsp);
		if(q = inline(sp-spectab, mtype, argsp->listp))
			{
			frchain( &(argsp->listp) );
			free(argsp);
			}
		else if(sp->othername)
			{
			ap = builtin(sp->rtype,
				varstr(XL, callbyvalue[sp->othername-1]) );
			q = fixexpr( mkexpr(OPCCALL, ap, argsp) );
			}
		else
			{
			ap = builtin(sp->rtype, varstr(XL, sp->spxname) );
			q = fixexpr( mkexpr(OPCALL, ap, argsp) );
			}
		return(q);

	case INTRMIN:
	case INTRMAX:
		if(nargs < 2)
			goto badnargs;
		if( ! ONEOF(mtype, MSKINT|MSKREAL) )
			goto badtype;
		argsp->vtype = mtype;
		q = mkexpr( (packed.bits.f1==INTRMIN ? OPMIN : OPMAX), argsp, NULL);

		q->vtype = mtype;
		rettype = packed.bits.f2;
		if(rettype == TYLONG)
			rettype = tyint;
		else if(rettype == TYUNKNOWN)
			rettype = mtype;
		return( mkconv(rettype, q) );

	default:
		error("intrcall: bad intrgroup %d", packed.bits.f1,0,FATAL1);
	}
badnargs:
	error("bad number of arguments to intrinsic %s",
		varstr(VL,np->varname),0,ERR1);
	goto bad;

badtype:
	error("bad argument type to intrinsic %s", varstr(VL, np->varname),
		0,ERR1);

bad:
	return( errnode() );
}

intrfunct(s)
char s[VL];
{
register struct intrblock *p;
char nm[VL];
register int i;

for(i = 0 ; i<VL ; ++s)
	nm[i++] = (*s==' ' ? '\0' : *s);

for(p = intrtab; p->intrval.intrgroup!=INTREND ; ++p)
	{
	if( eqn(VL, nm, p->intrfname) )
		{
		packed.bits.f1 = p->intrval.intrgroup;
		packed.bits.f2 = p->intrval.intrstuff;
		packed.bits.f3 = p->intrval.intrno;
		return(packed.ijunk);
		}
	}

return(0);
}

struct addrblock *intraddr(np)
struct nameblock *np;
{
struct addrblock *q;
struct specblock *sp;

if(np->vclass!=CLPROC || np->vprocclass!=PINTRINSIC)
	error("intraddr: %s is not intrinsic", varstr(VL,np->varname),0,FATAL1);
packed.ijunk = np->vardesc.varno;

switch(packed.bits.f1)
	{
	case INTRGEN:
		/* imag, log, and log10 arent specific functions */
		if(packed.bits.f3==31 || packed.bits.f3==43 || packed.bits.f3==47)
			goto bad;

	case INTRSPEC:
		sp = spectab + packed.bits.f3;
		if(tyint==TYLONG && sp->rtype==TYSHORT)
			++sp;
		q = builtin(sp->rtype, varstr(XL,sp->spxname) );
		return(q);

	case INTRCONV:
	case INTRMIN:
	case INTRMAX:
	case INTRBOOL:
	bad:
		error("cannot pass %s as actual",
			varstr(VL,np->varname),0,ERR1);
		return( errnode() );
	}
error("intraddr: impossible f1=%d", packed.bits.f1,0,FATAL1);
/* NOTREACHED */
}

struct exprblock *inline(fno, type, args)
int fno;
int type;
chainp args;
{
register struct exprblock *q, *t, *t1;

switch(fno)
	{
	case 8:	/* real abs */
	case 9:	/* short int abs */
	case 10:	/* long int abs */
	case 11:	/* double precision abs */
		if( addressable(q = args->datap) )
			{
			t = q;
			q = NULL;
			}
		else
			t = mktmp(type);
		t1 = mkexpr(OPQUEST,  mkexpr(OPLE, mkconv(type,ICON(0)), cpexpr(t)),
			mkexpr(OPCOLON, cpexpr(t),
				mkexpr(OPNEG, cpexpr(t), NULL) ));
		if(q)
			t1 = mkexpr(OPCOMMA, mkexpr(OPASSIGN, cpexpr(t),q), t1);
		frexpr(t);
		return(t1);

	case 26:	/* dprod */
		q = mkexpr(OPSTAR, args->datap, args->nextp->datap);
		q->vtype = TYDREAL;
		return(q);

	case 27:	/* len of character string */
		q = cpexpr(args->datap->vleng);
		frexpr(args->datap);
		return(q);

	case 14:	/* half-integer mod */
	case 15:	/* mod */
		return( mkexpr(OPMOD, args->datap, args->nextp->datap) );
	}
return(NULL);
}
