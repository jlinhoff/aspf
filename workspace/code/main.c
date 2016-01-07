// Copyright (C) 2015 Joe Linhoff - see license
#include <stdio.h>
#include "vers.h"
#include "base.h"
#include "sz.h"
#include "main.h"

MainGlobals MainG;

// JFL 28 Mar 13
int mainInit()
{
   int r,fail=0;
   if((r=SPFInit())<0)
      fail=1;
   if((r=szInit())<0)
      fail=1;
   r=fail?-1:0;
   return r;
} // mainInit()

// JFL 28 Mar 13
void mainFinal()
{
   szFinal();
   SPFFinal();
} // mainFinal()

///////////////////////////////////////////////////////////////////////////////

// JFL 28 Mar 13
int main(int argc,char *argv[])
{
   int r;

   //
   // INIT
   //

   if((r=mainInit())<0)
      goto BAIL;

   //
   // FINAL
   //

BAIL:
   mainFinal();
   return 0;
} // main()

// EOF
