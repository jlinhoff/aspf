// file: sz.h
#ifndef SZ_H
#define SZ_H

// Copyright 2005-2016 Joe Linhoff
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BASE_H
#include "base.h"
#endif // ndef BASE_H

#ifdef __cplusplus  // C in C++ wrapper
extern "C" {        //
#endif              //

///////////////////////////////////////////////////////////////////////////////
// SZ STRINGS

#define S_SZCHR_SIZE 0 // shift for character size

#define C_SZ_SKIP 1 // this character value is skipped in %c

// string routines follow these rules:
// - buffer building functions guarantee a zero-terminator (size > 0)
//
// parameter naming conventions
// - 'ss' points to the string start, i.e. the first chr in the string
// - 'sx' points to the string terminator, i.e. one past the last chr
//   a null pointer can always be passed in for zero-terminated strings
// - 'ts' and 'tx' are used for the second string
// - pass the byte size of the whole buffer
//
// flag table:
// uint32_t flagtable[0] = number of entries in table
//       flagtable[1] = flags for character 1 ..

// reserve top 8 bits for system use
#define M_SZF_RESERVED       0xff000000 // not in table -- system use
#define M_SZF_RSVDFORAPP     0x000ff000 // not in table -- app use
#define S_SZF_RSVDFORAPP     12         // shift
#define M_SZF_NEST           0x01000000 // ({[<
#define M_SZF_DEEPNEST       0x02000000 // not in table -- system use
#define M_SZF_ONENEST        0x04000000 // not in table -- system use
#define M_SZF_PREINC         0x10000000 // not in table -- signals pre inc
#define M_SZF_DSTSIZE_NOTERM 0x20000000 // not in table -- system use
#define M_SZF_DSTSIZE_LEN    0x40000000 // not in table -- system use

// default flag table bits -- bottom 24 bits
#define M_SZF_ALPHA     0x00000001 // a-zA-Z
#define M_SZF_NUM       0x00000002 // 0-9
#define M_SZF_WHITE     0x00000004 // space \t
#define M_SZF_EOL       0x00000008 // \r\n
#define M_SZF_ID        0x00000010 // a-zA-Z0-9_.
#define M_SZF_SYMBOLS   0x00000020 // _=~`!@#$%^&*:?|\/
#define M_SZF_BRACKETS  0x00000040 // (){}[]<>
#define M_SZF_MODNUM    0x00000080 // -.+eE
#define M_SZF_SLASHMETA 0x00000100 // e.g. \a
#define M_SZF_QUOTE     0x00000200 // '"
#define M_SZF_SLASH     0x00000400 // \/
#define M_SZF_DELIM     0x00000800 // ,;/
#define M_SZF_NUMSYM    0x00001000 // +-.
#define M_SZF_APP1      0x00002000 // for app use
#define M_SZF_APP2      0x00004000 // for app use
#define M_SZF_APP3      0x00008000 // for app use
#define M_SZF_APP4      0x00010000 // for app use

#define SZ_ERR_RETURN (-253)
#define SZ_MISMATCH_RETURN (-254)

SPF_API int szInit();
SPF_API void szFinal();

SPF_API int szeos(chr *s1,chr *s1x); // utf8 end-of-string test
SPF_API chr* szadv(chr *s1,chr *s1x,int add); // utf8 advance
SPF_API int szcsize(chr *str); // utf8 size in bytes for character
SPF_API int szccpy(chr *dst,int dstsize,chr *src); // utf8 copy one character
SPF_API int szlen(chr *s1,chr *s1x); // utf8 number of characters
SPF_API int szsize(chr *s1,chr *s1x); // utf8 size of string in bytes
SPF_API int szcpy(chr *dst,int dstsize,chr *s1,chr *s1x); // utf8 copy
SPF_API int szcat(chr *dst,int n,chr *s1,chr *s1x,chr *s2,chr *s2x); // utf8 cat
SPF_API int szcpycatm(chr *dst,int n,chr *cpy,chr *cpyx,...); // (chr *cat,chr *catx) cat==0 term
SPF_API int szcmp(chr *s1,chr *s2,chr *s2x); // utf8 compare
SPF_API int szicmp(chr *s1,chr *s2,chr *s2x); // utf8 compare (avoid)
SPF_API int szcmp2(chr *s1,chr *s1x,chr *s2,chr *s2x); // utf8 compare
SPF_API int szicmp2(chr *s1,chr *s1x,chr *s2,chr *s2x); // utf8 compare

// return SZ_MISMATCH_RETURN when not matched
SPF_API int szmap(chr *table[],chr *s1,chr *s1x);
SPF_API int szimap(chr *table[],chr *s1,chr *s1x);

typedef struct { // used for szmapv
   chr *name; // 0 to terminate array
   int v;
} szMapVRec;

// return SZ_MISMATCH_RETURN when not matched
SPF_API int szmapv(szMapVRec *table,chr *str,chr *strx);

// dsttype: i=int u=uns f=float d=double j=int16 J=uns16 l,q=int64
SPF_API chr* sztobin(chr *s1,chr *s1x,int base,chr dsttype,void *dst);
SPF_API chr* szutf8c(chr *s1,chr *s2,uint32_t *dstUTF8);
SPF_API int sztobinarray(ptr dst,int dstMax,chr dstType,chr **strp,chr *strx);

// % commands different from standard sprintf (incomplete)
//  S (chr *str,chr *strx) pointer-terminated string
//  qd,qi (int64 int) base10
//  qx,qX (uns64 int) base16
//  < begin special command mode
//   f (chr *fmt) reload fmt string, fmtx=0
//   > end special command mode
//   a (vargs args) reload args
//   o (fnc_ipi outf) output function: int outf(chr *buf,int len)
//   r return to normal mode / end special command mode
SPF_API int szfmt(chr *dst,int dstsize,chr *fmt,...);
SPF_API int szfmt_v(chr *dst,int dstsize,chr *fmt,chr *fmtx,chr **fmtp,...);

SPF_API uint32_t *szft;
SPF_API uint32_t sztand(uint32_t c,uint32_t flags); // test c
SPF_API uint32_t sztandc(chr *str,chr *strx,uint32_t flags); // utf8 first c tested
SPF_API uint32_t* szflagtable(uint32_t *flagtable);
SPF_API chr* szchr(chr *s1,chr *s1x,uint32_t c);
SPF_API int szmap(chr *table[],chr *s1,chr *s1x);
SPF_API chr* sztskip(chr *s1,chr *s1x,uint32_t flags);
SPF_API chr* szttill(chr *s1,chr *s1x,uint32_t flags);
SPF_API chr *sznextline(chr *s1,chr *s1x,int *countp);
SPF_API chr* szsub(chr *sub,chr *subx,chr *s2,chr *s2x);
SPF_API int szstart(chr *start,chr *s2,chr *s2x);
SPF_API int szstart2(chr *start,chr *startx,chr *s2,chr *s2x);
SPF_API int szistart(chr *start,chr *s2,chr *s2x);
SPF_API int szistart2(chr *start,chr *startx,chr *s2,chr *s2x);

SPF_API chr* szquotes(chr *qstart,chr *bufx,chr **startp,chr **endp);
SPF_API int szpackslasheolwhite(chr *s1,chr *s1x,chr **s1xp);
SPF_API int szpackmeta(chr *s1,chr *s1x,chr **s1xp,
   chr *metatable,chr *replacetable);
SPF_API int szhash(chr *s1,chr *s1x); // non-zero, non-negative
SPF_API int szcountlines(chr *str,chr *strx);

#ifdef __cplusplus  // C in C++ wrapper
}                   //
#endif              //

#endif // SZ_H
