// Copyright (C) 2016 Joe Linhoff - see license
#include "base.h"
#include "sz.h"
#include "support.h"

#include <time.h>

#define TIMERLONG(_tr_) (_tr_)->priv[0]

#if !BUILD_MSVC

// JFL 12 Apr 07
int64_t glueTimerOp(glueTimerRec *tr,uns op,...)
{ // hopefully: a nano-second, free running timer
   int64_t r;
   struct timespec ts;
   clock_t t;

   switch(op)
   {
   case GLUETIMEROP_START:
      // save cur running time
      if(1)
      {
         TIMERLONG(tr)=clock();
      }
      else
      {
         if(clock_gettime(CLOCK_MONOTONIC,&ts))
            ret(-1);
         TIMERLONG(tr)=ts.tv_nsec;
      }
      break;
   case GLUETIMEROP_STOP:
      // no stopping
      break;
   case GLUETIMEROP_TICKS:
      if(1)
      {
         t=clock();
         r=t-TIMERLONG(tr);
      }
      else
      {
         if(clock_gettime(CLOCK_MONOTONIC,&ts))
            ret(-1);
         r=ts.tv_nsec;
      }
      goto BAIL;
   case GLUETIMEROP_TICKS_RESTART:
      if(1)
      {
         t=clock();
         r=t-TIMERLONG(tr);
         TIMERLONG(tr)=t; // reset
         if(r<0)
            r+=CLOCKS_PER_SEC;
      }
      else
      {
         if(clock_gettime(CLOCK_MONOTONIC,&ts))
            ret(-1);
         r=ts.tv_nsec-TIMERLONG(tr); // subtract cur from prev
         TIMERLONG(tr)=ts.tv_nsec; // save cur
         if(r<0)
            r+=CLOCKS_PER_SEC;
      }
      goto BAIL;
   case GLUETIMEROP_TICKSPERSECOND:
      if(1)
      {
         r=CLOCKS_PER_SEC;
      }
      else
      {
         if(clock_getres(CLOCK_MONOTONIC,&ts))
            ret(-1);
         r=ts.tv_nsec;
      }
      goto BAIL;
   case GLUETIMEROP_TICKSPERMILLISECOND:
      if(1)
      {
         r=CLOCKS_PER_SEC/1000;
      }
      else
      {
         if(clock_getres(CLOCK_MONOTONIC,&ts))
            ret(-1);
         r=ts.tv_nsec/1000;
      }
      goto BAIL;
   default:
      ret(-1);
   } // switch

   r=0;
BAIL:
   return r;
} // glueTimerOp()

#endif // !BUILD_MSVC
#if BUILD_MSVC

// JFL 12 Apr 07
int64 glueTimerOp(glueTimerRec *tr,uns op,...)
{
   int64 r;
   LARGE_INTEGER tt;
   uns64 nn;

   switch(op)
   {
   case GLUETIMEROP_START:
      QueryPerformanceCounter(&tt);
      tr->priv[0]=tt.QuadPart;
      break;
   case GLUETIMEROP_STOP: // no stopping
   case GLUETIMEROP_TICKS:
      QueryPerformanceCounter(&tt);
      nn=tt.QuadPart-tr->priv[0];
      if(nn<=0x7fffffffffffffff)
         r=nn;
      else
      {
         r=-2; // overflow
         BRK();
      }
      goto BAIL;
   case GLUETIMEROP_TICKS_RESTART:
      QueryPerformanceCounter(&tt);
      nn=tt.QuadPart-tr->priv[0];
      if(nn<=0x7fffffffffffffff)
         r=nn;
      else
         r=-2; // overflow
      tr->priv[0]=tt.QuadPart; // reset
      goto BAIL;
   case GLUETIMEROP_TICKSPERSECOND:
      if(!(r=QueryPerformanceFrequency(&tt))) // in counts per second
         {r=-3; goto BAIL;}
      if(tt.QuadPart<=0x7fffffffffffffff)
         r=tt.QuadPart;
      else
         r=-2; // overflow
      goto BAIL;
   case GLUETIMEROP_TICKSPERMILLISECOND:
      if(!(r=QueryPerformanceFrequency(&tt))) // in counts per second
         {r=-4; goto BAIL;}
      r=tt.QuadPart/1000;
      goto BAIL;
   default:
      bret(-1);
   } // switch

   r=0;
BAIL:
   return r;
} // glueTimerOp()
#endif // BUILD_MSVC

// EOF