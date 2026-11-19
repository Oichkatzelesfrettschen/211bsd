#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)config_f.h      2.0 (2.11BSD) 2026/7/23";
#endif
/*
 * config_f.h -- configure various defines for tcsh
 *
 * This is included by config.h.
 *
 * Edit this to match your particular feelings; this is set up to the
 * way I like it.
 */
/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _h_config_f
#define _h_config_f

/*
 * SHORT_STRINGS Use 16 bit characters instead of 8 bit chars
 * 	         This fixes up quoting problems and eases implementation
 *	         of nls...
 *
 */
#undef SHORT_STRINGS

/*
 * NLS:		Use Native Language System
 *		Routines like setlocale() are needed
 *		if you don't have <locale.h>, you don't want
 *		to define this.
 */
#undef NLS

/*
 * LOGINFIRST   Source ~/.login before ~/.cshrc
 */
#undef LOGINFIRST

/*
 * VIDEFAULT    Make the VI mode editor the default
 */
#undef VIDEFAULT

/*
 * CSHDIRS    save a history like stack of directories
 */
#define CSHDIRS

/*
 * DOTLAST      put "." last in the default path, for security reasons
 */
#define DOTLAST

/*
 * AUTOLOGOUT	tries to determine if it should set autologout depending
 *		on the name of the tty, and environment.
 *		Does not make sense in the modern window systems!
 */
#undef AUTOLOGOUT

/*
 * KANJI	Ignore meta-next, and the ISO character set. Should
 *		be used with SHORT_STRINGS
 *
 */
#undef KANJI

/*
 * TCLONGNAME	print descriptive strings as well t_c_ names
 *		Don't define this for BSD2: limited data space 
 */
#undef TCLONGNAME

/*
 * LONGFUNCS	print descriptive strings as well function names
 *		Don't define this for BSD2: limited data space 
 */
#undef LONGFUNCS

#endif /* _h_config_f */
