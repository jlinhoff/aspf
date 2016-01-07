// base.h
#ifndef BASE_H
#define BASE_H

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

#include <stdint.h>
#include <signal.h>
#include <string.h>

#ifdef __cplusplus  // C in C++ wrapper
extern "C" {        //
#endif              //

#ifndef SPF_API
#if BUILD_SPF_EXPORTS
   #define SPF_API __declspec(dllexport)
#elif BUILD_SPF_IMPORTS
   #define SPF_API __declspec(dllimport)
#else
   #define SPF_API extern
#endif
#endif // ndef SPF_API

////////////////////////////////////////////////////////////////////////////////
// SPF

SPF_API int SPFInit();
SPF_API void SPFFinal();

////////////////////////////////////////////////////////////////////////////////
// ListNode

// linked list
typedef struct _listnode {
   struct _listnode *n;
   struct _listnode *p;
   uint16_t t; // type: listhead=0, others=non-zero
   uint16_t f; // flags
} ListNode;

// LN(n) -- casts to (ListNode*)
#define LN(_n_) ((ListNode*)(_n_))

// ListMakeHead(n) -- setup node as list head
// ListMakeNode(n,t) -- setup node with type t
SPF_API void ListMakeHead(ListNode *head);
SPF_API void ListMakeNode(ListNode *node,int t);

// ListLinkAfter(h,n) -- link n after h
// ListLinkBefore(h,n) -- link n before h
// ListUnlink(n) -- unlink n
SPF_API void ListLinkAfter(ListNode *head,ListNode *node);
SPF_API void ListLinkBefore(ListNode *head,ListNode *node);
SPF_API void ListUnlink(ListNode *node);

///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus  // C in C++
}                   //
#endif              //

#endif // BASE_H
