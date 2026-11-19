#if	defined(DOSCCS) && !defined(lint)
static	char *sccsid = "@(#)size.c	4.6 (2.11BSD) 2026/9/13";
#endif

/*
 * size
 */

#include	<stdio.h>
#include 	<a.out.h>
#include	<string.h>
#include	<errno.h>

int	header;

main(argc, argv)
char **argv;
{
	struct exec buf;
	long sum;
	register int i;
	int gorp, err = 0;
	register FILE *f;
	struct ovlhdr	ovlbuf;		/* overlay structure */
	long	coresize;		/* total text size */
	short	skip;			/* skip over overlay sizes of 0 */

	if (argc==1) {
		*argv = "a.out";
		argc++;
		--argv;
	}
	gorp = argc;
	while(--argc) {
		++argv;
		if ((f = fopen(*argv, "r"))==NULL) {
			printf("size: %s: %s\n", *argv, strerror(errno));
			err++;
			continue;
		}
		if (fread((char *)&buf, sizeof(buf), 1, f) != 1 ||
		    N_BADMAG(buf)) {
			printf("size: %s not an object file\n", *argv);
			fclose(f);
			err++;
			continue;
		}
		if (header == 0) {
			printf("text\tdata\tbss\tdec\thex\n");
			header = 1;
		}
		printf("%u\t%u\t%u\t", buf.a_text,buf.a_data,buf.a_bss);
		sum = (long) buf.a_text + (long) buf.a_data + (long) buf.a_bss;
		printf("%ld\t%lx", sum, sum);
		if (gorp>2)
			printf("\t%s", *argv);
		if (buf.a_magic == A_MAGIC5 || buf.a_magic == A_MAGIC6) {
			fread(&ovlbuf,sizeof(ovlbuf),1,f);
			coresize = buf.a_text;
			for (i = 0; i < NOVL; i++)
				coresize += ovlbuf.ov_siz[i];
			printf("\ttotal text: %ld\n\toverlays: ", coresize);
			for (i = 0,skip = 0; i < NOVL; i++) {
				if (!ovlbuf.ov_siz[i]) {
					++skip;
					continue;
				}
				for (;skip;--skip)
					fputs(",0",stdout);
				if (i > 0)
					putchar(',');
				printf("%u", ovlbuf.ov_siz[i]);
			}
		}
		printf("\n");
		fclose(f);
	}
	exit(err);
}
