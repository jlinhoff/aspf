// Copyright (C) 2016 Joe Linhoff - see license
#include "base.h"
#include "sz.h"
#include "support.h"

#include <time.h>

#define TIMERLONG(_tr_) (_tr_)->priv[0]

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

// EOF