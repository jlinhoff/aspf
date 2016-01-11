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

#if UINTPTR_MAX==UINT64_MAX
#define BUILD_64BIT 1
#else // !BUILD_64BIT
#define BUILD_64BIT 0
#endif // !BUILD_64BIT

#define NUM(_a_) (sizeof(_a_)/sizeof(_a_[0])) ///< number of entries

#define PTR_ADD(_p_,_i_) (void*)(((uint8_t*)(_p_))+(_i_))
#define PTR_SUB(_p_,_i_) (void*)(((uint8_t*)(_p_))-(_i_))
#define PTR_DIFF(_p1_,_p2_) ((int)((uint8_t*)(_p1_)-(uint8_t*)(_p2_)))

typedef char chr;
typedef void* ptr;
typedef unsigned int uns;

typedef int (*fnc_ipi)(ptr,int);
typedef int (*fnc_ipia)(ptr,int,...);

#if BUILD_64BIT
typedef int64_t intptr; // pointer as int
#else // !BUILD_64BIT
typedef int32_t intptr; // pointer as int
#endif // !BUILD_64BIT

#define ALIGN(_n_,_m_) ((((uns)_n_)+(_m_))&~(_m_))
#define ALIGNMACH(_n_) ALIGN(_n_,7) // machine alignment

#ifndef SPF_API
#if BUILD_SPF_EXPORTS
   #define SPF_API __declspec(dllexport)
#elif BUILD_SPF_IMPORTS
   #define SPF_API __declspec(dllimport)
#else
   #define SPF_API extern
#endif
#endif // ndef SPF_API

#include <stdarg.h>
#define vargs va_list
#define varg_start va_start
#define varg_end va_end
#define varg_next va_arg

#define varg_next_vargs(_args_) \
   {vargs *varp=varg_next(args,vargs*);va_copy(_args_,*varp);}
   
#define FLOATS_PASSED_AS double

#define BRK()

// test for NaN, inf, ind
#define FLT_IS_ERR(_f_) (((_f_)!=(_f_))||(0!=(_f_)-(_f_)))

////////////////////////////////////////////////////////////////////////////////

#include <malloc.h>
#define MemAlloc(_m_,_s_) (*(_m_)=malloc(_s_))?1:-1
#define MemFree(_m_) free(_m_)

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

////////////////////////////////////////////////////////////////////////////////

typedef struct {
   ListNode lnk;
   fnc_ipia fnc; // int fnc(ListObj *obj,int LISTOBJOP_,..)
   chr *name; // name
} ListObj;

enum {
   LISTOBJOP_NONE,
   LISTOBJOP_ZAP,
};

#define LISTOBJ_ZAP(_o_) \
   (((ListNode*)(_o_))->t>LISTOBJID_FIRST)&&(((ListNode*)(_o_))->t<LISTOBJID_LAST)&& \
   ((ListObj*)(_o_))->fnc?(((ListObj*)(_o_))->fnc((_o_),LISTOBJOP_ZAP)):(ListUnlink((ListNode*)(_o_)),glueMemFree(_o_))
   
#define LISTOBJ_MAKE(_obj_,_n_,_fnc_) \
   ListMakeNode(LN(_obj_),(_n_)),((ListObj*)(_obj_))->fnc=(fnc_ipia)(_fnc_)

SPF_API int ListObjNew(ListObj **objp,
   int listObjId,int objSize,int extraSize,
   fnc_ipia fnc,chr *name,chr *namex);
   
// create ListObj object
// object is build: obj,extra,name
#define LISTOBJNEW(_objp_,_objid_,_extra_,_fnc_,_name_,_namex_) \
   ListObjNew(((ListObj**)_objp_),_objid_,sizeof(**(_objp_)),_extra_,_fnc_,_name_,_namex_)

///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus  // C in C++
}                   //
#endif              //

#endif // BASE_H
