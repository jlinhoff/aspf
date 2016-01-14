// support.h
#ifndef SUPPORT_H
#define SUPPORT_H

// Copyright 2016 Joe Linhoff
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

#ifdef __cplusplus  // C in C++
extern "C" {        //
#endif              //

///////////////////////////////////////////////////////////////////////////////

// high speed timer record
typedef struct {
   uint64_t priv[2];
} glueTimerRec;

// high speed timer enums
enum {
   GLUETIMEROP_NONE,
   GLUETIMEROP_START, // start a timer
   GLUETIMEROP_TICKS, // get ticks
   GLUETIMEROP_STOP, // stop -- get ticks before stopping
   GLUETIMEROP_TICKS_RESTART, // get ticks & restart
   GLUETIMEROP_TICKSPERSECOND, // get ticks per second
   GLUETIMEROP_TICKSPERMILLISECOND, // get ticks per millisecond
};

SPF_API int64_t glueTimerOp(glueTimerRec *tr,uns op,...);

///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus  // C in C++
}                   //
#endif              //

#endif // SUPPORT_H
