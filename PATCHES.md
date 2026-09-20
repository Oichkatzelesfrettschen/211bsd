# 2.11BSD patches in this repository

Each patch commit against the original file it was applied from. The
originals are the bundles and numbered files from http://www.2bsd.com/2.11BSD/,
kept as immutable content in the titor-labs archive under
`archives/bsd/2bsd/patches/` and hashed in its
`provenance/CHECKSUMS-2bsd-patches.tsv`; the SHA-256 here is of the individual
patch text (a bundle member for 430-459, the numbered file for 460 onward).
Patch 437 is upstream's placeholder and has no commit. Patch 500 is, as of
its 2026-09-15 text, an announcement that the 35th Anniversary Edition is
close, not a patch.

This repository holds one deliberate deviation from upstream, in
`lib/libc/stdio/findfp.c`: `_fwalk` advances its dynamic-stream cursor and
tests `_flags != 0`, where patch 499 as published discards `g->next` and
tests `== 0`. The commit carrying it says so.

| Patch | Commit | Original | SHA-256 of the patch text | Subject |
| ---: | --- | --- | --- | --- |
| 432 | 267082b | bundle:430-439.tar.bz2 | `eb7cc204b485b99ebe8e1fd51e1bf1b061bba57ee758daf2c2e914ee7a72896f` | Y2K troff fix, 4.3compat remnant removed, misc cleanup (#432) |
| 433 | ef270ef | bundle:430-439.tar.bz2 | `ec935e39e74fcf2450aa79dd7720ee0a2130a5d8fd271826c48a32b75ac7a89b` | cpuid, standalone mkfs, libc makedep, inode allocation bugs (#433) |
| 434 | 55a36eb | bundle:430-439.tar.bz2 | `a928ff2fc84b2e03e68c333a0c338a6c38812df4e2d24a46d5a48d8799b33a4b` | standalone programs should not use FP (#434) |
| 435 | 306b50f | bundle:430-439.tar.bz2 | `a0512973ed8dad6d71234573094c25cde4700c673fa49962a6f443eaf5267578` | Y2K bug in FTPD (#435) |
| 436 | 6617235 | bundle:430-439.tar.bz2 | `dd0ea2b53b949cdb208d71dd5b49eaa02159c0cde07697ae18b715b68d2dee6f` | ftpd security fix, setenv documentation error |
| 437 | (none) | bundle:430-439.tar.bz2 | `80b07f37e72a7f2723f85f4119fb4e58cea610d31e73790776eae55e45c49204` | placeholder: Place Holder (#437) |
| 438 | 194c1b6 | bundle:430-439.tar.bz2 | `3f0e3858f1f60023d29b143b6a60977865c89d1c697aec1752ef5400d124599a` | RL driver disklabel problem (#438) |
| 439 | 178c297 | bundle:430-439.tar.bz2 | `ace2c924ff15952c5d7c8aca962fcced6d64184113c5d47dc20a092678462547` | #elif in cpp, disklabels in HK(4), more (#439) |
| 440 | 569e136 | bundle:440-447.tar.bz2 | `923a79da683766ac85301a8c0dd5ae33ae021a8e3f5351413be5c8cd938d0f16` | m11 & l11 illegal syscall use (#440) |
| 441 | e011359 | bundle:440-447.tar.bz2 | `bb20a7150013898a80482c426047122efa051cf613d6c3e584cf7e0c6970c180` | libvmf,cpp,rauboot,mkstr(1) fixes, ld feature (#441) |
| 442 | c27e677 | bundle:440-447.tar.bz2 | `7a11156dfb1f10784bff24fcfcd75d553c0c9c5e1c11b0d9e5a7bd06c629c015` | ANSI C reserved words used as variables, stdint.h missing (#442) |
| 443 | 3a401fd | bundle:440-447.tar.bz2 | `757e4f66459227ccc400768b8dbc035bd086a18fcec14c3c8c3a948b47a7e0e4` | man/man1/Makefile bug (#443) |
| 444 | 3c22d69 | bundle:440-447.tar.bz2 | `e2deb58f1450a01410519e498a6c9e3b8e7112431c6d1a3012324d9b3f217c9b` | ftp client long passwords, getty forcing even parity (#444) |
| 445 | 5d27421 | bundle:440-447.tar.bz2 | `f81bbbac30517f68f425739f2fa971bf5efd34c554b52687ab38b398b184b7d2` | kernel floating point simulation crash, postscript printer Makefile update |
| 446 | 400a5ae | bundle:440-447.tar.bz2 | `3deb380577fcad1eee6c80f636ea95c800d07d5cce876692607148417393e118` | fixes for libkern, autoconfig, umount, tar; add rkboot.s (#446) |
| 447 | 1a1be23 | bundle:440-447.tar.bz2 | `f599992b3f7ef8ca1c188d36f5e7e24d591c039bd7d62057b833a247a8b0d32b` | fixes for tcsh, ps, apropos, vmstat (#447) |
| 448 | 9046d34 | bundle:448-459.tar.bz2 | `6e11c540bdacf58b9aa0dc70384db2235fe1961f23435225f09e001c4015be5c` | 2BSD patches... |
| 449 | 5941983 | bundle:448-459.tar.bz2 | `f34afd7264679a30d1e8111063e5d374b160d2d8b9c37025b0e5d23e87932be7` | Revert /boot breakage, update VERSION |
| 450 | d4fed99 | bundle:448-459.tar.bz2 | `b6c3d395fe473455a3578b89a442073865095ce7866daa59da00fbf70ced7cf5` | /boot |
| 451 | 930227f | bundle:448-459.tar.bz2 | `9102c56e0274ebef6d9640d18b815ca9b2a1abecfc85445aa3f429998049508a` | sysctl securelevel -1 |
| 452 | f11187e | bundle:448-459.tar.bz2 | `4086d1dee4099b1864a242b9cab17869f7f597e571d3f0eb60b747d9087de3e9` | srandom(3) and initstate(3) incorrect input argument |
| 453 | 8f14494 | bundle:448-459.tar.bz2 | `a38beaacbd090ff4529c39a4df622f9fb156de8b8ba14f4198d6d1576086fa52` | fp simulator kernel crash, tcsh here doc crash, welcome y2k bug |
| 454 | 043475c | bundle:448-459.tar.bz2 | `15617007b4cf34243e32bb75d1fdb2c2996b9a3498225e77d52c7ecef07e735c` | kernel timezone struct deprecated |
| 455 | 2645afd | bundle:448-459.tar.bz2 | `c6bb2590ed6db6c5b6ed87643917f152b1aa72097438e78153db3dfd58175ec9` | timezone file dump utility not installed |
| 456 | e4dc5d6 | bundle:448-459.tar.bz2 | `22d1ce524f22622fa30263f10a42da27a9d16754ae706ecbe69e4f699270f267` | filesystem clean |
| 457 | 830017b | bundle:448-459.tar.bz2 | `ee1ea200d072208273b17a8732e466e70508435cf907c88707bc786b91381c7a` | tcsh, csh, pascal, timezone, rwhod, mkstr fixes and cleanup |
| 458 | 43746ac | bundle:448-459.tar.bz2 | `d457a9cac3d1600683aa42756d1f8502cd0d646d4763003bee7ba660b22f59cb` | superblock modified time set to 0 on boot |
| 459 | 3c38818 | bundle:448-459.tar.bz2 | `69159bbf72ec4520d25950b47d88cbce9758b62d7cf1b6842f2bff1203c3e24c` | unmaintained/obsolete/unnecessary file cleanup + new include file |
| 460 | 81df012 | file | `2472fcffd941f389e690fb088c442125c868e349fdaee9848a0fa2bbb99cfd48` | 2.11BSD completely lacks ANSI C capability |
| 461 | 47b8ef2 | file | `d107b74b788ed4065ac3884e0fdd59b93baa05a294f22a727604394717252e66` | buffer flush incomplete at reboot / raising vi max editable filesize |
| 462 | 0b3a842 | file | `279dc33c2e13a9f4b07a688c51e09bf645f9cfa4166a5ab978f78baca58af2bc` | f77: build error, stop statement error, error message bug, Makefile bug |
| 463 | de64824 | file | `9728c21f88efdfde9709d2a3324471276baaa0a97e85a62a6b0503b0ce7fd435` | sysctl(3) returns invalid VM_COREMAP and VM_SWAPMAP data, sendbug(1) uses obsolete addresse |
| 464 | 2362449 | file | `6951eb889a6305e30edbe795d0dfe2a666cf8f6076e0422a23dc3f19d7e36db5` | sysctl coremap/swapmap retrieval error |
| 465 | bf8ab8d | file | `710a8d65f94e6cf475e985d74dc5516b9082a033ee37763db4a858ef6d01b7c1` | C compiler prototype redefines static variable, new top(1) program |
| 466 | 349da1e | file | `56199df8340b78c98febd25d39fd76900709108f1b5d50c5f78422042a308663` | size(1) incorrect error message, df(1) output format, top(1) enhancements |
| 467 | e9c65a1 | file | `080b1eb6f7eb8b76dc3e57617c678f7ac5abbb0f8e3c04f8812544f708127a36` | pdpstand/prf.c compile error, top(1) swap space calculation, tn3270 comile error |
| 468 | 5c33b3a | file | `fc5a32a6a88ad06abf2c9af221238fbbe73eeee2f9e649bc7b69c53fc08ea13d` | nroff/troff 4.4BSD mdoc manpage format |
| 469 | 8b99336 | file | `dc181b6f3242a850d366e1d1f3e519e8e7a651ae31e189ccd95a9317ea9d6f54` | North America timezone files outdated |
| 470 | afa0e3e | file | `8fe1bca220408e83aba4b2ff3a0a0efc70ce9fcf2ff47400799afd666ae8970b` | nroff updates require updated makewhatis.sed |
| 471 | b91c3d2 | file | `19dc1b8783e4bdd0ad49dbf11771b5a30020e30991289d3764f612be38f3375a` | if_qe+if_qt crash, more(1) bold bug, top(1) cleanup, new program netcat(1) |
| 472 | 102b489 | file | `16e32fd8951d42ccd49a97c2dec313b7fff60533ed2fdc5aa15422a72a12c6b4` | at(1) Y2K and leap year bugs |
| 473 | 05fac22 | file | `08959b84fe40bdb9a96739e354fc6480a256b46ebb003a09cccbee3a38144fcb` | compiler bugs, missing prototypes, hex format strings fixed |
| 474 | 8464550 | file | `21822f26fc5f2aa8b37364bf8aa865bfa999e05824a0db59a09e80be8969f1e3` | J11 hack 29th anniversary patch (#474) |
| 475 | 58f8483 | file | `95f75254cc025f9900b053229d55d36d7b894f2140fd8b157777074dde550ee4` | sysctl(2) string handling error (#475) |
| 476 | fb02a4d | file | `080ef615f7c124038ddcc298b8d2bdc2963c5f8cddfa6b687a07ae7391f67eb8` | RP07 boot support / MASSBUS bootblock consolidation (#476) |
| 477 | 069570a | file | `09bcb166e55407a6662ef022ca8f52b175fd193c6d83a86dff5150272b7cf281` | cleanup compilation warnings (#477) |
| 478 | 27b636a | file | `620706afac3726fca3c07f7b25930e1bb9ef0809446d1601c07db0a25ace6471` | cleanup - remove warning errors and Makefile cruft (#478) |
| 479 | d9878e0 | file | `16ebf63be984e589bd19e72981cae62f8ac14a7eb5b37d33e488e782c462af74` | Y2K bug in ftp(1) |
| 480 | 8a1e7f0 | file | `6e679e5cc5f5e25fdab615dae1f04ab25ac190d36a708afa0d2f5394610a0fc7` | DZ driver missing 8bit support, getty missing NoParity option, login loses 8bit flag(s) fro |
| 481 | b53de8a | file | `96e9f9164d05522bfcc6777fa23aebea36e1da3504f66321bc32cbfa670f756a` | telnetd missing gettytab support, minor warp Makefile cleanup |
| 482 | 19292d8 | file | `d2fe710431aa9c1bc3c3e2c8e748c9864c23899b50a53d249ee0a599c6c808ac` | /vmunix removal, disktab errors, bad144 fails, dump exit status wrong, cc escaped char omis |
| 483 | c27beb4 | file | `c6d1db66dabd84b61055d37f8907c434be22e20bfd226a4ed1e216b4db71fdab` | misc cleanup, unistd.h bug, telnetd pty speed, hk&xp badsector partition |
| 484 | 05a1fb7 | file | `de8963d152222940f42ca02dacb4fad3aae6c3f70965f833272c0035785a1951` | 8bit clean tty driver, ethernet card attach info, xterm-color, telnetd window size change,  |
| 485 | 17b0d9c | file | `55e154f2b878bc486ba03c38fc6be7f76e406a5964042805d245893ab668297b` | vi display/file corruption and/or crash when window is resized. |
| 486 | ca51282 | file | `2e2126d1e49e981a92b6581e616b43bf7d7004c4bbd3ee4df41f78d03222306f` | adb incorrect prints function arguments, local variables and external symbols |
| 487 | 3bec6c1 | file | `30f220acd9c99e27cea8822edd12f5380b4fabf5698985a32de0ece73b1e6f92` | login & telnetd editing/message ignore gettytab, ftp lacks PASV, chflags(1) source and chfl |
| 488 | 646f751 | file | `f9e342e2a3cc04ca4d8d2d01f20d38de260ce6aa65f22d85999f4b2c58317e87` | adb single step bug, login disconnect, cc missing __TIME__/__DATE__, cpp bug, longjmp numbe |
| 489 | e680203 | file | `0cf4234f197fe3a6372355a838909179ddf61cd98d6779b1e8d44f6ffe23e4ec` | bin/sort symlink, rcmd&ruserpass.c unused variables, telnet window changes, top malloc erro |
| 490 | abdf952 | file | `f9e6879a0df481d61f8940921a9874b34645f7e7597e5b3fa6c7db1fb7b4ea96` | C Compiler bug with right shift of unsigned long  |
| 491 | 6e8b1d7 | file | `211cc14633ac387322fb2e81cab73a3292b51e1b0aff6f55ee8d66035ec7c130` | cpp(1) lacks cpp.0, adb tutorial can't be made, |
| 492 | 332d464 | file | `5db288725a349421c1801a4d5af2ad40531750952f7ab51e3bebb71776f68394` | dd(1) improvements, as(1) NULL dereference, adb(1) no memory message, printf(3) return valu |
| 493 | f775e19 | file | `b9af55404e3850b6eb1cdfa104c3fff0bbb8b0012990b27606fcc28dd4d40882` | dhu.c should support the DHQ11 with 8 lines in DHU11 mode (#493) |
| 494 | 5e1db53 | file | `fcbb9880f8f764633738016cdc8df6b134d6b55477c5e49e03ebbad6566eb40b` | DZ driver syntax error, top(1) long usernames bug (#494) |
| 495 | dd1eb9f | file | `dde6ae610336537a958d2b9b576e0897f4c45635643f646b5c9a75200368ff3d` | __DATE__, __TIME__ Macros defined in wrong place, dd bug (#495) |
| 496 | a89e138 | file | `e1f16d5c3bb208249997b5760429d4eb4b3679fde2671b64b1c950cd2485cab7` | C compiler bugs, cpp runs out of memory, yacc  (#496) |
| 497 | 0bd957b | file | `6399206c77b72c5c7bb25090da86f7753da34ba1785d92d3a94698c0c51bb48e` | efl (Extended Fortran Language) manpage installed but no executable (#497) |
| 498 | 4200bce | file | `2e52770a764447e7a0c583d3b0a165848855cf47ed03ab0fa8cdd283225fee00` | remove trailer support, fix savecore(8) bug, delete obsolete files/directories (#498) |
| 499 | ab5dc1d | file | `139439fdea8615e3b543011fdb9b71291f96b25e8ea8b3713b14e6103ded6189` | massive cleanup, restor pipe input, ANSI compliant stdio,  inconsistent LDFLAGS/CFLAGS use, |
