// Copyright (C) 2016 Joe Linhoff - see license
#include "base.h"
#include "sz.h"
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
// base

struct {
   uint16_t sidForObj;
} baseG;

int SPFInit()
{
   memset(&baseG,0,sizeof(baseG));
   return 0;
} // SPFInit()

void SPFFinal()
{
} // SPFFinal

// JFL 11 Jan 16
uint16_t BaseSidNext()
{
   uint16_t sid;
   if(!(sid=++baseG.sidForObj))
      sid=baseG.sidForObj=1;
   return sid;
} // BaseSidNext()

////////////////////////////////////////////////////////////////////////////////
// ListNode

// JFL 23 Aug 06
// JFL 20 Mar 08; re-worked from DL
// JFL 25 Oct 15
void ListMakeHead(ListNode *h)
{
   h->n=h->p=h;
   h->t=h->f=0;
} // ListMakeHead()

// JFL 20 Mar 08; re-worked from DL
// JFL 18 May 08; link to self
// JFL 25 Oct 15
void ListMakeNode(ListNode *n,int t)
{
   n->n=n->p=n;
   n->t=t,n->f=0;
} // ListMakeNode()

// JFL 23 Aug 06
// JFL 20 Mar 08; re-worked from DL
// JFL 08 Mar 10; fixed link bug
void ListLinkAfter(ListNode *h,ListNode *n)
{
   n->p=h;
   n->n=h->n;
   h->n->p=n;
   h->n=n;
} // ListLinkAfter()

// JFL 05 May 06
// JFL 20 Mar 08; re-worked from DL
void ListLinkBefore(ListNode *h,ListNode *n)
{
   n->p=h->p;
   n->p->n=n;
   n->n=h;
   h->p=n;
} // ListLinkBefore()

// JFL 05 May 06
// JFL 20 Mar 08; re-worked from DL
void ListUnlink(ListNode *n)
{
   n->p->n=n->n;
   n->n->p=n->p;
   n->n=n->p=n; // multiple unlinks OK
} // ListUnlink()

///////////////////////////////////////////////////////////////////////////////

// JFL 11 Jan 16
int ListObjRefMake(ListObjRef *ref,ListNode *obj)
{
   if((obj->t<LISTOBJID_FIRST)||(obj->t>LISTOBJID_LAST))
      return -1;
   if(!((ListObj*)obj)->sid)
      ((ListObj*)obj)->sid=BaseSidNext();
   ref->obj=obj;
   ref->sid=((ListObj*)obj)->sid;
   ref->t=obj->t;
   return 0;
} // ListObjRefMake();

// JFL 11 Jan 16
ListNode* ListObjRefGet(ListObjRef *ref,uint16_t t)
{
   ListNode *obj;
   if((obj=ref->obj))
      return 0;
   if(ref->sid != ((ListObj*)obj)->sid)
      return 0;
   if(t && (ref->t!=t))
      return 0;
   return obj;
} // ListObjRefGet()

///////////////////////////////////////////////////////////////////////////////

// JFL 10 Jan 16
static int listObjDefaultOp(ListObj *obj,int op,...)
{
   int r=0;
   switch(op)
   {
   case LISTOBJOP_ZAP:
      ListUnlink(LN(obj));
      MemFree(obj);
      break;
   } // switch
   return r;
} // listObjDefaultOp()

// JFL 10 Jan 16
int ListObjNew(ListObj **objp,
   int listObjId,int objSize,int extraSize,
   fnc_ipia fnc,chr *name,chr *namex)
{
   int r,s,ns;
   ListObj *obj;
   uint8_t *p;
   
   // calc aligned sizes
   objSize=ALIGNMACH(objSize);
   extraSize=ALIGNMACH(extraSize);
   ns=ALIGNMACH(szsize(name,namex));

   // alloc
   s=objSize+extraSize+ns;
   if((r=MemAlloc(&obj,s))<0)
      goto BAIL;
   memset(obj,0,s);
   
   // set
   ListMakeNode(LN(obj),listObjId);
   if(!fnc)
      fnc=(fnc_ipia)listObjDefaultOp;
   obj->fnc=fnc;
   if(ns)
   { // place name after extraSize
      p=(uint8_t*)obj;
      p+=objSize+extraSize;
      obj->name=(chr*)p;
      szcpy(obj->name,ns,name,namex);
   }
   
   *objp=obj;
   r=0;
BAIL:
   return r;
} // ListObjNew()

// EOF