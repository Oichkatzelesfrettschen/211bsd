/*
 *	Program Name:   symcompact.c
 *	Date: March 24, 2026
 *	Author: S.M. Schultz
 *
 *	-----------------   Modification History   ---------------
 *      Version Date            Reason For Modification
 *      1.0     21Jan1994       1. Initial release into the public domain.
 *	1.1	11Feb1994	2. Remove register symbols to save memory.
 *	2.0     24Mar2026       3. Rewrite to use libvmf to hold symbol table.
*/

/*
 * This program compacts the symbol table of an executable.  This is
 * done by removing '~symbol' references when _both_ the '~symbol' and
 * '_symbol' have an overlay number of 0.  The assembler always generates
 * both forms.  The only time both forms are needed is in an overlaid 
 * program and the routine has been relocated by the linker, in that event
 * the '_' form is the overlay "thunk" and the '~' form is the actual 
 * routine itself.  Only 'text' symbols have both forms.  Reducing the
 * number of symbols greatly speeds up 'nlist' processing as well as 
 * cutting down memory requirements for programs such as 'adb' and 'nm'.
 *
 * NOTE: This program holds the string table in memory and uses vmf routines
 * with 12 segments (12KB) for the symbol table.  More than 12 makes no speed
 * difference and reduces the string table size that can be handled.
 *
 * register symbols (N_REG) are removed.  See comments in 'symorder.c' for the
 * the explaination.
 *
 * filename symbols (N_FN) are removed. There were exactly 3 uses of N_FN in
 * the system: ld generates them, nm lists them, and adb ignores them.  Now
 * there are 4 uses as symcompact.c will also ignore N_FN symbols.
 * 
*/

#include <stdio.h>
#include <a.out.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <sysexits.h>
#include <sys/file.h>
#include <vmf.h>

extern	char	*__progname;

/*
 * Compute a symbol's virtual address from its index number.
*/

#define	SYMSPERSEG (BYTESPERSEG / sizeof (struct nlist))
#define sym2va(x) (((x % SYMSPERSEG) * sizeof (struct nlist)) + ((long) (x / SYMSPERSEG) << LOG2BPS))

void	cleanup();

	struct	vspace Vspace;	/* Virtual address space for symbol table */

	VADDR	vsym, vsym2;
	FILE	*fp, *strfp;
	off_t	symoff, stroff, ltmp;
	u_short	maxsymno, i2;
	char	*strtab, strtmp[20];
	int	segmapped, segmapped2, vsg;
	int	cnt, esyms, osyms, nsyms, len, symsremoved = 0;
	long	strsiz;
	struct	nlist	syment;
	struct	xexec	xhdr;
	struct	vseg	*seg, *seg2;

main(argc, argv)
	int	argc;
	char	**argv;
	{
	register u_short	i;
	register struct	nlist	*sp, *sp2;
	char	fbuf1[BUFSIZ], fbuf2[BUFSIZ];

	signal(SIGQUIT, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGHUP, cleanup);

	if	(vminit(12) < 0)
		{
		fprintf(stderr, "%s: vminit failed\n", __progname);
		exit(EX_OSERR);
		}
	if	(vmopen(&Vspace, (char *)NULL) < 0)
		{
		fprintf(stderr, "%s: vmopen failed\n", __progname);
		exit(EX_OSERR);
		}

	if	(argc != 2)
		{
		fprintf(stderr, "%s: filename argument missing\n", __progname);
		exit(EX_USAGE);
		}
	fp = fopen(argv[1], "r+");
	if	(!fp)
		{
		fprintf(stderr, "%s: can't open '%s' for update\n", __progname,
			argv[1]);
		exit(EX_NOINPUT);
		}
	setbuf(fp, fbuf1);

	cnt = fread(&xhdr, 1, sizeof (xhdr), fp);
	if	(cnt < sizeof (xhdr.e))
		{
		fprintf(stderr, "%s: Premature EOF reading header\n", __progname);
		exit(EX_DATAERR);
		}
	if	(N_BADMAG(xhdr.e))
		{
		fprintf(stderr, "%s: Bad magic number\n", __progname);
		exit(EX_DATAERR);
		}
	osyms = xhdr.e.a_syms / sizeof (struct nlist);
	if	(!osyms)
		{
		fprintf(stderr, "%s: '%s' stripped\n", __progname);
		exit(EX_OK);
		}
	stroff = N_STROFF(xhdr);
	symoff = N_SYMOFF(xhdr);
/*
 * Seek to the string table size longword and read it.  Then attempt to
 * malloc memory to hold the string table.  First make a sanity check on
 * the size.
*/
	fseek(fp, stroff, L_SET);
	fread(&strsiz, sizeof (long), 1, fp);
	if	(strsiz > 48 * 1024L)
		{
		fprintf(stderr, "%s: string table > 48kb\n", __progname);
		exit(EX_DATAERR);
		}
	strtab = (char *)malloc((int)strsiz);
	if	(!strtab)
		{
		fprintf(stderr, "%s: no memory for strings\n", __progname);
		exit(EX_OSERR);
		}
/*
 * Now read the string table into memory.  Reduce the size read because
 * we've already retrieved the string table size longword.  Adjust the
 * address used so that we don't have to adjust each symbol table entry's
 * string offset.
*/
	cnt = fread(strtab + sizeof (long), 1, (int)strsiz - sizeof (long), fp);
	if	(cnt != (int)strsiz - sizeof (long))
		{
		fprintf(stderr, "%s: Premature EOF reading strings\n", __progname);
		exit(EX_DATAERR);
		}

/*
 * NOTE: vmmapseg() is very expensive!  Access to the symbol table in this
 *       program is not random but sequential.  vmmapseg() need only be 
 *       called when crossing a segment boundary. Within a segment increment
 *       the 'struct nlist' pointer.
*/

/*
 * Read symbols excluding nonsignificant ones. Assign in-memory string address.
*/
	fseek(fp, symoff, L_SET);

	for	(segmapped = -1, esyms = 0, maxsymno = 0, i = 0; i < osyms; i++)
		{
		fread(&syment, sizeof (syment), 1, fp);
		if	(exclude(&syment))
			{
			esyms++;
			continue;
			}
		vsym = sym2va(maxsymno);
		vsg = VSEG(vsym);
		if	(vsg != segmapped)
			{
			segmapped = vsg;
			seg = vmmapseg(&Vspace, vsg);
			sp = (struct nlist *)(seg->s_cinfo + VOFF(vsym));
			VMMODIFY(seg);
			}
		else
			sp++;
		bcopy(&syment, sp, sizeof (syment));
		sp->n_un.n_name = strtab + (int)sp->n_un.n_strx;
		maxsymno++;
		}

/*
 * Now look for symbols with overlay numbers of 0 (root/base segment) and
 * of type 'text'.  For each symbol found check if there exists both a '~'
 * and '_' prefixed form of the symbol.  Preserve the '_' form and clear
 * the '~' entry by zeroing the string address of the '~' symbol.
*/
	segmapped = -1;

	for	(i = 0; i < maxsymno; i++)
		{
		vsym = sym2va(i);
		vsg = VSEG(vsym);
		if	(vsg != segmapped)
			{
			seg = vmmapseg(&Vspace, vsg);
			sp = (struct nlist *)(seg->s_cinfo + VOFF(vsym));
			segmapped = vsg;
			}
		else
			sp++;
		if	(sp->n_ovly)
			continue;
		if	((sp->n_type & N_TYPE) != N_TEXT)
			continue;
		if	(sp->n_un.n_name[0] != '~')
			continue;
		vmlock(seg);
/*
 * At this point we have the '~' form of a non overlaid text symbol.  Look
 * thru the symbol table for the '_' form.  All of 1) symbol type, 2) Symbol
 * value and 3) symbol name (starting after the first character) must match.
*/
		for	(segmapped2 = -1, i2 = 0; i2 < maxsymno; i2++)
			{
			vsym2 = sym2va(i2);
			vsg = VSEG(vsym2);
			if	(vsg != segmapped2)
				{
				seg2 = vmmapseg(&Vspace, vsg);
				sp2 = (struct nlist *)(seg2->s_cinfo + VOFF(vsym2));
				segmapped2 = vsg;
				}
			else
				sp2++;
			if	(sp2->n_ovly)
				continue;
			if	((sp2->n_type & N_TYPE) != N_TEXT)
				continue;
			if	(sp2->n_un.n_name[0] != '_')
				continue;
			if	(sp2->n_value != sp->n_value)
				continue;
			if	(strcmp(sp->n_un.n_name+1, sp2->n_un.n_name+1))
				continue;
/*
 * Found a match.  Null out the '~' symbol's string table offset and in memory 
 * string address (n_strx & n_name are a union).
*/
			symsremoved++;
			sp->n_un.n_strx = NULL;
			VMMODIFY(seg);
			break;
			}
		vmunlock(seg);
		}
/*
 * Done with the nested scanning of the symbol table.  Now create a new
 * string table (from the remaining symbols) in a temporary file.
*/
	strcpy(strtmp, "/tmp/strXXXXXX");
	mktemp(strtmp);
	strfp = fopen(strtmp, "w+");
	if	(!strfp)
		{
		fprintf(stderr, "%s: can't create '%s'\n", __progname, strtmp);
		exit(EX_CANTCREAT);
		}
	setbuf(strfp, fbuf2);

/*
 * As each symbol is written to the tmp file the symbol's string offset
 * is updated with the new file string table offset.
*/
	ltmp = sizeof (long);

	for	(segmapped = -1, i = 0; i < maxsymno; i++)
		{
		vsym = sym2va(i);
		vsg = VSEG(vsym);
		if	(vsg != segmapped)
			{
			seg = vmmapseg(&Vspace, vsg);
			sp = (struct nlist *)(seg->s_cinfo + VOFF(vsym));
			segmapped = vsg;
			VMMODIFY(seg);
			}
		else
			sp++;
		if	(!sp->n_un.n_name)
			continue;
		len = strlen(sp->n_un.n_name) + 1;
		fwrite(sp->n_un.n_name, len, 1, strfp);
		sp->n_un.n_strx = ltmp;
		ltmp += len;
		}
/*
 * We're done with the memory string table - give it back.  Then reposition
 * the new string table file to the beginning.
*/
	free(strtab);
	rewind(strfp);

/*
 * Position the executable file to where the symbol table begins.  Truncate
 * the file.  Write out the valid symbols, counting each one so that the 
 * a.out header can be updated when we're done.
*/
	fseek(fp, symoff, L_SET);
	ftruncate(fileno(fp), ftell(fp));

	for	(segmapped = -1, nsyms = 0, i = 0; i < maxsymno; i++)
		{
		vsym = sym2va(i);
		vsg = VSEG(vsym);
		if	(vsg != segmapped)
			{
			seg = vmmapseg(&Vspace, VSEG(vsym));
			sp = (struct nlist *)(seg->s_cinfo + VOFF(vsym));
			segmapped = vsg;
			}
		else
			sp++;
		if	(sp->n_un.n_strx == 0)
			continue;
		nsyms++;
		fwrite(sp, sizeof (struct nlist), 1, fp);
		}
/*
 * Next write out the string table size longword.
*/
	fwrite(&ltmp, sizeof (long), 1, fp);
/*
 * We're done with the virtual address space, release it.  Then append
 * the string table to the executable file.
*/
	vmclose(&Vspace);

	while	((i = getc(strfp)) != EOF)
		putc(i, fp);
	fclose(strfp);

	rewind(fp);
	xhdr.e.a_syms = nsyms * sizeof (struct nlist);
	fwrite(&xhdr.e, sizeof (xhdr.e), 1, fp);
	fclose(fp);
	printf("%s: initial(%d)/excluded(%d)/removed(%d)/current(%d) symbols, old(%ld)/new(%ld) strings sbrk(0): %u\n", __progname, osyms, esyms, symsremoved, nsyms, strsiz, ltmp, sbrk(0));
	cleanup();
	}

void
cleanup()
	{
	if	(strtmp[0])
		unlink(strtmp);
	exit(EX_OK);
	}

/*
 * Place any symbol exclusion rules in this routine, return 1 if the
 * symbol is to be excluded, 0 if the symbol is to be retained.
*/

exclude(sp)
	register struct nlist *sp;
	{

	if	((sp->n_type == N_REG) || (sp->n_type == N_FN))
		return(1);
	if	(sp->n_un.n_strx == 0)
		return(1);
	return(0);
	}
