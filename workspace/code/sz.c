// sz.c
// Copyright (C) 2016 Joe Linhoff - see license
#include "base.h"
#include "sz.h"

chr S0_ERRFLOAT[] = {'1','.','#','E','R','R',0};
chr S0_ERRARG[] = {'#','E','R','R',0};

#define SZ_ERR_RETURN (-253)
#define SZ_MISMATCH_RETURN (-254)

///////////////////////////////////////////////////////////////////////////////

uint32_t *szft=0; // this is global
uint32_t szDefaultFlagTable[256];

// JFL 23 Jul 06
int szFTSetup()
{
   uint32_t *ft;
   int j;
   uint8_t c;
   const chr *s1;

   //
   // setup sz flag table for parsing
   //

   for(j=0;j<sizeof(szDefaultFlagTable)/sizeof(szDefaultFlagTable[0]);j++)
      szDefaultFlagTable[j]=0;
   ft=szDefaultFlagTable;
   ft[0]=NUM(szDefaultFlagTable); // first is num

   for(j='a';j<='z';j++)
      ft[j]|=M_SZF_ALPHA|M_SZF_ID;
   for(j='A';j<='Z';j++)
      ft[j]|=M_SZF_ALPHA|M_SZF_ID;
   for(j='0';j<='9';j++)
      ft[j]|=M_SZF_NUM|M_SZF_ID;
   s1="_.";
   while((c=*s1++))
      ft[c]|=M_SZF_ID;
   s1=",_=~`!@#$%^&*:?|\\/";
   while((c=*s1++))
      ft[c]|=M_SZF_SYMBOLS;
   s1="(){}[]<>";
   while((c=*s1++))
      ft[c]|=M_SZF_BRACKETS;
   s1="-.+eE";
   while((c=*s1++))
      ft[c]|=M_SZF_MODNUM;
   s1="-.+";
   while((c=*s1++))
      ft[c]|=M_SZF_NUMSYM;
   s1="\\/";
   while((c=*s1++))
      ft[c]|=M_SZF_SLASH;
   s1=",;/";
   while((c=*s1++))
      ft[c]|=M_SZF_DELIM;
  s1="\'\"";
   while((c=*s1++))
      ft[c]|=M_SZF_QUOTE;
   s1=" \t";
   while((c=*s1++))
      ft[c]|=M_SZF_WHITE;
   s1="\r\n";
   while((c=*s1++))
      ft[c]|=M_SZF_EOL;
   s1="({[<"; // nesting starts
   while((c=*s1++))
      ft[c]|=M_SZF_NEST;

   szflagtable(szDefaultFlagTable);

   return 0;
} // szFTSetup()

///////////////////////////////////////////////////////////////////////////////

// JFL 12 Dec 09
int szInit()
{
   szFTSetup();
   return 0;
} // szInit()

// JFL 12 Dec 09
void szFinal()
{
   return;
} // szFinal()

///////////////////////////////////////////////////////////////////////////////
// SIZE & LENGTH & SIMPLE

#define SZUTF8N(_c_) !((_c_)&0x80)?1: \
   (((_c_)&0xe0)==0xc0)?2: \
   (((_c_)&0xf0)==0xe0)?3: \
   (((_c_)&0xf8)==0xf0)?4: \
   (((_c_)&0xfc)==0xf8)?5: \
   (((_c_)&0xfe)==0xf8)?6:1

// JFL 23 Oct 06
// JFL 01 Feb 07
int szlen(chr *s1,chr *s1x)
{ // return number of characters in string
   int i;
   uint8_t k;
   if(!s1) return 0;
   if(s1x) return (int)(s1x-s1);
   for(i=0;*s1;i++)
   {
      k=SZUTF8N(*s1);
      while(k && *s1)
         k--,s1++;

      //if(!(*s1&0x80))
      //   {s1++;continue;}
      //else if((*s1&0xe0)==0xc0)
      //   {s1+=2;continue;}
      //else if((*s1&0xf0)==0xe0)
      //   {s1+=3;continue;}
      //else if((*s1&0xf8)==0xf0)
      //   {s1+=4;continue;}
      //else if((*s1&0xfc)==0xf8)
      //   {s1+=5;continue;}
      //else if((*s1&0xfe)==0xfc)
      //   {s1+=6;continue;}
      //else
      //{
      //   BRK();
      //   s1++;
      //}
   } // for
   return i;
} // szlen()

// JFL 23 Oct 06
// JFL 01 Feb 07
// JFL 25 Feb 12; test for s1x < s1
int szsize(chr *s1,chr *s1x)
{ // return size of buffer needed for string
   int i;
   if(!s1) return 0;
   if(s1x) {
     if((i = (int)(sizeof(chr)+PTR_DIFF(s1x,s1)))<0)
        i=0;
     return i;
   }
   i=sizeof(chr); // for term
   while(*s1++)
      i+=sizeof(chr);
   return i;
} // szsize()

// JFL 29 Sep 05
int szeos(chr *s1,chr *s1x)
{ // test if s1 is end-of-string
   if(!s1) return 1; // eos
   if(!*s1) return 1; // eos
   if(s1x&&(s1>=s1x)) return 1; // eos
   return 0; // not eos
} // szeos()

// 1 0xxx xxxx
// 2 110x xxxx
// 3 1110 xxxx
// 4 1111 0xxx
// 5 1111 10xx
// 6 1111 110x

// JFL 29 Sep 05
// JFL 03 Sep 14; utf8
chr* szadv(chr *s1,chr *s1x,int add)
{ // advance string forward add characters
   uint8_t k;
   if(!s1) return s1;
   while(add-->0)
   {
      if(!*s1)
         break;
      if(s1x&&(s1>=s1x))
         return s1x;
      k=SZUTF8N(*s1);
      while(k && *s1)
         k--,s1++;
   } // while
   return s1;
} // szadv()

// JFL 03 Sep 14; utf8
int szcsize(chr *str)
{ // byte size of character
   if(!str || !*str)
      return 0;
   if(!(*str&0x80))
      return 1;
   else if((*str&0xe0)==0xc0)
      return 2;
   else if((*str&0xf0)==0xe0)
      return 3;
   else if((*str&0xf8)==0xf0)
      return 4;
   else if((*str&0xfc)==0xf8)
      return 5;
   else if((*str&0xfe)==0xfc)
      return 6;
   BRK();
   return 1;
} // szcsize()

// JFL 03 Sep 14; utf8
int szccpy(chr *dst,int dstsize,chr *str)
{ // byte size of character
   if(!str || !*str)
      return 0;
   if(!(*str&0x80))
   {
      if(dstsize<1)
         return 0;
      *dst=*str;
      return 1;
   }
   else if((*str&0xe0)==0xc0)
   {
      if(dstsize<2)
         return 0;
      *dst++=*str++;
      *dst++=*str++;
      return 2;
   }
   else if((*str&0xf0)==0xe0)
   {
      if(dstsize<3)
         return 0;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      return 3;
   }
   else if((*str&0xf8)==0xf0)
   {
      if(dstsize<4)
         return 0;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      return 4;
   }
   else if((*str&0xfc)==0xf8)
   {
      if(dstsize<5)
         return 0;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      return 5;
   }
   else if((*str&0xfe)==0xfc)
   {
      if(dstsize<6)
         return 0;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      *dst++=*str++;
      return 6;
   }
   BRK();
   return 0;
} // szccpy()

uns hashspread[] = {
   0x00208001,
   0x40020010,
   0x01002100,
   0x10010001,
   0x04010008,
   0x00104080,
   0x01000204,
   0x00101010,
};

// JFL 10 May 12
// JFL 05 Sep 12
int szhash(chr *s1,chr *s1x)
{ // top uint8_t never set w/valid hash
   int ival;
   uns xval,rval=0;
   if(!s1)
      goto BAIL;
   for(;!s1x || (s1<s1x);)
   {
      if(!(ival=*s1++))
         break;
      xval=rval>>27;
      rval<<=5;
      rval^=xval;
      if(ival&1)
         rval^=hashspread[0];
      if(ival&2)
         rval^=hashspread[1];
      if(ival&4)
         rval^=hashspread[2];
      if(ival&8)
         rval^=hashspread[3];
      if(ival&0x10)
         rval^=hashspread[4];
      if(ival&0x20)
         rval^=hashspread[5];
      if(ival&0x40)
         rval^=hashspread[6];
      if(ival&0x80)
         rval^=hashspread[7];
   } // for
   rval<<=1,rval>>=1; // clean top uint8_t
BAIL:
   return rval;
} // szhash()

///////////////////////////////////////////////////////////////////////////////
// COPY & CONCATENATION

// JFL 15 Aug 04
// JFL 06 Feb 05
// JFL 01 Feb 07; sizeof(chr)
// JFL 17 Jun 07; return review
int szcpy(chr *dst,int n,chr *s1,chr *s1x)
{
   int org;
   uint8_t noterm=0;

   // size to len
   if(n&M_SZF_DSTSIZE_LEN)
      n&=~M_SZF_DSTSIZE_LEN;
   else
      n>>=S_SZCHR_SIZE;

   // noterm option
   if(n&(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE))
   {
      n&=~(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE);
      noterm=1;
   }
   else
      n--; // room for term

   if((n<0) || !s1) return 0;
   org=n;

   while(n && (!s1x || (s1<s1x)))
   {
      if(!*s1) break;
      *dst++=*s1++;
      n--;
   } // while

   if(!noterm)
      *dst=0;

   return org-n;
} // szcpy()

// JFL 15 Aug 04
// JFL 01 Feb 07; sizeof(chr)
// JFL 17 Jun 07; return review
// JFL 18 Dec 10; two string version
int szcat(chr *dst,int n,chr *s1,chr *s1x,chr *s2,chr *s2x)
{ // concatenate string to existing
   int org;
   uint8_t noterm=0;

   // size to len
   if(n&M_SZF_DSTSIZE_LEN)
      n&=~M_SZF_DSTSIZE_LEN;
   else
      n>>=S_SZCHR_SIZE;

   // noterm option
   if(n&(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE))
   {
      n&=~(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE);
      noterm=1;
   }
   else
      n--; // room for term

   if((n<0) || !s1) return 0;
   org=n;

   // skip over current string
   while(n)
   {
      if(!*dst) break;
      dst++;
      n--;
   } // while

   // concatenate first string
   if(s1)
   while(n && (!s1x || (s1<s1x)))
   {
      if(!*s1) break;
      *dst++=*s1++;
      n--;
   } // while

   // concatenate second string
   if(s2)
   while(n && (!s2x || (s2<s2x)))
   {
      if(!*s2) break;
      *dst++=*s2++;
      n--;
   } // while

   if(!noterm)
      *dst=0;

   return org-n;
} // szcat()

// JFL 15 Aug 04
// JFL 01 Feb 07; sizeof(chr)
// JFL 17 Jun 07; return review
// JFL 18 Dec 10; two string version
// JFL 20 Dec 15; multi-cpycat version
int szcpycatm(chr *dst,int n,chr *cpy,chr *cpyx,...)
{ // concatenate string to existing
   int org=n;
   chr *cat,*catx;
   vargs args;
   
   varg_start(args,cpyx);

   n--; // room for term
   if((n<0) || !cpy) return 0;
   
   // copy first string
   while(n && (!cpyx || (cpy<cpyx)))
   {
      if(!*cpy) break;
      *dst++=*cpy++;
      n--;
   } // while

   // concatenate next string
   while((cat=varg_next(args,chr*)))
   {
      catx=varg_next(args,chr*);
      while(n && (!catx || (cat<catx)))
      {
         if(!*cat) break;
         *dst++=*cat++;
         n--;
      } // while
   
   } // while

   *dst=0;

   varg_end(args);
   return org-n;
} // szcpycatm()

///////////////////////////////////////////////////////////////////////////////
// COMPARISONS

// JFL 15 Aug 04
// JFL 22 Sep 05; s2 end fix
int szcmp(chr *s1,chr *s2,chr *s2x)
{ // compare two strings
   chr s,t;

   if(!s1 || !s2)
      return SZ_ERR_RETURN;

   while(!s2x || (s2<s2x))
   {
      s=*s1++;
      t=*s2++;
      if(s!=t) // mismatch
         return s-t;
      if(!s) // source hit end, match
         return 0;
   } // while

   if(!*s1)
      return 0;

   return SZ_MISMATCH_RETURN;
} // szcmp()

// JFL 03 Nov 05
// JFL 24 Jan 07; from szicmp2
int szcmp2(chr *s1,chr *s1x,chr *s2,chr *s2x)
{ // compare two strings
   chr s,t;

   if(!s1 || !s2)
      return SZ_ERR_RETURN;

   if(s1x && s2x && ((s1x-s1)!=(s2x-s2)))
      return (int)((s1x-s1)-(s2x-s2));

   while(1)
   {
      if(s1x && (s1>=s1x))
      { // s1x
         // s1 hit end
         if(s2x)
         {
            if(s2>=s2x) return 0;
            else return SZ_MISMATCH_RETURN;
         }
         else
         {
            if(!*s2) return 0;
            else return SZ_MISMATCH_RETURN;
         }
      } // s1x
      if(s2x && (s2>=s2x))
      {
         if(s1x) return SZ_MISMATCH_RETURN; // if s1x, it didn't end string
         if(!*s1) return 0;
         else return SZ_MISMATCH_RETURN;
      }
      s=*s1++;
      t=*s2++;
      if(s!=t) // mismatch
         return s-t;
      if(!s) // source hit end and s==t, match
         return 0;
   } // while
   return SZ_MISMATCH_RETURN;
} // szcmp2()

// JFL 15 Aug 04
// JFL 22 Sep 05; s2 end fix
int szicmp(chr *s1,chr *s2,chr *s2x)
{ // case insensitive compare of two strings
   chr s,t;

   if(!s1 || !s2)
      return SZ_ERR_RETURN;

   while(!s2x || (s2<s2x))
   {
      s=*s1++;
      t=*s2++;
      if((s>='A')&&(s<='Z')) s=s-'A'+'a';
      if((t>='A')&&(t<='Z')) t=t-'A'+'a';
      if(s!=t) // mismatch
         return s-t;
      if(!s) // source hit end, match
         return 0;
   } // while

   if(!*s1)
      return 0;

   return SZ_MISMATCH_RETURN;
} // szicmp()

// JFL 03 Nov 05
// JFL 24 Jan 07; from szicmp2
// JFL 29 Aug 11; icmp2
int szicmp2(chr *s1,chr *s1x,chr *s2,chr *s2x)
{ // compare two strings
   chr s,t;

   if(!s1 || !s2)
      return SZ_ERR_RETURN;

   if(s1x && s2x && ((s1x-s1)!=(s2x-s2)))
      return (int)((s1x-s1)-(s2x-s2));

   while(1)
   {
      if(s1x && (s1>=s1x))
      { // s1x
         // s1 hit end
         if(s2x)
         {
            if(s2>=s2x) return 0;
            else return SZ_MISMATCH_RETURN;
         }
         else
         {
            if(!*s2) return 0;
            else return SZ_MISMATCH_RETURN;
         }
      } // s1x
      if(s2x && (s2>=s2x))
      {
         if(s1x) return SZ_MISMATCH_RETURN; // if s1x, it didn't end string
         if(!*s1) return 0;
         else return SZ_MISMATCH_RETURN;
      }
      s=*s1++;
      t=*s2++;
      if((s>='A')&&(s<='Z')) s=s-'A'+'a';
      if((t>='A')&&(t<='Z')) t=t-'A'+'a';
      if(s!=t) // mismatch
         return s-t;
      if(!s) // source hit end and s==t, match
         return 0;
   } // while
   return SZ_MISMATCH_RETURN;
} // szicmp2()

///////////////////////////////////////////////////////////////////////////////
// SCANNING

// JFL 26 Oct 05
uint32_t sztand(uint32_t c,uint32_t flags)
{
   if(c<szft[0])
      return szft[c]&flags;
   return 0;
} // sztand()

// JFL 19 Jun 14
// JFL 29 Nov 14; utf7
uint32_t sztandc(chr *str,chr *strx,uint32_t flags)
{
   uns c;
   if(!str || (strx&&(str>=strx)))
      return 0;
   str=szutf8c(str,strx,&c);
   if(c<szft[0])
      return szft[c]&flags;
   return 0;
} // sztandc()

// JFL 21 Sep 05
uint32_t* szflagtable(uint32_t *flagtable)
{
   uint32_t *old=szft;
   if(flagtable)
      szft=flagtable;
   return old;
} // szflagtable()

// JFL 15 Aug 04
// JFL 29 Nov 14; utf8
chr* szchr(chr *s1,chr *s1x,uint32_t c)
{ // find first chr in string
   chr *s2;
   uint32_t cc;
   if(!s1) return 0;
   for(;!s1x || (s1<s1x);)
   {
      if(!*s1)
         break;
      s2=s1;
      s1=szutf8c(s1,s1x,&cc);
      if(c==cc)
         return s2;
   } // for
   return 0;
} // szchr()

// JFL 21 Sep 05
// JFL 27 Sep 05
chr* sztskip(chr *s1,chr *s1x,uint32_t flags)
{ // advance while chrs flags are set, simple nesting
   chr c;
   if(!szft) return s1;
   if(!s1) return s1;
   if(flags&M_SZF_PREINC)
   {
      if(!(c=*s1)) return s1;
      if(s1x&&(s1>=s1x)) return s1x-1;
      s1++;
   } // preinc
lp:
   if(!(c=*s1)) return s1;
   if(s1x&&(s1>=s1x)) return s1x;
   if((uint8_t)c<szft[0])
   { // table
      if((szft[(uint8_t)c]&M_SZF_SLASHMETA)&&(flags&M_SZF_SLASHMETA))
         s1++;
      else if(!(szft[(uint8_t)c]&flags)) // return first that doesn't match
         return s1;
   } // table
   s1++;
   goto lp;
} // sztskip()

// JFL 21 Sep 05
// JFL 27 Sep 05
chr* szttill(chr *s1,chr *s1x,uint32_t flags)
{ // advance until chr flag is hit, simple nesting
   chr c;
   if(!szft) return s1;
   if(!s1) return s1;
   if(flags&M_SZF_PREINC)
   {
      if(!(c=*s1)) return s1;
      s1++;
   } // preinc
lp:
   if(!(c=*s1)) return s1;
   if(s1x&&(s1>=s1x)) return s1x;
   if((uint8_t)c<szft[0])
   { // table
      if((szft[(uint8_t)c]&M_SZF_SLASHMETA)&&(flags&M_SZF_SLASHMETA))
         s1++;
      else if(szft[(uint8_t)c]&flags) // return first match
         return s1;
   } // table
   s1++;
   goto lp;
} // szttill()

// JFL 19 Nov 19
chr *sznextline(chr *s1,chr *s1x,int *countp)
{
   chr c,d;
   if(countp) *countp=0;
   if(!szft) return s1;
   if(!s1) return s1;
lp:
   if(!(c=*s1)) return s1;
   if(s1x&&(s1>=s1x)) return s1x;
   if(((uint8_t)c>=szft[0]) || !(szft[(uint8_t)c]&M_SZF_EOL))
   {
      s1++;
      goto lp;
   }

eollp1:
   if(countp) (*countp)++;
eollp2:
   d=*(++s1);
   if(((uint8_t)d>=szft[0]) || !(szft[(uint8_t)d]&M_SZF_EOL))
      return s1;
   if(c==d)
      goto eollp1;
   c=d; // both eols, but different
   goto eollp2;
} // sznextline()

// JFL 03 Dec 10
// JFL 25 May 11; EOL after start and before end
// JFL 03 Aug 11; delim can be '
chr*szquotes(chr *s1,chr *bufx,chr **startp,chr **endp)
{
   uint8_t n=0;
   chr delim='\"';

   if(*s1=='\'')
      delim=*s1;
   if(!(s1=szchr(s1,bufx,delim)))
   {
      s1=bufx;
      goto syntax;
   }

   s1++;
   if((s1[0]=='>')&&(s1[1]=='>')&&(s1[2]==delim)&&sztand(s1[3],M_SZF_EOL))
   {
      n=1,s1+=4;
      if((s1[0]!=s1[-1])&&sztand(s1[0],M_SZF_EOL))
         s1++;
   }

   if(startp)
      *startp=s1;

   longstrlp:
   if(!(s1=szchr(s1,bufx,delim)))
   {
      goto syntax;
      s1=bufx;
   }
   if(n)
   { // big-delim
      if((s1[1]!='<')||(s1[2]!='<')||(s1[3]!=delim)||!sztand(s1[-1],M_SZF_EOL))
         {s1++;goto longstrlp;} // doesn't match big-delim
      n=4; // always get one more

      // back up to end before the EOL
      s1--;
      if((s1[-1]!=s1[0]) && sztand(s1[-1],M_SZF_EOL))
         s1--,n++;
   } // big-delim

   if(endp)
      *endp=s1;

   s1+=1+n; // skip to after the delim
syntax:
   return s1;
} // szquotes()

// JFL 22 Nov 11
int szpackslasheolwhite(chr *s1,chr *s1x,chr **s1xp)
{ // pack aways \_EOL_WHITE at the end of line
   int r=0;
   chr *ss,*tt;
   if(!s1x)
      s1x=s1+szlen(s1,s1x);
   while(*s1 && (s1<s1x))
   {
      if(!(ss=szchr(s1,s1x,'\\')))
         break;
      s1=ss+1; // advance to past slash
      if(!sztand(ss[1],M_SZF_EOL))
         continue; // not slasheol

      // pack away the slasheol
      s1=sztskip(s1,s1x,M_SZF_EOL|M_SZF_WHITE);
      for(tt=s1;tt<s1x;)
         *ss++=*tt++;
      r+=s1x-ss; // number removed
      s1x=ss; // new terminator
   } // while

   if(s1xp)
      *s1xp=s1x;

   return r;
} // szpackslasheolwhite()

// JFL 22 Nov 11
int szpackmeta(chr *s1,chr *s1x,chr **s1xp,chr *metatable,chr *replacetable)
{ // pack "\n" --> '\n'
   int r=0;
   chr *ss,*tt,*rr;
   if(!s1x)
      s1x=s1+szlen(s1,s1x);
   while(*s1 && (s1<s1x))
   {
      if(!(ss=szchr(s1,s1x,'\\')))
         break;
      for(tt=metatable,rr=replacetable;*tt;tt++,rr++)
      {
         if(*tt==ss[1])
         {
            *ss++=*rr;
            goto packdown;
         }
      } // for
      s1++;
      continue; // no match
      packdown:

      // packdown one
      s1=ss;
      for(tt=ss+1;tt<s1x;)
         *ss++=*tt++;
      r+=s1x-ss; // number removed
      s1x=ss; // new terminator
   } // while

   if(s1xp)
      *s1xp=s1x;

   return r;
} // szpackmeta()

// JFL 20 Oct 05
// JFL 22 Jan 07; return NUL on failure
chr* szsub(chr *sub,chr *subx,chr *s2,chr *s2x)
{ // find first occurrence of sub in string s2
   chr c,d,*ss,*tt;

   if(!s2)
      return s2;
lp:
   if(s2x&&(s2>=s2x))
      return 0; // fail

   for(ss=sub,tt=s2;;ss++,tt++)
   {
      if(subx&&(ss>=subx))
         return s2; // success
      if(!*ss)
         return s2; // success
      if(s2x&&(tt>=s2x))
         return 0; // fail
      if(!(c=*tt))
         return 0; // fail
      d=*ss;
      if(c!=d)
         break; // mismatch
   } // for
   s2++;
   goto lp;
} // szsub()

// JFL 02 Nov 05
// JFL 01 Feb 07
int szstart(chr *start,chr *s2,chr *s2x)
{ // test if string s2 starts with s1
   chr s,t;

   if(!start || !s2)
      return SZ_ERR_RETURN;

   while(!s2x || (s2<s2x))
   {
      s=*start++;
      t=*s2++;
      if(!s) // source hit end, match
         return 0;
      if(s!=t) // mismatch
         return s-t;
   } // while

   if(!*start)
      return 0;

   return SZ_MISMATCH_RETURN;
} // szstart()

// JFL 02 Nov 05
// JFL 01 Feb 07
// JFL 15 Sep 11; start startx
int szstart2(chr *start,chr *startx,chr *s2,chr *s2x)
{ // test if string s2 starts with s1
   chr s,t;

   if(!start || !s2)
      return SZ_ERR_RETURN;

   while((!s2x || (s2<s2x)) && (!startx || (start<startx)))
   {
      s=*start++;
      t=*s2++;
      if(!s) // source hit end, match
         return 0;
      if(s!=t) // mismatch
         return s-t;
   } // while

   if(!*start || (startx && (start>=startx)))
      return 0;

   return SZ_MISMATCH_RETURN;
} // szstart2()

// JFL 02 Nov 05
// JFL 01 Feb 07
// JFL 12 Feb 12
int szistart(chr *start,chr *s2,chr *s2x)
{ // test if string s2 starts with s1
   chr s,t;

   if(!start || !s2)
      return SZ_ERR_RETURN;

   while(!s2x || (s2<s2x))
   {
      s=*start++;
      if((s>='A')&&(s<='Z')) s+='a'-'A';
      t=*s2++;
      if((t>='A')&&(t<='Z')) t+='a'-'A';
      if(!s) // source hit end, match
         return 0;
      if(s!=t) // mismatch
         return s-t;
   } // while

   if(!*start)
      return 0;

   return SZ_MISMATCH_RETURN;
} // szstart()

// JFL 02 Nov 05
// JFL 01 Feb 07
// JFL 15 Sep 11; start startx
// JFL 12 Feb 12
int szistart2(chr *start,chr *startx,chr *s2,chr *s2x)
{ // test if string s2 starts with s1
   chr s,t;

   if(!start || !s2)
      return SZ_ERR_RETURN;

   while((!s2x || (s2<s2x)) && (!startx || (start<startx)))
   {
      s=*start++;
      if((s>='A')&&(s<='Z')) s+='a'-'A';
      t=*s2++;
      if((t>='A')&&(t<='Z')) t+='a'-'A';
      if(!s) // source hit end, match
         return 0;
      if(s!=t) // mismatch
         return s-t;
   } // while

   if(!*start || (startx && (start>=startx)))
      return 0;

   return SZ_MISMATCH_RETURN;
} // szstart2()

///////////////////////////////////////////////////////////////////////////////

// JFL 15 Aug 04
int szmap(chr *table[],chr *s1,chr *s1x)
{ // find first match of string in a nul-terminated string table
   int i;
   for(i=0;*table;i++,table++)
   {
      if(!szcmp(*table,s1,s1x))
         return i;
   } // for
   return SZ_MISMATCH_RETURN;
} // szmap()

// JFL 15 Aug 04
int szimap(chr *table[],chr *s1,chr *s1x)
{ // find first match of string in a nul-terminated string table, ignore case
   int i;
   for(i=0;*table;i++,table++)
   {
      if(!szicmp(*table,s1,s1x))
         return i;
   } // for
   return SZ_MISMATCH_RETURN;
} // szimap()

// JFL 15 Mar 13
int szmapv(szMapVRec *table,chr *str,chr *strx)
{
   if(!table || !str) return(SZ_ERR_RETURN);
   for(;table->name;table++)
   {
      if(!szcmp(table->name,str,strx))
         return table->v;
   } // for
   return(SZ_MISMATCH_RETURN);
} // szmapv()

// JFL 19 Dec 15
int szcountlines(chr *str,chr *strx)
{
   int lines;
   for(lines=1;!szeos(str,strx);)
   {
      str=szttill(str,strx,M_SZF_EOL);
      if(!sztand(*str,M_SZF_EOL))
         break;
      lines++;
      if(sztand(str[1],M_SZF_EOL) && (str[0]!=str[1]))
         str++;
      str++;
   } // for
   return lines;
} // szcountlines()

///////////////////////////////////////////////////////////////////////////////
// CONVERSIONS

// JFL 16 Feb 05
// JFL 15 Sep 05; backwards
// JFL 21 Sep 05; float
// JFL 22 Sep 05; neg
// JFL 28 Jan 07; ' match C4
// JFL 27 Nov 09; re-added sci (18 Nov 06)
// JFL 12 Mar 12; uint16_t, int16_t
// JFL 30 Jan 13; return sx not s1 -- sci exponent fix
// JFL 26 Mar 13; test for !s1
chr* sztobin(chr *s1,chr *s1x,int base,chr dsttype,void *dst)
{
   int vali;
   int64_t vall;
   int loop,neg;
   int c;
   chr *ss,*s2,*sx=s1x;
   double vald=0,dec=0,scimul;
   int scival,scibase=0;

   if(!s1)
      goto done;

   neg=0;
   //loop=0;
   switch(dsttype)
   {
   case 'j': // int16_t
   case 'J': // uint16_t
   case 'i':
   case 'u':
      vali=0;
      loop=1;
      break;
   case 'l': // int64_t
   case 'q': // int64_t
      vall=0;
      loop=3;
      break;
   case 'f':
   case 'd':
      vald=0;
      loop=2;
      break;
   default:
      // BRK();
      goto done;
   } // switch

   if(*s1=='-')
      neg=1,s1++;
   if(((s1[0])=='0')
      && (((s1[1])=='x')||((s1[1])=='X')))
   {
      s1+=2;
      base=16;
   }
   if(!base)
      base=10;

   if((*s1)=='\'')
   { // 4 chr constant
      s1++;
      if((*s1!='\'') && (!s1x || (s1<s1x))) vali|=*((uint8_t*)s1),s1++;
      if((*s1!='\'') && (!s1x || (s1<s1x))) vali|=*((uint8_t*)s1)<<8,s1++;
      if((*s1!='\'') && (!s1x || (s1<s1x))) vali|=*((uint8_t*)s1)<<16,s1++;
      if((*s1!='\'') && (!s1x || (s1<s1x))) vali|=*((uint8_t*)s1)<<24,s1++;
      goto done;
   } // 4 chr constant

   for(ss=s1;(c=*ss) && (!s1x || (ss<s1x));ss++)
   {
      if((c>='0')&&(c<='9'))
         c-='0';
      else if((c>='a')&&(c<='z'))
         c=c-'a'+10;
      else if((c>='A')&&(c<='Z'))
         c=c-'A'+10;
      else if((c=='.') && (dsttype=='f'||dsttype=='d'))
      {
         dec=1; // decimal found/implied
         continue;
      }
      else
         break;
      if(c>=base)
      {
         c=*ss;
         if((c=='e')||(c=='E'))
            scibase=1; // scientific notation
         break;
      }
      if(dec)
         dec*=base;
   } // for
   sx=ss;

   if(scibase)
   { // sci
      for(s2=1+ss;(c=*s2) && (!s1x || (s2<s1x));s2++)
      {
         if((c>='0')&&(c<='9'))
            ;
         else if(c=='-')
            ;
         else
            break;
      } // for

      sx=s2;
      scival=0;
      while(s2>(1+ss))
      {
         c=*--s2;
         if(c>='0'&&c<='9')
            scival+=(c-'0')*scibase,scibase*=10;
         else if(c=='-')
            scival=-scival;
         else
            break;
      } // for

      scimul=1;
      if(scival<0)
      {
         while(scival<0)
            scimul*=0.1,scival++;
      }
      else if(scival>0)
      {
        while(scival>0)
            scimul*=10,scival--;
      }
   } // sci

   while(s1<ss)
   {
      c=*s1++;
      if((c>='0')&&(c<='9'))
         c-='0';
      else if((c>='a')&&(c<='z'))
         c=c-'a'+10;
      else if((c>='A')&&(c<='Z'))
         c=c-'A'+10;
      else if((c=='.') && (dsttype=='f'||dsttype=='d'))
         continue;
      else
         {s1--;goto done;}
      if(c>=base)
         {s1--;goto done;}

      if(loop==1)
      {
         vali*=base;
         vali+=c;
      }
      else if(loop==2)
      {
         vald*=base;
         vald+=c;
      }
      else if(loop==3)
      {
         vall*=base;
         vall+=c;
      }

   } // while

done:
   if(dec)
      vald/=dec;

   switch(dsttype)
   {
   case 'i':
      if(neg)
         vali=-vali;
      *((int*)dst)=vali;
      break;
   case 'u':
      *((uns*)dst)=vali;
      break;
   case 'j':
      if(neg)
         vali=-vali;
      *((int16_t*)dst)=vali;
      break;
   case 'J':
      *((uint16_t*)dst)=vali;
      break;
   case 'f':
      if(neg)
         vald=-vald;
      if(scibase)
         vald*=scimul;
      *((float*)dst)=(float)vald;
      break;
   case 'd':
      if(neg)
         vald=-vald;
      if(scibase)
         vald*=scimul;
      *((double*)dst)=vald;
      break;
   case 'l':
   case 'q':
      *((int64_t*)dst)=vall;
      if(neg)
         *((int64_t*)dst)=-*((int64_t*)dst);
      break;
   } // switch

   return sx;
} // sztobin()

// JFL 06 Feb 05
// JFL 14 Feb 05
// JFL 17 Jun 07; return review
int s0binto(chr *dst,int dstsize,int base,chr srctype,void *src)
{
   int nn;
   uint64_t valu;
   uns d,i;
   chr digits[128];
   uint8_t lower=0,neg=0,noterm=0,showsign=0;

   if(base<0)
      base=-base,lower=1;

   // size to len
   if(dstsize&M_SZF_DSTSIZE_LEN)
      dstsize&=~M_SZF_DSTSIZE_LEN;
   else
      dstsize>>=S_SZCHR_SIZE;

   // noterm option
   if(dstsize&(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE))
   {
      dstsize&=~(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE);
      noterm=1;
   }
   else
      dstsize--; // leave room for 0

   if(dstsize<=0)
      return 0;
   nn=dstsize;

   switch(srctype)
   {
   case 'I':
      showsign=1;
   case 'i':
      if(*((int*)src)<0)
         neg=1,valu=-*((int*)src);
      else
         valu=*((int*)src);
      valu&=0xffffffff;
      break;
   case 'x':
   case 'u':
      lower=1;
      // falls through
   case 'X':
      valu=*((uns*)src);
      break;
   case 'U':
      valu=*((uint64_t*)src);
      break;
   default:
      if(dstsize)
         dstsize--,*dst++='?';
      goto term;
   }  // switch

   if(!base)
      base=10;

   if(!valu)
   {
      if(dstsize)
         dstsize--,*dst++='0';
      goto term;
   }

   for(i=0;valu && (i<NUM(digits));i++)
   {
      d = (uns)(valu % base);
      valu /= base;
      if(d<=9)
         d+='0';
      else if(!lower)
         d+='A'-10;
      else
         d+='a'-10;
      digits[i]=(chr)d;
   } // for

   if(dstsize>0)
   {
      if(neg)
         dstsize--,*dst++='-';
      else if(showsign)
         dstsize--,*dst++='+';
   }

   while((dstsize>0) && (i>0))
      dstsize--,*dst++=digits[--i];

term:
   if(!noterm)
      *dst=0;

   return nn-dstsize;
} // s0binto()

// JFL 02 Apr 12
chr* szutf8c(chr *s1,chr *s1x,uint32_t *dst)
{
   chr c;
   uint8_t k;
   uint32_t utf=0;

   k=SZUTF8N(*s1); // 1..6
   if(s1x && (s1+k>s1x))
      goto done;
   k--; // 0..5
   if(!k)
   {
      utf=*s1++;
      goto done;
   }
   utf=*s1++ & (0x3f>>k);
   while(k && (c=*s1))
   {
      k--,s1++;
      utf<<=6;
      utf|=c&0x3f;
   } // while

done:
   if(dst)
      *dst=utf;
   return s1;
} // szutf8c()

// JFL 16 Jan 14
int sztobinarray(ptr dst,int dstMax,chr dstType,chr **strp,chr *strx)
{
   int num;
   chr *str=*strp,*s1;

   for(num=0;num<dstMax;num++)
   {
      s1=szttill(str,strx,M_SZF_NUM|M_SZF_MODNUM);
      str=sztskip(str,strx,M_SZF_WHITE|M_SZF_EOL|M_SZF_SYMBOLS|M_SZF_DELIM);
      if(s1 && str && (s1<str))
         str=s1;
      if(str && !sztand(*str,M_SZF_NUM|M_SZF_MODNUM))
         break;
      if(szeos(str,strx))
         break;
      str=sztobin(str,strx,0,dstType,dst);
      switch(dstType)
      {
      default:
         dst=PTR_ADD(dst,sizeof(int));
         break;
      case 'd':
         dst=PTR_ADD(dst,sizeof(double));
         break;
      } // switch
   } // for

   *strp=str;
   return num;
} // sztobinarray()

///////////////////////////////////////////////////////////////////////////////
// FORMATTING

///////////////////////////////////////////////////////////////////////////////
// szfmt codes
//
// %[fmt-loop]<specifier>
//
// [fmt-loop] while these arguments
//  .# digits after decimal, # is integer
//  -  left justify
//  +  show sign of result with +/-
//  #  width, # is non-zero integer
//  0  pads with '0' instead of space
//  <  special processing
//
// <specifier>
//  c -- chr -- c=varg_next(args,chr)
//  C -- unsigned -- u=varg_next(args,unsigned) -- 32 uint8_t value as characters
//  s -- 0 terminated string -- sz=varg_next(args,chr*)
//  S -- string start & end -- ss=varg_next(args,chr*), sx=varg_next(args,chr*)
//  i,d -- int -- i=varg_next(args,int)
//  f,g,G,e,E -- float/double -- f=varg_next(args,double)
//  rC -- repeat single chr C n times -- n=varg_next(args,int)
//  u -- unsigned -- u=varg_next(args,unsigned)
//  x,X -- unsigned hex -- u=varg_next(args,unsigned), X for uppercase
//  n -- store current length in intptr, intptr = varg_next(args,int*)
//
///////////////////////////////////////////////////////////////////////////////

#define SZPRECISION_DEFAULT 2
#define SZPRECISION_G_MAX   10
#define SZPRECISION_MAX     18

// JFL 06 Feb 05
// JFL 04 Jun 05
// JFL 30 Jul 06; added szStream
// JFL 17 Jun 07
// JFL 28 Jun 07; cleanup
// JFL 12 Dec 09; total len returned
// JFL 14 Nov 10; new format chaining
// JFL 12 May 11; %g & max
int szfmt_v(chr *dst,int dstsize,chr *fmt,chr *fmtx,chr **fmtp,...)
{
   chr c,pad;
   int len=0,totlen=0,precision,width,orglen,outlen;
   int i,n,vali;
   uint64_t uu;
   uns valu;
   double valf,modf,round;
   float f1;
   chr *s1,*s1x,*orgdst;
   uint8_t leftjustify,showsign;
   uint8_t noterm=0;
   fnc_ipi outf=0; // output function
   vargs args;

   varg_start(args,fmtp);

   ///////////////////////////////////////////////////////////////////////////
   // sz programming doc
   //
   // dstsize is buffer size
   // guarantee a 0 at the end of the string
   // we can only copy dstsize-1 characters (max len returned is dstsize-1)
   // when calling another routine, we pass the buffer size
   // always parse the whole fmt string (even when dstsize<=0)
   // use (dstsize>0) before storing
   // don't advance fmt past the EOS
   //
   ///////////////////////////////////////////////////////////////////////////

   // size to len
   if(dstsize&M_SZF_DSTSIZE_LEN)
      dstsize&=~M_SZF_DSTSIZE_LEN;
   else
      dstsize>>=S_SZCHR_SIZE;

   // noterm option
   if(dstsize&(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE))
   {
      dstsize&=~(M_SZF_DSTSIZE_NOTERM>>S_SZCHR_SIZE);
      noterm=1;
   }
   else
      dstsize--; // room for term

   orglen=dstsize;
   orgdst=dst;
   if((outlen=orglen-1)<0)
      outlen=0;

restart:
   for(;;)
   {
      if(fmtx && (fmt>=fmtx))
         break;
      if(!(c=*fmt))
         break;
      fmt++;
      if(c!='%')
      { // store
store:
         if(outf && (len>outlen))
         { // output function
            if(dstsize>=0)
               *dst=0;
            (*outf)(orgdst,len);
            totlen+=len,len=0,dst=orgdst,dstsize=orglen; // restart buf
         } // output function

         if(dstsize>0)
            *dst++=c,dstsize--,len++;

         continue;
      } // store

      if(outf && (len>(outlen-8)))
      { // output function
         if(dstsize>=0)
            *dst=0;
         (*outf)(orgdst,len);
         totlen+=len,len=0,dst=orgdst,dstsize=orglen; // restart buffer
      } // output function

      // reset each specifier
      precision=SZPRECISION_DEFAULT;
      leftjustify=showsign=0;
      width=0;
      pad=' ';
fmtloop:
      switch(c=*fmt++)
      {
      case 0:
         fmt--; // back up to term
         goto done;
      case '%':
         goto store;
      case '.': // precision
         s1=sztobin(fmt,0,10,'i',&precision);
         if(s1==fmt) // nothing converted
            precision=varg_next(args,int);
         goto fmtloop;
      case '-': // leftjustify
         leftjustify=1;
         goto fmtloop;
      case '+': // showsign
         showsign=1;
         goto fmtloop;
      case 'c': // chr
         c=(chr)varg_next(args,int);
         if(c==C_SZ_SKIP)
            continue;
         if(!c) c=' ';
         goto store;
      case 'C': // 4 chr constant
         valu=varg_next(args,uns);
         c=(valu>>0)&0xff;
         if(!c) if(!(c=pad)) break;
         if(dstsize)
            *dst++=c,dstsize--,len++;
         c=(valu>>8)&0xff;
         if(!c) if(!(c=pad)) break;
         if(dstsize)
            *dst++=c,dstsize--,len++;
         c=(valu>>16)&0xff;
         if(!c) if(!(c=pad)) break;
         if(dstsize)
            *dst++=c,dstsize--,len++;
         c=(valu>>24)&0xff;
         if(!c) if(!(c=pad)) break;
         if(dstsize)
            *dst++=c,dstsize--,len++;
         break; // 4 chr constant
      case 'i':
      case 'd':
         vali=varg_next(args,int);
         c=showsign?'I':'i';
         n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,
            10,c,&vali);

padifneeded:
         vali=width-n; // additional padding wanted
         if(vali>(dstsize-n))
            vali=dstsize-n;
         if(vali<=0)
         {
            dst+=n,dstsize-=n,len+=n;
            break;
         }

         // dst=buf, n=chrs in buf, vali=pad to add
         if(!leftjustify)
            goto rightjustify;

         // left justify (add pad at the end)
         s1=dst+n;
         i=vali;
         while(i>0)
            *s1++=pad,i--;
         goto justified;

rightjustify:
         s1=dst+n;
         s1x=dst+vali+n;
         i=n;
         while(i>0)
            *--s1x=*--s1,--i;

         i=vali;
         while(i>0)
            *--s1x=pad,--i;

justified:
         n+=vali;
         dst+=n,dstsize-=n,len+=n;
         break;

      case 'g': // floating point
         precision=-1; // as needed..
      case 'f': // floating point
      case 'G': // floating point
      case 'e': // floating point
      case 'E': // floating point

            f1=valf=varg_next(args,FLOATS_PASSED_AS);
            if(FLT_IS_ERR(f1))
            {
               s1=S0_ERRFLOAT;
               while((c=*s1++) && (dstsize>0))
                  *dst++=c,dstsize--,len++;
               break;
            }

            if(valf<0)
            {
               valf=-valf;
               if(dstsize)
                  *dst++='-',dstsize--,len++;
            }
            else if(showsign)
            {
               if(dstsize)
                  *dst++='+',dstsize--,len++;
            }

            // precision is the number of places after the decimal
            if(precision>0)
            { // decimal places
               if(precision>SZPRECISION_MAX)
                  precision=SZPRECISION_MAX; // clamp
               if(precision<0)
                  precision=0;
               modf=10.0;
               round=0.05;

               n=precision;
               if(n>0) while(--n)
                  modf*=10,round*=0.1;

               valf+=round;
            } // decimal places
            else
            { // no decimal places
               modf=1.0;
               round=0;
            } // no decimal places

            // integer part of value
            uu=(uint64_t)valf; // 18446744073709551615 max uint64_t
            n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,
               10,'U',&uu);
            dst+=n,dstsize-=n,len+=n;

            valf-=(double)uu; // subtract out integer part
            if((precision<0) && valf)
            { // precision as needed
               // find how many digits we need..
               round=valf;
               modf=1;
               n=precision=0;
               while(round && (precision<SZPRECISION_G_MAX))
               {
                  modf*=10,precision++;
                  round*=10;
                  uu=(uint64_t)round;
                  if(uu)
                     n=precision;
                  round-=(double)uu;
               } // while

               if(n!=precision)
               { // trim trailing zeros
                  precision=n;
                  modf=1;
                  while(n>0)
                     modf*=10,n--;
               } // trim trailing zeros
            } // precision as needed

            if(precision>0)
            { // right of decimal
               //valf-=(double)uu; // subtract out integer part
               valf*=modf; // shift to left of decimal
               uu=(uint64_t)valf; // turn into integer

               if(dstsize)
                  *dst++='.',dstsize--,len++;

               n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,
                  10,'U',&uu);

               if(((vali=precision-n)>0) && (dstsize>vali))
               {
                  pad='0';

                  s1=dst+n;
                  s1x=dst+vali+n;
                  i=n;
                  while(i>0)
                     *--s1x=*--s1,--i;

                  i=vali;
                  while(i>0)
                     *--s1x=pad,--i;

                  leftjustify=1;
                  n+=vali;
                  goto padifneeded;
               }

               dst+=n,dstsize-=n,len+=n;
            } // right of decimal

            break; // floating point
      case 'n': // store current len
         if((s1=(chr*)varg_next(args,int*)))
            *((int*)s1)=len;
         break;
      case 'r': // repeat next chr
         vali=varg_next(args,int);
         if((c=*fmt))
         {
            fmt++;
            while((vali>0) && (dstsize>0))
               *dst++=c,dstsize--,len++,vali--;
         }
         break; // repeat
      case 'u':
         valu=varg_next(args,uns);
         n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,
            10,'u',&valu);
         goto padifneeded;
      case 'p': // pointer-size
         uu=(intptr)varg_next(args,ptr);
         goto uu_uns64;
      case 'l':
      case 'q':
         uu=varg_next(args,uint64_t);
      uu_uns64:
         if(*fmt=='x')
         {
            fmt++;
            n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,-16,'U',&uu);
         }
         else if(*fmt=='X')
         {
            fmt++;
            n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,16,'U',&uu);
         }
         else
         {
            if((*fmt=='d')||(*fmt=='i')||(*fmt=='u'))
               fmt++;
            n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,10,'U',&uu);
         }
         
         goto padifneeded;
      case 'x':
      case 'X':
         valu=varg_next(args,uns);
         n=s0binto(dst,dstsize|M_SZF_DSTSIZE_NOTERM|M_SZF_DSTSIZE_LEN,
            16,c,&valu);
         goto padifneeded;
      case 's': // zero-terminated string
         if(!(s1=varg_next(args,chr*)))
            break;
         while((c=*s1++) && (dstsize>0))
            *dst++=c,dstsize--,len++;
         break;

      case 'S': // pointer-terminated
         s1=varg_next(args,chr*);
         s1x=varg_next(args,chr*);
         if(!s1) break;
         while(*s1 && (!s1x || (s1<s1x)) && (dstsize>0))
            *dst++=*s1++,dstsize--,len++;
         break;

      case '<': // special
         s1=fmt;
         while((c=*s1)&&s1++) switch(c)
         {
         case 'A': // chain args
            varg_next_vargs(args);
            
           // {
           // vargs *varp=varg_next(args,vargs*);
           // va_copy(args,*varp);
           // }
            break;
         case 'a': // chain args -- defunct
            BRK();
            goto done;
            //varg_next_vargs(args,args);
         case 'o': // output function
            outf=(fnc_ipi)varg_next(args,void*);
            break;
         case 'f': // chain the format string
            fmt=varg_next(args,chr*);
            fmtx=0;
            break;
         case 'r': // restart
            goto restart;
         case '>':
            fmt=s1;
            break; // done
         } // while switch
         goto fmtloop;
      case '0':
         pad=c; // padding character is '0'
         goto fmtloop;
      case '*':
         width=varg_next(args,int);
         goto fmtloop;
      default:
         if((c>='1')&&(c<='9'))
         {
            fmt=sztobin(fmt-1,0,10,'i',&width);
            goto fmtloop;
         }

         // unknown argument, error
         fmt=S0_ERRARG;
         fmtx=0;
         break;
      } // switch
   } // for

done:
   if(!noterm)
   {
      if(dstsize>=0)
         *dst=0;
      else if(len>0)
         dst[-1]=0;
   }

   if(outf && (len>0))
   { // output function
      (*outf)(orgdst,len);
      totlen+=len;
   } // output function

   if(!outf)
      totlen=len;

   if(fmtp)
      *fmtp=fmt;

   varg_end(args);
   return totlen;
} // szfmt_v()

// JFL 06 Feb 05
int szfmt(chr *dst,int dstsize,chr *fmt,...)
{
   int len;
   vargs args;
   //varg_rec args2;

   if(!fmt)
      return -1;
   varg_start(args,fmt);
   //args2=*((varg_rec*)&args);
   len=szfmt_v(dst,dstsize,"%<fAr",0,0,fmt,&args);
   varg_end(args);
   return len;
} // szfmt()

// EOF
